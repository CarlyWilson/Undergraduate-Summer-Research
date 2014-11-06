//-----------------------------
//Name: trainSOM.cc
//
//Description: Takes N waveforms and trains a SOM to save to a file.
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
		cout<<"Error: Missing arguement for number of Waveforms"<<endl;
		return 0;
	}

	size_t nTraining;
	size_t numWeights = 200;

	double energy;

	GATHistoToVector h2v;
	h2v.SetfN(200);

	nTraining = atoi(argv[1]);

	GATSOM *som;

	vector<vector<double> > trainingData;

	vector<size_t> dimensions(2);
	dimensions[0] = 200;// nx
	dimensions[1] = 200;// ny
	
	char infile[200], infilename[500], calibrationfile[500];

  	size_t startrun = 10000496;
  	size_t endrun = 10000500;

	TChain *t = new TChain("MGTree");
	
	for(size_t i = startrun; i < endrun + 1; i++)
	{
    	sprintf(infile,"OR_run%d", i);
    	sprintf(infilename, "/home/neutrino/mjd/ortec/data/built/P42661B/%s.root", infile);   

    	cout<<"added "<<infilename<<endl;
    	t->AddFile(infilename);
  	}

	MGTEvent *event = new MGTEvent();
  	t->SetBranchAddress("event", &event);

	MGTWaveform *Wave = new MGTWaveform();
	MGWFBaselineRemover *base = new MGWFBaselineRemover();

	size_t nentries = t->GetEntries();

	for(size_t k = 0; k < nTraining; k++)
	{
		t->GetEntry(k);
		Wave = event->GetWaveform(0);
		base->TransformInPlace(*Wave);
		energy = event->GetDigitizerData(0)->GetEnergy();
		TH1D* h = Wave->GimmeHist();
		trainingData.push_back(h2v.ConvertToVector(h, energy));
	}

	som = new GATSOM(dimensions, numWeights);
	som->SetNEpochs(10000);
	som->SetInitialLearningRate(0.9);

	//som->PrintNetwork();
	som->TrainNetwork(trainingData);
	//som->PrintNetwork();

	cout<<"TrainingData is ";

	for(int p = 0; p < trainingData[0].size(); p++)
	{
		cout<<trainingData[0][p]<<" ";
	}

	cout<<endl;

	ofstream outfile;
	outfile.open("TrainedSOM.dat");
	outfile<<som;
	outfile.close();

	//TCanvas c1;
	//c1.Print("neuronwaveform.gif");
}

