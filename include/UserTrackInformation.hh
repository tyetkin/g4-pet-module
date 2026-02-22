
#ifndef UserTrackInformation_h
#define UserTrackInformation_h 1

#include "G4VUserTrackInformation.hh"

#include "G4ThreeVector.hh"

class UserTrackInformation : public G4VUserTrackInformation
{

  public:

    UserTrackInformation();
    virtual ~UserTrackInformation();
 
};

#endif
