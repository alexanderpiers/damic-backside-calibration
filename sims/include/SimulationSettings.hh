// General Simulation Settings

#ifndef SimulationSettings_H
#define SimulationSettings_H

#include "G4SystemOfUnits.hh"


// Defining geometries of the problem
const G4double siliconXY = 90. * mm;
const G4double siliconZ = 0.675 * mm;
const G4double worldDimension = 500. * mm;

// Source geometry
const G4double sourcePosition = 1 * mm;

// Sensitive Detector parameters
const G4String SiliconHitsCollectionName = "SiliconHitsCollection";

// Defining default units to use to keep analysis consistent
const double DEFAULT_LENGTH = um;
const double DEFAULT_ENERGY = keV;

const bool SIMPLE = false;

#endif
