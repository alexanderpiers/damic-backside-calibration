// Silicon hit header

#ifndef SiliconHit_H
#define SiliconHit_H

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4ParticleDefinition.hh"
#include "G4VProcess.hh"
#include "tls.hh"

class SiliconHit : public G4VHit
{
	public:
		SiliconHit();
		SiliconHit(const SiliconHit&);
		virtual ~SiliconHit();

		// Overloading certain operators
		const SiliconHit& operator=(const SiliconHit&);
		G4int operator==(const SiliconHit&) const;

		inline void* operator new(size_t);
		inline void operator delete(void *);

		virtual void Print();

		// Set methods
		void SetTrackID(G4int track) { fTrackID = track; };
		void SetEdep(G4double energy) { fEdep = energy; };
		void SetPosition(G4ThreeVector xyz) { fPos = xyz; };
		void SetInitialPosition(G4ThreeVector xyz) { fPosInit = xyz; };
		void SetParticleDefinition(G4ParticleDefinition* particle) { fParticleDefinition = particle; };

		// Get methods
		G4int GetTrackID() const { return fTrackID; };
		G4double GetEdep() const { return fEdep; };
		G4ThreeVector GetPosition() const { return fPos; };
		G4ThreeVector GetInitialPosition() const { return fPosInit; };
		G4ParticleDefinition* GetParticleDefinition() const{ return fParticleDefinition; };

	private:
	
		// Set up member of track ID, position and energy
		G4int fTrackID;
		G4double fEdep;
		G4ThreeVector fPos;
		G4ThreeVector fPosInit;
		G4ParticleDefinition* fParticleDefinition;
};

typedef G4THitsCollection<SiliconHit> SiliconHitsCollection;
extern G4ThreadLocal G4Allocator<SiliconHit>* SiliconHitAllocator;

inline void* SiliconHit::operator new(size_t)
{
	if (!SiliconHitAllocator)
		SiliconHitAllocator = new G4Allocator<SiliconHit>;
	return (void *)SiliconHitAllocator->MallocSingle();
}

inline void SiliconHit::operator delete(void *hit)
{
	SiliconHitAllocator->FreeSingle((SiliconHit*) hit);
}
#endif