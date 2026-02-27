#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "G4Cache.hh"



class G4Material;
class Materials;
class G4LogicalVolume;
class G4GlobalMagFieldMessenger;
class DetectorMessenger;


class DetectorConstruction: public G4VUserDetectorConstruction
{
  public:
  	DetectorConstruction();
  	virtual ~DetectorConstruction();

  public:
  	virtual G4VPhysicalVolume* Construct();
  	virtual void ConstructSDandField();

  	const G4VPhysicalVolume* GetWorld(){return fPhysiWorld;}
    void SetCoatingType(G4String val) { fCoatingType = val; }
    G4double GetScintZLength() { return fScintZLength; }

private:
  G4VPhysicalVolume* ConstructVolumes();
  G4Material* FindMaterial(G4String);

  G4LogicalVolume*   fLogicWorld;
  G4VPhysicalVolume* fPhysiWorld;

  G4LogicalVolume* fLogicScint;
  G4LogicalVolume* fLogicSideCoating;
  G4LogicalVolume* fLogicBottomCoating;
  G4LogicalVolume* fLogicTopCoating;
  G4LogicalVolume* fLogicCoating;
  G4LogicalVolume* fLogicOpGrease; 
  G4LogicalVolume* fLogicPhoDet; 
  G4LogicalVolume* fSiPMActiveLogical = nullptr;
  std::vector<G4VPhysicalVolume*> fSiPMs;

  Materials* fMaterials;

  G4bool fCheckOverlaps;
  G4double fXtalSizeRadius;
  G4double fXtalSizeXY;  
  G4double fXtalSizeZ;
  G4double fPhoDetThickness;
  G4double fCoatingThickness;
  G4double fOpticalGreaseThickness;
  G4double fPhoDetActiveX;
  G4double fPhoDetActiveY;
  G4double fPhoDetX;
  G4double fPhoDetY;
  G4double fPhoDetPitch;
  G4int fNumPhoDetsX;
  G4int fNumPhoDetsY;
  G4double fScintZLength;

  G4String fCoatingType;
  DetectorMessenger* fMessenger;

};

#endif
