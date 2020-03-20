// Silicon Event action code

#include "SiliconEventAction.hh"
#include "SiliconRunAction.hh"
#include "SiliconAnalysis.hh"
#include "SimulationSettings.hh"
#include "SiliconHit.hh"


#include "G4Event.hh"
#include "G4SystemOfUnits.hh"
#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "G4ThreeVector.hh"

// Silicon Event Action constructor
SiliconEventAction::SiliconEventAction()
:G4UserEventAction(),
 fEnergyDepositSi(0.),
 fWeightedZ(0.)
{

}

// Destructor
SiliconEventAction::~SiliconEventAction()
{

}

void SiliconEventAction::BeginOfEventAction(const G4Event* event)
{
	fEnergyDepositSi = 0.; // Initialize energy deposition
	fWeightedZ = 0.; // Initialize mean position of deposition
}

void SiliconEventAction::EndOfEventAction(const G4Event* event)
{
	// Get analysis manager
	G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

	// Add deposited energy to histograms
	if ( fEnergyDepositSi > 0. )
	{
		analysisManager->FillH1(0, fEnergyDepositSi);
		this->NormalizeWeightedZ();
		analysisManager->FillH1(1, fWeightedZ);
		analysisManager->FillH2(0, fEnergyDepositSi, fWeightedZ );
	}

	if(!SIMPLE)
	{
		// Get the number of hits in the Silicon. For each hit add
		// it to the Energy vs position histogram
		G4HCofThisEvent* hce = event->GetHCofThisEvent();

		// Get the hits collection ID number
		G4int SiliconHCID = G4SDManager::GetSDMpointer()->GetCollectionID(SiliconHitsCollectionName);

		if (SiliconHCID >= 0)
		{
			SiliconHitsCollection* hc = dynamic_cast<SiliconHitsCollection*>(hce->GetHC(SiliconHCID));
			SiliconHit* hit;
			G4int eventID = event->GetEventID();
			// Get number of hits. Iterate over all and add to Ntuple
			G4int numberOfHits = hc->GetSize();


			if(numberOfHits > 0)G4cout << numberOfHits << G4endl;
			for( int i = 0; i < numberOfHits; i++)
			{
				hit = (*hc)[i];

				// Fill ntuples with hit values
				analysisManager->FillNtupleIColumn(0, 0, eventID);
				analysisManager->FillNtupleIColumn(0, 1, hit->GetTrackID());
				analysisManager->FillNtupleDColumn(0, 2, hit->GetPosition().getX());
				analysisManager->FillNtupleDColumn(0, 3, hit->GetPosition().getY());
				analysisManager->FillNtupleDColumn(0, 4, hit->GetPosition().getZ());
				analysisManager->FillNtupleDColumn(0, 5, hit->GetInitialPosition().getX());
				analysisManager->FillNtupleDColumn(0, 6, hit->GetInitialPosition().getY());
				analysisManager->FillNtupleDColumn(0, 7, hit->GetInitialPosition().getZ());
				analysisManager->FillNtupleDColumn(0, 8, hit->GetEdep());
				analysisManager->AddNtupleRow(0);

			}

		}
	}



}
