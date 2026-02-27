#ifndef AnalysisManager_h
#define AnalysisManager_h 

#include "globals.hh"
#include "PhotonDetSD.hh"
#include "PrimDetSD.hh"
#include "XtalSD.hh"
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
   void FillXtalTree(int evtNo, XtalHitsCollection* xtalc);

   void CloseFile(); 
 
 private:

   void Clear();
   static AnalysisManager* instance;
   TFile* fFile;   
   TTree* fTree;
   TTree* fPrimDetTree;
   TTree* fXtalTree;
   static const int MAX_PHOT = 50000;
   
   int fNumPhot;
   float fWavelength[MAX_PHOT];
   float fLocalTime[MAX_PHOT];
   float fGlobalTime[MAX_PHOT];
   int fDetId[MAX_PHOT];
   float fHitPosX[MAX_PHOT];
   float fHitPosY[MAX_PHOT];
   
   
   int fNumPrim;
   float fPrimDetTime[MAX_PHOT];
   float fPrimDetPosX[MAX_PHOT];
   float fPrimDetPosY[MAX_PHOT];
   float fPrimDetPosZ[MAX_PHOT];
   float fPrimDetEn[MAX_PHOT];

   int fNumXtal;
   float fXtalTime[MAX_PHOT];
   float fXtalPosX[MAX_PHOT];
   float fXtalPosY[MAX_PHOT];
   float fXtalPosZ[MAX_PHOT];
   float fXtalEn[MAX_PHOT];
};


#endif
