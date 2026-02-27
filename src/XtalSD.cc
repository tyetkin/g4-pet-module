

#include "XtalSD.hh"
#include "XtalHit.hh"
#include "DetectorConstruction.hh"
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

XtalSD::XtalSD(const G4String& name, const G4String& hitsCollectionName, G4int numDets, DetectorConstruction* det)
  : G4VSensitiveDetector(name), 
    fXtalHitCollection(0)
{
  //G4cout << "TEST" << G4endl;
  fNumDets = numDets;
  fDet = det;
  collectionName.insert(hitsCollectionName);
  hPlanck = 4.13566733e-15*eV*s;
  c_light = 299792458*m/s;
}

XtalSD::~XtalSD() { 
  // if (fXtalHitCollection) delete fXtalHitCollection;
  // if (fDet) delete fDet;
}

void XtalSD::Initialize(G4HCofThisEvent* HCE)
{
  fRecordedPrimaryGammaTracks.clear();
  fXtalHitCollection = new XtalHitsCollection(SensitiveDetectorName,collectionName[0]);
  
  G4int HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  HCE->AddHitsCollection( HCID, fXtalHitCollection );
}

G4bool XtalSD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
  // Pre/post points of THIS step
  G4StepPoint* pre  = aStep->GetPreStepPoint();
  G4StepPoint* post = aStep->GetPostStepPoint();

  // Safety checks
  if (!pre || !post) return false;
  if (!pre->GetTouchable() || !pre->GetTouchable()->GetVolume()) return false;

  // G4cout << "volume name " << pre->GetTouchable()->GetVolume()->GetName() << G4endl;
  // If this SD is attached only to LYSO, this check is usually redundant.
  if (pre->GetTouchable()->GetVolume()->GetName() != "Scintillator") return false;

  // Get the track (particle) responsible for this step
  G4Track* track = aStep->GetTrack();

  // IMPORTANT: your original comment says "optical photons", but here you want the incoming gamma.
  // Filter for gamma only.
  if (track->GetDefinition() != G4Gamma::GammaDefinition()) return false;

  // Keep only PRIMARY particles (parentID == 0).
  // (If your gammas come from radioactive decay, they may NOT be primary; then this condition must change.)
  if (track->GetParentID() != 0) return false;
  //Get copy number
  G4int xtalCopyNo = pre->GetTouchable()->GetCopyNumber();
  // G4cout << "xtalCopyNo " << xtalCopyNo << G4endl;

  // ------------------------------------------------------------
  // OPTION A: first ENTRY into LYSO (not first interaction)
  // ------------------------------------------------------------
  // if (aStep->IsFirstStepInVolume()) {
  //   G4ThreeVector pos = pre->GetPosition();         // entry point
  //   G4double gtime = pre->GetGlobalTime()/ns;
  //   G4double ltime = pre->GetLocalTime()/ns;
  //   fXtalHitCollection->insert(new XtalHit(pos, gtime, ltime));
  //   return true;
  // }

  // ------------------------------------------------------------
  // OPTION B: first INTERACTION in LYSO
  // ------------------------------------------------------------
  // A gamma can have many steps in the crystal. ParentID==0 is NOT enough,
  // because the same primary gamma remains parentID==0 after scattering.
  // So detect the first non-transport step and store only once.

  const G4VProcess* proc = post->GetProcessDefinedStep();
  if (!proc) return false;

  // "Transportation" means no physics interaction yet (just movement/boundary crossing)
  if (proc->GetProcessName() == "Transportation") return false;

  // Optional: ignore Rayleigh if you only care about first energy-depositing interaction
  // if (proc->GetProcessName() == "Rayleigh") return false;

  // Prevent multiple inserts for the same gamma track.
  // You need an event-local set/member like: std::unordered_set<G4int> fRecordedPrimaryGammaTracks;
  // and clear it in Initialize().
  G4int trackID = track->GetTrackID();
  if (fRecordedPrimaryGammaTracks.count(trackID)) return false;

  // Use POST point for the interaction location/time
  G4ThreeVector pos = post->GetPosition();
  G4double gtime = post->GetGlobalTime()/ns;
  G4double ltime = post->GetLocalTime()/ns;
  //need correction to z since scintillator is at z=0 and crystal is at z=0
  //by using the half lenght in Z-axis
  G4double scintZpos = fDet->GetScintZLength()/2.;
  pos.setZ(pos.z() + scintZpos);
  
  fXtalHitCollection->insert(new XtalHit(pos, gtime, ltime, xtalCopyNo));

  // Mark this primary gamma as already recorded
  fRecordedPrimaryGammaTracks.insert(trackID);

  return true;
}


void XtalSD::EndOfEvent(G4HCofThisEvent*)
{
  
}



