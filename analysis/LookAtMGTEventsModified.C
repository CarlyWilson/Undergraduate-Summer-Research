{
	gROOT->Reset();

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
	MGTWaveform* FilteredWave= new MGTWaveform();

	MGWFBaselineRemover* base = new MGWFBaselineRemover();
	base->SetBaselineSamples(0);
	base->SetStartSample(0);
	base->SetBaselineTime(3000);
	base->SetStartTime(0.);

	MGWFTrapSlopeFilter* filter= new MGWFTrapSlopeFilter();
	filter->SetIntegrationTime(200);
	filter->SetPeakingTime(10);
	filter->SetAverageTime(00);
	filter->SetDecayTime(0);
	filter->UseSum(false);
	filter->SetEvaluateMode(7);
	double filterscale=1.;//304./2.11568e+09;
	filter->OutputInternalParameter("s2");

	size_t nentries=t->GetEntries();
  
	TCanvas c1;
	size_t next = 1, maxBin, T50Bin;
	double energy=0.0, filterenergy, maxHeight, norm = 1;
	vector<double> v100(100);
	char title[100];

	size_t n = 10;
	TH1F** h1 = new TH1F* [n];
	TH1F h100("h100", "", -100, 0, 100);

	for(size_t i = 0; i < n; i++)
	{
		t->GetEntry(i);

		if(event->GetWaveforms()<1)continue;

    	Wave=event->GetWaveform(0);
    	base->TransformInPlace(*Wave);
    	energy=event->GetDigitizerData(0)->GetEnergy();
    	filter->TransformOutOfPlace(*Wave,*FilteredWave);

    	filterenergy=filter->GetMaximum();

		h1[i] = Wave->GimmeHist();
		h1[i]->SetName("wave");

    	sprintf(title,"%.0lf ADC",energy);

		h1[i]->SetTitle(title);

    	cout<<"at i = "<<i<<" of "<<nentries<<" "<<event->GetNWaveforms()<<" waveforms"<<endl;

		h1[i]->GetYaxis()->SetRangeUser(TMath::Min(h1[i]->GetMinimum(), 0.0), 1.05*TMath::Max(h1[i]->GetMaximum(), 0.0));
		h1[i]->SetTitle(title);

		//h1[i]->SetLineWidth(3);
		h100->SetLineWidth(3);

		//h1[i]->SetLineColor(i % 9 + 1);
		h100->SetLineColor(i % 9 + 1);
				
		h1[i]->Scale(norm/h1[i]->Integral());
		
		//if(i == 0) h1[i]->Draw();
		//else h1[i]->Draw("same");

		maxHeight = h1[i]->GetMaximum();
  		maxBin = h1[i]->GetMaximumBin();
  		T50Bin = maxBin;

  		while(h1[T50Bin] > 0.5 * maxHeight && maxBin > 0)
		{
   			maxBin--;
  		}

  		cout<<"The maximum is "<<maxHeight<<" at "<<h1[i]->GetBinCenter(maxBin)<<" T50 is at "<<h1[i]->GetBinCenter(T50Bin)<<endl;
		cout<<"T50Bin: "<<T50Bin<<endl;
		cout<<"maxBin: "<<maxBin<<endl;
		for(size_t j = 1; j < 101; j++);
		{
			h100->SetBinContent(j, h1[i]->GetBinContent(T50Bin - 50 + j));
			v100[j] = h1[i]->GetBinContent(T50Bin - 50 + j);
			h100->Fill(v100[j]);
		}

		h100->Scale(norm/h100->Integral());

		if(i == 0) h100->Draw();//be careful if T50Bin<50 or > hwave->GetNbinsX()
		else h100->Draw("same");

        cout<<"energy "<<energy<<" filter energy "<<filterenergy<<endl;
		
		c1.Update();

        cin>>next;
		if(next<1)break;
        }
}

