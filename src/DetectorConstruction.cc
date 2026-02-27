

#include "DetectorConstruction.hh"
#include "Materials.hh"
#include "PhotonDetSD.hh"
#include "PrimDetSD.hh"
#include "XtalSD.hh"
#include "SiPM.hh"
#include "DetectorMessenger.hh"


#include "G4PVParameterised.hh"
#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4UnitsTable.hh"

#include "G4OpBoundaryProcess.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalBorderSurface.hh"


#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Trd.hh"
#include "G4Sphere.hh"
#include "G4Polyhedra.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"

#include "G4SDManager.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "G4RunManager.hh"

#include "DetectorParam.hh"
#include "CollimatorParam.hh"
#include "globals.hh"

#include <sstream>
#include <cmath>

DetectorConstruction::DetectorConstruction()
:G4VUserDetectorConstruction(), fLogicWorld(0), fPhysiWorld(0), fMaterials(0),
  fCheckOverlaps(true), fCoatingType("BR") {
  fMessenger = new DetectorMessenger(this);
  fXtalSizeXY = 50.0*mm;//initial value
  fXtalSizeZ = 10 * mm;
  fPhoDetThickness = 1.40 * mm;
  fPhoDetActiveX = 3*mm;
  fPhoDetActiveY = 3*mm;
  fPhoDetX = 4*mm;
  fPhoDetY = 4*mm;
  fPhoDetPitch = 0.2*mm;
  fCoatingThickness = 0.6 * mm;
  fOpticalGreaseThickness = 0.02*mm;
  fNumPhoDetsX = 8;
  fNumPhoDetsY = 8;
  fScintZLength = 0*mm;
}

DetectorConstruction::~DetectorConstruction() {
//  if (fMaterials)
//    delete fMaterials;
  delete fMessenger;
}

G4VPhysicalVolume *DetectorConstruction::Construct() {
  fMaterials = Materials::GetInstance();
  return ConstructVolumes();
}

G4VPhysicalVolume *DetectorConstruction::ConstructVolumes() {
 
  G4double fWorldSizeXYZ = 80.0*cm;
  if(fNumPhoDetsX != fNumPhoDetsY){
    G4cout << "Error" << G4endl;
    exit(1);
  }
  fXtalSizeXY = fPhoDetX*fNumPhoDetsX + (fNumPhoDetsX-1)*fPhoDetPitch;

  G4double coatingSizeXY = 2.0*fXtalSizeRadius + 2.*fCoatingThickness;
  G4double sideCoatingSizeZ = fXtalSizeZ;
  G4double sideCoatingSizeXY = fXtalSizeXY + 2.*fCoatingThickness;




  //scintillator will be placed inside
  G4double coatingZLength = fXtalSizeZ + 
	                    fOpticalGreaseThickness + fPhoDetThickness + 2*fCoatingThickness;
  
  G4cout << "Total length " << coatingZLength/mm << " mm" << G4endl; 
  
 
  // WORLD
  G4VSolid *solidWorld = new G4Box("World", fWorldSizeXYZ, fWorldSizeXYZ, fWorldSizeXYZ);
  fLogicWorld = new G4LogicalVolume(solidWorld, FindMaterial("G4_AIR"), "World");
  fPhysiWorld = new G4PVPlacement(0, G4ThreeVector(), fLogicWorld, "World_PV", 0, false, 0, fCheckOverlaps);
  G4cout << "Finished world" << G4endl;
    
  // scintillator
  G4double ScintZpos = 0.*mm;
  fScintZLength = fXtalSizeZ;
  G4VSolid* fSolidScint = new G4Box("Scintillator", fXtalSizeXY/2., fXtalSizeXY/2., fXtalSizeZ/2.);
  fLogicScint = new G4LogicalVolume(fSolidScint, FindMaterial("LYSOCe"), "Scintillator");
  auto fPhysiScint = new G4PVPlacement(0, G4ThreeVector(0, 0, ScintZpos), fLogicScint, "Scintillator", fLogicWorld, false, 0, fCheckOverlaps);
  G4cout << "Finished scintillator" << G4endl;

  //side coating
  G4VSolid* fSolidSideCoating;
  G4VSolid* fSolidBoxA = new G4Box("SideBoxA", sideCoatingSizeXY/2., sideCoatingSizeXY/2., fXtalSizeZ/2.);;
  G4VSolid* fSolidBoxB = new G4Box("SideBoxB", fXtalSizeXY/2., fXtalSizeXY/2., fXtalSizeZ/2.);;
  fSolidSideCoating = new G4SubtractionSolid("SideCoating", fSolidBoxA, fSolidBoxB, 0, G4ThreeVector(0));    
  

  fLogicSideCoating = new G4LogicalVolume(fSolidSideCoating, FindMaterial("Coating"), "SideCoating");
  auto fPhysiSideCoating = new G4PVPlacement(0, G4ThreeVector(0, 0, ScintZpos), fLogicSideCoating, "SideCoating", fLogicWorld, false, 0, fCheckOverlaps);
  G4cout << "Finished side coating" << G4endl;

  //top coating
  G4VSolid* fSolidTopCoating;
  G4double topCoatingZpos = ScintZpos-fXtalSizeZ/2. - fCoatingThickness/2.;
  fSolidTopCoating = new G4Box("TopCoating", sideCoatingSizeXY/2., sideCoatingSizeXY/2., fCoatingThickness/2.);    
  
  fLogicTopCoating = new G4LogicalVolume(fSolidTopCoating, FindMaterial("Coating"), "TopCoating");
  auto fPhysiTopCoating = new G4PVPlacement(0, G4ThreeVector(0, 0, -topCoatingZpos), fLogicTopCoating, "TopCoating", fLogicWorld, false, 0, fCheckOverlaps);
  G4cout << "Finished top coating" << G4endl;
  
  //optical grease
  G4double opGreaseZPos = fXtalSizeZ/2. + fOpticalGreaseThickness/2.;
  G4VSolid *solidOpGrease = new G4Box("OpticalGrease", fXtalSizeXY/2., fXtalSizeXY/2., fOpticalGreaseThickness/2.);
  fLogicOpGrease = new G4LogicalVolume(solidOpGrease, FindMaterial("Silicone"), "LogicOpticalGrease");
  auto fPhysiOpGrease = new G4PVPlacement(0, G4ThreeVector(0, 0, -opGreaseZPos), fLogicOpGrease, "PhysiOpticalGrease", fLogicWorld, false, 0, fCheckOverlaps);
  
  // Photon Detector -- SiPM
  G4double PhoDetzPos = ScintZpos + fXtalSizeZ/2. + fOpticalGreaseThickness + fPhoDetThickness/2.;
  SiPM* sipm = new SiPM(fPhoDetActiveX, fPhoDetActiveY, fPhoDetX, fPhoDetY, fPhoDetThickness);
  fLogicPhoDet = sipm->GetLogicalVolume();
  fSiPMActiveLogical = sipm->GetActiveLogicalVolume();

  std::vector<G4VPhysicalVolume*> sipmPhysicalVols;
  fSiPMs.clear(); // We need a class member to keep track of these to assign SDs later!
  for(G4int i = 0; i < fNumPhoDetsX; ++i){
    for(G4int j = 0; j < fNumPhoDetsY; ++j){
      int copyNo = i * fNumPhoDetsY + j;
      G4double xx = (i - (fNumPhoDetsX-1)/2.) * (fPhoDetX+fPhoDetPitch);
      G4double yy = (j - (fNumPhoDetsY-1)/2.) * (fPhoDetY+fPhoDetPitch); 
      // G4cout << copyNo << " " << xx/mm << " " << yy/mm << G4endl;
      G4VPhysicalVolume* physSiPM = new G4PVPlacement(0, G4ThreeVector(xx, yy, -PhoDetzPos), fLogicPhoDet, "PhoDet", fLogicWorld, false, copyNo, fCheckOverlaps);        
      sipmPhysicalVols.push_back(physSiPM);
    }
  }        
  

  
  // a thin primary particle counter in front of the detector
  G4double counterZThickness = 0.5*mm;
  G4double counterZPos = topCoatingZpos - 2*counterZThickness; // Changed from bottomCoatingZpos
  
  G4Box *fPrimCounterBox = new G4Box("PrimaryCounter", sideCoatingSizeXY/2., sideCoatingSizeXY/2., counterZThickness/2);
  G4LogicalVolume* fLogicPrimCounterBox = new G4LogicalVolume(fPrimCounterBox, FindMaterial("G4_AIR"), "PrimaryCounter");
  
  new G4PVPlacement(0, G4ThreeVector(0, 0, -counterZPos), fLogicPrimCounterBox, "PrimaryCounter", fLogicWorld, false, 0, fCheckOverlaps);
  

  
  // // Create main collimator block
  // G4Box* collimatorBlock = new G4Box("CollimatorBlock", 
  //                                  collimatorSizeX/2, 
  //                                  collimatorSizeY/2, 
  //                                  collimatorSizeZ/2);
  // // counterZThickness
  // G4double collimatorZpos = coatingZLength/2. + counterZThickness + collimatorSizeZ/2.;
  // G4LogicalVolume* collimatorBlockLV = new G4LogicalVolume(collimatorBlock, FindMaterial("G4_Pb"), "CollimatorBlock");
  // new G4PVPlacement(0, G4ThreeVector(0, 0, collimatorZpos), collimatorBlockLV, "CollimatorBlock", fLogicWorld, false, 0, fCheckOverlaps);
                       

  

  // 1- LUT DAVIS Models 
  G4OpticalSurface* OpSurfaceBlack = new G4OpticalSurface("OpSurfaceBlack");
  OpSurfaceBlack->SetType(dielectric_LUTDAVIS);
  OpSurfaceBlack->SetModel(DAVIS);
  OpSurfaceBlack->SetFinish(Rough_LUT);

  // For the black surface, we set reflectivity to 0.0 to simulate absorption as asked.
  G4MaterialPropertiesTable* mptBlack = new G4MaterialPropertiesTable();
  std::vector<G4double> ephoton = { 1.0*eV, 7.0*eV };
  std::vector<G4double> reflectivity = { 0.0, 0.0 };
  mptBlack->AddProperty("REFLECTIVITY", ephoton, reflectivity);
  OpSurfaceBlack->SetMaterialPropertiesTable(mptBlack);


  G4OpticalSurface* OpSurfaceReflective = new G4OpticalSurface("OpSurfaceReflective");
  OpSurfaceReflective->SetType(dielectric_LUTDAVIS);
  OpSurfaceReflective->SetModel(DAVIS);
  OpSurfaceReflective->SetFinish(PolishedESRGrease_LUT);

  // Assign according to fCoatingType
  G4OpticalSurface* activeSideSurface  = (fCoatingType == "R") ? OpSurfaceReflective : OpSurfaceBlack; // Default B/BR => Black
  G4OpticalSurface* activeTopSurface   = (fCoatingType == "B") ? OpSurfaceBlack      : OpSurfaceReflective; // Default R/BR => Reflective

  new G4LogicalBorderSurface("Scintillator/SideCoating Surface", fPhysiScint, fPhysiSideCoating, activeSideSurface);
  new G4LogicalBorderSurface("SideCoating/Scintillator Surface", fPhysiSideCoating, fPhysiScint, activeSideSurface);

  new G4LogicalBorderSurface("Scintillator/TopCoating Surface", fPhysiScint, fPhysiTopCoating, activeTopSurface);
  new G4LogicalBorderSurface("TopCoating/Scintillator Surface", fPhysiTopCoating, fPhysiScint, activeTopSurface);
  
  
  // Optical surfaces between Scintillator -> Optical Grease -> SiPM Encapsulant
  // Without these, or without dielectric_dielectric finish, photons might get trapped/reflected
  // G4OpticalSurface* OpSurfaceGrease = new G4OpticalSurface("OpSurfaceGrease");
  // OpSurfaceGrease->SetType(dielectric_dielectric);
  // OpSurfaceGrease->SetFinish(polished);
  // OpSurfaceGrease->SetModel(glisur);
  
  // new G4LogicalBorderSurface("Scintillator/Grease Surface", fPhysiScint, fPhysiOpGrease, OpSurfaceGrease);
  // new G4LogicalBorderSurface("Grease/Scintillator Surface", fPhysiOpGrease, fPhysiScint, OpSurfaceGrease);

  // // We loop over all SiPM physical placements to attach the border surfaces to the grease
  // for(auto physSiPM : sipmPhysicalVols) {
  //     new G4LogicalBorderSurface("Grease/SiPM Surface", fPhysiOpGrease, physSiPM, OpSurfaceGrease);
  //     new G4LogicalBorderSurface("SiPM/Grease Surface", physSiPM, fPhysiOpGrease, OpSurfaceGrease);
  // }
  // Visualization attributes
  G4Colour red(1.0, 0.0, 0.0);
  G4Colour black(0.0, 0.0, 0.0);
  G4Colour magenta(1.0, 0.0, 1.0);
  G4Colour lblue(0.0, 0.0, .75);
  G4Colour lgray(0.5, 0.5, 0.5);
  G4Colour blueCobalt(0., 0., 0.7);
  G4Colour colorTungsten(0.3, 0.3, 0.3);
  G4Colour yellow(1., 1., 0.);
  G4Colour darkGreen(0 / 255., 100 / 255., 0 / 255.);
  G4Colour darkOrange3(205 / 255., 102 / 255., 000 / 255.);
  G4Colour purple(0.85, 0.44, 0.84);
  G4Colour grayAl(0.7, 0.7, 0.7);

  G4VisAttributes *worldVisAtt = new G4VisAttributes(magenta);
  worldVisAtt->SetVisibility(false);
  fLogicWorld->SetVisAttributes(worldVisAtt);


  G4VisAttributes *coatingVisAtt = new G4VisAttributes(lgray);
  coatingVisAtt->SetVisibility(true); 
  // coatingVisAtt->SetForceSolid(false);
  // fLogicCoating->SetVisAttributes(coatingVisAtt);

  fLogicSideCoating->SetVisAttributes(coatingVisAtt);
  fLogicTopCoating->SetVisAttributes(coatingVisAtt);
  
  // G4VisAttributes *pdVisAtt = new G4VisAttributes(blueCobalt);
  // pdVisAtt->SetVisibility(true);
  // // pdVisAtt->SetForceSolid(true);
  // fLogicPhoDet->SetVisAttributes(pdVisAtt);
  // hexHoleLV->SetVisAttributes(pdVisAtt);


  // G4VisAttributes *holeVisAtt = new G4VisAttributes(black);
  //Set opacity = 0.1 then transparency = 1 - 0.1 = 0.9
  // G4VisAttributes *holeVisAtt = new G4VisAttributes(G4Colour(0.0,0.0,0.0,0.9));
  // holeVisAtt->SetVisibility(true);
  // holeVisAtt->SetForceSolid(true);
  // hexHoleLV->SetVisAttributes(holeVisAtt);
  
  G4VisAttributes *greaseVisAtt = new G4VisAttributes(magenta);
  greaseVisAtt->SetVisibility(true);
  greaseVisAtt->SetForceSolid(true);
  fLogicOpGrease->SetVisAttributes(greaseVisAtt);
  
   
  G4VisAttributes *scintVisAtt = new G4VisAttributes(darkOrange3);
  scintVisAtt->SetVisibility(true);
  // scintVisAtt->SetForceSolid(true);
  fLogicScint->SetVisAttributes(scintVisAtt);
  // collimatorExteriorBlockLV->SetVisAttributes(scintVisAtt);
  // solidBlockLV->SetVisAttributes(scintVisAtt);
  // collimatorBlockLV->SetVisAttributes(scintVisAtt);
  

  G4VisAttributes *primCntrVisAtt = new G4VisAttributes(red);
  primCntrVisAtt->SetVisibility(true);
  primCntrVisAtt->SetForceSolid(true);
  fLogicPrimCounterBox->SetVisAttributes(primCntrVisAtt);
  

  return fPhysiWorld;
}

G4Material *DetectorConstruction::FindMaterial(G4String name) {
  G4Material *material = G4Material::GetMaterial(name, true);
  return material;
}

void DetectorConstruction::ConstructSDandField() {
  
  // Sensitive detectors
  PhotonDetSD* pdSD = new PhotonDetSD("PhotonDetSD", "PhotonDetHitsCollection", fNumPhoDetsX*fNumPhoDetsY);
  G4SDManager::GetSDMpointer()->AddNewDetector(pdSD);
  
  // Set SD on the actual logical volume representing the active SiPM area
  if(fSiPMActiveLogical) {
      SetSensitiveDetector(fSiPMActiveLogical, pdSD);
  } else {
      SetSensitiveDetector("PhoDet", pdSD); // Fallback
  }
  // G4cout << "SetSensitiveDetector" << G4endl;
  
  PrimDetSD* primSD = new PrimDetSD("PrimDetSD", "PrimDetHitsCollection");
  G4SDManager::GetSDMpointer()->AddNewDetector(primSD);
  SetSensitiveDetector("PrimaryCounter", primSD);

  XtalSD* xtalSD = new XtalSD("XtalSD", "XtalHitsCollection", fNumPhoDetsX*fNumPhoDetsY, this);
  G4SDManager::GetSDMpointer()->AddNewDetector(xtalSD);
  SetSensitiveDetector("Scintillator", xtalSD);
  
  G4SDManager::GetSDMpointer()->ListTree();
}


