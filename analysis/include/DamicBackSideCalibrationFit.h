#ifndef DAMICBACKSIDECALIBRATIONFIT_H
#define DAMICBACKSIDECALIBRATIONFIT_H

// Custom includes
#include "DamicParticleInteraction.h"

// Standard includes
#include <cassert>
#include <cmath>
#include <random>
#include <chrono>

// ROOT includes
#include "TF1.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TMath.h"

// Minuit files
#include "Minuit2/FunctionMinimum.h"
#include "Minuit2/MnMigrad.h"
#include "Minuit2/MnHesse.h"
#include "Minuit2/MnUserParameters.h"
#include "Minuit2/MnPrint.h"
#include "Minuit2/FCNBase.h"
#include "Minuit2/CombinedMinimizer.h"
#include "Minuit2/MnSimplex.h"
#include "Minuit2/MnFumiliMinimize.h"
#include "Minuit2/MnMinimize.h"
#include "Minuit2/MnScan.h"

// Defining PCC function
const int npolynomial = 8;
const int npar = 10;
extern std::array<double, npar> nominalParameters;// = {0.00745023, 0.027043, 0.00371954, 0.0728877, -0.0416052, 0.00999033, -0.0011136, 4.68929e-05, 0};
double fpcc(double *x, double * par);

// Define data containers
struct CalibrationFitData_t
{
	CalibrationFitData_t() {};
	CalibrationFitData_t(double tSh, double tSc, double fSh, double fSc, bool conv) :
		trueShift(tSh), trueScale(tSc), fitShift(fSh), fitScale(fSc), isConverged(conv) {};
	double trueShift, trueScale, fitShift, fitScale;
	bool isConverged;
};

// Define overall fit functions
ROOT::Minuit2::FunctionMinimum PerformCalibrationFit(TH1D& data, ParticleCollection& simulation);
void MonteCarloCalibrationFit(ParticleCollection& simulation, std::vector<CalibrationFitData_t> vCalData, unsigned seed = 0);

// Define Monte Carlo Functions
TH1D GeneratePoissonBinHisto(TH1D & histo, unsigned seed = 0, std::string name = "poissFluc");

// Define write functions
void ConvertCalFitDataToTree(std::vector<CalibrationFitData_t> & vCalData, TTree &t); 

// Define minimizer function 
class CalibrationFitFcn : public ROOT::Minuit2::FCNBase{
public:
	CalibrationFitFcn(TH1D& data, ParticleCollection& sim, double * params);
	~CalibrationFitFcn();

	virtual double Up() const {return theErrorDef; }
	virtual double operator()(const std::vector<double>& ) const;

	void setErrorDef(double def) {theErrorDef = def; };
	

private:
	TH1D theData;
	TH1D * theTemplate;
	ParticleCollection theCollection;
	double theErrorDef;
	TF1 * fPartialCharge; // Partial Charge transformation
	double * fPartialChargeParams;
};

#endif