#include "SiPM.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

SiPM::SiPM(G4double activeSizeX, G4double activeSizeY, G4double physSizeX, G4double physSizeY, G4double thickness, G4bool checkOverlaps)
: fActiveSizeX(activeSizeX), fActiveSizeY(activeSizeY), fPhysSizeX(physSizeX), fPhysSizeY(physSizeY), fThickness(thickness),
  fCheckOverlaps(checkOverlaps), fLogicSiPM(nullptr), fLogicActiveArea(nullptr)
{
    Construct();
}

SiPM::~SiPM() {
}

void SiPM::Construct() {
    // SiPM Encapsulant (Physical Volume)
    // We use Silicone as the encapsulant material, which is typical for clear epoxy in G4 optical simulations
    G4Material* encapsulantMat = FindMaterial("Silicone");
   
    G4VSolid* solidSiPM = new G4Box("SiPM_Solid", fPhysSizeX/2., fPhysSizeY/2., fThickness/2.);
    fLogicSiPM = new G4LogicalVolume(solidSiPM, encapsulantMat, "SiPM_Logic");

    // SiPM Active Area (Silicon)
    G4Material* activeMat = G4NistManager::Instance()->FindOrBuildMaterial("Silicone");
   
    

    G4VSolid* solidActive = new G4Box("SiPM_Active_Solid", fActiveSizeX/2., fActiveSizeY/2., fThickness/2.);
    // IMPORTANT: Naming the logical volume "PhoDet" so it matches the Sensitive Detector name assigned in DetectorConstruction
    fLogicActiveArea = new G4LogicalVolume(solidActive, activeMat, "PhoDet"); 
    
    // Place active area inside the encapsulant
    new G4PVPlacement(0, G4ThreeVector(0, 0, 0), fLogicActiveArea, "SiPM_Active_Physical", fLogicSiPM, false, 0, fCheckOverlaps);

    // Visualization attributes
    G4VisAttributes* encapsulantVis = new G4VisAttributes(G4Colour(0.0, 0.0, 0.7, 0.3)); // Transparent blue
    encapsulantVis->SetVisibility(true);
    encapsulantVis->SetForceSolid(true); 
    fLogicSiPM->SetVisAttributes(encapsulantVis);

    G4VisAttributes* activeVis = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0)); // Red for active area
    activeVis->SetVisibility(true);
    fLogicActiveArea->SetVisAttributes(activeVis);
}

G4Material* SiPM::FindMaterial(G4String name) {
    G4Material* material = G4Material::GetMaterial(name, false);
    return material;
}
