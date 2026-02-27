
#include "XtalHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"


G4ThreadLocal G4Allocator<XtalHit>* XtalHitAllocator=0;

XtalHit::XtalHit()
{
  fLocTime       = 0.;
  fGloTime       = 0.;
  fPos        = G4ThreeVector(0., 0., 0.);
  fXtalId    = -999;
  fEnergy    = 0.;
}

XtalHit::XtalHit(G4ThreeVector pArrive,
                           G4float gloTime, G4float locTime, 
                           G4int pId)
{
  fPos         = pArrive;
  fLocTime     = locTime;
  fGloTime     = gloTime;
  fXtalId    = pId;
  fEnergy    = 0.;
}

XtalHit::~XtalHit() { }

XtalHit::XtalHit(const XtalHit &right)
  : G4VHit()
{
  *this = right;
}

const XtalHit& XtalHit::operator=(const XtalHit &right)
{
  fPos         = right.fPos;
  fLocTime     = right.fLocTime;
  fGloTime     = right.fGloTime;
  fXtalId    = right.fXtalId;
  fEnergy    = right.fEnergy;

  return *this;
}

G4int XtalHit::operator==(const XtalHit& right) const
{
  return (
          fPos         == right.fPos        &&
          fLocTime     == right.fLocTime    &&
          fGloTime     == right.fGloTime    &&
	        fXtalId    == right.fXtalId
         );
}

void XtalHit::Draw()
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

void XtalHit::Print()
{}
