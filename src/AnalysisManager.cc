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
  
  /*
  if (!fXtalBGOTree){
    fXtalBGOTree = new TTree("xtal_bgo_tree","Global results");
    fXtalBGOTree->Branch("NXtalBGOHits",&fNumXtalBGOHit,"NXtalBGOHits/I");
    fXtalBGOTree->Branch("XtalBGODetId",fXtalBGOId,"XtalBGODetId[NXtalBGOHits]/I");
    fXtalBGOTree->Branch("EnergyBGO",fEneXtalBGO,"EnergyBGO[NXtalBGOHits]/F");
    fXtalBGOTree->Branch("GlobalTimeXtalBGO",fTimeXtalBGO,"GlobalTimeXtalBGO[NXtalBGOHits]/F");
    fXtalBGOTree->Branch("EnergySpecBGO",fEneSpecXtalBGO,"EnergySpecBGO[NXtalBGOHits]/F");
    //fXtalBGOTree->Branch("WeightBGO",fWeightBGO,"WeightBGO[NXtalBGOHits]/F");
  }
  if (!fXtalGAGGTree){
    fXtalGAGGTree = new TTree("xtal_gagg_tree","Global results");
    fXtalGAGGTree->Branch("NXtalGAGGHits",&fNumXtalGAGGHit,"NXtalGAGGHits/I");
    fXtalGAGGTree->Branch("XtalGAGGDetId",fXtalGAGGId,"XtalGAGGDetId[NXtalGAGGHits]/I");
    fXtalGAGGTree->Branch("EnergyGAGG",fEneXtalGAGG,"EnergyGAGG[NXtalGAGGHits]/F");
    fXtalGAGGTree->Branch("GlobalTimeXtalGAGG",fTimeXtalGAGG,"GlobalTimeXtalGAGG[NXtalGAGGHits]/F");
    fXtalGAGGTree->Branch("EnergySpecGAGG",fEneSpecXtalGAGG,"EnergySpecGAGG[NXtalGAGGHits]/F");
    //fXtalGAGGTree->Branch("WeightGAGG",fWeightGAGG,"WeightGAGG[NXtalGAGGHits]/F");
  }
  */
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
/*
void AnalysisManager::FillPrimGenTree(float posX, float posY, float posZ, float energy){
  
  fPrimGenPosX = posX;
  fPrimGenPosY = posY;
  fPrimGenPosZ = posZ;
  fPrimGenEn = energy;
  fPrimGenTree->Fill();
  fPrimGenPosX = 0.;
  fPrimGenPosY = 0.;
  fPrimGenPosZ = 0.;
  fPrimGenEn = 0.;
}
*/
/*
void AnalysisManager::AddEventXtalBGO(vector<float> ene,
                                   vector<float> enesp, vector<float> gtime)
                                   //std::vector<float> enesp, std::vector<float> w)
{
  G4cout << "AnalysisManager::AddEventXtal started " << endl;
  //G4AutoLock l(&dataManipulationMutex);
  fNumXtalBGOHit = (G4int) ene.size();
  G4cout << "AnalysisManager::AddEventXtalBGO " << fNumXtalBGOHit << " hits " << G4endl;
  for (G4int i = 0; i < fNumXtalBGOHit; i++){
      fEneXtalBGO[i] = ene[i];
      fTimeXtalBGO[i] = gtime[i];
      fEneSpecXtalBGO[i] = enesp[i];
  }
  G4cout << "Filling tree" << endl;
  fXtalBGOTree->Fill();
  G4cout << "AnalysisManager::AddEventXtalBGO finished " << endl;
}

void AnalysisManager::AddEventXtalGAGG(vector<float> ene,
                                   vector<float> enesp, vector<float> gtime)
                                   //std::vector<float> enesp, std::vector<float> w)
{
  G4cout << "AnalysisManager::AddEventXtalGAGG started " << endl;
  //G4AutoLock l(&dataManipulationMutex);
  fNumXtalGAGGHit = (G4int) ene.size();
  G4cout << "AnalysisManager::AddEventXtalGAGG " << fNumXtalGAGGHit << " hits " << G4endl;
  for (G4int i = 0; i < fNumXtalGAGGHit; i++){
      fEneXtalGAGG[i] = ene[i];
      fTimeXtalGAGG[i] = gtime[i];
      fEneSpecXtalGAGG[i] = enesp[i];
  }
  G4cout << "Filling tree" << endl;
  fXtalGAGGTree->Fill();
  G4cout << "AnalysisManager::AddEventXtalGAGG finished " << endl;
}
*/

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
  // if (fPrimGenTree) fPrimGenTree->Write(fPrimGenTree->GetName());
  //if (fXtalBGOTree) fXtalBGOTree->Write(fXtalBGOTree->GetName());
  //if (fXtalGAGGTree) fXtalGAGGTree->Write(fXtalGAGGTree->GetName());
  fFile->Close();
}
