
#include "StackingAction.hh"

#include "G4RunManager.hh"

#include "G4Track.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleDefinition.hh"

StackingAction::StackingAction() : fPhotonCounter(0) { }

StackingAction::~StackingAction() { }

G4ClassificationOfNewTrack
      StackingAction::ClassifyNewTrack(const G4Track * aTrack)
{
  G4ParticleDefinition* particleType = aTrack->GetDefinition();

  // keep primary particle
  if (aTrack->GetParentID() == 0) return fUrgent;

  if (particleType == G4OpticalPhoton::OpticalPhotonDefinition()) {
     // keep optical photon
     fPhotonCounter++;
     return fUrgent;
  } else {
     // discard all other secondaries
     // return fKill;
  }
  return fUrgent;
}

void StackingAction::NewStage() {
  // G4cout << "Number of optical photons produces in this event : "
  //        << fPhotonCounter << G4endl;
}

void StackingAction::PrepareNewEvent() { fPhotonCounter = 0; }
