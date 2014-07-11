#include <TH1F.h>
#include <TH1D.h> //class needs this
#include <TCanvas.h>
#include <TChain.h>
#include <MGTWaveform.hh>
#include <MGTEvent.hh>
#include <MGTBaselineRemover.hh>
#include <iostream>


int main(int argc, char* argv[])
{
//file and start run end run can later be made into arguments
	char infile[200],infilename[500],calibrationfile[500];
	size_t startrun = 10000496;
	size_t endrun=startrun;

	TChain *t = new TChain("MGTree");

	for(size_t i=startrun;i<endrun+1;i++)
	{
		sprintf(infile, "OR_run%d", i);
   		sprintf(infilename, "/home/neutrino/mjd/ortec/data/built/P42661B/%s.root", infile);
   		cout<<"added "<<infilename<<endl;
   		t->AddFile(infilename);
	}

	MGTEvent* event = new MGTEvent();
	t->SetBranchAddress("event",&event);

	MGTWaveform* Wave = new MGTWaveform();

	MGWFBaselineRemover* base = new MGWFBaselineRemover();
	base->SetBaselineSamples(0);
	base->SetStartSample(0);
	base->SetBaselineTime(3000);
	base->SetStartTime(0.);

	size_t nentries=t->GetEntries();
  
	TCanvas c1;
	size_t next = 1, maxBin, T50Bin;
	double energy=0.0, maxHeight, norm = 1;
	char title[100];

	size_t n = 10;
	size_t nBins = 100;
	TH1F** h1 = new TH1F* [n];
	TH1F h100("h100", "", nBins, 0, nBins);
	vector<double> v100(nBins);

	for(size_t i = 0; i < n; i++)
	{
		t->GetEntry(i);

//		if(event->GetWaveforms()<1)continue; unlikley to happen?

    	Wave=event->GetWaveform(0);
    	base->TransformInPlace(*Wave);
    	energy=event->GetDigitizerData(0)->GetEnergy();

		h1[i] = Wave->GimmeHist();
//trainingData.append(h2v.ConvertToVector(h1[i], energy));
    	sprintf(title,"%.0lf ADC",energy);


    	cout<<"at i = "<<i<<" of "<<nentries<<" "<<event->GetNWaveforms()<<" waveforms"<<endl;


		h100->SetLineWidth(3);

		h100->SetLineColor(i % 9 + 1);
				
		maxHeight = h1[i]->GetMaximum();
  		maxBin = h1[i]->GetMaximumBin();
  		T50Bin = maxBin;

  		while(h1[T50Bin] > 0.5 * maxHeight && maxBin > 0)
		{
   			maxBin--;
  		}

  		cout<<"The maximum is "<<maxHeight<<" at "<<h1[i]->GetBinCenter(maxBin)<<" T50 is at "<<h1[i]->GetBinCenter(T50Bin)<<endl;
		
		for(size_t j = 0; j < nBins; j++);
		{
				v100[j] = h1[i]->GetBinContent(T50Bin - nBins/2 + j);
				h100->SetBinContent(j, v100[j]);
		}

		if(i == 0) h100->Draw();//be careful if T50Bin<50 or > hwave->GetNbinsX()
		else h100->Draw("same");

        cout<<"energy "<<energy<<" filter energy "<<filterenergy<<endl;
		
		c1.Update();

        cin>>next;
		if(next<1)break;
        }
}

