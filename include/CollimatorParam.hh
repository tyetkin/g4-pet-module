#ifndef CollimatorParam_h
#define CollimatorParam_h 1


#include "G4VPVParameterisation.hh"
#include "G4Cache.hh"
#include "G4Types.hh"


class G4Polyhedra;
class G4VPhysicalVolume;


// Your parameterisation class
class CollimatorParam : public G4VPVParameterisation
{
public:
    CollimatorParam(G4double holeSpacing, G4double septalThickness, G4int numHolesX, G4int numHolesY);

    virtual void ComputeDimensions(G4Polyhedra& hexHole, G4int copyNo, const G4VPhysicalVolume*) const;
    

    virtual void ComputeTransformation(const G4int copyNo, G4VPhysicalVolume* physVol) const;

private:
    G4double fHoleSpacing;
    G4double fSeptalThickness;
    G4int fNumHolesX, fNumHolesY;
};
#endif