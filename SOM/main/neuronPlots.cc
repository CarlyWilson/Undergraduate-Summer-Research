//----------------------------------
//
// Name: neuronPLots.cc
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

#include "../HistoToVector.hh"
#include "../Neuron.hh"
#include "../SOM.hh"

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

	infile.open("trainedNetwork.dat");
	SOM *s = new SOM(junky, numJunk);
	infile>>s;
	infile.close();

	for(int i = 0; i < 10000; i++)
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
	}
}
