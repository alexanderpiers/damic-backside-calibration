// Silicon Action Initialization code

#include "SiliconActionInitialization.hh"
#include "SiliconPrimaryGeneratorAction.hh"
#include "SiliconRunAction.hh"
#include "SiliconEventAction.hh"
#include "SiliconSteppingAction.hh"
#include "SiliconDetectorConstruction.hh"

// Constructor. Calls the parent class constructor and passes in the argument to the fDetectorConstruction member variable
SiliconActionInitialization::SiliconActionInitialization(SiliconDetectorConstruction* detector)
	: G4VUserActionInitialization(),
	  fDetectorConstruction(detector)
{

}

// Destructor
SiliconActionInitialization::~SiliconActionInitialization()
{

}

// Run on master. Executed on all threads
void SiliconActionInitialization::BuildForMaster() const 
{
	SetUserAction(new SiliconRunAction());
}

//
void SiliconActionInitialization::Build() const
{
	// Creating and assigning the various action needed for the simulation
	// Run Action
	SetUserAction(new SiliconRunAction());

	// Primary generator action
	SiliconPrimaryGeneratorAction* spga = new SiliconPrimaryGeneratorAction();
	SetUserAction(spga);

	// Event action
	SiliconEventAction* event = new SiliconEventAction;
	SetUserAction(event);

	// Stepping action
	SiliconSteppingAction* stepping = new SiliconSteppingAction(fDetectorConstruction, event);
	SetUserAction(stepping);

}

