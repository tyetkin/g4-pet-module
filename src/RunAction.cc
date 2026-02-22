
#include "RunAction.hh"
#include "AnalysisManager.hh"
#include "DetectorConstruction.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

RunAction::RunAction()
 : G4UserRunAction(), fDet(0)
{
   G4RunManager::GetRunManager()->SetPrintProgress(1);
}

RunAction::RunAction(DetectorConstruction* det)
: fDet(det)
{
  // set printing event number per each event
  G4RunManager::GetRunManager()->SetPrintProgress(1);
}


RunAction::~RunAction()
{
}

void RunAction::BeginOfRunAction(const G4Run* /*run*/)
{
}

void RunAction::EndOfRunAction(const G4Run* /*run*/)
{}

