#include <TH1F.h>
#include <TH1D.h>
#include <TChain.h>
#include <TCanvas.h>

#include <MGTWaveform.hh>
#include <MGTEvent.hh>
#include <MGWFBaselineRemover.hh>

#include "../HistoToVector.hh"
#include "../Neuron.hh"
#include "../SOM.hh"

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>

using namespace std;

TChain *t = NULL;
SOM* som = NULL; 
MGTWaveform* Wave = NULL;
MGWFBaselineRemover* base = NULL; 
Neuron* bmu = NULL;
MGTEvent* event = NULL;

TCanvas c1;
HistoToVector h2v;

size_t nentries;
double energy = 0.0;

size_t nTraining = 5000;
double numOfWaveforms = 20000;
double numOfClassifications = 10000;


void trainAndSaveSOM()
{
	vector<vector<double> > trainingData;

	h2v.SetfN(200);

	nentries = t->GetEntries();

	for(size_t k = 0; k < nTraining; k++)
	{
		t->GetEntry(k);
    	
		Wave=event->GetWaveform(0);
    	base->TransformInPlace(*Wave);
    	energy=event->GetDigitizerData(0)->GetEnergy();
    	
		TH1D* h = Wave->GimmeHist();   
		trainingData.push_back(h2v.ConvertToVector(h, energy));
	}

	vector<size_t> dimensions(2);

	dimensions[0] = 200;// nx
	dimensions[1] = 200;// ny
	
	size_t numWeights = 200;

	som = new SOM(dimensions, numWeights);
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
	outfile.open("40000NeuronNetwork.dat");
	outfile<<som;
	outfile.close();

	c1.Print("neuronwaveform.gif");
}

void loadSOM(char* filename)
{
	vector<double> junk;
	vector<size_t> junky;
	size_t numJunk = 0;

	ifstream infile;
	infile.open(filename);
	som = new SOM(junky, numJunk);
	infile>>som;
	infile.close();
}

void determinePopularityOfNeurons()
{
	vector<vector<double> > popularityContest;

	for(size_t l = nTraining; l < (numOfWaveforms + nTraining); l++)
	{
		t->GetEntry(l);
    	
		Wave=event->GetWaveform(0);
    	base->TransformInPlace(*Wave);
    	energy=event->GetDigitizerData(0)->GetEnergy();
    	
		TH1D* h3 = Wave->GimmeHist();   
		popularityContest.push_back(h2v.ConvertToVector(h3, energy));
	}

	for(size_t q = 0; q < numOfWaveforms; q++)
	{
		bmu = som->FindBMU(popularityContest[q]);
		bmu->IncreasePopularity(numOfWaveforms);
	}

	ofstream outfile;
	outfile.open("popularNeurons.dat");
	outfile<<som;
	outfile.close();
}

void classifyWaveformsByPopularity()
{
	for(size_t b = (numOfWaveforms + nTraining); b < (numOfClassifications + numOfWaveforms + nTraining); b++)
	{
		t->GetEntry(b);
    	
		Wave=event->GetWaveform(0);
    	base->TransformInPlace(*Wave);
    	double energy=event->GetDigitizerData(0)->GetEnergy();
    	
		TH1D* h4 = Wave->GimmeHist();   
		bmu = som->FindBMU(h2v.ConvertToVector(h4, energy));

		TCanvas c2;
		char fileName[256];

		double popularity = bmu->GetPopularity();
		
		h4->GetYaxis()->SetRangeUser(-0.0002, 0.0012);
		h4->Draw();

		if(popularity * 20000 > 20)
		{	
			snprintf(fileName, sizeof(fileName), "popular/popular%d.gif", b+100000);
			c2.Print(fileName);
		}
		else
		{
			snprintf(fileName, sizeof(fileName), "notPopular/notPopular%d.gif", b+100000);
			c2.Print(fileName);
		}
	}
}

int main(int argc, char* argv[])
{
	char infile[200], infilename[500], calibrationfile[500];

  	size_t startrun = 10000496;
  	size_t endrun = startrun;


  	t = new TChain("MGTree");

  	for(size_t i = startrun; i < endrun + 1; i++)
	{
    	sprintf(infile,"OR_run%d", i);
    	sprintf(infilename, "/home/neutrino/mjd/ortec/data/built/P42661B/%s.root", infile);   

    	cout<<"added "<<infilename<<endl;
    	t->AddFile(infilename);
  	}

  	event = new MGTEvent();
  	t->SetBranchAddress("event", &event);

  	Wave = new MGTWaveform();
 
  	base = new MGWFBaselineRemover();
  	base->SetBaselineSamples(0);
  	base->SetStartSample(0);
  	base->SetBaselineTime(3000);
  	base->SetStartTime(0.);

  	nentries = t->GetEntries();

	size_t next = 1, maxBin, T50Bin;
  	double maxHeight, norm = 1;
  	char title[100];

	size_t n = 10;
	size_t nBins = 100;
	vector<double> v100(nBins);

	TH1D** h1 = new TH1D* [n];
	TH1D h100("h100", "", nBins, 0, nBins);

  	for(size_t i = 0; i < n; i++)
	{
    	t->GetEntry(i);
    
    	//if(event->GetWaveforms()<1)continue;

    	Wave=event->GetWaveform(0);
    	base->TransformInPlace(*Wave);
    	energy=event->GetDigitizerData(0)->GetEnergy();

    	h1[i] = Wave->GimmeHist();   

    	//trainingData.append(h2v.ConvertToVector(h1[i],energy));

		sprintf(title, "%.01f ADC", energy);

    	cout<<"at i = "<<i<<" of "<<nentries<<" "<<event->GetNWaveforms()<<" waveforms"<<endl;

		h100.SetLineWidth(3);

		h100.SetLineColor(i % 9 + 1);

		double fPeakHeightFrac = 0.5;
		maxHeight = h1[i]->GetMaximum();
		double halfMaxHeight = fPeakHeightFrac*maxHeight;
		maxBin = h1[i]->GetMaximumBin();
		T50Bin = maxBin;


		while(h1[i]->GetBinContent(T50Bin) > halfMaxHeight && T50Bin > 0)
		{
			T50Bin--;
		}	

	//	cout<<"The maximum is "<<maxHeight<<" at "<<h1[i]->GetBinCenter(maxBin)<<" T50 is at "<<h1[i]->GetBinCenter(T50Bin)<<"T50 is "<<h1[i]->GetBinContent(T50Bin)<<endl;

		for(size_t j = 0; j < nBins; j++)
		{
			v100[j] = h1[i]->GetBinContent(T50Bin - nBins/2 + j);
			h100.SetBinContent(j, v100[j]);
		}

		if(i == 0) h100.Draw(); //Be careful is T50Bin<50 or T50Bin > h1[i]->GetNbinsX()
		else h100.Draw("same");

		c1.Update();
		cin>>next;
		if(next < 1) break; 
  	}


	// trainAndSaveSOM();
	// determinePopularityOfNeurons();
	loadSOM("popularNeurons.dat");
	classifyWaveformsByPopularity();

}



