#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"

#include "DetectorConstruction.hh"

#include "globals.hh"

class G4Track;

class EventAction : public G4UserEventAction
{

public:
  EventAction(DetectorConstruction* det);
  virtual ~EventAction();

  virtual void  BeginOfEventAction(const G4Event* event);
  virtual void  EndOfEventAction(const G4Event* event);
     
private:
  
  void Clear(){};

  DetectorConstruction* fDet;
  G4int  fPhoDetHCID; 
  G4int  fPrimDetHCID; 
  //G4int fXtalBGOCollID;
  //G4int fXtalGAGGCollID;

};
                     
#endif

    
