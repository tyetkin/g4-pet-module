#ifndef SiPM_h
#define SiPM_h 1

#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4String.hh"

class SiPM {
  public:
    SiPM(G4double activeSizeX, G4double activeSizeY, G4double physSizeX, G4double physSizeY, G4double thickness = 1.0, G4bool checkOverlaps = false);
    ~SiPM();

    G4LogicalVolume* GetLogicalVolume() const { return fLogicSiPM; }
    G4LogicalVolume* GetActiveLogicalVolume() const { return fLogicActiveArea; }

    G4double GetPhysicalSizeX() const { return fPhysSizeX; }
    G4double GetPhysicalSizeY() const { return fPhysSizeY; }
    G4double GetThickness() const { return fThickness; }
    G4double GetActiveSizeX() const { return fActiveSizeX; }
    G4double GetActiveSizeY() const { return fActiveSizeY; }

  private:
    void Construct();
    G4Material* FindMaterial(G4String name);

    G4double fActiveSizeX;
    G4double fActiveSizeY;
    G4double fPhysSizeX;
    G4double fPhysSizeY;
    G4double fThickness;
    G4bool fCheckOverlaps;

    G4LogicalVolume* fLogicSiPM;
    G4LogicalVolume* fLogicActiveArea;
};

#endif
