/**
 * \file EcalDigiDumperModule.h 
 * dummy module  for the test of  DaqFileInputService
 *   
 * 
 * $Date: 2007/11/20 10:58:02 $
 * $Revision: 1.19 $
 * \author: 
 *
 */

#ifndef ECALRAWTODIGI_ECALDIGIDUMPER_h
#define ECALRAWTODIGI_ECALDIGIDUMPER_h

#include <memory>
#include <vector>
#include <string>

#include <FWCore/Framework/interface/EDAnalyzer.h>
#include <FWCore/Framework/interface/Event.h>
#include <FWCore/Framework/interface/MakerMacros.h>
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"

// class declaration

class EcalDigiDumperModule : public edm::EDAnalyzer {
 public:
  //Constractor
  EcalDigiDumperModule(const edm::ParameterSet& ps);
  //Distractor
  ~EcalDigiDumperModule();
  
 protected:
  virtual void analyze( const edm::Event & e, const  edm::EventSetup& c);
  virtual void beginJob(const edm::EventSetup& c);
  virtual void endJob();
  
 private:
  void readEBDigis (edm::Handle<EBDigiCollection> digis, int Mode);
  void readEEDigis (edm::Handle<EEDigiCollection> digis, int Mode);
  void readPNDigis (edm::Handle<EcalPnDiodeDigiCollection> PNs, int Mode);
  

  std::string ebDigiCollection_;
  std::string eeDigiCollection_;
  std::string digiProducer_;

  bool inputIsOk;
  bool tpDigi;
  bool cryDigi;
  bool pnDigi;
    
  //Mode is set 1 or 2
  int mode;
 
  //For Mode 1
  int numChannel;
  int numPN;

  //For Mode 2
  std::vector<int> listChannels;
  std::vector<int> listTowers;
  std::vector<int> listPns;
  
  //std::vector<int> realFeds;
  std::vector<int> requestedFeds;

};
#endif
