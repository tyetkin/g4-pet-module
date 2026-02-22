
#include "PrimaryGeneratorAction.hh"

#include "G4RunManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"

#include "G4GeneralParticleSource.hh"

#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "Randomize.hh"

#include "AnalysisManager.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction()
 : G4VUserPrimaryGeneratorAction(),
   fGeneralParticleSource(0)
{
  fGeneralParticleSource  = new G4GeneralParticleSource();

}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fGeneralParticleSource;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  
  fGeneralParticleSource->GeneratePrimaryVertex(anEvent);
  G4ThreeVector primPos = fGeneralParticleSource->GetParticlePosition();
  G4double primEn = fGeneralParticleSource->GetParticleEnergy();
  G4double primTime = fGeneralParticleSource->GetParticleTime();
  G4String primName = fGeneralParticleSource->GetParticleDefinition()->GetParticleName();
  G4cout << "primary " <<  primName 
         << " primEn " << primEn/keV 
         << " primTime " << primTime/ns
         << " primpos " << primPos.x()/mm << "\t" << primPos.y()/mm << G4endl;
  // AnalysisManager::getInstance()->FillPrimGenTree(primPos.x()/mm, primPos.y()/mm, primPos.z()/mm, primEn/keV);
  //fGeneralParticleSource->SetParticleTime(200*ns); 
}
