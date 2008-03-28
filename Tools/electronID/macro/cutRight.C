
void cutRight(TString name, TH1F* sig, TH1F* bkg) {
  
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

  for (int i=0; i <= nPoints; ++i) {
    double cut = left + ((right - left)/nPoints)*i;
    int ibin = (int)((cut - xMin)/binSize);
    //    double sigRatio = sig->Integral(1,ibin)/sumSig;
    //    double bkgRatio = bkg->Integral(1,ibin)/sumBkg;
    double sigRatio = sig->Integral(ibin,nBins)/sumSig;
    double bkgRatio = bkg->Integral(ibin,nBins)/sumBkg;
    if ( sigRatio > 0.9 && bkgRatio < 0.9 )
      cout << cut << " " << ibin << " " << sigRatio << " " << bkgRatio << endl;
    perf->Fill(sigRatio, bkgRatio);
  }
  perf->SetMarkerStyle(21);
  perf->SetMarkerColor(2);
  perf->GetXaxis()->SetTitle("signal efficiency");
  perf->GetYaxis()->SetTitle("background efficiency");
  perf->Draw();
  TFile* out = TFile::Open(name + "_eff.root", "RECREATE");
  perf->Write();
  return perf;
}
    
