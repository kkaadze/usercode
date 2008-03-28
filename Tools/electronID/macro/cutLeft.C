
TH2D* cutLeft(TString name, TH1F* sig, TH1F* bkg) {
  
  // nBins, xMin, xMax need to coincide ones in macro.C
  int nBins = 200; 
  double xMin = -0.5;
  double xMax = 5;  

  double binSize = (xMax-xMin)/nBins;
  if ( nBins != sig->GetNbinsX() || nBins != bkg->GetNbinsX() ) {
    cout << "Please update the code! Wrong nbins provided" << endl;
    return;
  }
  //========================================

  double maxEntry = -1;
  int binMaxEntry = -1;
  for ( int i=0; i<nBins; ++i ) {
    if ( sig->GetBinContent(i) < maxEntry ) continue;
    maxEntry = sig->GetBinContent(i);
    binMaxEntry = i;
  }

  double sumSig = sig->Integral();
  double sumBkg = bkg->Integral();

  TH2D* perf = new TH2D(name,"",1000,0,1,1000,0,1);
  
  int nPoints = nBins;
  double left = xMin;
  double right = xMax;
  
  double* cuts = new double[nPoints+1];
  double* significance = new double[nPoints+1];

  for (int i=0; i <= nPoints; ++i) {
    double cut = left + ((right - left)/nPoints)*i;
    int ibin = (int)((cut - xMin)/binSize);
    //    int ibin_left = (int)((-0.07 -(-1))/binSize);
    int ibin_left = 1;
    double sigRatio = sig->Integral(ibin_left,ibin)/sumSig;
    double bkgRatio = bkg->Integral(ibin_left,ibin)/sumBkg;
    // FILE* f = fopen(name+".txt","w");    
    if ( sigRatio > 0.8 && bkgRatio<0.9)
      cout << cut << " " << ibin << " " << sigRatio << " " << bkgRatio << endl;
    cuts[i] = cut;
    if ( sigRatio > 0.001 )
      significance[i] = 1./sqrt(1  + bkgRatio/sigRatio);
    else
      significance[i] = 1./sqrt(2);

    perf->Fill(sigRatio, bkgRatio);
  }
  perf->SetMarkerStyle(21);
  perf->SetMarkerColor(2);
  perf->GetXaxis()->SetTitle("signal efficiency");
  perf->GetYaxis()->SetTitle("background efficiency");
  perf->Draw();

  TGraph* graph = new TGraph(nPoints+1, cuts, significance);
  //graph->Draw("APL");

  TFile* out = TFile::Open(name + "_eff.root", "RECREATE");
  perf->Write();
  graph->Write();

  return perf;
}
    
