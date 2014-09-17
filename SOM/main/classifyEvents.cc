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
	cout<<"hi"<<endl;
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
	cout<<"hi"<<endl;
	size_t nTraining; //the original number of waveforms from trainSom.cc
	size_t numOfWaveforms; //the additional waveforms
	size_t numOfClassifications; //the new data to be classified
	size_t numWeights = 200;

	nTraining = atoi(argv[1]);
	numOfWaveforms = atoi(argv[2]);
	numOfClassifications = atoi(argv[3]);
	cout<<"hi"<<endl;
	vector<size_t> placeHolder;
	vector<double> placeHolder2;

	size_t numHolder = 0;
	double energy;
	double popularity;
	cout<<"hi"<<endl;
	TH2D energyVsPopularity("eVsP", "Energy vs. Popularity", 2000, 0.0, 1000000.0, 2000, 0.00001, 0.003);
	cout<<"hi"<<endl;
	GATSOM *som;

	ifstream infile;
	infile.open("neuronPopularity.dat");
	
	som = new GATSOM(placeHolder, numHolder);
	cout<<"hi"<<endl;
	infile>>som;
	infile.close();

	ofstream outfile;
	outfile.open("energyAndPopularity.dat");

	char file[200], filename[500], calibrationfile[500];

	size_t startrun = 10000496;
	size_t endrun = startrun;

	TChain *t = new TChain("MGTree");
	cout<<"hi"<<endl;
	for(size_t k = startrun; k < endrun + 1; k++)
	{
		sprintf(file, "OR_run%d", k);
		sprintf(filename, "/home/neutrino/mjd/ortec/data/built/P42661B/%s.root", file);

		cout<<"added "<<filename<<endl;
		t->AddFile(filename);
	}
	cout<<"hi"<<endl;
	MGTEvent *event = new MGTEvent();
	t->SetBranchAddress("event", &event);

	MGTWaveform *Wave = new MGTWaveform();
	MGWFBaselineRemover *base = new MGWFBaselineRemover();

	GATHistoToVector h2v;
	h2v.SetfN(200);
	cout<<"hi"<<endl;
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
	}
	cout<<"hi"<<endl;
	TCanvas c1;
	energyVsPopularity.Draw();
	c1.Print("energyVsPopularity.gif");

	outfile.close();
}
