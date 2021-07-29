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
	static const int ngamma = 3;
	const double vGammaEnergy[ngamma] = 
	{
		13.9 * keV,
		26.34 * keV,
		59.65 * keV
	};
	double vGammaIntensity[ngamma] = {
		0.37,
		0.0227,
		0.359
	};
	double gammaNorm;


	// Conversion Electrons
	static const int nce = 7;
	const double vElectronEnergy[nce] = 
	{
		3.91 * keV,
		10.77 * keV,
		20.6 * keV,
		20.99 * keV,
		27.47 * keV,
		37.1 * keV,
		53.89 * keV
	};
	double vElectronIntensity[nce] = 
	{
		0.14,
		0.18,
		0.036,
		0.091,
		0.044,
		0.302,
		0.081,
	};
	double ceNorm;
	double gammaCeNorm;



	// Fluorescence electrons?

	
};

#endif