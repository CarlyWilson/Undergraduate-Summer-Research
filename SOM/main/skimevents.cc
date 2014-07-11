#include <TH1D.h>
#include <TChain.h>
#include <TCanvas.h>

#include <MGTWaveform.hh>
#include <MGTEvent.hh>
#include <MGWFBaselineRemover.hh>

#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
  
  char infile[200],infilename[500],calibrationfile[500];

  int startrun=20001921;
  int endrun=startrun;


  TChain *t = new TChain("MGTree");

  for(int i=startrun;i<endrun+1;i++){
    sprintf(infile,"OR_run%d",i);
    sprintf(infilename,"/home/neutrino/mjd/surfacc/data/built/P23517A/%s.root",infile);   

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

  int nentries=t->GetEntries();

  TH1D* hwave;
  TCanvas c1;
  double energy=0.;
  char title[100];


  nentries=10;
  for(int i=0;i<nentries;i++){
    t->GetEntry(i);
    
    //if(event->GetWaveforms()<1)continue;

    Wave=event->GetWaveform(0);
    base->TransformInPlace(*Wave);
    energy=event->GetDigitizerData(0)->GetEnergy();

    hwave=Wave->GimmeHist();   
    //trainingData.append(h2v.ConvertToVector(hwave,energy));

    for(int j=0;j<5;j++)cout<<hwave->GetBinContent(j)<<" ";
    cout<<endl;

    cout<<"at i = "<<i<<" of "<<nentries<<" "<<event->GetNWaveforms()<<" waveforms"<<endl;

  }
//put SOM code here

}



