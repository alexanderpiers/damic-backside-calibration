// Silicon Action Initializer header

#ifndef SiliconActionInitialization_H
#define SiliconActionInitialization_H

#include "G4VUserActionInitialization.hh"
#include "globals.hh"

// Define class for detector construction
class SiliconDetectorConstruction;

// Define action initialization class
class SiliconActionInitialization: public G4VUserActionInitialization
{

	public:
		SiliconActionInitialization(SiliconDetectorConstruction* detector);
		virtual ~SiliconActionInitialization();

		virtual void BuildForMaster() const;
		virtual void Build() const;

		
	private:
		SiliconDetectorConstruction* fDetectorConstruction;
};

#endif

