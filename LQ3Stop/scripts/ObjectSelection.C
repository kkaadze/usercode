ObjectSelection(){
  
  //==================================================================
  // ====== Vertex selection
  //==================================================================

  bool goodVertex = true;
  goodVertex &= !(VertexIsFake->at(ivtx));
  goodVertex &= VertexNDF->at(v) > 4;
  goodVertex &= TMath::Abs(VertexZ->at(v)) < 24;
  goodVertex &= TMath::Abs(VertexRho->at(v)) < 2;
  
  // muons and taus (electrons and taus) must come fromt the same vertex

  //==================================================================
  //  =====MuTau Channel
  //==================================================================
  //  ------Trigger selection
  HLTLine = "HLT_IsoMu24";
  
  //loop over trigger names
  bool goodTrigger = false;
  for(int h = 0; h < HLTInsideDatasetTriggerNames->size(); h++){
    //check:
    //1) if the line matches the current inside dataset line
    //2) if the decision was true (the line fired)
    //3) if the line was not prescaled
    if(HLTInsideDatasetTriggerNames->at(h).compare(0,HLTLine.size(),HLTLine)==0 && HLTInsideDatasetTriggerDecisions->at(h) && HLTInsideDatasetTriggerPrescales->at(h)==1) {
      goodTrigger = true;
      break;
    }
  }
	
  //  -----Muon Selection
    
  bool goodMuon = true;
  goodMuon &= (MuonPt->at(m)>30); //pT cut in GeV
  goodMuon &= TMath::Abs(MuonEta->at(m)) < 2.1; // eta cut
  goodMuon &= (MuonIsPF->at(m)==1); //require PF muon
  goodMuon &= (MuonPassID->at(m)==1); //require global, chi2 < 10, chamber hits
  goodMuon &= (MuonStationMatches->at(m)>=1); //require station hits
  goodMuon &= (fabs(MuonBestTrackVtxDistXY->at(m))<0.2); //require impact parameter < 0.2
  goodMuon &= (fabs(MuonBestTrackVtxDistZ->at(m))<0.5); //require longitudinal distance of track < 5 mm = .5 cm
  goodMuon &= (MuonPixelHits->at(m)>0); //require pixel hits
  goodMuon &= (MuonTrackLayersWithMeasurement->at(m)>5); //require 5+ tracker layers with hits
  goodMuon &= ((MuonPFIsoR04ChargedHadron->at(m) + max(MuonPFIsoR04Photon->at(m) + MuonPFIsoR04NeutralHadron->at(m) - 0.5*MuonPFIsoR04PU->at(m),0.))/MuonPt->at(m)<0.12); //require tight (<0.12) relative PF isolation
  
  
  //  ----- Tau Selection
  bool goodTau = true;
  goodTau &= (HPSTauPt->at(t)>30); //require pT > 30 GeV
  goodTau &= (fabs(HPSTauEta->at(t))<2.3); //require eta < 2.3
  //  goodTau &= (HPSTauLooseCombinedIsolationDeltaBetaCorrDiscr->at(t)==1); //require loose isolation
  goodTau &= (HPSTaubyLooseCombinedIsolationDeltaBetaCorr3Hits->at(t)==1); //require loose 3Hit isolation
  goodTau &= (HPSTauAgainstElectronLooseDiscr->at(t)==1); //require loose electron-tau discrimination
  goodTau &= (HPSTauagainstMuonTight2->at(t)==1); //require tight (version 2) muon-tau discrimination
  
  //
  bool vsep = (fabs(HPSTauVertexZ->at(goodTaus[t]) - VertexZ->at(MuonVtxIndex->at(theGoodMuon))) < 0.2); //require same vertex


  //==================================================================
  //====== ElTau channel
  //==================================================================

  // ------ Trigger selection
  // Matthieu's method
  /*
  bool LQAnalysis::triggerDecision(size_t ihlt, string ref) {
    
    if(!_au->simpleCut<size_t>(_vc->getS("HLTInsideDatasetTriggerNames",ihlt).find(ref),-1,"!=") ) return false;
    if(!_au->simpleCut<int>(_vc->getI("HLTInsideDatasetTriggerPrescales", ihlt),1,"=")) return false;
    if(!_au->simpleCut(_vc->getB("HLTInsideDatasetTriggerDecisions",ihlt) ) ) return false;
    return true;
  }
  
  bool LQAnalysis::passHLT() {
    
    bool pass=false;    
    for(size_t ih=0;ih<_vc->getSize("HLTInsideDatasetTriggerNames");ih++) {
      pass = triggerDecision(ih, "HLT_Ele27_WP80");
      if(pass) return true;
    }
    return false;
  }
  */

  
  HLTLine = "HLT_Ele27_WP80";
  
  //loop over trigger names
  bool goodTrigger = false;
  for(int h = 0; h < HLTInsideDatasetTriggerNames->size(); h++){
    //check:
    //1) if the line matches the current inside dataset line
    //2) if the decision was true (the line fired)
    //3) if the line was not prescaled
    if(HLTInsideDatasetTriggerNames->at(h).compare(0,HLTLine.size(),HLTLine)==0 && HLTInsideDatasetTriggerDecisions->at(h) && HLTInsideDatasetTriggerPrescales->at(h)==1) {
      goodTrigger = true;
      break;
    }
  }


  // ------ Electron Selection
  bool goodEle = true;
  goodEle &= (ElectronPt->at(e)>30);
  goodEle &= (fabs(ElectronEta->at(e))<1.444||fabs(ElectronEta->at(e))>1.56&&fabs(ElectronEta->at(e))<2.5);  //
  goodEle &= (ElectronPassEGammaIDMedium->at(e)==1023); //1023 is full ID/isolation, 895 is fullID and anti-iso

    //  ----- Tau Selection
  bool goodTau = true;
  goodTau &= (HPSTauPt->at(t)>30); //require pT > 30 GeV
  goodTau &= (fabs(HPSTauEta->at(t))<2.3); //require eta < 2.3
  //  goodTau &= (HPSTauLooseCombinedIsolationDeltaBetaCorrDiscr->at(t)==1); //require loose isolation
  goodTau &= (HPSTaubyLooseCombinedIsolationDeltaBetaCorr3Hits->at(t)==1); //require loose 3Hit isolation
  goodTau &= (HPSTauAgainstElectronMVADiscr->at(t)==1); //require loose electron-tau discrimination
  goodTau &= (HPSTauagainstMuonLoose2->at(t)==1); //require tight (version 2) muon-tau discrimination
  

  //==================================================================
  //===== Jet Selection -- common between channels
  //==================================================================
  bool goodJet = true;
  goodJet &= (PFJetPt->at(j)>30); //pT cut
  goodJet &= (fabs(PFJetEta->at(j))<2.4); //eta cut
  goodJet &= PFJetPassLooseID->at(j); //loose ID
  double dRm = DeltaR(MuonPhi->at(theGoodMuon),MuonEta->at(theGoodMuon),PFJetPhi->at(j),PFJetEta->at(j));
  goodJet &= (dRm > 0.5);
  double dRe = DeltaR(ElectronPhi->at(theGoodElectron),ElectronEta->at(theGoodElectron),PFJetPhi->at(j),PFJetEta->at(j));
  goodJet &= (dRe >0.5);
  double dRt = DeltaR(HPSTauPhi->at(theGoodTau),HPSTauEta->at(theGoodTau),PFJetPhi->at(j),PFJetEta->at(j));
  goodJet &= (dRt > 0.5);
  
  //theGoodMuon and theGoodTau are indeces of selected muon and taus 
  //Similarly needs to be done for ElTau channel
  
  bool btagJet = true;
  btagJet &= goodJet;
  btagJet &= PFJetCombinedSecondaryVertexBTag->at(ijet) > 0.244; // pass CSVL
 
  //------- Veto additional muons (electrons) for muTau (eTau) channel that have 
  //------- opposite charge from already selected muon (electron)
  //------- This will be documented after Kevins' study
  
}
