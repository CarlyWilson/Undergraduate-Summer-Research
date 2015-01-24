{
//=========Macro generated from canvas: c1_n2/c1_n2
//=========  (Thu Nov  6 10:31:11 2014) by ROOT version5.34/10
   TCanvas *c1_n2 = new TCanvas("c1_n2", "c1_n2",0,0,700,500);
   c1_n2->SetHighLightColor(2);
   c1_n2->Range(-1.250001e-05,-132.1688,0.0011125,1189.519);
   c1_n2->SetFillColor(0);
   c1_n2->SetBorderMode(0);
   c1_n2->SetBorderSize(2);
   c1_n2->SetFrameBorderMode(0);
   c1_n2->SetFrameBorderMode(0);
   
   TH1D *eBig = new TH1D("eBig","",15000,0.0001,0.001);
   eBig->SetBinContent(1,1007);
   eBig->SetBinContent(834,637);
   eBig->SetBinContent(1667,320);
   eBig->SetBinContent(2501,237);
   eBig->SetBinContent(3334,212);
   eBig->SetBinContent(4167,137);
   eBig->SetBinContent(5001,149);
   eBig->SetBinContent(5834,143);
   eBig->SetBinContent(6667,72);
   eBig->SetBinContent(7501,104);
   eBig->SetBinContent(8334,123);
   eBig->SetBinContent(9167,114);
   eBig->SetBinContent(10001,93);
   eBig->SetBinContent(10834,86);
   eBig->SetBinContent(11667,115);
   eBig->SetBinContent(12500,12);
   eBig->SetBinContent(13334,35);
   eBig->SetBinContent(14167,131);
   eBig->SetBinContent(15001,1001);
   eBig->SetEntries(4728);
   
   TPaveStats *ptstats = new TPaveStats(0.78,0.775,0.98,0.935,"brNDC");
   ptstats->SetName("stats");
   ptstats->SetBorderSize(1);
   ptstats->SetFillColor(0);
   ptstats->SetTextAlign(12);
   ptstats->SetTextFont(42);
   TText *text = ptstats->AddText("eBig");
   text->SetTextSize(0.0368);
   text = ptstats->AddText("Entries = 4728   ");
   text = ptstats->AddText("Mean  = 0.0003176");
   text = ptstats->AddText("RMS   = 0.0002481");
   ptstats->SetOptStat(1111);
   ptstats->SetOptFit(0);
   ptstats->Draw();
   eBig->GetListOfFunctions()->Add(ptstats);
   ptstats->SetParent(eBig);

   Int_t ci;   // for color index setting
   ci = TColor::GetColor("#000099");
   eBig->SetLineColor(ci);
   eBig->GetXaxis()->SetLabelFont(42);
   eBig->GetXaxis()->SetLabelSize(0.035);
   eBig->GetXaxis()->SetTitleSize(0.035);
   eBig->GetXaxis()->SetTitleFont(42);
   eBig->GetYaxis()->SetLabelFont(42);
   eBig->GetYaxis()->SetLabelSize(0.035);
   eBig->GetYaxis()->SetTitleSize(0.035);
   eBig->GetYaxis()->SetTitleFont(42);
   eBig->GetZaxis()->SetLabelFont(42);
   eBig->GetZaxis()->SetLabelSize(0.035);
   eBig->GetZaxis()->SetTitleSize(0.035);
   eBig->GetZaxis()->SetTitleFont(42);
   eBig->Draw("colz");
   
   TH1D *eSmall = new TH1D("eSmall","",15000,1e-08,0.0001);
   eSmall->SetBinContent(0,3447);
   eSmall->SetBinContent(7500,1825);
   eSmall->SetEntries(5272);
   eSmall->SetLineColor(2);
   eSmall->GetXaxis()->SetLabelFont(42);
   eSmall->GetXaxis()->SetLabelSize(0.035);
   eSmall->GetXaxis()->SetTitleSize(0.035);
   eSmall->GetXaxis()->SetTitleFont(42);
   eSmall->GetYaxis()->SetLabelFont(42);
   eSmall->GetYaxis()->SetLabelSize(0.035);
   eSmall->GetYaxis()->SetTitleSize(0.035);
   eSmall->GetYaxis()->SetTitleFont(42);
   eSmall->GetZaxis()->SetLabelFont(42);
   eSmall->GetZaxis()->SetLabelSize(0.035);
   eSmall->GetZaxis()->SetTitleSize(0.035);
   eSmall->GetZaxis()->SetTitleFont(42);
   eSmall->Draw("same");
   c1_n2->Modified();
   c1_n2->cd();
   c1_n2->SetSelected(c1_n2);
}
