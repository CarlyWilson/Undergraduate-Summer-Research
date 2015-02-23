#include <TH1F.h>
#include <TH1D.h>
#include <TChain.h>
#include <TCanvas.h>

#include <MGTWaveform.hh>
#include <MGTEvent.hh>
#include <MGWFBaselineRemover.hh>

#include "../GATHistoToVector.hh"
#include "../GATNeuron.hh"
#include "../GATSOM.hh"

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>

using namespace std;

const size_t nTraining = 5000;
const size_t numOfWaveforms = 20000;
const size_t numOfClassifications = 10000;


void trainAndSaveSOM(GATSOM* som, TChain* t, MGTWaveform* Wave, MGWFBaselineRemover* base, MGTEvent* event, GATHistoToVector h2v)
{
	vector<vector<double> > trainingData;

	vector<size_t> dimensions(2);

	dimensions[0] = 200;// nx
	dimensions[1] = 200;// ny
	
	size_t numWeights = 200;


	som = new GATSOM(dimensions, numWeights);
	som->SetNEpochs(10000);
	som->SetInitialLearningRate(0.9);


	h2v.SetfN(200);

	size_t nentries = t->GetEntries();

	for(size_t k = 0; k < nTraining; k++)
	{
		t->GetEntry(k);
    	
		Wave=event->GetWaveform(0);
    	base->TransformInPlace(*Wave);
    	double energy=event->GetDigitizerData(0)->GetEnergy();
    	
		TH1D* h = Wave->GimmeHist();   
		trainingData.push_back(h2v.ConvertToVector(h, som->GetDistCalcType(), energy));
	}

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

	TCanvas c1;
	c1.Print("neuronwaveform.gif");
}

void loadSOM(GATSOM* som, char* filename)
{
	vector<double> junk;
	vector<size_t> junky;
	size_t numJunk = 0;

	ifstream infile;
	infile.open(filename);
	som = new GATSOM(junky, numJunk);
	infile>>som;
	infile.close();
}

void determinePopularityOfNeurons(GATSOM* som, TChain* t, MGTWaveform* Wave, MGWFBaselineRemover* base, MGTEvent* event, GATHistoToVector h2v)
{
	vector<vector<double> > popularityContest;

	for(size_t l = nTraining; l < (numOfWaveforms + nTraining); l++)
	{
		t->GetEntry(l);
    	
		Wave=event->GetWaveform(0);
    	base->TransformInPlace(*Wave);
    	double energy=event->GetDigitizerData(0)->GetEnergy();
    	
		TH1D* h3 = Wave->GimmeHist();   
		popularityContest.push_back(h2v.ConvertToVector(h3, som->GetDistCalcType(), energy));
	}

	for(size_t q = 0; q < numOfWaveforms; q++)
	{
		GATNeuron* bmu = som->FindBMU(popularityContest[q]);
		bmu->IncreasePopularity(numOfWaveforms);
	}

	ofstream outfile;
	outfile.open("popularNeurons.dat");
	outfile<<som;
	outfile.close();
}

void classifyWaveformsByPopularity(GATSOM* som, TChain* t, MGTWaveform* Wave, MGWFBaselineRemover* base, MGTEvent* event, GATHistoToVector h2v)
{
	TH1D pop("pop", "Popular vs. Unpopular Energy", 200, 0, 20000);
	TH1D unPop("unpop", "Popular vs. Unpopular Energy",200, 0, 20000);

	ofstream outfile;
	outfile.open("popularityOfNeurons.dat");

int total = 0, pop1 = 0, unPop1 = 0;
	for(size_t b = (numOfWaveforms + nTraining); b < (numOfClassifications + numOfWaveforms + nTraining); b++)
	{
		t->GetEntry(b);
    	
		Wave = event->GetWaveform(0);
    	base->TransformInPlace(*Wave);
    	double energy = event->GetDigitizerData(0)->GetEnergy();
    	
		TH1D* h4 = Wave->GimmeHist();
		vector<double> converted = h2v.ConvertToVector(h4, som->GetDistCalcType(), energy);
		GATNeuron* bmu = som->FindBMU(converted);

		char fileName[256];

		double popularity = bmu->GetPopularity();
		cout<<h2v.ConvertToVector(h4, som->GetDistCalcType(), energy)[180]<<endl;
	cout<<" energy "<<energy<<" h4 "<<h4->GetMean()<<" Pop "<<popularity<<endl;
	cout << bmu->GetPosition()[0] << " " << bmu->GetPosition()[1] << endl;
		if (b % 200 == 100)
			exit(0);

		outfile<<b<<" "<<popularity<<" "<<energy<<endl;
		
		h4->GetYaxis()->SetRangeUser(-0.0002, 0.0012);
		//h4->Draw();
total++;
		if(popularity * 20000 > 20)
		{	
			//snprintf(fileName, sizeof(fileName), "popular/popular%d.gif", b+100000);
			//c2.Print(fileName);
			pop.Fill(energy);
			pop1++;
		}
		else
		{
			//snprintf(fileName, sizeof(fileName), "notPopular/notPopular%d.gif", b+100000);
			//c2.Print(fileName);
			unPop.Fill(energy);
			unPop1++;
		}
	}

	TCanvas c2;
	unPop.SetLineColor(2);
	pop.Draw("same");
	unPop.Draw("same");
	c2.Print("POPvsUNPOP.gif");
cout<<"Total "<<total<<" pop "<<pop1<<" unPop "<<unPop1<<endl;
	outfile.close();
}

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

	size_t next = 1, maxBin, T50Bin;
  	double maxHeight, norm = 1;
  	char title[100];

	size_t n = 10;
	size_t nBins = 100;
	vector<double> v100(nBins);

	TH1D** h1 = new TH1D* [n];
	TH1D h100("h100", "", nBins, 0, nBins);

  	/*for(size_t i = 0; i < n; i++)
	{
    	t->GetEntry(i);
    
    	//if(event->GetWaveforms()<1)continue;

    	Wave=event->GetWaveform(0);
    	base->TransformInPlace(*Wave);
    	energy=event->GetDigitizerData(0)->GetEnergy();// write energies out

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
  	}*/

	GATSOM* som;
	GATHistoToVector h2v;

	// trainAndSaveSOM(som ,t, Wave, base, event, h2v);
	// determinePopularityOfNeurons(som, t, Wave, base, event, h2v);
	loadSOM(som, "popularNeurons.dat");
	classifyWaveformsByPopularity(som, t, Wave, base, event, h2v);

}



