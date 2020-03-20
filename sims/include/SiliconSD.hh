// Silicon sensitive detector header file

#ifndef SiliconSD_H
#define SiliconSD_H

#include "G4VSensitiveDetector.hh"
#include "SiliconHit.hh"

#include <vector>

// Define the necessary classes for SiliconSD class
class G4Step;
class G4HCofThisEvent;

// Class definition
class SiliconSD : public G4VSensitiveDetector
{
	public:
		// Constructor and destructor
		SiliconSD(const G4String& name, const G4String& hitsCollectionName);
		virtual ~SiliconSD();

	public:
		// overloads the process hits base class
		virtual void Initialize(G4HCofThisEvent* hitCollection);
		virtual G4bool ProcessHits(G4Step *step, G4TouchableHistory* history);
		virtual void EndOfEvent(G4HCofThisEvent* hitCollection);

private:
	SiliconHitsCollection* fHitsCollection;
	G4int fHitsCollectionID;

};


#endif