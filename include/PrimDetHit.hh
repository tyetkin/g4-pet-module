
#ifndef PrimDetHit_h
#define PrimDetHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"

#include "G4ThreeVector.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"

#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"

#include "tls.hh"

class PrimDetHit : public G4VHit
{
  public:

    PrimDetHit();
    PrimDetHit(G4ThreeVector pArrive, G4double pEnergy, G4double pTime);
    virtual ~PrimDetHit();

    PrimDetHit(const PrimDetHit &right);
    const PrimDetHit& operator=(const PrimDetHit& right);

    G4int operator==(const PrimDetHit& right) const;

    inline void *operator new(size_t);
    inline void operator delete(void *aHit);

    void Draw();
    void Print();

    inline void SetPos(G4ThreeVector xyz) { fPos = xyz; }
    inline G4ThreeVector GetPos() { return fPos; }

    inline void SetTime(G4double t) { fTime = t; }
    inline G4double GetTime() { return fTime; }
    
    inline void SetEnergy(G4double en)   {fEne = en;}
    inline G4double      GetEnergy()    {return fEne; }


  private:

    G4double      fTime;
    G4double      fEne;
    G4ThreeVector fPos;
};

typedef G4THitsCollection<PrimDetHit> PrimDetHitsCollection;

extern G4ThreadLocal G4Allocator<PrimDetHit>* PrimDetHitAllocator;

inline void* PrimDetHit::operator new(size_t)
{
  if(!PrimDetHitAllocator)
      PrimDetHitAllocator = new G4Allocator<PrimDetHit>;
  return (void *) PrimDetHitAllocator->MallocSingle();
}

inline void PrimDetHit::operator delete(void *aHit)
{
  PrimDetHitAllocator->FreeSingle((PrimDetHit*) aHit);
}

#endif
