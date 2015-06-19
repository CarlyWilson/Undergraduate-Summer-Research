//----------------------------------
//
// Name: GATNeuronPLots.cc
//
// Description: loads a trained som and uses it to print various neurons to a graph.
//
// Author: Carly Jo Wilson
//
//----------------------------------

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

#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;

int main(int argc, char* argv[])
{
	ifstream infile;

	vector<double> junk;
	vector<size_t> junky;
	size_t numJunk = 0;

	//infile.open("trainedNetwork.dat");
	infile.open("TrainedSOMBatchOpt.dat");
	GATSOM *s = new GATSOM(junky, numJunk);
	infile>>s;
	infile.close();

	//int xpos=0; int ypos=0;
	//int index=200*xpos+ypos; 
	//cout<<neuron->GetPosition()[0]<<" "<<neuron->GetPosition()[1]<<endl;
	//this way you can check you got the right index....

	TCanvas c1;

	for(int j = 0; j < s->GetNeurons().size(); j++){
		GATNeuron* neuron=s->GetNeuron(j);
		vector<double> weights=neuron->GetWeight();
		TH1D* hn = new TH1D("hn","",weights.size(),0,weights.size());

		for(int i=0;i<weights.size();i++)
		{
			 hn->SetBinContent(i+1,weights[i]);
		}

		hn->Draw("same");
	}

	c1.Print("batchOpt.gif");
	c1.Print("batchOpt.C");

	/*for(size_t i = 0; i < 10000; i++)
	{
		TCanvas c1;
		TH1D plot1;

		plot1 = s->PlotNeuron(i);
		plot1.GetYaxis()->SetRangeUser(-0.0002, 0.0012);
		plot1.Draw();
		char fileName[256];
		//snprintf(fileName, sizeof(fileName), "neuronWaveforms/search%d.gif", i+100000);
		snprintf(fileName, sizeof(fileName), "columnsForGifs/column%drow%d.gif", (i%100) + 1000, (i/100) + 1000);
		//snprintf(fileName, sizeof(fileName), "neuron%d.gif", i);
		c1.Print(fileName);
	}*/
}
