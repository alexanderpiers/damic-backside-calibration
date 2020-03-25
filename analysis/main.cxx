#include "DamicParticleInteraction.h"
#include "DamicBackSideCalibrationFit.h"

#include "TApplication.h"
#include "TCanvas.h"
#include "TRandom3.h"

using namespace std;

int main(int argc, char const *argv[])
{
	auto app = new TApplication("backside_cal", 0, 0);

	printf("DAMIC Backside Calibration Analysis Code\n");
	printf("------------------------------------------------\n");

	// Create particle collection from Geant4 file
	printf("Read Particle Collection from file...\n");
	ParticleCollection collection;
	collection.ReadROOTFile("../sims/build/si_penetration_depth.root");

	// Create output file
	TFile * fout = new TFile("output.root", "RECREATE");

	// Draw spectrum
	printf("Creating raw spectrum...\n");
	TH1D * rawSpectrum = new TH1D("rawSpec", "rawSpec", 200, 0, 60);
	collection.PlotEnergyProjection(rawSpectrum);
	new TCanvas(); rawSpectrum->Draw("ahist");

	// Apply Transformation to spectrum with random seed offset
	printf("Applying PCC transformation...\n");
	std::array<double, npar> randompar = nominalParameters;
	TRandom3 * fitrand = new TRandom3(1);
	double shift = fitrand->Uniform() - 0.2;
	randompar[npolynomial] = shift;
	printf("Random shift: %0.4f \n", shift);

	TF1 * chargeEff = new TF1("fpcc", fpcc, 0, 675, npar);
	chargeEff->SetParameters(randompar.data());

	TH1D * correctedSpectrum = new TH1D("corrSpec", "corrSpec", 200, 0, 60);
	collection.ApplyPartialChargeModel(correctedSpectrum, chargeEff);
	new TCanvas(); correctedSpectrum->Draw("ahist");

	// Try to fit spectrum
	printf("Perform minimization between data and template...\n");
	ROOT::Minuit2::FunctionMinimum calmin = PerformCalibrationFit(*correctedSpectrum, collection);
	std::cout << calmin << std::endl;

	// Write output files
	fout->Write();
	

	printf("Displaying Plots...\n");
	app->Run();
	fout->Close();
	return 0;
}