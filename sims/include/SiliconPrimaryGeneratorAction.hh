// Silicon Primary Generator Action header file

#ifndef SiliconPrimaryGeneratorAction_H
#define SiliconPrimaryGeneratorAction_H

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class G4GeneralParticleSource;
class G4ParticleGun;
class G4Event;

class SiliconPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{

	public:
		SiliconPrimaryGeneratorAction();
		virtual ~SiliconPrimaryGeneratorAction();

		virtual void GeneratePrimaries(G4Event* event);

		// const G4GeneralParticleSource* GetParticleGun() const {return fParticleGun;};
		const G4ParticleGun* GetParticleGun() const { return fParticleGun; };

	private:
		// G4GeneralParticleSource* fParticleGun; // G4 Particle gun pointer
		G4ParticleGun* fParticleGun;
};

#endif