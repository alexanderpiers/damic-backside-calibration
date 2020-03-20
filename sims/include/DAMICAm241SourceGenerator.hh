#ifndef DAMICAm241SourceGenerator_H
#define DAMICAm241SourceGenerator_H

#include "G4ThreeVector.hh"
#include "Randomize.hh"
#include "G4DynamicParticle.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"



class DAMICAm241Source
{
public:
	DAMICAm241Source();
	~DAMICAm241Source();

	// Generate Decays
	G4DynamicParticle * generateAlphaDecay();
	G4DynamicParticle * generateGammaDecay();
	G4DynamicParticle * generateConversionElectronDecay();
	G4DynamicParticle * generateGammaCEDecay();
	std::vector < G4DynamicParticle * > generateDecays();
	G4ThreeVector generateIsotropicMomentum();
	G4ThreeVector generateGeneralMomentum(double phiMin = 0., double phiMax = twopi, double thetaMin = 0., double thetaMax = pi);

private:

	// Decay "tables"
	// Alpha decays
	static const int nalpha = 3;
	const double vAlphaEnergy[nalpha] =
	{
		5388.0 * keV,
		5443.0 * keV,
		5485.7 * keV
	};
	double vAlphaIntensity[nalpha] = 
	{
		0.014,
		0.128,
		0.852
	};
	double alphaNorm;


	// Gamma Decays
	static const int ngamma = 2;
	const double vGammaEnergy[ngamma] = 
	{
		26.38 * keV,
		59.65 * keV
	};
	double vGammaIntensity[ngamma] = {
		0.04,
		0.40
	};
	double gammaNorm;


	// Conversion Electrons
	static const int nce = 9;
	const double vElectronEnergy[nce] = 
	{
		10.6 * keV,
		27.3 * keV,
		20.8 * keV,
		21.8 * keV,
		25.8 * keV,
		37.1 * keV,
		42.0 * keV,
		54.4 * keV,
		59.2 * keV
	};
	double vElectronIntensity[nce] = 
	{
		0.067,
		0.018,
		0.017,
		0.035,
		0.039,
		0.236,
		0.054,
		0.064,
		0.014
	};
	double ceNorm;
	double gammaCeNorm;



	// Fluorescence electrons?

	
};

#endif