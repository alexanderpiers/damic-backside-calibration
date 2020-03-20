// Silicon Detector Run Action

#ifndef SiliconRunAction_H
#define SiliconRunAction_H

#include "G4UserRunAction.hh"
#include "globals.hh"

// Define the G4Run class
class G4Run;

// Define the Run Action class
class SiliconRunAction : public G4UserRunAction
{

	public:
		SiliconRunAction();
		virtual ~SiliconRunAction();

		virtual void BeginOfRunAction(const G4Run*);
		virtual void EndOfRunAction(const G4Run*);
		
		// Defines a filename for saving output data
		G4String filename;

};

#endif