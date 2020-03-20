// Definition of the user defined physiscs list for the 

#include "SiliconPhysicsList.hh"
#include "G4EmPenelopePhysics.hh"

#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "G4IonConstructor.hh"
#include "G4PhysicsListHelper.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4NuclideTable.hh"
#include "G4LossTableManager.hh"
#include "G4UAtomicDeexcitation.hh"
#include "G4NuclearLevelData.hh"
#include "G4DeexPrecoParameters.hh"
	

SiliconPhysicsList::SiliconPhysicsList() 
 : G4VModularPhysicsList(), 
   fEmPhysics(nullptr)
{
	// Set the default cut value
	SetDefaultCutValue(10.*nm);

	SetVerboseLevel(1);

	// mandatory for G4NuclideTable
	//
	G4NuclideTable::GetInstance()->SetThresholdOfHalfLife(0.1*picosecond);
	G4NuclideTable::GetInstance()->SetLevelTolerance(1.0*eV);

	//read new PhotonEvaporation data set 
	//
	G4DeexPrecoParameters* deex = 
	G4NuclearLevelData::GetInstance()->GetParameters();
	deex->SetCorrelatedGamma(false);
	deex->SetStoreAllLevels(true);
	deex->SetMaxLifeTime(G4NuclideTable::GetInstance()->GetThresholdOfHalfLife()
	            /std::log(2.));


	// Creates the Penelope physics list
	fEmPhysics = new G4EmPenelopePhysics();
	fRadioActivePhysics = new G4RadioactiveDecayPhysics();
}

SiliconPhysicsList::~SiliconPhysicsList()
{

}

// Include headers of particles that we need 

// Bosons
#include "G4ChargedGeantino.hh"
#include "G4Geantino.hh"
#include "G4Gamma.hh"
#include "G4OpticalPhoton.hh"

// leptons
#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"
#include "G4NeutrinoMu.hh"
#include "G4AntiNeutrinoMu.hh"

#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4NeutrinoE.hh"
#include "G4AntiNeutrinoE.hh"

// Baryons
#include "G4Proton.hh"
#include "G4AntiProton.hh"
#include "G4Neutron.hh"
#include "G4AntiNeutron.hh"

// Nuclei
#include "G4Deuteron.hh"
#include "G4Triton.hh"
#include "G4Alpha.hh"
#include "G4GenericIon.hh"

void SiliconPhysicsList::ConstructParticle()
{

	// Imaginary tracking particles
	G4Geantino::GeantinoDefinition();
	G4ChargedGeantino::ChargedGeantinoDefinition();

	// Gamma
	G4Gamma::GammaDefinition();

	// Optical photon
	G4OpticalPhoton::OpticalPhotonDefinition();

	// Leptons
	G4Electron::ElectronDefinition();
	G4Positron::PositronDefinition();

	G4NeutrinoE::NeutrinoEDefinition();
	G4AntiNeutrinoE::AntiNeutrinoEDefinition();

	// Barions
	G4Proton::ProtonDefinition();
	G4AntiProton::AntiProtonDefinition();
	G4Neutron::NeutronDefinition();
	G4AntiNeutron::AntiNeutronDefinition();

	G4Deuteron::DeuteronDefinition();
	G4Triton::TritonDefinition();
	G4Alpha::AlphaDefinition();
	G4GenericIon::GenericIonDefinition();

	// ions
	G4IonConstructor iConstructor;
	iConstructor.ConstructParticle();  

}

void SiliconPhysicsList::ConstructProcess()
{

	// Transportation
    AddTransportation();

	// Register the penelope physics list
	fEmPhysics->ConstructProcess();


	// Create radioactive physics list
	// G4fRadioActivePhysics* fRadioActivePhysics = new G4fRadioActivePhysics();
	G4bool ARMflag = false;
	// fRadioActivePhysics->SetARM(ARMflag);        //Atomic Rearangement

	// need to initialize atomic deexcitation
	//
	G4LossTableManager* man = G4LossTableManager::Instance();
	G4VAtomDeexcitation* deex = man->AtomDeexcitation();
	if (!deex) {
		///G4EmParameters::Instance()->SetFluo(true);
		G4EmParameters::Instance()->SetAugerCascade(ARMflag);
		deex = new G4UAtomicDeexcitation();
		deex->InitialiseAtomicDeexcitation();
		man->SetAtomDeexcitation(deex);
	}

	// register fRadioActivePhysics
	fRadioActivePhysics->ConstructProcess();
	// G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();
	// ph->RegisterProcess(fRadioActivePhysics, G4GenericIon::GenericIon());


}

void SiliconPhysicsList::SetCuts()
{
	// Set limits of cut
	G4double lowerLimit = 1*eV;
	G4double upperLimit = 100*GeV;
	G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(lowerLimit, upperLimit);

	// Set cuts by calling parent function. Sets to default value
	G4VUserPhysicsList::SetCuts();

	// Print the cut values if verbosity dictates
	if( verboseLevel>0 )
	{
		DumpCutValuesTable();
	}



}