#include "DAMICAm241SourceGenerator.hh"
#include <numeric>
#include <cmath>

#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"

DAMICAm241Source::DAMICAm241Source(){

	// Compute the norm of the intensities
	alphaNorm = std::accumulate(vAlphaIntensity, vAlphaIntensity + nalpha, 0.);
	gammaNorm = std::accumulate(vGammaIntensity, vGammaIntensity + ngamma, 0.);
	ceNorm = std::accumulate(vElectronIntensity, vElectronIntensity + nce, 0.);
	gammaCeNorm = gammaNorm + ceNorm;

}

DAMICAm241Source::~DAMICAm241Source(){

}

G4DynamicParticle * DAMICAm241Source::generateAlphaDecay(){

	// Generate random number and go through the branching ratio to find the energy
	int si = 0;
	double selector = G4UniformRand();
	double intensitySum = vAlphaIntensity[si] / alphaNorm;
	while(selector >= intensitySum){
		si++;
		intensitySum += (vAlphaIntensity[si] / alphaNorm);
	}


	// Define particle with the selected energy
	double energy = vAlphaEnergy[si];
	G4String type = "alpha";
	G4ThreeVector momentum = generateIsotropicMomentum();
	G4ParticleDefinition * particle = G4ParticleTable::GetParticleTable()->FindParticle(type);

	// Create particle and return it
	return new G4DynamicParticle(particle, momentum, energy);


}

// G4DynamicParticle * DAMICAm241Source::generateGammaDecay();
// G4DynamicParticle * DAMICAm241Source::generateConversionElectronDecay();
G4DynamicParticle * DAMICAm241Source::generateGammaCEDecay(){

	// 
	int si = 0;
	double selector = G4UniformRand();
	double intensitySum = (si < ngamma) ? vGammaIntensity[si] / gammaCeNorm : vElectronIntensity[si-ngamma] / gammaCeNorm;
	while(selector >= intensitySum){
		si++;
		intensitySum += (si < ngamma) ? vGammaIntensity[si] / gammaCeNorm : vElectronIntensity[si-ngamma] / gammaCeNorm;
	}

	// Get particle energy, type, momentum direction
	double energy = (si < ngamma) ? vGammaEnergy[si] : vElectronEnergy[si-ngamma];
	G4String type = (si < ngamma) ? "gamma" : "e-";
	G4ThreeVector momentum = generateIsotropicMomentum();
	G4ParticleDefinition * particle = G4ParticleTable::GetParticleTable()->FindParticle(type);

	// Create particle and return it
	return new G4DynamicParticle(particle, momentum, energy);

}

// std::vector < G4DynamicParticle * > DAMICAm241Source::generateDecays();

G4ThreeVector DAMICAm241Source::generateGeneralMomentum(double minPhi, double maxPhi, double minTheta, double maxTheta){

	double phi = minPhi + G4UniformRand() * ( maxPhi - minPhi );
	double theta = minTheta + G4UniformRand() * ( maxTheta - minTheta );
	G4ThreeVector generalMomemtum(cos(phi)*sin(theta), sin(phi)*sin(theta), cos(theta));
	return generalMomemtum;

}

G4ThreeVector DAMICAm241Source::generateIsotropicMomentum(){

	double phi = G4UniformRand() * twopi;
	double theta = G4UniformRand() * pi;

	return G4ThreeVector(cos(phi)*sin(theta), sin(phi)*sin(theta), cos(theta));
	// return G4ThreeVector(0., 0., -1.); // Temporary return
}
