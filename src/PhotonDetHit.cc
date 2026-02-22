
#include "PhotonDetHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"


G4ThreadLocal G4Allocator<PhotonDetHit>* PhotonDetHitAllocator=0;

PhotonDetHit::PhotonDetHit()
{
  fWaveLength = 0.;
  fLocTime       = 0.;
  fGloTime       = 0.;
  fPos        = G4ThreeVector(0., 0., 0.);
  fPhoDetId    = -999;
}

PhotonDetHit::PhotonDetHit(G4ThreeVector pArrive,
                           G4double pWaveLength, 
                           G4float gloTime, 
                           G4float locTime, 
                           G4int pId)
{
  fWaveLength  = pWaveLength;
  fPos         = pArrive;
  fLocTime     = locTime;
  fGloTime     = gloTime;
  fPhoDetId    = pId;
}

PhotonDetHit::~PhotonDetHit() { }

PhotonDetHit::PhotonDetHit(const PhotonDetHit &right)
  : G4VHit()
{
  *this = right;
}

const PhotonDetHit& PhotonDetHit::operator=(const PhotonDetHit &right)
{
  fWaveLength  = right.fWaveLength;  
  fPos         = right.fPos;
  fLocTime     = right.fLocTime;
  fGloTime     = right.fGloTime;
  fPhoDetId    = right.fPhoDetId;

  return *this;
}

G4int PhotonDetHit::operator==(const PhotonDetHit& right) const
{
  return (
         fWaveLength  == right.fWaveLength &&         
         fPos         == right.fPos        &&
         fLocTime     == right.fLocTime    &&
         fGloTime     == right.fGloTime    &&
	       fPhoDetId    == right.fPhoDetId
         );
}

void PhotonDetHit::Draw()
{
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager){
    G4Circle circle(fPos);
    circle.SetScreenSize(4.);
    circle.SetFillStyle(G4Circle::filled);
    G4Colour colour(1.,0.,0.);
    G4VisAttributes attribs(colour);
    circle.SetVisAttributes(attribs);
    pVVisManager->Draw(circle);
  }
}

void PhotonDetHit::Print()
{
}
