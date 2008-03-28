void plotEfficiency() {

  TCut PtCut="Pt>10";
  TCut EtaCut ="abs(Eta)<2.5";
  TCut Signal ="MatchedGen && MatchedGenFromWZ";
  TCut Bkg ="(!MatchedGen || (MatchedGen && !MatchedGenFromWZ ))";
  TCut barr = "abs(Eta)<1.479";
  TCut endc = "abs(Eta)>1.550&&abs(Eta)<2.5";
  TCut qcd ="(processID==11 || processID==12 || processID==13 || processID==28 || processID==53 || processID==68)";

  TFile *_file2 = TFile::Open("Data/Gumbo/New/Gumbo_All.root"); 
  TTree* g = _file2->Get("ElTree");
  TFile *_file1 = TFile::Open("Data/WZ_3l/New/WZCSA07-WZ.root");        
  TTree* s = _file1->Get("ElTree");

  TH1D* h1 = new TH1D("h1","",100,0,100);
  TH1D* h2 = new TH1D("h2","",100,0,100);
  TH1D* sig = new TH1D("sig","",100,0,100);
  h1->Sumw2();
  h2->Sumw2();

  TH1D* b1 = new TH1D("b1","",100,0,100);
  TH1D* b2 = new TH1D("b2","",100,0,100);
  b1->Sumw2();
  b2->Sumw2();
  TH1D* bkg = new TH1D("bkg","",100,0,100);

  TCut sig_preselection = PtCut && Signal && barr;
  TCut bkg_preselection = PtCut && Bkg && qcd && barr;

  // Barrel- Loose Id
  TCut eb_deta = "DeltaEtaVtx <  0.009"; 
  TCut eb_dphi = "DeltaPhiVtx < 0.05";  
  TCut eb_sEE = "sqrt(CovEtaEta)<0.012";
  TCut eb_trkIso = "(IsoKFTrackDR030 - IsoKFTrackDR005)/Pt<0.1";
  TCut eb_EseedPout = "ESeed/Trk_P_Out>0.9";
  // additional cut for Tight criteria
  TCut eb_EmHadIso = "(IsoEmCaloRecHitDR030 + IsoHadCaloRecHitDR030 - ESC)/ESC<0.18";   

  
  TCut eb_id = eb_deta && eb_dphi && eb_sEE && eb_trkIso && eb_EseedPout; // && eb_EmHadIso; 

  s->Draw("Pt>>h1", sig_preselection);
  s->Draw("Pt>>h2", sig_preselection && eb_id);

  g->Draw("Pt>>b1", "weight"*(bkg_preselection));
  g->Draw("Pt>>b2", "weight"*(bkg_preselection && eb_id));

  sig->Divide(h2, h1, 1, 1, "B");
  bkg->Divide(b2, b1, 1, 1, "B");

  sig->SetMarkerColor(2);
  sig->SetMarkerStyle(21);
  sig->SetMarkerSize(1);

  bkg->SetMarkerColor(4);
  bkg->SetMarkerStyle(22);
  bkg->SetMarkerSize(1);
  
  gStyle->SetOptStat(0);
  sig->GetXaxis()->SetTitle("P_{T} (GeV)");
  sig->GetYaxis()->SetTitle("Efficiency");
  sig->Draw("E");
  bkg->Draw("SAMEE");

  TLegend* leg = new TLegend(0.5,0.5,0.8,0.8);
  leg->AddEntry(sig, "Signal electron");
  leg->AddEntry(bkg, "mis-identified jet (Gumbo)");
  leg->SetLineColor(0);
  leg->SetBorderSize(0);
  leg->Draw();
}
