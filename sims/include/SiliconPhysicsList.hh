// Silicon Detector Physics List

#ifndef SiliconPhysicsList_H
#define SiliconPhysicsList_H

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class G4VPhysicsConstructor;

class SiliconPhysicsList: public G4VModularPhysicsList
{
	public:
		SiliconPhysicsList();
		virtual ~SiliconPhysicsList();

		virtual void ConstructParticle();
		virtual void ConstructProcess();

		virtual void SetCuts();

	private:
		G4VPhysicsConstructor* fEmPhysics;	
		G4VPhysicsConstructor* fRadioActivePhysics;
};


#endif