
#ifndef XtalHit_h
#define XtalHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"

#include "G4ThreeVector.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"

#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"

#include "tls.hh"
#include "G4TouchableHistory.hh"

class XtalHit : public G4VHit
{
  public:

    XtalHit();
    XtalHit(G4ThreeVector pArrive, G4float gloTime, G4float locTime, G4int pId);
    virtual ~XtalHit();

    XtalHit(const XtalHit &right);
    const XtalHit& operator=(const XtalHit& right);

    G4int operator==(const XtalHit& right) const;

    inline void *operator new(size_t);
    inline void operator delete(void *aHit);

    void Draw();
    void Print();

    inline void SetPos(G4ThreeVector xyz) { fPos = xyz; }
    inline G4ThreeVector GetPos() { return fPos; }

    inline void SetLocalTime(G4float t) { fLocTime = t; }
    inline G4float GetLocalTime() { return fLocTime; }
    
    inline void SetGlobalTime(G4float t) { fGloTime = t; }
    inline G4float GetGlobalTime() { return fGloTime; }

    inline void SetEnergy(G4double e) { fEnergy = e; }
    inline G4double GetEnergy() { return fEnergy; }

    // Alias so AnalysisManager can call GetTime()
    inline G4float GetTime() { return fGloTime; }
 
  private:

    // the arrival time of the particle
    G4float      fLocTime;
    G4float      fGloTime;
    // where the particle hit the detector (detector's coordinate)
    G4ThreeVector fPos;
    // the particle id
    G4int         fXtalId;
    G4double      fEnergy;
};

typedef G4THitsCollection<XtalHit> XtalHitsCollection;

extern G4ThreadLocal G4Allocator<XtalHit>* XtalHitAllocator;

inline void* XtalHit::operator new(size_t)
{
  if(!XtalHitAllocator)
      XtalHitAllocator = new G4Allocator<XtalHit>;
  return (void *) XtalHitAllocator->MallocSingle();
}

inline void XtalHit::operator delete(void *aHit)
{
  XtalHitAllocator->FreeSingle((XtalHit*) aHit);
}

#endif
