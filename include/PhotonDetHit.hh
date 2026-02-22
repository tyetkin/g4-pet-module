
#ifndef PhotonDetHit_h
#define PhotonDetHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"

#include "G4ThreeVector.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"

#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"

#include "tls.hh"


class PhotonDetHit : public G4VHit
{
  public:

    PhotonDetHit();
    PhotonDetHit(G4ThreeVector pArrive, G4double pWaveLength, G4float gloTime, G4float locTime, G4int pId);
    virtual ~PhotonDetHit();

    PhotonDetHit(const PhotonDetHit &right);
    const PhotonDetHit& operator=(const PhotonDetHit& right);

    G4int operator==(const PhotonDetHit& right) const;

    inline void *operator new(size_t);
    inline void operator delete(void *aHit);

    void Draw();
    void Print();

    inline void SetPos(G4ThreeVector xyz) { fPos = xyz; }
    inline G4ThreeVector GetPos() { return fPos; }

    inline void SetLocalTime(G4double t) { fLocTime = t; }
    inline G4double GetLocalTime() { return fLocTime; }
    
    inline void SetGlobalTime(G4double t) { fGloTime = t; }
    inline G4double GetGlobalTime() { return fGloTime; }
    
    inline void SetWaveLength(G4double w) { fWaveLength = w; }
    inline G4double GetWaveLength() { return fWaveLength; }
    
    inline void SetPhoDetId(G4int i) { fPhoDetId = i; }
    inline G4int GetPhoDetId() { return fPhoDetId; }
    
 
  private:

    // the wavelength of the photon
    //
    G4double      fWaveLength;
    // the arrival time of the photon
    G4float      fLocTime;
    G4float      fGloTime;
    // where the photon hit the detector (detector's coordinate)
    G4ThreeVector fPos;
    // the photon MPPC id
    G4int         fPhoDetId;
};

typedef G4THitsCollection<PhotonDetHit> PhotonDetHitsCollection;

extern G4ThreadLocal G4Allocator<PhotonDetHit>* PhotonDetHitAllocator;

inline void* PhotonDetHit::operator new(size_t)
{
  if(!PhotonDetHitAllocator)
      PhotonDetHitAllocator = new G4Allocator<PhotonDetHit>;
  return (void *) PhotonDetHitAllocator->MallocSingle();
}

inline void PhotonDetHit::operator delete(void *aHit)
{
  PhotonDetHitAllocator->FreeSingle((PhotonDetHit*) aHit);
}

#endif
