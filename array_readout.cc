
#include "ActionInitialization.hh"
#include "DetectorConstruction.hh"
#include "AnalysisManager.hh"

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIcommand.hh"
#include "Randomize.hh"
#include "G4SystemOfUnits.hh"


#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "FTFP_BERT.hh"
#include "G4EmStandardPhysics_option4.hh"


#include "G4OpticalParameters.hh"
#include "G4OpticalPhysics.hh"
#include "G4RunManagerFactory.hh"

#include "G4String.hh"
#include "G4Types.hh"
#include "G4UImanager.hh"

int main(int argc, char** argv)
{
  // detect interactive mode (if no arguments) and define UI session
  G4UIExecutive* ui = nullptr;
  if(argc == 1)
  {
    ui = new G4UIExecutive(argc, argv);
  }

  G4String outFileName = "testA.root";
  if (argc  > 2) outFileName = argv[2]; 
  G4long seed = 123;
  if (argc  > 3) seed = atoi(argv[3]);
  G4Random::setTheEngine(new CLHEP::RanecuEngine);
  G4Random::setTheSeed(seed);

  
  
  AnalysisManager::getInstance()->Book(outFileName, "SingleLYSO");


  auto runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::SerialOnly);
  //runManager->SetVerboseLevel(1);
  DetectorConstruction* det = new DetectorConstruction();
  runManager->SetUserInitialization(det);

  

  G4VModularPhysicsList* physicsList = new FTFP_BERT;
  physicsList->ReplacePhysics(new G4EmStandardPhysics_option4());

  G4OpticalPhysics* opticalPhysics = new G4OpticalPhysics();
  auto opticalParams               = G4OpticalParameters::Instance();

  opticalParams->SetWLSTimeProfile("exponential");
  opticalParams->SetScintFiniteRiseTime(true);
 

  physicsList->RegisterPhysics(opticalPhysics);
  runManager->SetUserInitialization(physicsList);
  
  runManager->SetUserInitialization(new ActionInitialization(det));

  // Initialize G4 kernel
  //
  runManager->Initialize();

  // initialize visualization
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();

  // get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  if(ui)
  {
    // interactive mode
    UImanager->ApplyCommand("/control/execute vis.mac");
    if(ui->IsGUI())
    {
      UImanager->ApplyCommand("/control/execute gui.mac");
    }
    ui->SessionStart();
    delete ui;
  }
  else
  {
    G4cout << "BATCH MODE" << G4endl;	  
    // batch mode
    G4String command  = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command + fileName);
  }

  // job termination
  AnalysisManager::getInstance()->CloseFile();
  delete visManager;
  delete runManager;
  return 0;
}

