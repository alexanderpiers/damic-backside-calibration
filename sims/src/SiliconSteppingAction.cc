// Silicon Stepping Action code

#include "SiliconSteppingAction.hh"
#include "SiliconEventAction.hh"
#include "SiliconDetectorConstruction.hh"

#include "G4Step.hh"
#include "G4RunManager.hh"


// Constructor
SiliconSteppingAction::SiliconSteppingAction(
									SiliconDetectorConstruction* detector,
									SiliconEventAction* event)
	: G4UserSteppingAction(),
	  fDetectorConstruction(detector),
	  fEventAction(event)
{

}

// Destructor
SiliconSteppingAction::~SiliconSteppingAction()
{

}

// Stepping Action. Gets the energy and adds it to the current event
void SiliconSteppingAction::UserSteppingAction(const G4Step* step)
{
	// Get the volume
	G4VPhysicalVolume* volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();

	

	if( volume == fDetectorConstruction->GetSiliconPV())
	{
		// Energy deposit
		G4double eDepStep = step->GetTotalEnergyDeposit();

		// Energy deposit location
		G4double eZStep = (step->GetPostStepPoint()->GetPosition().getZ() + step->GetPreStepPoint()->GetPosition().getZ() ) / 2; 
		fEventAction->AddEnergyDepSilicon(eDepStep);
		fEventAction->AddWeightedZ(eZStep, eDepStep);
	} 

}
