#ifndef DetectorMessenger_h
#define DetectorMessenger_h 1

#include "G4UImessenger.hh"

class DetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithAString;

class DetectorMessenger : public G4UImessenger {
public:
  DetectorMessenger(DetectorConstruction*);
  virtual ~DetectorMessenger();
  virtual void SetNewValue(G4UIcommand*, G4String);

private:
  DetectorConstruction* fDetector;
  G4UIdirectory*        fDetectorDir;
  G4UIcmdWithAString*   fCoatingCmd;
};

#endif
