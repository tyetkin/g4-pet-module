#ifndef DetectorParam_h
#define DetectorParam_h 1

#include "G4VPVParameterisation.hh"
#include "G4Cache.hh"
#include "G4Types.hh"

class DetectorParam : public G4VPVParameterisation {
private:
  G4double fScintillatorSpacing;
  G4int fScintillatorsPerRow, fScintillatorsPerColumn;

public:
  DetectorParam(G4double scintillatorSpacing, G4int scintillatorsPerRow, G4int scintillatorsPerColumn);
  
  void ComputeTransformation(const G4int copyNo, G4VPhysicalVolume* physVol) const;
  void ComputeDimensions(G4Box& scintillatorBox, const G4int copyNo, const G4VPhysicalVolume* physVol) const;
};
#endif