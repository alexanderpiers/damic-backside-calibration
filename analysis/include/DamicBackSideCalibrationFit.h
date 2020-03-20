#ifndef DAMICBACKSIDECALIBRATIONFIT_H
#define DAMICBACKSIDECALIBRATIONFIT_H

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
const int npar = 9;
extern double nominalParameters[npar];// = {0.00745023, 0.027043, 0.00371954, 0.0728877, -0.0416052, 0.00999033, -0.0011136, 4.68929e-05, 0};
double fpcc(double *x, double * par);

// Define minimizer function 
class CalibrationFitFcn : public ROOT::Minuit2::FCNBase{
public:
	CalibrationFitFcn(TH1D& data, TH1D& sim) : theData(data), 
		theTemplate(sim), theErrorDef(1.) {};
	~CalibrationFitFcn() {};

	virtual double up() const {return theErrorDef; }; 
	virtual double operator()(const std::vector<double>& ) const;

	void setErrorDef(double def) {theErrorDef = def; };
	

private:
	TH1D theData, theTemplate;
	double theErrorDef;
};

#endif