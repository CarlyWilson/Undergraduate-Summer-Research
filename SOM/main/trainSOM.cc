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

#include <time.h>

using namespace std;

double diffclock(timespec start, timespec finish)
{
	double elapsed = static_cast<double>(finish.tv_sec - start.tv_sec);
	elapsed += static_cast<double>(finish.tv_nsec - start.tv_nsec)/10000000000.0;
	return elapsed * (-1000);
}

int main(int argc, char* argv[])
{
	if(argc == 1)
	{
		cout<<"Error: Missing arguement for number of Waveforms"<<endl;
		return 0;
	}

	int t1 = time(NULL);

	size_t nTraining;
	size_t numWeights = 200;

	double energy;

	GATHistoToVector h2v;
	h2v.SetfN(200);

	nTraining = atoi(argv[1]);

	GATSOM *som;

	vector<vector<double> > trainingData;

	vector<size_t> dimensions(2);
	dimensions[0] = 50;// nx
	dimensions[1] = 50;// ny
	
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

	som = new GATSOM(dimensions, numWeights);
	som->SetNumEpochs(10000);
	som->SetInitialLearningRate(0.9);

	size_t nentries = t->GetEntries();
	for(size_t k = 0; k < nTraining; k++)
	{
		t->GetEntry(k);
		Wave = event->GetWaveform(0);
		base->TransformInPlace(*Wave);
		energy = event->GetDigitizerData(0)->GetEnergy();
		TH1D* h = Wave->GimmeHist();
		trainingData.push_back(h2v.ConvertToVector(h, som->GetDistCalcType(), energy));
	}
int t3 = time(NULL);
struct timespec start, end;
clock_gettime(CLOCK_MONOTONIC, &start);
	//som->PrintNetwork();
	som->TrainNetwork(trainingData, som->GetAlgorithmType());
	//som->PrintNetwork();
int t4 = time(NULL);
clock_gettime(CLOCK_MONOTONIC, &end);

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
	int t2 = time(NULL);

	cout<<"Entire program time "<<(t2-t1)<<endl;
	cout<<"TrainNetwork "<<(t4-t3)<<endl;
	cout<<"Milli "<<diffclock(end, start)<<endl;
}

