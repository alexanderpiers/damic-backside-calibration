// main.cc for running Silicon detector


#include "SiliconDetectorConstruction.hh"
#include "SiliconActionInitialization.hh"
#include "SiliconPhysicsList.hh"

#include "G4RunManager.hh"
#include "G4VisExecutive.hh"
#include "G4UImanager.hh"
#include "G4UIExecutive.hh"
#include "G4UIcommand.hh"
#include "Randomize.hh"

#include "SimulationSettings.hh"
#include "time.h"

#include "DAMICAm241SourceGenerator.hh"


using namespace std;

int main(int argc, char** argv)
{

	// DAMICAm241Source * am241 = new DAMICAm241Source();
	
	// Detect interactive mode if no arguments passed and define ui session
	G4UIExecutive* ui = 0;
	if( argc == 1 )
	{
		ui = new G4UIExecutive(argc, argv);
	}

	// Select the random engine and set the seed
	G4Random::setTheEngine(new CLHEP::HepJamesRandom);
	G4Random::setTheSeed(time(0));

	// Creating the G4RunManager
	G4RunManager* manager = new G4RunManager;

	// Set required initializations--detector geometry, physics list, action steps, etc.

	// Building the detector
	G4cout << "Build Detector" << G4endl;
	SiliconDetectorConstruction* detector = new SiliconDetectorConstruction();
	manager->SetUserInitialization(detector);

	// Defining the physics we want to use
	G4cout << "Physics" << G4endl;
	SiliconPhysicsList* physics = new SiliconPhysicsList();
	manager->SetUserInitialization(physics);

	// Set User action initialization
	G4cout << "User Action Initialization" << G4endl;
	manager->SetUserInitialization(new SiliconActionInitialization(detector));

	// Initialize visualization
	G4cout << "Visualization" << G4endl;
	G4VisManager* visManager = new G4VisExecutive;
	visManager->Initialize();

	// UI manager
	G4cout << "UI Manager" << G4endl;
	G4UImanager* UImanager = G4UImanager::GetUIpointer();

	// Process macro or start interactive.
	// For now only have interactive for simplicity
	UImanager->ApplyCommand("/control/macroPath ./");
	if(argc==1){
		ui->SessionStart();
		delete ui;
	}else if(argc==2){
		char *macroCommand = new char[100];
		sprintf(macroCommand, "/control/execute %s", argv[1]);
		cout << "Running macro: " << macroCommand << endl;
		UImanager->ApplyCommand(macroCommand);
	}else{
		cout << "Invalid number of inputs" << endl;
	}

	// Job Termination
	delete visManager;
	delete manager;

}
