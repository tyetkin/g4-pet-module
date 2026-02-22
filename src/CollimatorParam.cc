#include "CollimatorParam.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4Polyhedra.hh"
#include "G4UnitsTable.hh"
#include "G4RotationMatrix.hh"
#include "G4SystemOfUnits.hh"


// #include "globals.hh"

CollimatorParam::CollimatorParam(G4double holeSpacing, G4double septalThickness, G4int numHolesX, G4int numHolesY) 
: fHoleSpacing(holeSpacing), fSeptalThickness(septalThickness), fNumHolesX(numHolesX), fNumHolesY(numHolesY) 
{}

void CollimatorParam::ComputeDimensions(G4Polyhedra& hexHole, G4int copyNo, const G4VPhysicalVolume*) const {
    // Dimensions are constant, so nothing needs to be done here
}

void CollimatorParam::ComputeTransformation (const G4int copyNo, G4VPhysicalVolume* physVol) const {
    G4double x, y;
    G4int column = copyNo / fNumHolesY;
    G4int row = copyNo % fNumHolesY;

    // Calculate x position
    double xoffset = 1.0*mm;
    double yoffset = xoffset/2.;
    x = (column - fNumHolesX / 2.0) * fHoleSpacing + xoffset;
    // Calculate y position, offset every second row
    G4double totalHoleRadius = fHoleSpacing / 2.0 + fSeptalThickness;
    G4double ySpacing = 2.0 * totalHoleRadius;
    if (column % 2 == 0) {
        y = (row - fNumHolesY / 2.0) * ySpacing;
    } else {
        y = (row - fNumHolesY / 2.0 + 0.5) * ySpacing;
    }
    y += yoffset;
    // G4cout << "x, y " << x << ", " << y << G4endl;
    // Apply the transformation to the physical volume
    physVol->SetTranslation(G4ThreeVector(x, y, 0));
}



