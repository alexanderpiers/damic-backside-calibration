// Silicon SD source code

#include "SiliconSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"


SiliconSD::SiliconSD(const G4String& name, const G4String& hitsCollectionName)
 : G4VSensitiveDetector(name),
   fHitsCollection(nullptr),
   fHitsCollectionID(-1)
{

	collectionName.insert(hitsCollectionName);
}

SiliconSD::~SiliconSD()
{

}

void SiliconSD::Initialize(G4HCofThisEvent* hce)
{
	// Create hits collection
	fHitsCollection = new SiliconHitsCollection(SensitiveDetectorName, collectionName[0]);

	// Get the collection ID based on the actual collection. Set it to the class member
	if ( fHitsCollectionID < 0)
		fHitsCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID(fHitsCollection);

	// Add collection to hits collection of this event hce
	hce->AddHitsCollection(fHitsCollectionID, fHitsCollection);

}

G4bool SiliconSD::ProcessHits(G4Step* step, G4TouchableHistory* history)
{

	// Check that energy was deposited in hit
	G4double edep = step->GetTotalEnergyDeposit();
	if ( edep == 0. ) return false;

	// Create a new hit. Add properties to it and add to the collection
	SiliconHit* hit = new SiliconHit();

	// Set track ID, energy deposited, position, and particle of the hit
	hit->SetTrackID(step->GetTrack()->GetTrackID());
	hit->SetEdep(edep);
	hit->SetPosition(step->GetPostStepPoint()->GetPosition());
	hit->SetInitialPosition(step->GetPreStepPoint()->GetPosition());
	hit->SetParticleDefinition(step->GetTrack()->GetDefinition());

	// Add hit to hit collection
	fHitsCollection->insert(hit);

	return true;

}

void SiliconSD::EndOfEvent(G4HCofThisEvent* hce)
{
	if ( verboseLevel > 1 ) {
     	G4int nofHits = fHitsCollection->entries();
     	G4cout << G4endl
            << "-------->Hits Collection: in this event they are " << nofHits
            << " hits in the tracker chambers: " << G4endl;
	     for ( G4int i=0; i<nofHits; i++ ) (*fHitsCollection)[i]->Print();
  	}

}
