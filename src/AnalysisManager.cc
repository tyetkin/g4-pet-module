#include "AnalysisManager.hh"
#include "G4AutoLock.hh"
#include "G4Version.hh"
#include "G4SystemOfUnits.hh"

#include <vector>


using namespace std;

AnalysisManager* AnalysisManager::instance = 0;

namespace { 
  //Mutex to acquire access to singleton instance check/creation
  G4Mutex instanceMutex = G4MUTEX_INITIALIZER;
  //Mutex to acquire accss to histograms creation/access
  //It is also used to control all operations related to histos 
  //File writing and check analysis
  G4Mutex dataManipulationMutex = G4MUTEX_INITIALIZER;
}

AnalysisManager::AnalysisManager() 
:fFile(0), fTree(0)//, fPrimGenTree(0), fPrimDetTree(0)
{
  // event = new Event();
  fNumPhot = 0;
  fNumPrim = 0;
  fNumXtal = 0;
  for(G4int k = 0; k < MAX_PHOT; k++){
    
    fWavelength[k] = 0.;
    fGlobalTime[k] = 0.;
    fLocalTime[k] = 0.;
    
    fHitPosX[k] = 0.;
    fHitPosY[k] = 0.;
    //fHitPosZ[k] = 0.;
    
    fDetId[k] = -999;
    
    fPrimDetTime[k] = 0.;
    
    fPrimDetPosX[k] = 0.;
    fPrimDetPosY[k] = 0.;
    fPrimDetPosZ[k] = 0.;
    
    fPrimDetEn[k] = 0.;

    fXtalTime[k] = 0.;
    fXtalPosX[k] = 0.;
    fXtalPosY[k] = 0.;
    fXtalPosZ[k] = 0.;
    fXtalEn[k] = 0.;
  }
  // fPrimGenPosX = 0.;
  // fPrimGenPosY = 0.;
  // fPrimGenPosZ = 0.;
  // fPrimGenEn = 0.;
} 

AnalysisManager::~AnalysisManager()
{
  //No need to mutex, this is a real singleton. 
  if (fTree) delete fTree; 
  // if (fPrimGenTree) delete fPrimGenTree; 
  if (fPrimDetTree) delete fPrimDetTree; 
  if (fXtalTree) delete fXtalTree; 
  if (fFile) delete fFile;
}

AnalysisManager* AnalysisManager::getInstance()
{
  G4AutoLock l(&instanceMutex);
  if (instance == 0) 
    instance = new AnalysisManager();
  return instance;
}

void AnalysisManager::Book(G4String name, G4String activeDetType)
{
  //Booking of histograms has to be protected.
  //In addition there are issues with ROOT that is 
  //heavily non thread-safe. In particular I/O related operations
  //are not thread safe. To avoid problems let's mutex everything
  //here
  G4AutoLock l(&dataManipulationMutex);
  if (!fFile){
    G4cout << "==========" << name << G4endl;
    TString filename = name; 
    ROOT::EnableThreadSafety();
    fFile = new TFile(filename,"RECREATE");
  }

  if(!fTree){
    fTree = new TTree("phot_tree","Global results");    
    fTree->Branch("NumPhot",&fNumPhot,"NumPhot/I");      
    fTree->Branch("Wavelength",fWavelength,"Wavelength[20000]/F");
    fTree->Branch("HitPosX",fHitPosX,"HitPosX[20000]/F");
    fTree->Branch("HitPosY",fHitPosY,"HitPosY[20000]/F");
    fTree->Branch("LocalTime",fLocalTime,"LocalTime[20000]/F");
    fTree->Branch("GlobalTime",fGlobalTime,"GlobalTime[20000]/F");
    fTree->Branch("DetId",fDetId,"DetId[20000]/I");
  }

  if (!fPrimDetTree){
    fPrimDetTree = new TTree("primDet_tree","Global results");
    fPrimDetTree->Branch("NumPrim",&fNumPrim,"NumPrim/I");
    fPrimDetTree->Branch("PrimDetTime",fPrimDetTime,"PrimDetTime[NumPrim]/F");
    fPrimDetTree->Branch("PrimDetPosX",fPrimDetPosX,"PrimDetPosX[NumPrim]/F");
    fPrimDetTree->Branch("PrimDetPosY",fPrimDetPosY,"PrimDetPosY[NumPrim]/F");
    fPrimDetTree->Branch("PrimDetPosZ",fPrimDetPosZ,"PrimDetPosZ[NumPrim]/F");
    fPrimDetTree->Branch("PrimDetEn",fPrimDetEn,"Energy[NumPrim]/F");
  }

  if (!fXtalTree){
    fXtalTree = new TTree("xtal_tree","Global results");
    fXtalTree->Branch("NumXtal",&fNumXtal,"NumXtal/I");
    fXtalTree->Branch("XtalTime",fXtalTime,"XtalTime[NumXtal]/F");
    fXtalTree->Branch("XtalPosX",fXtalPosX,"XtalPosX[NumXtal]/F");
    fXtalTree->Branch("XtalPosY",fXtalPosY,"XtalPosY[NumXtal]/F");
    fXtalTree->Branch("XtalPosZ",fXtalPosZ,"XtalPosZ[NumXtal]/F");
    fXtalTree->Branch("XtalEn",fXtalEn,"Energy[NumXtal]/F");
  }
  return;
}

void AnalysisManager::FillTree(int evtNo, PhotonDetHitsCollection* phc){
  G4int n_hit = phc->entries();
  G4cout << "Nb. of SiPM Hits " << n_hit << G4endl;
  fNumPhot = n_hit;
  if(fNumPhot > MAX_PHOT){
    G4cout << "Error! Number of hits exceeds " << MAX_PHOT << G4endl;
    exit(1);
  }
  for(G4int iHit=0; iHit<n_hit; iHit++){
      fDetId[iHit] = (*phc)[iHit]->GetPhoDetId();
      // G4cout << "HIT DETID " << fDetId[iHit] << G4endl;
      fWavelength[iHit] = ((*phc)[iHit]->GetWaveLength());
      fLocalTime[iHit] = ((*phc)[iHit]->GetLocalTime());
      fGlobalTime[iHit] = ((*phc)[iHit]->GetGlobalTime());
      //G4cout << "RESULT " << fTime[iHit]-fGlobalTime << G4endl; 
      G4ThreeVector pos = (*phc)[iHit]->GetPos();
      fHitPosX[iHit] = pos.x()/mm;
      fHitPosY[iHit] = pos.y()/mm;
      //fHitPosZ[iHit] = pos.z()/mm;
  }
  if(fNumPhot > 0)fTree->Fill();
  fNumPhot = 0;
  for(G4int k = 0; k < MAX_PHOT; k++){
    fWavelength[k] = 0.;
    fLocalTime[k] = 0.;
    fGlobalTime[k] = 0.;
    fHitPosX[k] = 0.;
    fHitPosY[k] = 0.;
    //fHitPosZ[k] = 0.;
    fDetId[k] = -999;
  }
  Clear();
}


void AnalysisManager::FillPrimDetTree(int evtNo, PrimDetHitsCollection* pdhc){
  G4int n_hit = pdhc->entries();
  G4cout << "Nb. of PrimaryDetector Hits " << n_hit << G4endl;
  // G4cout << "Started filling " << endl;
  fNumPrim = n_hit;
  for(G4int iHit=0; iHit<n_hit; iHit++){
      fPrimDetTime[iHit] = ((*pdhc)[iHit]->GetTime())/ns;
      G4ThreeVector pos = ((*pdhc)[iHit]->GetPos());
      fPrimDetPosX[iHit] = pos.x()/mm;
      fPrimDetPosY[iHit] = pos.y()/mm;
      fPrimDetPosZ[iHit] = pos.z()/mm;
      G4cout << fPrimDetPosX[iHit] << " " << fPrimDetPosY[iHit] << G4endl;
      fPrimDetEn[iHit] = ((*pdhc)[iHit]->GetEnergy())/keV;
  }
  if(fNumPrim > 0)fPrimDetTree->Fill();
  fNumPrim = 0;
  for(G4int k = 0; k < MAX_PHOT; k++){
    fPrimDetTime[k] = 0.;
    fPrimDetPosX[k] = 0.;
    fPrimDetPosY[k] = 0.;
    fPrimDetPosZ[k] = 0.;
    fPrimDetEn[k] = 0.;
  }
  // G4cout << "Finished filling " << endl;
}

void AnalysisManager::FillXtalTree(int evtNo, XtalHitsCollection* xtalc){
  G4int n_hit = xtalc->entries();
  G4cout << "Nb. of Xtal Hits " << n_hit << G4endl;
  // G4cout << "Started filling " << endl;
  fNumXtal = n_hit;
  for(G4int iHit=0; iHit<n_hit; iHit++){
      fXtalTime[iHit] = ((*xtalc)[iHit]->GetTime())/ns;
      G4ThreeVector pos = ((*xtalc)[iHit]->GetPos());
      fXtalPosX[iHit] = pos.x()/mm;
      fXtalPosY[iHit] = pos.y()/mm;
      fXtalPosZ[iHit] = pos.z()/mm;
      G4cout << fXtalPosX[iHit] << " " << fXtalPosY[iHit] << G4endl;
      fXtalEn[iHit] = ((*xtalc)[iHit]->GetEnergy())/keV;
  }
  if(fNumXtal > 0)fXtalTree->Fill();
  fNumXtal = 0;
  for(G4int k = 0; k < MAX_PHOT; k++){
    fXtalTime[k] = 0.;
    fXtalPosX[k] = 0.;
    fXtalPosY[k] = 0.;
    fXtalPosZ[k] = 0.;
    fXtalEn[k] = 0.;
  }
}

void AnalysisManager::Clear()
{
  fNumPhot = 0;
  for(G4int k = 0; k < MAX_PHOT; k++){
    fWavelength[k] = 0.;
    fGlobalTime[k] = 0.;
    fLocalTime[k] = 0.;
    fHitPosX[k] = 0.;
    fHitPosY[k] = 0.;
    //fHitPosZ[k] = 0.;
    fDetId[k] = -999;
  }
}

void AnalysisManager::CloseFile()
{
  G4AutoLock l(&dataManipulationMutex);
  
  if (!fFile) //file not created at all: e.g. for a vis-only execution
    return;
  if (!fFile->IsOpen()){
      G4Exception("AnalysisManager::CloseFile()"," ",FatalException,
                  "Trying to close a ROOT file which is not open");
      return;
  }
  fFile->cd();    
  if (fTree) fTree->Write(fTree->GetName());
  if (fPrimDetTree) fPrimDetTree->Write(fPrimDetTree->GetName());
  if (fXtalTree) fXtalTree->Write(fXtalTree->GetName());
  fFile->Close();
}
