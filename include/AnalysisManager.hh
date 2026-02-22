#ifndef AnalysisManager_h
#define AnalysisManager_h 

#include "globals.hh"
#include "PhotonDetSD.hh"
#include "PrimDetSD.hh"
//#include "XtalBGOSD.hh"
//#include "XtalGAGGSD.hh"
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"


using namespace std;

class AnalysisManager{

 public:
   AnalysisManager();
   virtual ~AnalysisManager();
 
 public: 
   static AnalysisManager* getInstance();
   static void dispose();
   
   void Book(G4String name, G4String activeDetType);
   
   void FillTree(int evtNo, PhotonDetHitsCollection* phc);
   void FillPrimDetTree(int evtNo, PrimDetHitsCollection* pdhc);
  //  void FillPrimGenTree(float posX, float posY, float posZ, float energy);
   
   //void AddEventXtalBGO(vector<float> ene, vector<float> enesp, vector<float> gtime);
   //void AddEventXtalGAGG(vector<float> ene, vector<float> enesp, vector<float> gtime);

   void CloseFile(); 
 
 private:

   void Clear();
   static AnalysisManager* instance;
   TFile* fFile;   
   TTree* fTree;
   //tree to hold info from thin foil before detector.
   TTree* fPrimDetTree;
   //tree to hold info from primary generator action.
  //  TTree* fPrimGenTree;
   //TTree* fXtalBGOTree;
   //TTree* fXtalGAGGTree;
   static const int MAX_PHOT = 20000;
   
   int fNumPhot;
   float fWavelength[MAX_PHOT];
   float fLocalTime[MAX_PHOT];
   float fGlobalTime[MAX_PHOT];
   int fDetId[MAX_PHOT];
   float fHitPosX[MAX_PHOT];
   float fHitPosY[MAX_PHOT];
   
  //  float fPrimGenPosX;
  //  float fPrimGenPosY;
  //  float fPrimGenPosZ;
  //  float fPrimGenEn;
   
   int fNumPrim;
   float fPrimDetTime[MAX_PHOT];
   float fPrimDetPosX[MAX_PHOT];
   float fPrimDetPosY[MAX_PHOT];
   float fPrimDetPosZ[MAX_PHOT];
   float fPrimDetEn[MAX_PHOT];
   /*
   float fXtalBGOEdep;
   float fXtalGAGGEdep;
   int fNumXtalBGOHit;
   int fXtalBGOId[10];
   float fEneXtalBGO[10];
   float fTimeXtalBGO[10];
   float fEneSpecXtalBGO[10];
   float fWeightBGO[10];
   
   int fNumXtalGAGGHit;
   int fXtalGAGGId[10];
   float fEneXtalGAGG[10];
   float fTimeXtalGAGG[10];
   float fEneSpecXtalGAGG[10];
   float fWeightGAGG[10];
   */

   
};


#endif
