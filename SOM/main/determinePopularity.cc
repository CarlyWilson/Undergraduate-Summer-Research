//-----------------------------
//Name: determinePopularity.cc
//
//Description: Loads a trained SOM and extra Waveforms then determines the popularity of neurons to save to a file.
//
//Author: Carly Wilson
//------------------------------

#include "../GATHistoToVector.hh"
#include "../GATNeuron.hh"
#include "../GATSOM.hh"

#include <TH1D.h>
#include <TChain.h>

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
		cout<<"Error: Missing arguement for original number of Waveformns from trainSom.cc and number of additional Waveforms in that order"<<endl;
		return 0;
	}
	else if(argc == 2)
	{
		cout<<"Error: Missing arguement for number of additional Waveforms"<<endl;
		return 0;
	}

	size_t nTraining; //the original number of waveforms from trainSom.cc
	size_t numOfWaveforms; //the additional waveforms
	size_t numWeights = 200;

	nTraining = atoi(argv[1]);
	numOfWaveforms = atoi(argv[2]);

	double energy;
	
	GATHistoToVector h2v;
	h2v.SetfN(200);

	GATSOM *som;

	ifstream infile;
	infile.open("TrainedSOM.dat");
	
	som = new GATSOM();

	infile>>som;
	infile.close();

	char file[200], filename[500], calibrationfile[500];

	size_t startrun = 10000501;
	size_t endrun = 10000504;

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

	vector<vector<double> > neuronPopularity;

	for(size_t i = nTraining; i < (numOfWaveforms + nTraining); i++)
	{
		t->GetEntry(i);

		Wave = event->GetWaveform(0);
		base->TransformInPlace(*Wave);
		energy = event->GetDigitizerData(0)->GetEnergy();

		TH1D* h = Wave->GimmeHist();
		neuronPopularity.push_back(h2v.ConvertToVector(h, energy));
	}

	for(size_t j = 0; j < numOfWaveforms; j++)
	{
		GATNeuron* bmu = som->FindBMU(neuronPopularity[j]);
		bmu->IncreasePopularity(numOfWaveforms);
	}

	ofstream outfile;
	outfile.open("neuronPopularity.dat");
	outfile<<som;
	outfile.close();
}


