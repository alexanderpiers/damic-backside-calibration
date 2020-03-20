#include "DamicParticleInteraction.h"
#include "DamicBackSideCalibrationFit.h"

#include "TApplication.h"
#include "TCanvas.h"

using namespace std;

int main(int argc, char const *argv[])
{
	auto app = new TApplication("backside_cal", 0, 0);

	cout << "DAMIC Backside Calibration Analysis Code\n\n";

	// Create particle collection from Geant4 file
	cout << "Read Particle Collection from file...\n";
	ParticleCollection collection;
	collection.ReadROOTFile("../sims/build/si_penetration_depth.root");

	// Create output file
	TFile * fout = new TFile("output.root", "RECREATE");

	// Draw spectrum
	cout << "Creating raw spectrum...\n";
	TH1D * rawSpectrum = new TH1D("rawSpec", "rawSpec", 200, 0, 60);
	collection.PlotEnergyProjection(rawSpectrum);
	new TCanvas(); rawSpectrum->Draw("ahist");

	// Apply Transformation to spectrum with random seed offset
	cout << "Applying PCC transformation...\n";
	TF1 * chargeEff = new TF1("fpcc", fpcc, 0, 675, 9);
	chargeEff->SetParameters(nominalParameters);

	TH1D * correctedSpectrum = new TH1D("corrSpec", "corrSpec", 200, 0, 60);
	collection.ApplyPartialChargeModel(correctedSpectrum, chargeEff);
	new TCanvas(); correctedSpectrum->Draw("ahist");

	// Try to fit spectrum

	// Write output files
	fout->Write();
	fout->Close();

	cout << "Displaying Plots...\n";
	app->Run();
	return 0;
}