

#include "globals.hh"
#include "G4RunManager.hh"

#include "Trajectory.hh"

#include "UserTrackInformation.hh"

#include "G4Track.hh"
#include "G4ParticleTypes.hh"
#include "G4TrackingManager.hh"

#include "TrackingAction.hh"

void TrackingAction::PreUserTrackingAction(const G4Track* aTrack)
{
  //Let this be up to the user via vis.mac
  //  fpTrackingManager->SetStoreTrajectory(true);

  //Use custom trajectory class
  fpTrackingManager->SetTrajectory(new Trajectory(aTrack));
  
  UserTrackInformation* trackInformation = new UserTrackInformation();

  if (aTrack->GetMomentumDirection().z()>0.0) {
     //trackInformation->AddStatusFlag(right);
  } else {
     //trackInformation->AddStatusFlag(left);
  }

  G4String PVName = aTrack->GetVolume()->GetName();

  if (PVName == "Scinti" || PVName == "Coating")
     //trackInformation->AddStatusFlag(InsideOfBGO);

  fpTrackingManager->SetUserTrackInformation(trackInformation);
  
}

void TrackingAction::PostUserTrackingAction(const G4Track* aTrack)
{
  Trajectory* trajectory =
                      (Trajectory*)fpTrackingManager->GimmeTrajectory();

  if (aTrack->GetDefinition()==G4OpticalPhoton::OpticalPhotonDefinition())
  {
     if (aTrack->GetParentID()==0) trajectory->SetDrawTrajectory(true);
     else {
        const G4VProcess* creator = aTrack->GetCreatorProcess();
        if (creator && creator->GetProcessName()=="Scintillation")
        {
           trajectory->WLS();
           trajectory->SetDrawTrajectory(true);
        }
     }
  }
  else //draw all other trajectories
    trajectory->SetDrawTrajectory(true);
}
