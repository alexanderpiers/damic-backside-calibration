// Header file for Silicon Detector

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;

// Detector Construction class to define materials and geometry of detector

class SiliconDetectorConstruction : public G4VUserDetectorConstruction
{
	public:
		SiliconDetectorConstruction();
		virtual ~SiliconDetectorConstruction();


	public:
		virtual G4VPhysicalVolume* Construct();

		// Methods to get the physical volumes of the different parts of the detector
		const G4VPhysicalVolume* GetSiliconPV() const;


	private:
		G4VPhysicalVolume* fSiliconPV; // Silicon physical volume
};


inline const G4VPhysicalVolume* SiliconDetectorConstruction::GetSiliconPV() const
{
	return fSiliconPV;
}
