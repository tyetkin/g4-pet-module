
#include "PrimDetHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"


G4ThreadLocal G4Allocator<PrimDetHit>* PrimDetHitAllocator=0;

PrimDetHit::PrimDetHit()
{
  fEne  = 0.;
  fTime = 0.;
  fPos  = G4ThreeVector(0., 0., 0.);
}

PrimDetHit::PrimDetHit(G4ThreeVector pArrive, G4double pEnergy, G4double pTime)
{
  fPos  = pArrive;
  fTime = pTime;
  fEne  = pEnergy;
}

PrimDetHit::~PrimDetHit() { }

PrimDetHit::PrimDetHit(const PrimDetHit &right)
  : G4VHit()
{
  *this = right;
}

const PrimDetHit& PrimDetHit::operator=(const PrimDetHit &right)
{
  fPos  = right.fPos;
  fEne  = right.fEne;
  fTime = right.fTime;
  return *this;
}

G4int PrimDetHit::operator==(const PrimDetHit& right) const
{
  return ((fPos == right.fPos)&&(fTime == right.fTime) && (fEne == right.fEne));
}

void PrimDetHit::Draw()
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

void PrimDetHit::Print()
{}
