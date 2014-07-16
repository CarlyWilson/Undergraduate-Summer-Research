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

using namespace std;

int main(int argc, char* argv[])
{
	char infile[200], infilename[500], calibrationfile[500];

  	size_t startrun = 10000496;
  	size_t endrun = startrun;


  	TChain *t = new TChain("MGTree");

  	for(size_t i = startrun; i < endrun + 1; i++)
	{
    	sprintf(infile,"OR_run%d", i);
    	sprintf(infilename, "/home/neutrino/mjd/ortec/data/built/P42661B/%s.root", infile);   

    	cout<<"added "<<infilename<<endl;
    	t->AddFile(infilename);
  	}

  	MGTEvent* event = new MGTEvent();
  	t->SetBranchAddress("event", &event);

  	MGTWaveform* Wave = new MGTWaveform();
 
  	MGWFBaselineRemover* base = new MGWFBaselineRemover();
  	base->SetBaselineSamples(0);
  	base->SetStartSample(0);
  	base->SetBaselineTime(3000);
  	base->SetStartTime(0.);

  	size_t nentries = t->GetEntries();

  	TCanvas c1;
	size_t next = 1, maxBin, T50Bin;
  	double energy = 0.0, maxHeight, norm = 1;
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

	vector<vector<double> > trainingData;

	HistoToVector h2v;
	h2v.SetfN(200);
	size_t nTraining = 100;

	nentries = t->GetEntries();
	for(int k = 0; k < nTraining; k++)
	{
		t->GetEntry(k);
    
    	Wave=event->GetWaveform(0);
    	base->TransformInPlace(*Wave);
    	energy=event->GetDigitizerData(0)->GetEnergy();

    	TH1D* h = Wave->GimmeHist();   
		trainingData.push_back(h2v.ConvertToVector(h, energy));
	}

	vector<size_t> dimensions(2);

	dimensions[0] = 100;// nx
	dimensions[1] = 100;// ny

	size_t numWeights = 200;

	SOM* som = new SOM(dimensions, numWeights);
	som->SetNEpochs(10000);
	som->SetInitialLearningRate(0.9);

	//som->PrintNetwork();
	som->TrainNetwork(trainingData);
	//som->PrintNetwork();

	Neuron* bmu = som->FindBMU(trainingData[0]);
	cout<<bmu;
	cout<<"TrainingData is ";
	for(int p = 0; p < trainingData[0].size(); p++)
	{
		cout<<trainingData[0][p]<<" ";
	}
	cout<<endl;
	
	TH1D plot = som->PlotNeuron(rand()%som->fNeurons.size());
	plot.Draw();
	c1.Print("neuronwaveform.gif");
}



