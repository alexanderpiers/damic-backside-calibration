// Silicon Event Action definitions

#ifndef SiliconEventAction_H
#define SiliconEventAction_H

#include "G4UserEventAction.hh"
#include "globals.hh"
#include <vector>

class SiliconEventAction : public G4UserEventAction
{
	public:
		SiliconEventAction();
		virtual ~SiliconEventAction();

		virtual void BeginOfEventAction(const G4Event* event);
		virtual void EndOfEventAction(const G4Event* event);

		// Member functions to add energy to the materials
		void AddEnergyDepSilicon(G4double energy){ fEnergyDepositSi += energy; };
		void AddWeightedZ(G4double z, G4double energy){ fWeightedZ += ( z * energy ); };
		void NormalizeWeightedZ(){ fWeightedZ /= fEnergyDepositSi; }; 

	private:

		G4double fEnergyDepositSi;
		G4double fWeightedZ;
};

#endif
