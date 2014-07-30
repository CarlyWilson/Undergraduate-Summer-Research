//----------------------------------
//
// Name: popularNeuronPlots.cc
//
// Description: loads a trained som and uses it to print various neurons to a graph of popularity.
//
// Author: Carly Jo Wilson
//
//----------------------------------

#include <TH1F.h>
#include <TH2F.h>
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

	infile.open("popularNeurons.dat");
	SOM *s = new SOM(junky, numJunk);
	infile>>s;
	infile.close();

	TCanvas c1;
	TH2F *hcont4 = new TH2F("hcont4", "Popularity of Neurons", 200, -10, 200, 200, -10, 210);

	for(int i = 0; i < 20000; i++)
	{
		for(int j = 0; j < (s->fNeurons[i]->GetPopularity())*20000; j++)
		{
			hcont4->Fill(i/200, i%200);
		}
	}

	//gStyle->SetPalette(1);
	hcont4->Draw("CONT4Z");
	c1.Print("popularNeurons.gif");
}
	
