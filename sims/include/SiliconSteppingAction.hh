// Silicon Stepping Action Header

#ifndef SiliconSteppingAction_H
#define SiliconSteppingAction_H

#include "G4UserSteppingAction.hh"

class SiliconDetectorConstruction;
class SiliconEventAction;

// Define Stepping Action class. Collects energy deposited in different materials
class SiliconSteppingAction : public G4UserSteppingAction
{

	public:
		SiliconSteppingAction(SiliconDetectorConstruction* detector, 
									SiliconEventAction* event);
		virtual ~SiliconSteppingAction();

		virtual void UserSteppingAction(const G4Step* step);

	private:
		SiliconDetectorConstruction* fDetectorConstruction;
		SiliconEventAction* fEventAction;
};

#endif