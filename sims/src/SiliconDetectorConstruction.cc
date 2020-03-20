// SiliconDetectorConstruction Implementation

#include "SiliconDetectorConstruction.hh"
#include "SiliconSD.hh"

#include "G4NistManager.hh"
#include "G4VSolid.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4PSEnergyDeposit.hh"
#include "G4PSDoseDeposit.hh"
#include "G4VisAttributes.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "SimulationSettings.hh"


#include <cmath>


// Define Constructor. Defines the materials used in the detector
SiliconDetectorConstruction::SiliconDetectorConstruction() : G4VUserDetectorConstruction()
{

}

// Defines Destructor
SiliconDetectorConstruction::~SiliconDetectorConstruction()
{

}


// Defines the geometry of the detecotr. Retuns a G4VPhyscalVolume class
G4VPhysicalVolume* SiliconDetectorConstruction::Construct()
{

	// Get materials for the detector
	G4NistManager* nist = G4NistManager::Instance();
	G4Material* defaultMat = nist->FindOrBuildMaterial("G4_AIR");

	G4Material* Si = nist->FindOrBuildMaterial("G4_Si");

	double a;
	a = 14.01*g/mole;
	G4Element* elN  = new G4Element("Nitrogen","N",7.,a);
	a = 16.00*g/mole; 
	G4Element* elO  = new G4Element("Oxygen","O", 8.,a); 

	// double temperature = 293*kelvin;
	// double pressure = 1.e-6*hPa;
	double density = 1.e-9 * kg/m3;
	G4Material* DAMICVacuum = new G4Material("DAMICVacuum", density, 2 );//pressure=pressure, temp=temperature );
	DAMICVacuum->AddElement(elN, 79.0*perCent);
	DAMICVacuum->AddElement(elO, 21.0*perCent);

	// Define the world size and create the world

	// Create square world box
	G4Box* solidWorld = new G4Box(	"World",
									worldDimension/2, // world x dimension
									worldDimension/2, // world y dimension
									worldDimension/2); // world z dimension
	G4LogicalVolume* logicalWorld =
		new G4LogicalVolume(solidWorld, 	// solid object associated with logical volume
							DAMICVacuum, 	// material of the world
							"World");		//name

	G4VPhysicalVolume* physWorld =
		new G4PVPlacement(	0, 					// rotation
							G4ThreeVector(), 	// position at 0,0,0
							logicalWorld, 		// logical volume
							"World",			// name
							0,
							false,
							0);


	// Define Silicon block for detector
	G4Box* SiliconS = new G4Box("Silicon",
								siliconXY/2,
								siliconXY/2,
								siliconZ/2);

	G4LogicalVolume* SiliconLV =
		new G4LogicalVolume(SiliconS,
							Si,
							"Silicon");

	fSiliconPV =
		new G4PVPlacement(	0,
							G4ThreeVector(),
							SiliconLV,
							"Silicon",
							logicalWorld,
							false,
							0);

	
	// Setting up sensitive detector for the Silicon body. Only uses if SIMPLE is false
	if(!SIMPLE)
	{
		// Create a new Silicon sensitive detector
		SiliconSD* detector = new SiliconSD("SiliconSD", SiliconHitsCollectionName);

		// Get pointer to SD manager
		G4SDManager* sdManager = G4SDManager::GetSDMpointer();
		sdManager->AddNewDetector(detector);

		// Attach Silicon logical volume with the new sensitive detector
		SiliconLV->SetSensitiveDetector(detector);
	}


	// returns the physical world
	return physWorld;


}
