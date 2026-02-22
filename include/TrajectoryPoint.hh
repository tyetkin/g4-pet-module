
#ifndef TrajectoryPoint_h_seen
#define TrajectoryPoint_h_seen 1

#include "globals.hh"

#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4TrajectoryPoint.hh"

#include "G4StepStatus.hh"

class G4Track;
class G4Step;
class G4VProcess;

class TrajectoryPoint : public G4TrajectoryPoint {

  public: // without description

    TrajectoryPoint();
    TrajectoryPoint(const G4Track* );
    TrajectoryPoint(const G4Step* );
    TrajectoryPoint(const TrajectoryPoint &right);
    virtual ~TrajectoryPoint();

    inline void *operator new(size_t);
    inline void operator delete(void *aTrajectoryPoint);
    inline int operator==(const TrajectoryPoint& right) const
    { return (this==&right); };

    inline G4double GetTime() const { return fTime; };
    inline const G4ThreeVector GetMomentum() const { return fMomentum; };
    inline G4StepStatus GetStepStatus() const { return fStepStatus; };
    inline G4String GetVolumeName() const { return fVolumeName; };

   virtual const std::map<G4String,G4AttDef>* GetAttDefs() const;
   virtual std::vector<G4AttValue>* CreateAttValues() const;

  private:

    G4double fTime;
    G4ThreeVector fMomentum;
    G4StepStatus fStepStatus;
    G4String fVolumeName;

};

extern G4ThreadLocal G4Allocator<TrajectoryPoint>* TrajPointAllocator;

inline void* TrajectoryPoint::operator new(size_t)
{
    if(!TrajPointAllocator)
      TrajPointAllocator = new G4Allocator<TrajectoryPoint>;
    return (void *) TrajPointAllocator->MallocSingle();
}

inline void TrajectoryPoint::operator delete(void *aTrajectoryPoint)
{
    TrajPointAllocator->FreeSingle(
        (TrajectoryPoint *) aTrajectoryPoint);
}

#endif
