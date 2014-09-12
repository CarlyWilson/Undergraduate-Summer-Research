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

	vector<size_t> placeHolder;
	vector<double> placeHolder2;

	size_t numHolder = 0;
	double energy;
	double popularity;

	GATSOM *som;

	ifstream infile;
	infile.open("neuronPopularity.dat");
	
	som = new GATSOM(placeHolder, numHolder);

	infile>>som;
	infile.close();

	ofstream outfile;
	outfile.open("energyAndPopularity.dat");

	TChain *t = new TChain("MGTree");

	MGTEvent *event = new MGTEvent();
	MGTWaveform *Wave = new MGTWaveform();
	MGWFBaselineRemover *base = new MGWFBaselineRemover();

	GATHistoToVector h2v;
	h2v.SetfN(200);

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

		outfile<<i<<"Popularity: "<<popularity<<"Energy: "<<energy<<endl;

	}
	outfile.close();
}
