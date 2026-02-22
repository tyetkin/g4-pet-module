#ifndef PrimDetSD_h
#define PrimDetSD_h 1

#include "G4VSensitiveDetector.hh"

#include "PrimDetHit.hh"

#include <vector>

class G4Step;
class G4HCofThisEvent;

class PrimDetSD : public G4VSensitiveDetector
{
  public:
    PrimDetSD(const G4String& name, const G4String& hitsCollectionName);
    virtual ~PrimDetSD();
  
    // methods from base class
    virtual void   Initialize(G4HCofThisEvent* hitCollection);
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
    virtual void   EndOfEvent(G4HCofThisEvent* hitCollection);

  private:
    PrimDetHitsCollection* fPrimDetHitsCollection;
    G4int fHCID;
};

#endif

