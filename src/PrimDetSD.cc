
#include "PrimDetSD.hh"
#include "PrimDetHit.hh"
#include "AnalysisManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4MuonMinus.hh"
#include "G4MuonPlus.hh"
#include "G4ParticleDefinition.hh"

#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"

PrimDetSD::PrimDetSD(const G4String& name, const G4String& hitsCollectionName)
 : G4VSensitiveDetector(name),
   fPrimDetHitsCollection(0), fHCID(-1)
{
  collectionName.insert(hitsCollectionName);
}

PrimDetSD::~PrimDetSD()
{
}

void PrimDetSD::Initialize(G4HCofThisEvent* HCE)
{
  fPrimDetHitsCollection = new PrimDetHitsCollection(SensitiveDetectorName,collectionName[0]);
  if (fHCID<0)fHCID = G4SDManager::GetSDMpointer()->GetCollectionID(fPrimDetHitsCollection);
  HCE->AddHitsCollection( fHCID, fPrimDetHitsCollection );
}

G4bool PrimDetSD::ProcessHits(G4Step* step, G4TouchableHistory*)
{
  // G4cout << "PrimDetSD::ProcessHits-1" << G4endl;
  if (step == NULL) return false;
  // G4cout << "PrimDetSD::ProcessHits-2" << G4endl;
  // Get Material
  G4String thisVolume = step->GetTrack()->GetVolume()->GetName() ;
  //G4cout << thisVolume << G4endl;
  if(thisVolume != "PrimaryCounter")return false;
  // G4cout << "PrimDetSD::ProcessHits-3" << G4endl;
  
  // energy deposit
  // G4double edep = step->GetTotalEnergyDeposit();
  // step length
  // G4double stepLength = 0.;
  // if ( step->GetTrack()->GetDefinition()->GetPDGCharge() != 0. ) {
  //   stepLength = step->GetStepLength();
  // }

  G4double initialKinE = step->GetTrack()->GetVertexKineticEnergy();
  //G4cout << thisVolume << " " << initialKinE/keV << " " << stepLength/mm << G4endl;
  
  //if ( edep==0. && stepLength == 0. ) return false;
  //G4String parName = step->GetTrack()->GetDefinition()->GetParticleName();
  //G4cout << "Primary Counter hit " << G4endl;

  G4ThreeVector hitPoint = 0.5*(step->GetPostStepPoint()->GetPosition()+
                                step->GetPreStepPoint()->GetPosition());

  //G4ThreeVector pos = aStep->GetPostStepPoint()->GetPosition();
  G4double fTime = step->GetPostStepPoint()->GetGlobalTime();
  G4cout << "hitPoint " << hitPoint.x()/mm << " " << hitPoint.y()/mm << " " << fTime/ns << G4endl; 
  fPrimDetHitsCollection->insert(new PrimDetHit(hitPoint, initialKinE, fTime));//for now
  return true;
}

void PrimDetSD::EndOfEvent(G4HCofThisEvent*)
{
  /* 
  G4int nofHits = fPrimDetHitsCollection->entries();
  if ( verboseLevel>1 ) {
     G4cout << "\n-------->Hits Collection: in this event they are " << nofHits
            << " hits in the tracker chambers: " << G4endl;
     for ( G4int i=0; i<nofHits; i++ ) (*fPrimDetHitsCollection)[i]->Print();
  }
  */
}

