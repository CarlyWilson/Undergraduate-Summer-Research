//----------------------------------
//
// Name: GATPopularNeuronPlots.cc
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

	infile.open("popularNeurons.dat");
	SOM *s = new SOM(junky, numJunk);
	infile>>s;
	infile.close();

	TCanvas c1;
	TH2F *hcont4 = new TH2F("hcont4", "Popularity of Neurons", 200, 0, 100, 200, 0, 200);
	TH1D h("h", "Popularity", 200, 0, 60); 

	for(size_t i = 0; i < 20000; i++)
	{
		for(size_t j = 0; j < (s->fNeurons[i]->GetPopularity())*20000; j++)
		{
			hcont4->Fill(i/200, i%200);
		}

		h.Fill((s->fNeurons[i]->GetPopularity())*20000);
	}
	c1.SetLogz();
	hcont4->Draw("CONT4Z");
	c1.Print("popularNeurons.gif");

	c1.SetLogy();
	h.Draw();
	c1.Print("popularNeuronsHisto.gif");
}
	
