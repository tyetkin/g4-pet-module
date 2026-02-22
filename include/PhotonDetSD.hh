
#ifndef PhotonDetSD_h
#define PhotonDetSD_h 1

#include "PhotonDetHit.hh"

#include "G4VSensitiveDetector.hh"

class G4Step;
class G4HCofThisEvent;

class PhotonDetSD : public G4VSensitiveDetector
{
  public:

    PhotonDetSD(const G4String& name, const G4String& hitsCollectionName, G4int numDets);
    virtual ~PhotonDetSD();

    virtual void Initialize(G4HCofThisEvent* );

    virtual G4bool ProcessHits(G4Step* , G4TouchableHistory* );
    virtual void   EndOfEvent(G4HCofThisEvent* hitCollection);

  private:

    PhotonDetHitsCollection* fPhotonDetHitCollection;
    G4int  fNumDets;
    G4double hPlanck;
    G4double c_light;
};

#endif
