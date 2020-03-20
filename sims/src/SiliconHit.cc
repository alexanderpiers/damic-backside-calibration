// Silicon hit source code

#include "SiliconHit.hh"
#include "G4UnitsTable.hh"

#include <iomanip>


G4ThreadLocal G4Allocator<SiliconHit>* SiliconHitAllocator=0;

SiliconHit::SiliconHit()
 : G4VHit(),
   fTrackID(-1),
   fEdep(0.),
   fPos(G4ThreeVector()),
   fPosInit(G4ThreeVector())
{

}

SiliconHit::~SiliconHit()
{

}

// Overloaded constructor. Accepts an instance of SiliconHit. Copies member values over
SiliconHit::SiliconHit(const SiliconHit& right) : G4VHit()
{
	fTrackID 			= right.fTrackID;
	fEdep 				= right.fEdep;
	fPos 				= right.fPos;
	fPosInit			= right.fPosInit;
	fParticleDefinition = right.fParticleDefinition;
}

// Overload the equals operator. Sets all member variables to be the same
const SiliconHit& SiliconHit::operator=(const SiliconHit& right)
{
	fTrackID  		    = right.fTrackID;
	fEdep     		    = right.fEdep;
  	fPos       			= right.fPos;
  	fPosInit			= right.fPosInit;
  	fParticleDefinition = right.fParticleDefinition;

  	return *this;
}

// Overload the comparison operator
G4int SiliconHit::operator==(const SiliconHit& right) const
{
	return ( this == &right) ? 1 : 0;
}

// Print function
void SiliconHit::Print()
{
	G4cout
		<< " trackID: " << fTrackID << "\n"
		<< " Particle type: " << fParticleDefinition->GetParticleName() << "\n"
		<< " Edep: " << std::setw(7) << G4BestUnit(fEdep, "Energy") << "\n"
		<< " Initial Position: " << std::setw(7) << G4BestUnit( fPosInit, "Length") << "\n"
		<< " Final Position: " << std::setw(7) << G4BestUnit( fPos, "Length") << "\n"
		<< G4endl;
}