
#include <cmath>
#include "Materials.hh"

#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

#include "G4OpBoundaryProcess.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalBorderSurface.hh"

Materials *Materials::fInstance = 0;

Materials::Materials() {
  fNistMan = G4NistManager::Instance();

  fNistMan->SetVerbose(2);

  CreateMaterials();
}

Materials::~Materials() {
  delete fPMMA;
  delete fAcrylic;
  delete fPethylene;
  delete fFPethylene;
  delete fPolystyrene;
  delete fSilicone;
  delete fSiO2;
  delete fLYSOCe;
  delete fBGO;
  delete fGAGG;
  delete fVacuum;
}

Materials *Materials::GetInstance() {
  if (fInstance == 0) {
    fInstance = new Materials();
  }
  return fInstance;
}

G4Material *Materials::GetMaterial(const G4String material) {
  G4Material *mat = fNistMan->FindOrBuildMaterial(material);

  if (!mat)
    mat = G4Material::GetMaterial(material);
  if (!mat) {
    std::ostringstream o;
    o << "Material " << material << " not found!";
    G4Exception("WLSMaterials::GetMaterial", "", FatalException, o.str().c_str());
  }

  return mat;
}

void Materials::CreateMaterials() {
  G4double density;
  G4int ncomponents;
  G4double fractionmass;
  std::vector < G4int > natoms;
  std::vector < G4double > fractionMass;
  std::vector < G4String > elements;

  // Materials Definitions
  // =====================

  // --------------------------------------------------
  // Vacuum
  // --------------------------------------------------

  fVacuum = fNistMan->FindOrBuildMaterial("G4_Galactic");

  // --------------------------------------------------
  // Air
  // --------------------------------------------------

  fAir = fNistMan->FindOrBuildMaterial("G4_AIR");

  // Aluminum
  fAl = fNistMan->FindOrBuildMaterial("G4_Al");

  // SiO2
  fSiO2 = fNistMan->FindOrBuildMaterial("G4_SILICON_DIOXIDE");
  
  // BGO
  fBGO = fNistMan->FindOrBuildMaterial("G4_BGO");

  fNaI = fNistMan->FindOrBuildMaterial("G4_SODIUM_IODIDE");

  // --------------------------------------------------
  // Cladding (polyethylene)
  // --------------------------------------------------

  elements.push_back("C");
  natoms.push_back(2);
  elements.push_back("H");
  natoms.push_back(4);

  density = 1.200 * g / cm3;

  fPethylene = fNistMan->ConstructNewMaterial("Pethylene", elements, natoms, density);

  elements.clear();
  natoms.clear();

  // --------------------------------------------------
  // Double Cladding (fluorinated polyethylene)
  // --------------------------------------------------

  elements.push_back("C");
  natoms.push_back(2);
  elements.push_back("H");
  natoms.push_back(4);

  density = 1.400 * g / cm3;

  fFPethylene = fNistMan->ConstructNewMaterial("FPethylene", elements, natoms, density);

  elements.clear();
  natoms.clear();

  // --------------------------------------------------
  // Silicone (Template for Optical Grease)
  // --------------------------------------------------

  elements.push_back("C");
  natoms.push_back(2);
  elements.push_back("H");
  natoms.push_back(6);

  density = 1.060 * g / cm3;

  fSilicone = fNistMan->ConstructNewMaterial("Silicone", elements, natoms, density);

  elements.clear();
  natoms.clear();

  // --------------------------------------------------
  // Aluminium
  // --------------------------------------------------

  fNistMan->FindOrBuildMaterial("G4_Al");

  // --------------------------------------------------
  // Lead
  // --------------------------------------------------

  fNistMan->FindOrBuildMaterial("G4_Pb");

  // --------------------------------------------------
  // Polystyrene (Plasti Scintillator)
  // --------------------------------------------------

  elements.push_back("C");
  natoms.push_back(8);
  elements.push_back("H");
  natoms.push_back(8);

  density = 1.050 * g / cm3;

  fPolystyrene = fNistMan->ConstructNewMaterial("Polystyrene", elements, natoms, density);

  elements.clear();
  natoms.clear();


  // --------------------------------------------------
  // TiO2
  // --------------------------------------------------

  elements.push_back("Ti");
  natoms.push_back(1);
  elements.push_back("O");
  natoms.push_back(2);

  density = 4.26 * g / cm3;

  G4Material *TiO2 = fNistMan->ConstructNewMaterial("TiO2", elements, natoms, density);

  elements.clear();
  natoms.clear();

  // --------------------------------------------------
  // Scintillator Coating - 15% TiO2 and 85% polystyrene by weight.
  // --------------------------------------------------

  density = 1.52 * g / cm3;

  fCoating = new G4Material("Coating", density, ncomponents = 2);

  fCoating->AddMaterial(TiO2, fractionmass = 15 * perCent);
  fCoating->AddMaterial(fPolystyrene, fractionmass = 85 * perCent);


  // PMMA C5H8O2 ( Acrylic )
  density = 1.19 * g / cm3;
  elements.push_back("C");
  natoms.push_back(5);
  elements.push_back("H");
  natoms.push_back(8);
  elements.push_back("O");
  natoms.push_back(2);
  fAcrylic = fNistMan->ConstructNewMaterial("Acrylic", elements, natoms, density);
  elements.clear();
  natoms.clear();

  // LYSO(Ce)
  density = 7.10 * g / cm3;
  elements.push_back("Lu");
  fractionMass.push_back(0.7144);
  elements.push_back("Y");
  fractionMass.push_back(0.0403);
  elements.push_back("Si");
  fractionMass.push_back(0.0637);
  elements.push_back("O");
  fractionMass.push_back(0.1814);
  elements.push_back("Ce");
  fractionMass.push_back(0.0002);
  fLYSOCe = fNistMan->ConstructNewMaterial("LYSOCe", elements, fractionMass, density);
  elements.clear();
  fractionMass.clear();
  
  //GAGG(Ce)//ignore Ce for now
  density = 6.6 * g / cm3;
  elements.push_back("Gd");
  natoms.push_back(3);
  elements.push_back("Al");
  natoms.push_back(2);
  elements.push_back("Ga");
  natoms.push_back(3);
  elements.push_back("O");
  natoms.push_back(12);

  fGAGG = fNistMan->ConstructNewMaterial("GAGGCe", elements, fractionMass, density);
  elements.clear();
  natoms.clear();
  
  
  //Optical properties of BGO
  std::vector<G4double> refIdxBGO;
  //
  const G4int nEntries = 50;
  G4double c = 299792458 * m / s;
  G4double h = 4.13566733e-15 * eV * s;

  std::vector<G4double> photonEnergy = { 
    2.00 * eV, 2.03 * eV, 2.06 * eV, 2.09 * eV, 2.12 * eV,
    2.15 * eV, 2.18 * eV, 2.21 * eV, 2.24 * eV, 2.27 * eV,
    2.30 * eV, 2.33 * eV, 2.36 * eV, 2.39 * eV, 2.42 * eV,
    2.45 * eV, 2.48 * eV, 2.51 * eV, 2.54 * eV, 2.57 * eV,
    2.60 * eV, 2.63 * eV, 2.66 * eV, 2.69 * eV, 2.72 * eV,
    2.75 * eV, 2.78 * eV, 2.81 * eV, 2.84 * eV, 2.87 * eV,
    2.90 * eV, 2.93 * eV, 2.96 * eV, 2.99 * eV, 3.02 * eV,
    3.05 * eV, 3.08 * eV, 3.11 * eV, 3.14 * eV, 3.17 * eV,
    3.20 * eV, 3.23 * eV, 3.26 * eV, 3.29 * eV, 3.32 * eV,
    3.35 * eV, 3.38 * eV, 3.41 * eV, 3.44 * eV, 3.47 * eV
  };
  // use dispersion formula
  // http://refractiveindex.info/?shelf=main&book=Bi4Ge3O12&page=Williams
  for (int i = 0; i < nEntries; i++) 
  {
    G4double wavelength = h * c / photonEnergy[i];  // in meters
    G4double ridx = 1. + (3.12118393 * wavelength * wavelength) / (wavelength * wavelength - 0.1807 * 0.1807);
    refIdxBGO.push_back(ridx);
  }


  std::vector<G4double> absBGO =   // UNKOWN
  { 11.334 * cm, 11.2282 * cm, 11.1328 * cm, 11.0464 * cm, 10.9676 * cm, 10.8947 * cm,
    10.826 * cm, 10.76 * cm, 10.695 * cm, 10.6296 * cm, 10.5625 * cm, 10.4926 * cm, 10.4189 * cm,
    10.3408 * cm, 10.2578 * cm, 10.1697 * cm, 10.0763 * cm, 9.97798 * cm, 9.87496 * cm, 9.76786 * cm,
    9.65738 * cm, 9.54438 * cm, 9.42982 * cm, 9.31477 * cm, 9.2003 * cm, 9.08755 * cm, 8.97762 * cm,
    8.87159 * cm, 8.77044 * cm, 8.67505 * cm, 8.58617 * cm, 8.50438 * cm, 8.43005 * cm, 8.36335 * cm,
    8.3042 * cm, 8.25224 * cm, 8.20686 * cm, 8.16715 * cm, 8.13191 * cm, 8.09963 * cm, 8.06853 * cm,
    8.03653 * cm, 8.0013 * cm, 7.96026 * cm, 7.91063 * cm, 7.84944 * cm, 7.77362 * cm, 7.68 * cm,
    7.56546 * cm, 7.42688 * cm
  };


  std::vector<G4double> BGOSlow = { 0.193214, 0.233599, 0.272888, 0.312973, 0.355439, 0.401298,
    0.450925, 0.504098, 0.560097, 0.617833, 0.675972, 0.733053,
    0.787594, 0.83817, 0.883482, 0.9224, 0.953997, 0.977564, 0.992617,
    0.998895, 0.996347, 0.985123, 0.965549, 0.938111, 0.903429, 0.862239,
    0.815368, 0.763711, 0.708218, 0.649865, 0.589645, 0.528551, 0.467559,
    0.407618, 0.349642, 0.294495, 0.242992, 0.195886, 0.153865, 0.117553,
    0.087501, 0.0641905, 0.0480302, 0.0393571, 0., 0., 0., 0., 0., 0.
  };
  G4MaterialPropertiesTable *mptBGO = new G4MaterialPropertiesTable();
  mptBGO->AddProperty("ABSLENGTH", photonEnergy, absBGO);
  mptBGO->AddProperty("SCINTILLATIONCOMPONENT1", photonEnergy, BGOSlow);
  mptBGO->AddProperty("SCINTILLATIONCOMPONENT2", photonEnergy, BGOSlow);
  mptBGO->AddProperty("RINDEX", photonEnergy, refIdxBGO, nEntries);
  mptBGO->AddConstProperty("SCINTILLATIONYIELD", 80./keV);
  mptBGO->AddConstProperty("RESOLUTIONSCALE", 1.0);
  
  mptBGO->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 300.0*ns);
  mptBGO->AddConstProperty("SCINTILLATIONTIMECONSTANT2", 300.0*ns);
  mptBGO->AddConstProperty("SCINTILLATIONRISETIME1", 2.*ns);
  mptBGO->AddConstProperty("SCINTILLATIONRISETIME2", 2.*ns);
  mptBGO->AddConstProperty("SCINTILLATIONYIELD1", 1.0);
  mptBGO->AddConstProperty("SCINTILLATIONYIELD2", 0.0);
  fBGO->SetMaterialPropertiesTable(mptBGO);
  
  std::vector<G4double> absGAGG = { 
    11.334*cm, 11.2282*cm, 11.1328*cm, 11.0464*cm, 10.9676*cm, 
    10.8947*cm, 10.826*cm, 10.76 * cm, 10.695 * cm, 10.6296 * cm, 
    10.5625 * cm, 10.4926 * cm, 10.4189 * cm, 10.3408*cm, 10.2578 * cm, 
    10.1697 * cm, 10.0763 * cm, 9.97798 * cm, 9.87496 * cm, 9.76786 * cm,
    9.65738*cm, 9.54438 * cm, 9.42982 * cm, 9.31477 * cm, 9.2003 * cm, 
    9.08755 * cm, 8.97762 * cm, 8.87159*cm, 8.77044 * cm, 8.67505 * cm, 
    8.58617 * cm, 8.50438 * cm, 8.43005 * cm, 8.36335 * cm, 8.3042*cm, 
    8.25224 * cm, 8.20686 * cm, 8.16715 * cm, 8.13191 * cm, 8.09963 * cm, 
    8.06853 * cm, 8.03653*cm, 8.0013 * cm, 7.96026 * cm, 7.91063 * cm, 
    7.84944 * cm, 7.77362 * cm, 7.68 * cm, 7.56546*cm, 7.42688*cm
  };
  std::vector<G4double> GAGGSlow =
  {
    0.237372, 0.276796, 0.319430, 0.371921, 0.453008,
    0.536145, 0.616478, 0.695609, 0.771328, 0.846363, 
    0.911281, 0.960186, 0.991192, 1.000800, 0.991482, 
    0.940895, 0.800931, 0.593870, 0.358580, 0.211445, 
    0.099848, 0.045146, 0.005715, 0.001168, 0., 
    0.,       0.,       0.,       0.,       0., 
    0.,       0.,       0.,       0.,       0., 
    0.,       0.,       0.,       0.,       0., 
    0.,       0.,       0.,       0.,       0., 
    0.,       0.,       0.,       0.,       0.
  };
  //GAGG
  std::vector<G4double> refIdxGAGG; 
  for (int i = 0; i < nEntries; i++) 
  {
    refIdxGAGG.push_back(1.9);
  }
  
  G4MaterialPropertiesTable *mptGAGG = new G4MaterialPropertiesTable();
  mptGAGG->AddProperty("ABSLENGTH", photonEnergy, absGAGG);
  mptGAGG->AddProperty("SCINTILLATIONCOMPONENT1", photonEnergy, GAGGSlow);
  mptGAGG->AddProperty("SCINTILLATIONCOMPONENT2", photonEnergy, GAGGSlow);
  mptGAGG->AddProperty("RINDEX", photonEnergy, refIdxGAGG, nEntries);
  //https://www.sciencedirect.com/science/article/pii/S016890021401198X?via%3Dihub
  mptGAGG->AddConstProperty("SCINTILLATIONYIELD", 55./keV);
  mptGAGG->AddConstProperty("RESOLUTIONSCALE", 1.0);
  mptGAGG->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 150.0*ns);
  mptGAGG->AddConstProperty("SCINTILLATIONTIMECONSTANT2", 490.0*ns);
  mptGAGG->AddConstProperty("SCINTILLATIONRISETIME1", 2.*ns);
  mptGAGG->AddConstProperty("SCINTILLATIONRISETIME2", 2.*ns);
  mptGAGG->AddConstProperty("SCINTILLATIONYIELD1", 1.0);
  mptGAGG->AddConstProperty("SCINTILLATIONYIELD2", 0.0);
  fGAGG->SetMaterialPropertiesTable(mptGAGG);
  
  std::vector<G4double> energySmall = { 2.0 * eV, 3.47 * eV };
 
  // Optical properties of LYSO
  std::vector<G4double> Energy_LYSO = {
	  1.0*eV,1.77*eV,2.07*eV,2.14*eV,2.21*eV,2.30*eV,
	  2.38*eV,2.48*eV,2.58*eV,2.70*eV,2.82*eV,2.88*eV,
	  2.95*eV,3.10*eV,3.26*eV,3.44*eV,4.08*eV
  };
  
  std::vector<G4double> COMPONENT_LYSO = {
	  0.00,0.00,0.05,0.10,0.15,0.20,
	  0.40,0.60,0.80,1.40,2.20,2.20,
	  2.00,0.60,0.00,0.00,0.00
  };

  //https://arxiv.org/pdf/1305.3010.pdf
  std::vector<G4double> absLYSO = { 20. * cm, 20. * cm };
  std::vector<G4double> refIdxLYSO = {1.82, 1.82};
  //G4double ABS_LENGTH_LYSO[nLYSO1] = {5.*m, 5.*m};

  G4MaterialPropertiesTable* mptLYSOCe = new G4MaterialPropertiesTable();
  mptLYSOCe->AddProperty("SCINTILLATIONCOMPONENT1", Energy_LYSO, COMPONENT_LYSO);
  mptLYSOCe->AddProperty("SCINTILLATIONCOMPONENT2", Energy_LYSO, COMPONENT_LYSO);
  
  mptLYSOCe->AddProperty("RINDEX", energySmall, refIdxLYSO);
  mptLYSOCe->AddProperty("ABSLENGTH", energySmall, absLYSO);
  mptLYSOCe->AddConstProperty("SCINTILLATIONYIELD", 33.2/keV);
  mptLYSOCe->AddConstProperty("RESOLUTIONSCALE", 1.0);
  //https://github.com/OpenGATE/Gate/blob/develop/Materials.xml
  //mptLYSOCe->AddConstProperty("RESOLUTIONSCALE", 4.41);
  mptLYSOCe->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 40*ns);
  mptLYSOCe->AddConstProperty("SCINTILLATIONTIMECONSTANT2", 40*ns);
  mptLYSOCe->AddConstProperty("SCINTILLATIONRISETIME1", 0.1*ns);
  mptLYSOCe->AddConstProperty("SCINTILLATIONRISETIME2", 0.1*ns);

  mptLYSOCe->AddConstProperty("SCINTILLATIONYIELD1", 1.0);
  mptLYSOCe->AddConstProperty("SCINTILLATIONYIELD2", 0.0);
  
  fLYSOCe->SetMaterialPropertiesTable(mptLYSOCe);
 
  mptLYSOCe->DumpTable();

  
  // Air
  // --------------------------------------------------

  std::vector<G4double> refractiveIndex = { 
    1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
    1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
    1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
    1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
    1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00
  };

  G4MaterialPropertiesTable *mpt = new G4MaterialPropertiesTable();
  mpt->AddProperty("RINDEX", photonEnergy, refractiveIndex);

  fAir->SetMaterialPropertiesTable(mpt);

  // --------------------------------------------------
  // Silicone
  // --------------------------------------------------

  std::vector<G4double> energy = {
    2.00 * eV, 2.03 * eV, 2.06 * eV, 2.09 * eV, 2.12 * eV, 2.15 * eV, 2.18 * eV,
    2.21 * eV, 2.24 * eV, 2.27 * eV, 2.30 * eV, 2.33 * eV, 2.36 * eV, 2.39 * eV,
    2.42 * eV, 2.45 * eV, 2.48 * eV, 2.51 * eV, 2.54 * eV, 2.57 * eV, 2.60 * eV,
    2.63 * eV, 2.66 * eV, 2.69 * eV, 2.72 * eV, 2.75 * eV, 2.78 * eV, 2.81 * eV,
    2.84 * eV, 2.87 * eV, 2.90 * eV, 2.93 * eV, 2.96 * eV, 2.99 * eV, 3.02 * eV,
    3.05 * eV, 3.08 * eV, 3.11 * eV, 3.14 * eV, 3.17 * eV, 3.20 * eV, 3.23 * eV,
    3.26 * eV, 3.29 * eV, 3.32 * eV, 3.35 * eV, 3.38 * eV, 3.41 * eV, 3.44 * eV,
    3.47 * eV
  };


  //--------------------------------------------------
  //  Polyethylene
  //--------------------------------------------------

  std::vector<G4double> refractiveIndexClad1 = { 1.49, 1.49 };

  std::vector<G4double> absClad = { 20.0 * m, 20.0 * m };

  // Add entries into properties table
  G4MaterialPropertiesTable* mptClad1 = new G4MaterialPropertiesTable();
  mptClad1->AddProperty("RINDEX", energySmall, refractiveIndexClad1);
  mptClad1->AddProperty("ABSLENGTH", energySmall, absClad);

  fPethylene->SetMaterialPropertiesTable(mptClad1);

  //--------------------------------------------------
  // Fluorinated Polyethylene
  //--------------------------------------------------

  std::vector<G4double> refractiveIndexClad2 = { 1.42, 1.42 };

  // Add entries into properties table
  G4MaterialPropertiesTable* mptClad2 = new G4MaterialPropertiesTable();
  mptClad2->AddProperty("RINDEX", energySmall, refractiveIndexClad2);
  mptClad2->AddProperty("ABSLENGTH", energySmall, absClad);

  fFPethylene->SetMaterialPropertiesTable(mptClad2);

  //--------------------------------------------------
  // Silicone
  //--------------------------------------------------

  std::vector<G4double> refractiveIndexSilicone = { 1.46, 1.46 };

  // Add entries into properties table
  G4MaterialPropertiesTable* mptSilicone = new G4MaterialPropertiesTable();
  mptSilicone->AddProperty("RINDEX", energySmall, refractiveIndexSilicone);
  mptSilicone->AddProperty("ABSLENGTH", energySmall, absClad);

  fSilicone->SetMaterialPropertiesTable(mptSilicone);

  
  
  //
  //--------------------------------------------------
  //  Polystyrene
  //--------------------------------------------------

  std::vector<G4double> refractiveIndexPS = { 1.50, 1.50 };

  std::vector<G4double> absPS = { 2. * cm, 2. * cm };

  std::vector<G4double> scintilFast = {
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
    1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0
  };

  // Add entries into properties table
  G4MaterialPropertiesTable* mptPolystyrene = new G4MaterialPropertiesTable();
  mptPolystyrene->AddProperty("RINDEX", energySmall, refractiveIndexPS);
  mptPolystyrene->AddProperty("ABSLENGTH", energySmall, absPS);
  mptPolystyrene->AddProperty("SCINTILLATIONCOMPONENT1", energy, scintilFast);
  mptPolystyrene->AddConstProperty("SCINTILLATIONYIELD", 10. / keV);
  mptPolystyrene->AddConstProperty("RESOLUTIONSCALE", 1.0);
  mptPolystyrene->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 10. * ns);

  fPolystyrene->SetMaterialPropertiesTable(mptPolystyrene);


  //NaI
  const G4int nEntries2 = 32;

  std::vector<G4double> NaI_PhotonEnergy =
            { 2.034*eV, 2.068*eV, 2.103*eV, 2.139*eV,
              2.177*eV, 2.216*eV, 2.256*eV, 2.298*eV,
              2.341*eV, 2.386*eV, 2.433*eV, 2.481*eV,
              2.532*eV, 2.585*eV, 2.640*eV, 2.697*eV,
              2.757*eV, 2.820*eV, 2.885*eV, 2.954*eV,
              3.026*eV, 3.102*eV, 3.181*eV, 3.265*eV,
              3.353*eV, 3.446*eV, 3.545*eV, 3.649*eV,
              3.760*eV, 3.877*eV, 4.002*eV, 4.136*eV };


  std::vector<G4double> NaI_RefractiveIndex =
            { 1.766488159,1.7679973633,1.769572262,1.7712157643,
    1.772977708,1.7748162458,1.7767350536,1.7787873582,
    1.7809299411,1.7832187719,1.7856621319,1.7882152532,
    1.7909943174,1.7939575559,1.797116742,1.8004847152,
    1.804137281,1.8080957536,1.8123177871,1.8169595385,
    1.8219866195,1.8275060062,1.8334867877,1.8401328865,
    1.8474304253,1.8555377073,1.864643371,1.8747742158
    ,1.8862752549,1.8992321063,1.9141084948,1.9313637927};

  std::vector<G4double> NaI_Absorption =
           {3.448*m,  4.082*m,  6.329*m,  9.174*m, 12.346*m, 13.889*m,
           15.152*m, 17.241*m, 18.868*m, 20.000*m, 26.316*m, 35.714*m,
           45.455*m, 47.619*m, 52.632*m, 52.632*m, 55.556*m, 52.632*m,
           52.632*m, 47.619*m, 45.455*m, 41.667*m, 37.037*m, 33.333*m,
           30.000*m, 28.500*m, 27.000*m, 24.500*m, 22.000*m, 19.500*m,
           17.500*m, 14.500*m };

  std::vector<G4double> NaI_ScintilFast =
            { 0.000134, 0.004432, 0.053991, 0.241971, 0.398942,
    0.000134, 0.004432,0.053991, 0.241971, 0.398942,
    0.000134, 0.004432, 0.053991, 0.241971, 0.398942,
    0.000134, 0.004432, 0.053991, 0.241971, 0.398942,
    0.000134, 0.004432, 0.053991, 0.241971, 0.398942,
    0.000134, 0.004432, 0.053991, 0.241971, 0.398942,
    0.000134, 0.004432 };

  std::vector<G4double> NaI_ScintilSlow =
            { 0.000010, 0.000020, 0.000030, 0.004000, 0.008000, 0.005000, 0.020000,0.001000,
    0.000010, 0.000020, 0.000030, 0.004000, 0.008000, 0.005000, 0.020000, 0.001000,
    0.000010, 0.000020, 0.000030, 0.004000, 0.008000, 0.005000, 0.020000,0.001000,
    0.000010, 0.000020, 0.000030, 0.004000, 0.008000, 0.005000, 0.020000,0.001000,};

  G4MaterialPropertiesTable* myMPT_NaI = new G4MaterialPropertiesTable();


  myMPT_NaI->AddProperty("RINDEX", NaI_PhotonEnergy, NaI_RefractiveIndex);
  myMPT_NaI->AddProperty("ABSLENGTH", NaI_PhotonEnergy, NaI_Absorption);
  myMPT_NaI->AddProperty("SCINTILLATIONCOMPONENT1", NaI_PhotonEnergy, NaI_ScintilFast);
  myMPT_NaI->AddProperty("SCINTILLATIONCOMPONENT2", NaI_PhotonEnergy, NaI_ScintilSlow);
  myMPT_NaI->AddConstProperty("SCINTILLATIONYIELD", 38. / keV);
  myMPT_NaI->AddConstProperty("RESOLUTIONSCALE", 1.0);
  myMPT_NaI->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 1. * ns);
  myMPT_NaI->AddConstProperty("SCINTILLATIONTIMECONSTANT2", 10. * ns);
  myMPT_NaI->AddConstProperty("SCINTILLATIONYIELD1", 0.8);
  myMPT_NaI->AddConstProperty("SCINTILLATIONYIELD2", 0.2);
  myMPT_NaI->AddConstProperty("SCINTILLATIONRISETIME1", 2.*ns);
  myMPT_NaI->AddConstProperty("SCINTILLATIONRISETIME2", 2.*ns);

  fNaI->SetMaterialPropertiesTable(myMPT_NaI);
 

}
