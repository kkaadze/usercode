float btagSF(double x, int isb){

  //Using payloads for Moriond13
  //https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagPOG#2012_Data_and_MC_Moriond13_presc
  //https://twiki.cern.ch/twiki/pub/CMS/BtagPOG/SFb-pt_payload_Moriond13.txt
  float ptmin[16] = {20, 30, 40, 50, 60, 70, 80, 100, 120, 160, 210, 260, 320, 400, 500, 600};
  float ptmax[16] = {30, 40, 50, 60, 70, 80,100, 120, 160, 210, 260, 320, 400, 500, 600, 800};

  // Tagger: CSVL within 20 < pt < 800 GeV, abs(eta) < 2.4, x = pt
  float SFb = 0.981149*((1.+(-0.000713295*x))/(1.+(-0.000703264*x)));
  float SFb_error[] = {
    0.0484285,
    0.0126178,
    0.0120027,
    0.0141137,
    0.0145441,
    0.0131145,
    0.0168479,
    0.0160836,
    0.0126209,
    0.0136017,
    0.019182,
    0.0198805,
    0.0386531,
    0.0392831,
    0.0481008,
    0.0474291 };
  
  double SFunc = 0.;
  for (int i=0; i<15; ++i){
    if (x > ptmin[i] && x < ptmax[i+1])
      SFunc = SFb_error[i];
  }
  
  double unc = 0;
  
  if (isb) 
    unc = SFunc;
  else 
    unc = 2*SFunc;

  return SFb;
  //  return SFb-unc;
}
