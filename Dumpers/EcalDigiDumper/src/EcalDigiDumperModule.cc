/**
 * \file EcalDigiDumperModule.h 
 * dummy module  for the test of  DaqFileInputService
 *   
 * 
 * $Date: 2007/10/20 10:58:02 $
 * $Revision: 1.19 $
 * \author N. Amapane - S. Argiro'
 * \author G. Franzoni
 *
 */

#include <FWCore/Framework/interface/EDAnalyzer.h>
#include <FWCore/Framework/interface/Event.h>
#include <FWCore/Framework/interface/MakerMacros.h>
#include <FWCore/Framework/interface/ESHandle.h>
#include <FWCore/MessageLogger/interface/MessageLogger.h>

#include "Dumpers/EcalDigiDumper/interface/EcalDigiDumperModule.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "DataFormats/EcalDetId/interface/EcalDetIdCollections.h"
#include "DataFormats/EcalRawData/interface/EcalRawDataCollections.h"
#include "DataFormats/EcalRawData/interface/EcalDCCHeaderBlock.h"

#include "Geometry/EcalMapping/interface/EcalElectronicsMapping.h"

#include "DataFormats/EcalDigi/interface/EcalTriggerPrimitiveDigi.h"
#include "DataFormats/EcalDigi/interface/EcalTriggerPrimitiveSample.h"

#include <iostream>
#include <vector>
#include <set>

//==========================================================================
EcalDigiDumperModule::EcalDigiDumperModule(const edm::ParameterSet& ps) {
//=========================================================================
  //For cfg file  
  //ebDigiCollection = ebDigis;
  //eeDigiCollection = eeDigis;
  // digiProducer   = ecalEBunpacker;
  
  ebDigiCollection_ = ps.getParameter<std::string>("ebDigiCollection");
  eeDigiCollection_ = ps.getParameter<std::string>("eeDigiCollection");
  digiProducer_   = ps.getParameter<std::string>("digiProducer");
  
  cryDigi         = ps.getUntrackedParameter<bool>("cryDigi",true);
  pnDigi         = ps.getUntrackedParameter<bool>("pnDigi",true);
  tpDigi         = ps.getUntrackedParameter<bool>("tpDigi",true);
  
  mode           = ps.getUntrackedParameter<int>("mode");
  numChannel     = ps.getUntrackedParameter<int>("numChannel");
  numPN          = ps.getUntrackedParameter<int>("numPN");
  
  listChannels   = ps.getUntrackedParameter<std::vector<int> >("listChannels");
  listTowers     = ps.getUntrackedParameter<std::vector<int> >("listTowers");
  listPns        = ps.getUntrackedParameter<std::vector<int> >("listPns");
  
  //realFeds       = ps.getUntrackedParameter<std::vector<int> >("orderedFedList");
  requestedFeds  = ps.getUntrackedParameter<std::vector<int> >("requestedFeds");
}

//=========================================================================
EcalDigiDumperModule::~EcalDigiDumperModule() {
//=========================================================================
  //delete "SOMETHING";
}
    
//========================================================================
void EcalDigiDumperModule::beginJob(const edm::EventSetup& c) {
//========================================================================
  edm::LogInfo("EcalDigiDumperModule") << "entering beginJob! ";
}

//========================================================================
void EcalDigiDumperModule::analyze( const edm::Event & e, const  edm::EventSetup& c) {
  //========================================================================

  inputIsOk = true;
  // consistency checks checks
  std::vector<int>::iterator intIter;
  //Check wih FEDS
  for (intIter = requestedFeds.begin(); intIter!=requestedFeds.end(); ++intIter ) {
    if ( (*intIter) < 601 || (*intIter) > 654) {
      std::cout << "[EcalDigiDumperModule] FED id: "<<(*intIter)<<"found in listFeds."
	        << "Valid range is [601-654]. Returning. " << std::endl;
      return;
    }
  }
  // Check with channels 
  for (intIter = listChannels.begin(); intIter != listChannels.end(); intIter++)  {  
    if ( ((*intIter) < 1) ||  (1700 < (*intIter)) )       {  
      std::cout << "[EcalDigiDumperModule] ic value: " << (*intIter) << " found in listChannels. "
		<< " Valid range is 1-1700. Returning." << std::endl;
      inputIsOk = false;
      return;
    }
  }
  //Check with Towers
  for (intIter = listTowers.begin(); intIter != listTowers.end(); intIter++) {
    if ( ((*intIter) < 1) ||  (70 < (*intIter)) )       {  
      std::cout << "[EcalDigiDumperModule] ic value: " << (*intIter) << " found in listTowers. "
		<< " Valid range is 1-70. Returning." << std::endl;
      inputIsOk = false;
	return;
    }
  }
  //Check with Pns
  for (intIter = listPns.begin(); intIter != listPns.end(); intIter++) {
    if ( ((*intIter) < 1) ||  (10 < (*intIter)) )       {  
      std::cout << "[EcalDigiDumperModule] pn number : " << (*intIter) << " found in listPns. "
		<< " Valid range is 1-10. Returning." << std::endl;
      inputIsOk = false;
      return;
    }
  }
  if (!inputIsOk) return;
  
  //Get DCC headers
  edm::Handle<EcalRawDataCollection> dccHeader;
  try {
    e.getByLabel(digiProducer_,dccHeader);
  } catch (cms::Exception& ex) {
    edm::LogError("EcalDigiUnpackerModule") << "Can't get DCC Headers!";
  } 

  //
  bool ebDigisFound = true;
  bool eeDigisFound = true;
  bool pnDigisFound = true;
  // retrieving crystal data from Event
  edm::Handle<EBDigiCollection>  eb_digis;    
  try {
    e.getByLabel(digiProducer_,ebDigiCollection_, eb_digis);
  } catch (cms::Exception& ex) {
    edm::LogError("EcalDigiUnpackerModule") << "EB Digis were not found!";
    ebDigisFound = false;
  }
  
  //
  edm::Handle<EEDigiCollection>  ee_digis;    
  try {
    e.getByLabel(digiProducer_,eeDigiCollection_, ee_digis);
  } catch (cms::Exception& ex) {
    edm::LogError("EcalDigiUnpackerModule") << "EE Digis were not found!";
    eeDigisFound = false;
  }
  
  // retrieving crystal PN diodes from Event
  edm::Handle<EcalPnDiodeDigiCollection>  pn_digis;
  try {
    e.getByLabel(digiProducer_, pn_digis);
  } catch (cms::Exception& ex) {
    edm::LogError("EcalDigiUnpackerModule") << "PNs were not found!";
    pnDigisFound = false;
  }
  
  if ( cryDigi ) {
    if ( ebDigisFound )
      readEBDigis(eb_digis, mode);
    if ( eeDigisFound )
      readEEDigis(ee_digis, mode);
    if ( !(ebDigisFound || eeDigisFound) ) {
      std::cout << "No Digis were found! Returning.." << std::endl;
      return;
    }
  }
  
  if ( pnDigi ) {
    if (pnDigisFound )
      readPNDigis(pn_digis, mode);
  }
}

///////////////////////////////////
// FUNCTIONS
//////////////////////////////////

void EcalDigiDumperModule::readEBDigis (edm::Handle<EBDigiCollection> digis, int Mode) {

  EcalElectronicsMapping* theMapping    = new EcalElectronicsMapping();
  int dumpDigiCounter = 0;
  
  for ( EBDigiCollection::const_iterator digiItr= digis->begin();digiItr != digis->end(); 
	++digiItr ) {		
    //Make sure that digis are form right SM
    EBDetId detId = EBDetId((*digiItr).id());
    EcalElectronicsId elecId = theMapping->getElectronicsId(detId);

    int FEDid = elecId.dccId() + 600;
    std::vector<int>::iterator fedIter = find(requestedFeds.begin(), requestedFeds.end(), FEDid); 
    if (fedIter ==  requestedFeds.end()) continue;
    
    //If we are here the digis are from given SM
    
    //Check if Mode is set 1 or 2 
    //Mode 1: it will dump digis from all channels up to channel #numChannel
    //Mode 2: it will dump digis from certain channels given in listChannel
    if ( Mode ==1 ) {
      std::cout << "\n\n^^^^^^^^^^^^^^^^^^ [EcalDigiDumperModule]  digi cry collection size " << digis->size() << std::endl;
      std::cout << "                       [EcalDigiDumperModule]  dumping first " << numChannel << " crystals\n";
      //It will break if all required digis are dumpped
      if( (dumpDigiCounter++) >= numChannel) break;     
    } else if  ( Mode==2 ) {
      int ic = EBDetId((*digiItr).id()).ic();
      int tt = EBDetId((*digiItr).id()).tower().iTT();
      
      std::vector<int>::iterator icIterCh;
      std::vector<int>::iterator icIterTt;
      icIterCh = find(listChannels.begin(), listChannels.end(), ic);
      icIterTt = find(listTowers.begin(), listTowers.end(), tt);
      if (icIterCh == listChannels.end()  ) continue;   
      std::cout << "\n\n^^^^^^^^^^^^^^^^^^ [EcalDigiDumperModule]  digi cry collection size " << digis->size() << std::endl;      
      //&& icIterTt == listTowers.end()
    } else {
      std::cout << "[EcalDigiDumperModule] parameter mode set to: " << Mode
		<< ". Only mode 1 and 2 are allowed. Returning..." << std::endl;
      inputIsOk = false;
      return;
    }
    
    std::cout << "FEDID: " << FEDid << std::endl;
    std::cout << "ic-cry: " 
	      << EBDetId((*digiItr).id()).ic() << " i-phi: " 
	      << EBDetId((*digiItr).id()).iphi() << " j-eta: " 
	      << EBDetId((*digiItr).id()).ieta() << std::endl;
    //Get Samples
    for ( unsigned int i=0; i< (*digiItr).size() ; ++i ) {
      EBDataFrame df( *digiItr );
      if (!(i%3)  )  std::cout << "\n\t";
      std::cout << "sId: " << (i+1) << " " <<  df.sample(i) << "\t";
    }       
    std::cout << " " << std::endl;
  }
  delete theMapping;
}

//Function for EE Digis
void EcalDigiDumperModule::readEEDigis (edm::Handle<EEDigiCollection> digis, int Mode) {

  //I think for Endcap only  Mode 2 is reasonable
  if ( Mode!=2 ) {
    std::cout << "For Endcap mode needs to be set to 2" << std::endl;
    return;
  }
  
  EcalElectronicsMapping* theMapping   = new EcalElectronicsMapping(); 
  
  for ( EEDigiCollection::const_iterator digiItr= digis->begin();digiItr != digis->end(); 
	++digiItr ) {		
    
    //Make sure that digis are form requested place
    EEDetId detId = EEDetId((*digiItr).id());
    EcalElectronicsId elecId = theMapping->getElectronicsId(detId);

    int FEDid = elecId.dccId() + 600;
    std::vector<int>::iterator fedIter = find(requestedFeds.begin(), requestedFeds.end(), FEDid);
    if (fedIter ==  requestedFeds.end()) continue;

    std::cout << "\n\n^^^^^^^^^^^^^^^^^^ [EcalDigiDumperModule]  digi cry collection size " << digis->size() << std::endl;
    
    int crystalId = 10000 * FEDid + 100 * elecId.towerId() + 5 * (elecId.stripId()-1)+elecId.xtalId();
    
    std::vector<int>::iterator icIterCh;
    icIterCh = find(listChannels.begin(), listChannels.end(), crystalId);
    if ( icIterCh == listChannels.end()) continue; 
    
    std::cout << "FEDID: " << FEDid << std::endl;
    std::cout << "crystalId: " 
	      <<  crystalId << " i-x: " 
	      << EEDetId((*digiItr).id()).ix() << " j-y: " 
	      << EEDetId((*digiItr).id()).iy() << std::endl;
    
    //Get samples 
    for ( unsigned int i=0; i< (*digiItr).size() ; ++i ) {
      EEDataFrame df( *digiItr );
      if (!(i%3)  )  std::cout << "\n\t";
      std::cout << "sId: " << (i+1) << " " <<  df.sample(i) << "\t";
    }       
    std::cout << " " << std::endl;
  }
  delete theMapping;
}

void EcalDigiDumperModule::readPNDigis(edm::Handle<EcalPnDiodeDigiCollection> PNs, int Mode ) {

  int pnDigiCounter = 0;
  
  //Loop over PN digis
  for ( EcalPnDiodeDigiCollection::const_iterator pnItr = PNs->begin(); pnItr != PNs->end(); ++pnItr ) {
    EcalPnDiodeDetId pnDetId = EcalPnDiodeDetId((*pnItr).id());
    //Make sure that we look at the requested place
    int FEDid = pnDetId.iDCCId() + 600;
    std::vector<int>::iterator fedIter = find(requestedFeds.begin(), requestedFeds.end(), FEDid);
    if (fedIter ==  requestedFeds.end()) continue;
    int pnNum = (*pnItr).id().iPnId();
    
    //If Mode is 1
      if ( Mode == 1) {
	std::cout << "\n\n^^^^^^^^^^^^^^^^^^ EcalDigiDumperModule  digi PN collection.  Size: " << PNs->size() << std::endl;
	std::cout << "                                  [EcalDigiDumperModule]  dumping first " << numPN << " PNs ";
	
	if ( (pnDigiCounter++) >= numPN ) break;
      } else if ( Mode == 2) {
	std::cout << "\n\n^^^^^^^^^^^^^^^^^^ EcalDigiDumperModule  digi PN collection.  Size: " << PNs->size() << std::endl;
	
	// Check that we look at PN from the given list
	std::vector<int>::iterator pnIter;
	pnIter = find(listPns.begin(), listPns.end(), pnNum);
	if (pnIter == listPns.end())  continue; 
      } else {
	std::cout << "[EcalDigiDumperModule] parameter mode set to: " << Mode
		  << ". Only mode 1 and 2 are allowed. Returning..." << std::endl;
	inputIsOk = false;
	return;
      }
      
      std::cout << "DCCID: " << pnDetId.iDCCId() << std::endl;
      std::cout << "\nPN num: " << (*pnItr).id().iPnId();
      for ( int samId=0; samId < (*pnItr).size() ; samId++ ) {
	if (!(samId%3)  )  std::cout << "\n\t";
	std::cout <<  "sId: " << (samId+1) << " "
		  << (*pnItr).sample(samId) 
		  << "\t";
      }
  }
}
//===================================================
void EcalDigiDumperModule::endJob() {
//==================================================
  edm::LogInfo("EcalDigiDumperModule") << "DONE!.... " ;
}

 //     // retrieving crystal TP from the Event
 //     edm::Handle<EcalTrigPrimDigiCollection>  primitives;
 //     e.getByLabel("ecalEBunpacker", primitives);

 //     if (verbosity>0 && tpDigi)
 //       {
 // 	std::cout << "\n\n^^^^^^^^^^^^^^^^^^ EcalDigiDumperModule  digi TP collection.  Size: " << primitives->size() << std::endl;
 // 	std::cout << "                                  [EcalDigiDumperModule]  dumping primitives "  << std::endl;
// 	for ( EcalTrigPrimDigiCollection::const_iterator TPtr = primitives->begin();
// 	      ( TPtr != primitives->end()  && (TPtr-primitives->begin())<4 ); 
// 		++TPtr ) {

// 	  if (!  ((EcalTrigTowerDetId((*TPtr).id()).iDCC()==ieb_id) || (ieb_id==-1))   ) continue;

// 	  std::cout << "[EcalDigiDumperModule] tower: " << ( (TPtr-primitives->begin()) +1) 
// 	       << "\n" << (*TPtr) << std::endl;
// 	}
//       }


//} // produce

//};// class EcalDigiDumperModule

DEFINE_FWK_MODULE(EcalDigiDumperModule);

