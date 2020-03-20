// Primary Generator Action code

#include "SiliconPrimaryGeneratorAction.hh"
#include "SimulationSettings.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4ThreeVector.hh"
#include "G4IonTable.hh"
#include "G4DynamicParticle.hh"
#include "G4RadioactiveDecay.hh"
#include "G4DecayProducts.hh"
#include "G4DecayTable.hh"
#include "G4VDecayChannel.hh"
#include "G4PhysicalConstants.hh"

#include "G4Fragment.hh"
#include "G4PhotonEvaporation.hh"
#include "G4FragmentVector.hh"
#include "G4LorentzVector.hh"


#include "DAMICAm241SourceGenerator.hh"





SiliconPrimaryGeneratorAction::SiliconPrimaryGeneratorAction()
 : G4VUserPrimaryGeneratorAction(),
   fParticleGun(nullptr)
{

	// // Create a GPS and assign it to fParticleGun
	// fParticleGun = new G4GeneralParticleSource();
	G4int n_particle = 1;
  	fParticleGun  = new G4ParticleGun(n_particle);

	// Define parameters of the source
	// fParticleGun->SetParticleEnergy(0*eV);
	fParticleGun->SetParticlePosition(G4ThreeVector(0.,0.,10*mm));

	



	// Optional. Setting particle parameters. Can also do with macro
	// fParticleGun->SetParticleEnergy(122.*keV);
	// fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,-1.));

	// Using Particle gun for first pass simplicity
	// G4int nOfParticles = 1;
	// fParticleGun = new G4ParticleGun(nOfParticles);

	// fParticleGun->SetParticleDefinition(particle);
	// fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., -1.));
	// fParticleGun->SetParticleEnergy(136.*keV);


}

SiliconPrimaryGeneratorAction::~SiliconPrimaryGeneratorAction()
{
	delete fParticleGun;
}

void SiliconPrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
	G4RadioactiveDecay * decay = new G4RadioactiveDecay();
	static G4ParticleDefinition* ion = 0;

	// if(!ion){
	// 	// Get Radioactive nucleotide
	// 	G4int Z = 10, A = 24;
	//     G4double ionCharge   = 0.*eplus;
	//     G4double excitEnergy = 0.*keV;

	// 	 G4ParticleDefinition*  ion = G4IonTable::GetIonTable()->GetIon(Z,A,excitEnergy);
	// 	fParticleGun->SetParticleDefinition(ion);
	// 	fParticleGun->SetParticleCharge(ionCharge);
	// }

	// Force the decay of a given ion, collect decay products and "shoot" those products. 
	// Should speed up having to wait for decay

	// Test Am241 decay
	DAMICAm241Source * am241 = new DAMICAm241Source();

	// Test ion
	G4int Z = 95, A = 241;
    G4double ionCharge   = 0.*eplus;
    G4double excitEnergy = 0.*keV;

	/* G4ParticleDefinition* */ ion = G4IonTable::GetIonTable()->GetIon(Z,A,excitEnergy);
	ion->SetPDGLifeTime(1*ps);

	// Get decay table for ion
	G4DecayTable * decayTable = decay->LoadDecayTable(*ion);

	// If check to make sure there are decay channels
	if(decayTable->entries() == 0){
		G4cout << "No valid decays" << G4endl;
		return;
	}

	G4VDecayChannel * channel = decayTable->SelectADecayChannel();

	// Force decay
	G4DecayProducts * products = channel->DecayIt(ion->GetPDGMass());	

	// std::cout << "There are: " << products->entries() << " products" << std::endl;

	// Iterate over the decay products
	// for (G4int i = 0; i < products->entries(); ++i)
	// {
		G4DynamicParticle * particle = nullptr;
	// 	particle = (*products)[i];

	particle = am241->generateGammaCEDecay();

		// G4cout << "Particle type: " << particle->GetDefinition()->GetParticleName() << G4endl;
		// G4cout << "Particle energy [keV]: " << particle->GetKineticEnergy() / keV << G4endl;

		G4String particleName = particle->GetDefinition()->GetParticleName();
		// if(particleName == "e-" || particleName == "e+" || particleName == "alpha"){
			// G4cout << "Particle simulated" << G4endl;
			fParticleGun->SetParticleDefinition(particle->GetDefinition());
			fParticleGun->SetParticleEnergy(particle->GetKineticEnergy());
			// fParticleGun->SetParticleMomentumDirection()
			fParticleGun->SetParticleMomentumDirection(am241->generateGeneralMomentum(0, twopi, 0, halfpi));
			fParticleGun->SetParticleCharge(particle->GetCharge());

			fParticleGun->GeneratePrimaryVertex(event);
		// }
	// }

	// Photon products
	// G4Fragment * nucl = new G4Fragment(A, Z, G4LorentzVector(0,0,0,0));
	// G4PhotonEvaporation * evaporate = new G4PhotonEvaporation();
	// G4Fragment * photon = evaporate->EmittedFragment(nucl);

	// // for (int i = 0; i < photoDecays->size(); ++i)
	// // {
	// 	G4cout << "Particle Type: " << photon->GetParticleDefinition()->GetParticleName() << G4endl;
	// 	// G4cout << "Particle Energy: " << photon->GetParticleDefinition()->GetKineticEnergy() / keV << G4endl;
	// // }


	// Set particle source position
	// fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., worldZHalf));

	// fParticleGun->SetParticleDefinition(ion);
	// fParticleGun->GeneratePrimaryVertex(event);


}
