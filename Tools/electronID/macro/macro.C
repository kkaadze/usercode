void macro(TString name){
  //  gStyle->SetOptStat(11111111);
  gStyle->SetOptStat(0);

  //nBins, xmax, xmin need to vary according which variables you are plotting
  int nBins = 200;
  float xmin = -0.5; 
  float xmax = 5; 
  
  TCut PtCut="Pt>10";
  TCut EtaCut ="abs(Eta)<2.5";
  TCut Signal ="MatchedGen && MatchedGenFromWZ";
  TCut Bkg ="(!MatchedGen || (MatchedGen && !MatchedGenFromWZ ))";
  TCut Bkg_fromB = "MatchedGenFromB";
  
  // qcd processes
  TCut qcd ="(processID==11 || processID==12 || processID==13 || processID==28 || processID==53 || processID==68)";
  // TCut qcd = "processID==14 || processID==18 || processID==29";   // <==== these are photon+jets
  
  TCut barr = "abs(Eta)<1.479";
  TCut endc = "abs(Eta)>1.550&&abs(Eta)<2.5";

  //=========== Robust eID variables
  //   TString varToPlot = "DeltaEtaVtx";
  //   TString varToPlot = "DeltaPhiVtx";
  //   TString varToPlot = "sqrt(CovEtaEta)";  
  //   TString varToPlot = "HadOEM";
  
  
  //==========  Loose eID variables
  //  TString varToPlot = "DeltaEtaVtx";
  //  TString varToPlot = "DeltaPhiVtx";
  //  TString varToPlot = "sqrt(CovEtaEta)";
  //  TString varToPlot = "(IsoKFTrackDR030 - IsoKFTrackDR005)/Pt";
  //  TString varToPlot = "ESeed/Trk_P_Out";

  // --------- this variables is for tight criteria.
  TString varToPlot = "(IsoEmCaloRecHitDR030 + IsoHadCaloRecHitDR030 - ESC)/ESC";


  //+++++++   This are old set of loose Id variables after sEE
  //  TString varToPlot = "IsoHadCaloRecHitDR030/(IsoEmCaloRecHitDR030+IsoHadCaloRecHitDR030)";  //very similar to H/E
  //  TString varToPlot = "ESeed/Trk_P_Out";
  //  TString varToPlot = "(IsoEmCaloRecHitDR035 - ESC)/ESC";
  //+++++++    


  // CUTS
  // Those are BARREL variables for Loose eID ====================================================================================
  TCut eb_deta = "DeltaEtaVtx <  0.009"; 
  TCut eb_dphi = "DeltaPhiVtx < 0.05";  
  TCut eb_sEE = "sqrt(CovEtaEta)<0.012";
  TCut eb_trkIso = "(IsoKFTrackDR030 - IsoKFTrackDR005)/Pt<0.1"; 
  TCut eb_EseedPout = "ESeed/Trk_P_Out>0.9";
  //----------- this is for tight criteria
  TCut eb_EmHadIso = "(IsoEmCaloRecHitDR030 + IsoHadCaloRecHitDR030 - ESC)/ESC<0.18";     // a la d0 variable
  
  //+++++ These are the cuts for old set of loose Id vareables afetr sEE
  //  TCut eb_HadFrac = "IsoHadCaloRecHitDR030/(IsoEmCaloRecHitDR030+IsoHadCaloRecHitDR030)<0.1";
  //  TCut eb_EseedPout = "ESeed/Trk_P_Out>0.9";
  //  TCut eb_EmIso = "(IsoEmCaloRecHitDR035 - ESC)/ESC<0.3"; //0.2"; 
  //+++++
  //  TCut eb_deta_dphi = "(0.009*DeltaPhiVtx+0.05*DeltaEtaVtx)<(0.05*0.009)";  //<== triangular cut
  
  // Those are ENDCAP variables for loose eID ====================================================================================
  TCut ee_deta = "DeltaEtaVtx < 0.007"; 
  TCut ee_dphi = "DeltaPhiVtx < 0.05"; 
  TCut ee_sEE = "sqrt(CovEtaEta)<0.026";
  TCut ee_trkIso = "(IsoKFTrackDR030 - IsoKFTrackDR005)/Pt<0.2"; 
  TCut ee_EseedPout = "ESeed/Trk_P_Out>0.9"; //0.96";
  //--------- this is for tight criteria
  TCut ee_EmHadIso = "(IsoEmCaloRecHitDR030 + IsoHadCaloRecHitDR030 - ESC)/ESC<0.05";    // a la d0 variable

  //++++ 
  //  TCut ee_HadFrac = "IsoHadCaloRecHitDR030/(IsoEmCaloRecHitDR030+IsoHadCaloRecHitDR030)<0.12";
  //  TCut ee_EseedPout = "ESeed/Trk_P_Out>0.96";
  //  TCut ee_EmIso = "(IsoEmCaloRecHitDR035 - ESC)/ESC<0.12";
  //++++ 
  
  // Make plots
  //======================================================================================================================
  
  TCut sig_pres = PtCut && Signal && endc;                  // <--- need to vary barr/endc
  TCut bkg_pres = PtCut && Bkg && qcd && endc;              // <--- need to vary barr/endc
  TCut bkg_pres_chowder = PtCut && Bkg_fromB && barr;       // <--- need to vary barr/endc
  
  TCut eb_eId = eb_deta&&eb_dphi&&eb_sEE&&eb_trkIso&&eb_EseedPout;  //&&eb_EmHadIso; 
  // eb_deta_dphi&&eb_sEE&&eb_HadFrac&&eb_EseedPout&&eb_EmIso;    

  TCut ee_eId = ee_deta&&ee_dphi&&ee_sEE&&ee_trkIso&&ee_EseedPout;  //&&ee_EmHadIso;
  // ee_deta&&ee_dphi&&ee_sEE&&ee_HadFrac&&ee_EseedPout&&ee_EmIso;

  TFile *_file1 = TFile::Open("Data/WZ_3l/New/WZCSA07-WZ.root");           
  TH1F *h1 = new TH1F("h1","",nBins,xmin,xmax);
  ElTree->Draw(varToPlot+">>h1",sig_pres&&ee_eId,""); 
  
  TFile *_file2 = TFile::Open("Data/Gumbo/New/Gumbo_All.root"); 
  TH1F *h2 = new TH1F("h2","",nBins,xmin,xmax);
  ElTree->Draw( varToPlot+">>h2","weight"*(bkg_pres&&ee_eId),"SAME");


  h1->SetNormFactor(1.0);
  h2->SetNormFactor(1.0);
  //  h1->Scale(1.0/h1->Integral());
  //  h2->Scale(1.0/h2->Integral());
    
  h1->SetLineColor(4);
  h1->SetLineWidth(2);
  h2->SetLineColor(2);
  h1->GetXaxis()->SetTitle(name);
  h1->Draw();
  TLegend* leg1 = new TLegend(0.5,0.5,0.8,0.8);
  leg1->AddEntry(h1, "electrons");
  leg1->AddEntry(h2, "mis-id jet");

  // To plot WZ, Gumbo, Chowder overlayed
  /*
  TFile *_file3 = TFile::Open("/data/kkaadze/work/CastorFiles/Chowder/chowder_1/Chowder_add_1-2.root"); 
  TH1F *h4 = new TH1F("h4","",nBins,xmin,xmax);  
  ElTree->Draw( varToPlot+">>h4","weight"*(bkg_pres_chowder&&eb_eId),"SAME");
  h4->SetLineColor(8);
  h4->SetLineWidth(2);
  h4->SetNormFactor(1.0);
  */

  TH1F* h3 = new TH1F("h3", "Bkg", nBins, xmin, xmax);
  h3->Add(h2);
  h3->SetLineColor(2);
  h3->SetLineWidth(2);
  h3->SetName(name);

  h3->Draw("SAMES");
  TLegend* leg = new TLegend(0.5,0.5,0.8,0.8);
  leg->AddEntry(h1, "electrons (WZ)");
  leg->AddEntry(h3, "mis-id jet (Gumbo)");
  //  leg->AddEntry(h4, "el from b (Chowder)");
  leg->Draw("SAME");
  
  // TArrow *ar = new TArrow(0.009,0.01,0.009,0.03,0.05,"|>");
  // ar->Draw("SAME");
  
  TFile* tmp = TFile::Open(name + ".root", "RECREATE");
  h1->Write();
  h2->Write();
  leg1->Write();
}


//+++++++++++++++++++++++++++++++++++++++++++++++
/// Some extra variables which I have been studied
  //  TString varToPlot = "(IsoHadCaloRecHitDR035 - ESC)/ESC";
  //  TString varToPlot = "(IsoEmCaloRecHitDR035 - ESC)/ESC";
  //  TString varToPlot = "(IsoEmCaloDR035 - ESC)/ESC";
  //  TString varToPlot = "IsoHadCaloRecHitDR030/(IsoEmCaloRecHitDR030+IsoHadCaloRecHitDR030)";

  //  TString varToPlot = "(Trk_P_Vtx - Trk_P_Out)/Trk_P_Vtx";
  //  TString varToPlot = "SC_PhiWidth";
  
  //  TString varToPlot = "(IsoTrackDR030 - GSFTPt)/Pt";
  //  TString varToPlot = "(IsoKFTrackDR030 - GSFTPt)/Pt";
  //  TString varToPlot = "(IsoTrackDR030 - Pt)/Pt";
  //  TString varToPlot = "NbKFTracksDR040-NbKFTracksDR005";    

  //  TString varToPlot = "Energy/E5x5";
  //  TString varToPlot = "E5x5/Energy";
  //  TString varToPlot = "E3x3/E5x5";
  //  TString varToPlot = "EoP";
    
  //  TString etaRes = "(DeltaEtaVtx/(0.0006+0.0001*Eta*Eta*Eta*Eta))";
  //  TString etaRes2 = etaRes + "*" + etaRes;
  //  TString phiRes = "(DeltaPhiVtx/(0.0015+0.0013*Eta*Eta))";
  //  TString phiRes2 = phiRes + "*" + phiRes;
  //  TString varToPlot = etaRes+"+"+phiRes;


//+++++++++++++++++++++++++++++++++++++++++++++++
/// The cuts for the categirization of electrons for loose eID ( according Jim Brenson and others)
/// http://indico.cern.ch/materialDisplay.py?contribId=1&materialId=slides&confId=18239
  //TCut Cat0_barr = "abs(Eta)<1.479&&(Trk_P_Vtx-Trk_P_Out)/Trk_P_Vtx>=0.06&&EoP<1.2&&EoP>0.8";
  //TCut Cat0_endc = "abs(Eta)>1.479&&(Trk_P_Vtx-Trk_P_Out)/Trk_P_Vtx>=0.10&&EoP<1.2&&EoP>0.8";
  //TCut Cat1_barr = "abs(Eta)<1.479&&(Trk_P_Vtx-Trk_P_Out)/Trk_P_Vtx<0.06";
  //TCut Cat1_endc = "abs(Eta)>1.479&&(Trk_P_Vtx-Trk_P_Out)/Trk_P_Vtx<0.1";
  //TCut Cat2_barr = "abs(Eta)<1.479&&(Trk_P_Vtx-Trk_P_Out)/Trk_P_Vtx>=0.06&&(EoP>=1.2||EoP<=0.8)";
  //TCut Cat2_endc = "abs(Eta)>1.479&&(Trk_P_Vtx-Trk_P_Out)/Trk_P_Vtx>=0.10&&(EoP>=1.2||EoP<=0.8)";
