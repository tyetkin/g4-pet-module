#include "DetectorMessenger.hh"
#include "DetectorConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4RunManager.hh"

DetectorMessenger::DetectorMessenger(DetectorConstruction* det)
: G4UImessenger(), fDetector(det)
{
  fDetectorDir = new G4UIdirectory("/detector/");
  fDetectorDir->SetGuidance("Detector control.");

  fCoatingCmd = new G4UIcmdWithAString("/detector/coating", this);
  fCoatingCmd->SetGuidance("Select coating type for the detector faces.");
  fCoatingCmd->SetGuidance("  Choice : B (Black), R (Reflective), BR (Black sides, Reflective top)");
  fCoatingCmd->SetParameterName("CoatingType", true);
  fCoatingCmd->SetDefaultValue("BR"); // default value
  fCoatingCmd->SetCandidates("B R BR");
  fCoatingCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}

DetectorMessenger::~DetectorMessenger()
{
  delete fCoatingCmd;
  delete fDetectorDir;
}

void DetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  if(command == fCoatingCmd) {
    fDetector->SetCoatingType(newValue);
    G4RunManager::GetRunManager()->GeometryHasBeenModified();
  }
}
