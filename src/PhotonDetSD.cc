

#include "PhotonDetSD.hh"
#include "PhotonDetHit.hh"
#include "UserTrackInformation.hh"

#include "G4Track.hh"
#include "G4SDManager.hh"
#include "G4ThreeVector.hh"
#include "G4Step.hh"
#include "G4ParticleDefinition.hh"
#include "G4VTouchable.hh"
#include "G4VProcess.hh"
#include "G4TouchableHistory.hh"
#include "G4ios.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleDefinition.hh"

#include "G4SystemOfUnits.hh"

PhotonDetSD::PhotonDetSD(const G4String& name, const G4String& hitsCollectionName, G4int numDets)
  : G4VSensitiveDetector(name), 
    fPhotonDetHitCollection(0)
{
  //G4cout << "TEST" << G4endl;
  fNumDets = numDets;
  collectionName.insert(hitsCollectionName);
  hPlanck = 4.13566733e-15*eV*s;
  c_light = 299792458*m/s;
}

PhotonDetSD::~PhotonDetSD() { }

void PhotonDetSD::Initialize(G4HCofThisEvent* HCE)
{
  // G4cout << "AAAA Initialize Hits" << G4endl;
  fPhotonDetHitCollection = new PhotonDetHitsCollection(SensitiveDetectorName,collectionName[0]);
  
  // static G4int HCID = -1;
  G4int HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  HCE->AddHitsCollection( HCID, fPhotonDetHitCollection );
  // for (G4int i=0; i<fNumDets; i++ ) {
  //   fPhotonDetHitCollection->insert(new PhotonDetHit());
  // }
  G4cout << "Initialize Hits" << HCID << G4endl; 
}

G4bool PhotonDetSD::ProcessHits(G4Step* aStep, G4TouchableHistory* )
{

  G4StepPoint* thePrePoint = aStep->GetPreStepPoint();
  //process only the optical photons
  G4TouchableHistory* theTouchable
      = (G4TouchableHistory*)(thePrePoint->GetTouchable());
  
  if(aStep->GetTrack()->GetDefinition()->GetParticleName() != (G4String)"opticalphoton") return false;      

  G4String volName = theTouchable->GetVolume()->GetName();
  if(volName != "PhoDet" && volName != "SiPM_Active_Physical") return false;
  
  //G4cout << "PhotonDetSD Printing" << G4endl;      
  // // if(theTouchable->GetVolume()->GetName() == "LightGuide_PV")return false;
  //G4cout << theTouchable->GetVolume()->GetName() << G4endl;
  
  // The PhoDet active area is placed inside the SiPM_Logic encapsulant,
  // which is in turn placed in the World with the copy number. 
  // We need to fetch the copy number from the parent physical volume.
  G4int phoDetCopyNo = theTouchable->GetCopyNumber(1); 
  // G4cout << "PHODET_ID: " << phoDetCopyNo << G4endl;  
  // Find out information regarding the hit

  G4ThreeVector pos = aStep->GetPreStepPoint()->GetPosition();
  G4float gtime = aStep->GetPreStepPoint()->GetGlobalTime()/ns;
  //G4float gtime2 = aStep->GetTrack()->GetGlobalTime()/ns;
  //make sure that optical photon tracks generated inside the Xtal
  G4float ltime = aStep->GetPreStepPoint()->GetLocalTime()/ns;
  //global time from track or presteppoint must be same
  //G4int prec = G4cout.precision(20);
  //G4cout << "optical photon. From track" << gtime 
    //     << " from PreStepPoint " << gtime2
      //   << G4endl;
  //G4cout.precision(prec);
  G4double wavelength = (hPlanck*c_light)/aStep->GetTrack()->GetTotalEnergy();
  fPhotonDetHitCollection->insert(new PhotonDetHit(pos, wavelength, gtime, ltime, phoDetCopyNo));//for now
  // Stop Tracking when it hits the detector's surface
  G4Track* theTrack = aStep->GetTrack();
  theTrack->SetTrackStatus(fStopAndKill);
  return true;
  
}

void PhotonDetSD::EndOfEvent(G4HCofThisEvent*)
{
  
}



