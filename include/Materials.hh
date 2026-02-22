
#ifndef Materials_h
#define Materials_h 1

#include "globals.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"

class Materials
{
  public:

    virtual ~Materials();
 
    static Materials* GetInstance();

    G4Material* GetMaterial(const G4String);
 
  private:
 
    Materials();

    void CreateMaterials();

  private:

    static Materials* fInstance;

    G4NistManager* fNistMan;

    G4Material* fAir;
    G4Material* fVacuum;

    G4Material* fPMMA;
    G4Material* fAcrylic;
    G4Material* fPethylene;
    G4Material* fFPethylene;
    G4Material* fPolystyrene;
    G4Material* fSilicone;
    G4Material* fCoating;
    G4Material* fSiO2;
    G4Material* fAl;
    G4Material* fNaI;
    G4Material* fLYSOCe;
    G4Material* fBGO;
    G4Material* fGAGG;

};

#endif /*Materials_h*/
