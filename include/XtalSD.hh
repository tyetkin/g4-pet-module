
#ifndef XtalSD_h
#define XtalSD_h 1

#include "XtalHit.hh"

#include "G4VSensitiveDetector.hh"
#include <unordered_set>

class G4Step;
class G4HCofThisEvent;
class DetectorConstruction;

class XtalSD : public G4VSensitiveDetector
{
  public:

    XtalSD(const G4String& name, const G4String& hitsCollectionName, G4int numDets, DetectorConstruction* det);
    virtual ~XtalSD();

    virtual void Initialize(G4HCofThisEvent* );

    virtual G4bool ProcessHits(G4Step* , G4TouchableHistory* );
    virtual void   EndOfEvent(G4HCofThisEvent* hitCollection);

  private:

    XtalHitsCollection* fXtalHitCollection;
    G4int  fNumDets;
    G4double hPlanck;
    G4double c_light;
    std::unordered_set<G4int> fRecordedPrimaryGammaTracks;
    DetectorConstruction* fDet;
};

#endif
