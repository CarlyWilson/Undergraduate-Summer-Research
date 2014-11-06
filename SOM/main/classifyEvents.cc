//-----------------------------
//Name: classifyEvents.cc
//
//Description: Loads a trained SOM with popularity then looks at new data and determines the popularity of each event.
//
//Author: Carly Wilson
//------------------------------

#include "../GATHistoToVector.hh"
#include "../GATNeuron.hh"
#include "../GATSOM.hh"

#include <TH1D.h>
#include <TH2D.h>
#include <TChain.h>
#include <TCanvas.h>

#include <MGTWaveform.hh>
#include <MGTEvent.hh>
#include <MGWFBaselineRemover.hh>

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>

using namespace std;

int main(int argc, char* argv[])
{
	if(argc == 1)
	{
		cout<<"Error: Missing arguement for original number of Waveformns from trainSom.cc, number of additional Waveforms from determinePopularity, and the number of new Waveforms to be classified. In that order."<<endl;
		return 0;
	}
	else if(argc == 2)
	{
		cout<<"Error: Missing arguement for number of additional Waveforms from daterminePopularity.cc and the number of new Waveforms to be classified. In that order"<<endl;
		return 0;
	}
	else if(argc == 3)
	{
		cout<<"Error: Missing arguement for the number of new Waveforms to be classified."<<endl;
		return 0;
	}
	
	size_t nTraining; //the original number of waveforms from trainSom.cc
	size_t numOfWaveforms; //the additional waveforms
	size_t numOfClassifications; //the new data to be classified
	size_t numWeights = 200;

	nTraining = atoi(argv[1]);
	numOfWaveforms = atoi(argv[2]);
	numOfClassifications = atoi(argv[3]);
	
	double energy;
	double popularity;
	double emin = 0, pmin = 0, pmax = 0.01, emax = 0.7e6;
	
	GATHistoToVector h2v;
	h2v.SetfN(200);
	
	TH2D energyVsPopularity("energyVsPopularity", "", 15000, emin, emax, 100, pmin, pmax);

	TH1D he("he", "", 15000, emin, emax);
	TH1D hp("hp", "", 100, pmin, pmax);

	TH1D hb("hb", "", 15000, emin, emax);
	TH1D hs("hs", "", 15000, emin, emax);

	GATSOM *som;

	ifstream infile;
	infile.open("neuronPopularity.dat");
	
	som = new GATSOM();
	
	infile>>som;
	infile.close();

	ofstream outfile;
	outfile.open("energyAndPopularity.dat");

	char file[200], filename[500], calibrationfile[500];

	size_t startrun = 10000505;
	size_t endrun = 10000516;

	TChain *t = new TChain("MGTree");
	
	for(size_t k = startrun; k < endrun + 1; k++)
	{
		sprintf(file, "OR_run%d", k);
		sprintf(filename, "/home/neutrino/mjd/ortec/data/built/P42661B/%s.root", file);

		cout<<"added "<<filename<<endl;
		t->AddFile(filename);
	}
	
	MGTEvent *event = new MGTEvent();
	t->SetBranchAddress("event", &event);

	MGTWaveform *Wave = new MGTWaveform();
	MGWFBaselineRemover *base = new MGWFBaselineRemover();
	
	for(size_t i = (numOfWaveforms + nTraining); i < (numOfClassifications + numOfWaveforms + nTraining); i++)
	{
		t->GetEntry(i);

		Wave = event->GetWaveform(0);
		base->TransformInPlace(*Wave);
		energy = event->GetDigitizerData(0)->GetEnergy();

		TH1D* h = Wave->GimmeHist();
		GATNeuron* bmu = som->FindBMU(h2v.ConvertToVector(h, energy));

		char fileName[256];

		popularity = bmu->GetPopularity();

		outfile<<i<<" "<<"Energy: "<<energy<<" "<<"Popularity: "<<popularity<<" "<<endl;

		energyVsPopularity.Fill(energy, popularity);
		
		hp.Fill(popularity);
		he.Fill(energy);

		if(popularity > 0.0001)
		{
			hb.Fill(energy);
		}
		else
		{
			hs.Fill(energy);
		}
	}
	
	TCanvas c1, c2;

	c1.cd();
	energyVsPopularity.Draw("colz");

	c1.Print("energyVsPopularity.gif");
	c1.Print("energyVsPopularity.C");

	c2.cd();
	hb.SetLineColor(2);
	hs.SetLineColor(4);

	he.Draw();
	hb.Draw("same");
	hs.Draw("same");

	c2.Print("slice.gif");
	c2.Print("slice.C");

	outfile.close();
}
