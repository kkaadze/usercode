float mistagSF(double x, double y){
  //x= pT
  //y= eta
  //This function is only for CSVL taggger
  //Does not take into account syst+stat uncertainties on SF

  //Values are available only for jets with pT<800
  if (x>800) x=799;

  Float_t Etamin;
  Float_t Etamax;
  if (TMath::Abs(y)<0.5){
    Etamin = 0.0;
    Etamax = 0.5;
  }else if(TMath::Abs(y)>0.5&&TMath::Abs(y)<1.0){
    Etamin = 0.5;
    Etamax = 1.0;
  }else if(TMath::Abs(y)>1.0&&TMath::Abs(y)<1.5){
    Etamin = 1.0;
    Etamax = 1.5;
  }else if(TMath::Abs(y)>1.5&&TMath::Abs(y)<2.4){
    Etamin = 1.5;
    Etamax = 2.4;
  }    
  TString meanminmax = "mean";
  //  TF1 *tmpSFl = NULL;  
  TString Atagger = "CSVL"; //tagger+TaggerStrength;
  TString sEtamin = Form("%1.1f",Etamin);
  TString sEtamax = Form("%1.1f",Etamax);
  //  cout << sEtamin << endl;
  //  cout << sEtamax << endl;
  
  // Begin of definition of functions from SF_12ABCD ---------------


  float tmpSFl = 1.;

  if( sEtamin == "1.5" || sEtamin == "1.6" ) {
    if (x > 700)
      x = 699;
  }

  if( Atagger == "CSVL" && sEtamin == "0.0" && sEtamax == "0.5") {
    if( meanminmax == "mean" ) tmpSFl = ((1.04901+(0.00152181*x))+(-3.43568e-06*(x*x)))+(2.17219e-09*(x*(x*x)));
    if( meanminmax == "min" ) tmpSFl = ((0.973773+(0.00103049*x))+(-2.2277e-06*(x*x)))+(1.37208e-09*(x*(x*x)));
    if( meanminmax == "max" ) tmpSFl = ((1.12424+(0.00201136*x))+(-4.64021e-06*(x*x)))+(2.97219e-09*(x*(x*x)));
  }
  if( Atagger == "CSVL" && sEtamin == "0.5" && sEtamax == "1.0") {
    if( meanminmax == "mean" ) tmpSFl = ((0.991915+(0.00172552*x))+(-3.92652e-06*(x*x)))+(2.56816e-09*(x*(x*x)));
    if( meanminmax == "min" ) tmpSFl = ((0.921518+(0.00129098*x))+(-2.86488e-06*(x*x)))+(1.86022e-09*(x*(x*x)));
    if( meanminmax == "max" ) tmpSFl = ((1.06231+(0.00215815*x))+(-4.9844e-06*(x*x)))+(3.27623e-09*(x*(x*x)));
  }
  if( Atagger == "CSVL" && sEtamin == "1.0" && sEtamax == "1.5") {
    if( meanminmax == "mean" ) tmpSFl = ((0.962127+(0.00192796*x))+(-4.53385e-06*(x*x)))+(3.0605e-09*(x*(x*x)));
    if( meanminmax == "min" ) tmpSFl = ((0.895419+(0.00153387*x))+(-3.48409e-06*(x*x)))+(2.30899e-09*(x*(x*x)));
    if( meanminmax == "max" ) tmpSFl = ((1.02883+(0.00231985*x))+(-5.57924e-06*(x*x)))+(3.81235e-09*(x*(x*x)));
  }
  if( Atagger == "CSVL" && sEtamin == "1.5" && sEtamax == "2.4"){
    if( meanminmax == "mean" ) tmpSFl = ((1.06121+(0.000332747*x))+(-8.81201e-07*(x*x)))+(7.43896e-10*(x*(x*x)));
    if( meanminmax == "min" ) tmpSFl = ((0.983607+(0.000196747*x))+(-3.98327e-07*(x*x)))+(2.95764e-10*(x*(x*x)));
    if( meanminmax == "max" ) tmpSFl = ((1.1388+(0.000468418*x))+(-1.36341e-06*(x*x)))+(1.19256e-09*(x*(x*x)));
  }
   
 return tmpSFl;
}
