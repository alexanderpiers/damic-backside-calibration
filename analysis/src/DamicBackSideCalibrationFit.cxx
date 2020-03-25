#include "DamicBackSideCalibrationFit.h"

std::array<double, npar> nominalParameters = {0.00745023, 0.027043, 0.00371954, 0.0728877, -0.0416052, 0.00999033, -0.0011136, 4.68929e-05, 0, 1};
double fpcc(double *x, double * par){


	// Check to see if we are outside PCC region
	if(x[0] < 668.5) return 1;

	// Transform x
	double xtrans = -(x[0] - par[8] - 675);

	// Evaluate polynomial
	const int ndegrees = 7; // order of polynomial
	double fv = 0;
	for (int i = 0; i <= ndegrees; ++i)
	{
		fv += par[i] * TMath::Power(xtrans, i);
	}

	// Check boundary conditions and return
	if(fv < 0){
		return 0;
	}else if(fv > 1){
		return 1;
	}else{
		return fv;
	}
}

CalibrationFitFcn::CalibrationFitFcn(TH1D& data, ParticleCollection& sim, double * params) :
		theData(data), theCollection(sim), theErrorDef(0.5) 
{
	fPartialChargeParams = params;

	// Initialize template histogram
	theTemplate = new TH1D("template", "template", data.GetNbinsX(), data.GetXaxis()->GetXmin(), data.GetXaxis()->GetXmax());

	// Initialize the PCC transformation function
	fPartialCharge = new TF1("fpcc", fpcc, 0, 675, npar);
	fPartialCharge->SetParameters(params);

}

CalibrationFitFcn::~CalibrationFitFcn(){
	delete theTemplate;
	delete fPartialCharge;
}
double CalibrationFitFcn::operator()(const std::vector<double>& par) const{
	
	// Fit parameters are shift and scale
	assert(par.size() == 3);

	// Initial calculations
	int nbins = theData.GetNbinsX();

	// Alter the fPartialChargeParams
	for (int i = 0; i < par.size(); ++i)
	{
		fPartialChargeParams[npolynomial + i] = par[i];
		printf("%0.4f  ", par[i]);
	}
	fPartialCharge->SetParameters(fPartialChargeParams);

	// Apply the transformation to the simulated data
	theCollection.ApplyPartialChargeModel(theTemplate, fPartialCharge);

	// Perform compute squared error for minimization
	// double squaredError = 0;

	// for (int i = 1; i <= nbins; ++i)
	// {
	// 	double d = theData.GetBinContent(i);
	// 	double t = theTemplate->GetBinContent(i);
	// 	double binError = theData.GetBinContent(i) - theTemplate->GetBinContent(i);
	// 	squaredError += ( binError * binError );
	// }
	// printf("%.1f\n", squaredError);
	// return squaredError;

	double nloglike = 0;
	for (int i = 0; i < nbins; ++i)
	{
		double mu_i = theTemplate->GetBinContent(i);
		double n_i = theData.GetBinContent(i);
		if(mu_i <= 0) continue;
		nloglike += ( mu_i - n_i * log(mu_i) );
	}
	printf("%.2f\n", nloglike);
	return nloglike;
}

ROOT::Minuit2::FunctionMinimum PerformCalibrationFit(TH1D& data, ParticleCollection& simulation){

	// Set up constants
	double initialTranslation = 0;
	double initialScaling = 1;

	// Create function object
	CalibrationFitFcn calFcn(data, simulation, nominalParameters.data());

	// Create calibration fit function object
	ROOT::Minuit2::MnUserParameters upar;

	upar.Add("Translation", initialTranslation, 1);
	upar.Add("Scaling", initialScaling, 0.1);
	upar.Add("FlatAmplitude", 0, 0.1); // Needed for later when fitting a more general spectrum

	// Conditions on parameters
	upar.SetLimits("Translation", -1, 1);
	upar.Fix("Scaling");
	upar.Fix("FlatAmplitude");

	// Checking a manual scan
	// double min = -1;
	// double max = 1;
	// int nbins = 100;
	// std::vector<double> testpar = {0, 1, 0};
	// for (int i = 0; i < nbins; ++i)
	// {
	// 	testpar[0] = min + (max - min) * i / nbins;
	// 	calFcn(testpar);
	// }

	// Create Migrad minimizer and minimize 
	// ROOT::Minuit2::MnMigrad migrad(calFcn, upar);
	// ROOT::Minuit2::FunctionMinimum calMinimum = migrad();

	// Create simplex minimizer and minimize
	ROOT::Minuit2::MnSimplex simplex(calFcn, upar);
	ROOT::Minuit2::FunctionMinimum calMinimumSx = simplex(1000, 1.);


	return calMinimumSx;
}
