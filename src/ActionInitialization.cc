
#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "EventAction.hh"
//#include "MySteppingAction.hh"
#include "RunAction.hh"
#include "DetectorConstruction.hh"
#include "TrackingAction.hh"
#include "StackingAction.hh"

ActionInitialization::ActionInitialization(DetectorConstruction* det)
 : G4VUserActionInitialization(), fDet(det)
{}

ActionInitialization::~ActionInitialization()
{}

void ActionInitialization::BuildForMaster() const
{
  SetUserAction(new RunAction);
}

void ActionInitialization::Build() const
{
  SetUserAction(new PrimaryGeneratorAction);
  SetUserAction(new RunAction(fDet));
  EventAction* eventaction = new EventAction(fDet);
  SetUserAction(new TrackingAction);
  SetUserAction(new StackingAction());
  //SetUserAction(new MySteppingAction());
  SetUserAction(eventaction); 
}

