
#include "EventAction.hh"
#include "PhotonDetSD.hh"
#include "XtalSD.hh"
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
   fPrimDetHCID(-999),
   fXtalHCID(-999)
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
  XtalHitsCollection* xtalHC = 0;
  

  G4String colName = "PhotonDetHitsCollection";
  fPhoDetHCID = SDman->GetCollectionID(colName);
  
  
  colName = "PrimDetHitsCollection";
  fPrimDetHCID = SDman->GetCollectionID(colName);

  colName = "XtalHitsCollection";
  fXtalHCID = SDman->GetCollectionID(colName);


  if (HCE){
    if (fPhoDetHCID>=0){
      phoDetHC = (PhotonDetHitsCollection*)(HCE->GetHC(fPhoDetHCID));
      if (phoDetHC)AnalysisManager::getInstance()->FillTree(evt->GetEventID(), phoDetHC);
    }
    
    if (fPrimDetHCID>=0){
      primDetHC = (PrimDetHitsCollection*)(HCE->GetHC(fPrimDetHCID));
      if (primDetHC)AnalysisManager::getInstance()->FillPrimDetTree(evt->GetEventID(), primDetHC);
    }

    if (fXtalHCID>=0){
      xtalHC = (XtalHitsCollection*)(HCE->GetHC(fXtalHCID));
      if (xtalHC)AnalysisManager::getInstance()->FillXtalTree(evt->GetEventID(), xtalHC);
    }
  }
  
  Clear();
}


