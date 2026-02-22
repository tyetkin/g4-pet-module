
#include "EventAction.hh"
#include "PhotonDetSD.hh"
#include "AnalysisManager.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4UnitsTable.hh"

#include "G4Track.hh"

// #include "Event.h"
#include "Randomize.hh"
#include "G4SystemOfUnits.hh"
#include <iomanip>
#include <vector>

using namespace std;

EventAction::EventAction(DetectorConstruction* det)
 : G4UserEventAction(),
   fDet(det),
   fPhoDetHCID(-999),
   fPrimDetHCID(-999)//,
   //fXtalBGOCollID (-999),
   //fXtalGAGGCollID (-999)
{}


EventAction::~EventAction()
{}

void EventAction::BeginOfEventAction(const G4Event*)
{
  G4cout << "EventAction::BeginOfEventAction()" << G4endl;
}

void EventAction::EndOfEventAction(const G4Event* evt)
{

  G4cout << "EventAction::EndOfEventAction()" << G4endl;
  
  // Get Hits from the photon detector if any
  G4SDManager * SDman = G4SDManager::GetSDMpointer();

  G4HCofThisEvent* HCE = evt->GetHCofThisEvent();
  
  PhotonDetHitsCollection* phoDetHC = 0;
  PrimDetHitsCollection* primDetHC = 0;
  //XtalBGOHitsCollection* xtalBGOHC = 0;  
  //XtalGAGGHitsCollection* xtalGAGGHC = 0;  


  G4String colName = "PhotonDetHitsCollection";
  fPhoDetHCID = SDman->GetCollectionID(colName);
  
  
  colName = "PrimDetHitsCollection";
  fPrimDetHCID = SDman->GetCollectionID(colName);
  /*
  colName = "XtalBGOHitsCollection";
  fXtalBGOCollID = SDman->GetCollectionID(colName);
  
  colName = "XtalGAGGHitsCollection";
  fXtalGAGGCollID = SDman->GetCollectionID(colName);
  */

  if (HCE){
    if (fPhoDetHCID>=0){
      phoDetHC = (PhotonDetHitsCollection*)(HCE->GetHC(fPhoDetHCID));
      if (phoDetHC)AnalysisManager::getInstance()->FillTree(evt->GetEventID(), phoDetHC);
    }
    
    if (fPrimDetHCID>=0){
      primDetHC = (PrimDetHitsCollection*)(HCE->GetHC(fPrimDetHCID));
      if (primDetHC)AnalysisManager::getInstance()->FillPrimDetTree(evt->GetEventID(), primDetHC);
    }
    
    /*
    if (fXtalBGOCollID>=0){
       xtalBGOHC = (XtalBGOHitsCollection*)(HCE->GetHC(fXtalBGOCollID));
    }
    if (fXtalGAGGCollID>=0){
       xtalGAGGHC = (XtalGAGGHitsCollection*)(HCE->GetHC(fXtalGAGGCollID));
    }
    */
  }
  /*
  if (xtalBGOHC){
    vector<float> fSpec;
    vector<float> fWeight;
    vector<float> fEnergy;
    vector<float> fTime;
    vector<float> fEnergyS;
    G4int n_hit = xtalBGOHC->entries();
    G4cout << "Nb. of XtalBGO Hits " << n_hit << G4endl;   
    for(G4int iHit=0; iHit<n_hit; iHit++){
      G4ThreeVector hitPos = (*xtalBGOHC)[iHit]->GetPosition()/cm;
      G4double zPoz = hitPos.z();
      G4double edep = (*xtalBGOHC)[iHit]->GetEnergy()/MeV;
      G4double enSpec = (*xtalBGOHC)[iHit]->GetEnergyS()/MeV;
      G4double ftime = (*xtalBGOHC)[iHit]->GetTime();
      //G4int XtalId = (*xtalBGOHC)[iHit]->GetID();
      //fXtalDetId.push_back(XtalId);
      fEnergy.push_back(edep);
      fTime.push_back(ftime);
      fEnergyS.push_back(enSpec);
      //vector<float> fSpec_loc = (*xtalBGOHC)[iHit]->GetESpectrum();
      //vector<float> fWeight_loc = (*xtalBGOHC)[iHit]->GetWeight();
      //for(G4int kk = 0; kk < G4int(fSpec_loc.size()); kk++){
        //fSpec.push_back(fSpec_loc[kk]);
        //fWeight.push_back(fWeight_loc[kk]);
      //}
      //G4cout << "size : " << fSpec.size() << G4endl;
      //G4cout << "edep: " << edep << " z: " << zPoz/cm << G4endl;
    }
    if(n_hit > 0){
      //AnalysisManager::getInstance()->AddEventXtalBGO(fEnergy, fEnergyS, fTime);
    }
    fEnergy.clear();
    fEnergyS.clear();
    fTime.clear();
    
    fSpec.clear();
    fWeight.clear();
  }
  
  if (xtalGAGGHC){
    vector<float> fSpec;
    vector<float> fWeight;
    vector<float> fEnergy;
    vector<float> fTime;
    vector<float> fEnergyS;
    G4int n_hit = xtalGAGGHC->entries();
    G4cout << "Nb. of XtalGAGG Hits " << n_hit << G4endl;   
    for(G4int iHit=0; iHit<n_hit; iHit++){
      G4ThreeVector hitPos = (*xtalGAGGHC)[iHit]->GetPosition()/cm;
      G4double zPoz = hitPos.z();
      G4double edep = (*xtalGAGGHC)[iHit]->GetEnergy()/MeV;
      G4double enSpec = (*xtalGAGGHC)[iHit]->GetEnergyS()/MeV;
      G4double ftime = (*xtalGAGGHC)[iHit]->GetTime();
      //G4int XtalId = (*xtalGAGGHC)[iHit]->GetID();
      //fXtalDetId.push_back(XtalId);
      fEnergy.push_back(edep);
      fTime.push_back(ftime);
      fEnergyS.push_back(enSpec);
      //vector<float> fSpec_loc = (*xtalGAGGHC)[iHit]->GetESpectrum();
      //vector<float> fWeight_loc = (*xtalGAGGHC)[iHit]->GetWeight();
      //for(G4int kk = 0; kk < G4int(fSpec_loc.size()); kk++){
        //fSpec.push_back(fSpec_loc[kk]);
        //fWeight.push_back(fWeight_loc[kk]);
      //}
      //G4cout << "size : " << fSpec.size() << G4endl;
      //G4cout << "edep: " << edep << " z: " << zPoz/cm << G4endl;
    }
    if(n_hit > 0){
      //AnalysisManager::getInstance()->AddEventXtalGAGG(fEnergy, fEnergyS, fTime);
    }
    fEnergy.clear();
    fEnergyS.clear();
    fTime.clear();
    
    fSpec.clear();
    fWeight.clear();
  }
  */
  Clear();
}


