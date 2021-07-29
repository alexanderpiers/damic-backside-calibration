#include "DamicParticleInteraction.h"
#include "DamicBackSideCalibrationFit.h"

#include "TApplication.h"
#include "TCanvas.h"
#include "TRandom3.h"

using namespace std;

const int nbins = 300;
const double emin = 0;
const double emax = 20;

int main(int argc, char const *argv[])
{
	auto app = new TApplication("backside_cal", 0, 0);

	printf("DAMIC Backside Calibration Analysis Code\n");
	printf("------------------------------------------------\n");

	// Create particle collection from Geant4 file
	printf("Read Particle Collection from file...\n");
	ParticleCollection collection;
	collection.ReadROOTFile("../sims/build/test_fe55.root");

	// Create output file
	TFile * fout = new TFile("fe55_output.root", "RECREATE");

	// Draw spectrum
	printf("Creating raw spectrum...\n");
	TH1D * rawSpectrum = new TH1D("rawSpec", "rawSpec", nbins, 0, emax);
	collection.PlotEnergyProjection(rawSpectrum);
	new TCanvas();
	rawSpectrum->GetYaxis()->SetTitle("Counts");
	rawSpectrum->GetXaxis()->SetTitle("Energy [keV]");
	rawSpectrum->Draw("ahist");

	// Apply Transformation to spectrum with random seed offset
	printf("Applying PCC transformation...\n");
	std::array<double, npar> randompar = nominalParameters;
	TRandom3 * fitrand = new TRandom3(50);
	double shift = fitrand->Uniform() - 0.2;
	double scale = fitrand->Uniform() / 5 + 0.9;
	randompar[npolynomial] = 0;
	randompar[npolynomial + 1] = 1;
	printf("Random shift: %0.3f \t Random Scale: %0.3f \n", shift, scale);

	TF1 * chargeEff = new TF1("fpcc", fpcc, 0, 675, npar);
	chargeEff->SetParameters(randompar.data());
	chargeEff->SetNpx(1000);

	TH1D * correctedSpectrum = new TH1D("corrSpec", "corrSpec", nbins, 0, emax);
	collection.ApplyPartialChargeModel(correctedSpectrum, chargeEff);
	new TCanvas();
	correctedSpectrum->GetYaxis()->SetTitle("Counts");
	correctedSpectrum->GetXaxis()->SetTitle("Energy [keV]");
	// correctedSpectrum->Draw("hist");
	TH1D * poisfluc = GeneratePoissonBinHisto(*correctedSpectrum); 
	poisfluc->SetLineColor(kBlack);
	poisfluc->Draw("ep0");


	// Try to fit spectrum
	printf("Perform minimization between data and template...\n");
	ROOT::Minuit2::FunctionMinimum calmin = PerformCalibrationFit(*poisfluc, collection);
	std::cout << calmin << std::endl;
	randompar[npolynomial]     = calmin.UserParameters().Value(0);
	randompar[npolynomial + 1] = calmin.UserParameters().Value(1);
	TH1D * fitSpectrum = new TH1D("fitspec", "fitspec", nbins, emin, emax);
	collection.ApplyPartialChargeModel(fitSpectrum, chargeEff);
	fitSpectrum->SetLineColor(kRed);
	fitSpectrum->Draw("histsame");


	// Plot the fit charge collection model
	new TCanvas();
	chargeEff->Draw();

	// Perform MC test
	// const int ntrials = 1000;
	// std::vector<CalibrationFitData_t> vMonteCarloResults(ntrials);
	// MonteCarloCalibrationFit(collection, vMonteCarloResults);
	// TTree tout("MonteCarloFit", "MonteCarloFit");
	// ConvertCalFitDataToTree(vMonteCarloResults, tout);

	// Write output files
	fout->Write();
	

	printf("Displaying Plots...\n");
	app->Run();
	fout->Close();
	return 0;
}
