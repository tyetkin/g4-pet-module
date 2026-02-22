#include "DetectorParam.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4Polyhedra.hh"
#include "G4UnitsTable.hh"
#include "G4RotationMatrix.hh"
#include "G4SystemOfUnits.hh"


DetectorParam::DetectorParam(G4double scintillatorSpacing, G4int scintillatorsPerRow, G4int scintillatorsPerColumn)
: fScintillatorSpacing(scintillatorSpacing), fScintillatorsPerRow(scintillatorsPerRow), fScintillatorsPerColumn(scintillatorsPerColumn) {}

void DetectorParam::ComputeTransformation(const G4int copyNo, G4VPhysicalVolume* physVol) const {
    G4int row = copyNo / fScintillatorsPerRow;
    G4int column = copyNo % fScintillatorsPerRow;
    // G4cout << copyNo << " " << row << " " << column << G4endl;

    G4double x = (column - fScintillatorsPerRow / 2.0) * fScintillatorSpacing;
    G4double y = (row - fScintillatorsPerColumn / 2.0) * fScintillatorSpacing;
    x += fScintillatorSpacing/2.;
    y += fScintillatorSpacing/2.;
    
    physVol->SetTranslation(G4ThreeVector(x, y, 0));
}
  
void DetectorParam::ComputeDimensions(G4Box& scintillatorBox, const G4int copyNo, const G4VPhysicalVolume* physVol) const{

}



