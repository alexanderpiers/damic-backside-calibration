// Silicon detector Run Action

#include "SiliconRunAction.hh"
#include "SiliconAnalysis.hh"
#include "SiliconEventAction.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"

#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

#include "SimulationSettings.hh"

// Run Action Constructor
SiliconRunAction::SiliconRunAction() : G4UserRunAction()
{
	// set printing event number per each event
	G4RunManager::GetRunManager()->SetPrintProgress(1);

	// Create analysis manager
	G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
	analysisManager->SetVerboseLevel(1);
	analysisManager->SetFirstHistoId(0);

	// Create the histograms for total energy deposition
	analysisManager->CreateH1("EneDepsi", "Energy Deposited in Silicon", 1000, 0., 100*keV, "keV");
	analysisManager->CreateH1("WeightedZPosition", "Weighted Position of Interaction", 3000, 0, siliconZ, "um");
	analysisManager->CreateH2("EvZ", "Energy vs. Depth", 200, 0., 100.*keV, 675, 0, siliconZ, "keV", "um");


	if(!SIMPLE)
	{

		// Creating Ntuple to store hits in SiliconSD
		// ntuple id=0
		analysisManager->CreateNtuple("siData", "Silicon Hit Data");
		analysisManager->CreateNtupleIColumn("EventID");
		analysisManager->CreateNtupleIColumn("TrackID");
		analysisManager->CreateNtupleDColumn("x");
		analysisManager->CreateNtupleDColumn("y");
		analysisManager->CreateNtupleDColumn("z");
		analysisManager->CreateNtupleDColumn("xi");
		analysisManager->CreateNtupleDColumn("yi");
		analysisManager->CreateNtupleDColumn("zi");
		analysisManager->CreateNtupleDColumn("energy");
		analysisManager->FinishNtuple();
	}

}

// Run Action Destructor
SiliconRunAction::~SiliconRunAction()
{
	delete G4AnalysisManager::Instance();
}

// BeginOfRunAction method. Executed before the first run
void SiliconRunAction::BeginOfRunAction(const G4Run*)
{
	// Get the analysis manager
	G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

	// Open file for output of data
	analysisManager->OpenFile();
}

// Method run at the end of the run
void SiliconRunAction::EndOfRunAction(const G4Run*)
{
	// Save the histograms
	G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
	analysisManager->Write();
	analysisManager->CloseFile();

}
