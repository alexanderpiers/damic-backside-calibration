#include "DamicBackSideCalibrationFit.h"

std::array<double, npar> nominalParameters = {0.00745023, 0.027043, 0.00371954, 0.0728877, -0.0416052, 0.00999033, -0.0011136, 4.68929e-05, 0, 1};
double fpcc(double *x, double * par){


	// Check to see if we are outside PCC region
	double shift = 675;
	if(x[0] < (shift - 7 / par[9])) return 1;

	// Transform x
	double xtrans = -par[9] * (x[0] - par[8] -  shift);

	// Evaluate polynomial
	const int ndegrees = 7; // order of polynomial
	double fv = 0;
	for (int i = 0; i <= ndegrees; ++i)
	{
		fv += par[i] * TMath::Power(xtrans, i);
	}

	// Check boundary conditions and return
	// cout << xtrans << "  " << fv << endl;
	if(fv < 0){
		return 0;
	}else if(fv > 1){
		return 1;
	}else{
		return fv;
	}
}

void CalibrationFitData_t::Print(){
	printf("True Shift: %.3f, Fit Shift: %.3f, True Scale: %.3f, Fit Scale: %.3f, True Shift: %.3f, Converged: %i\n", 
		trueShift, fitShift, trueScale, fitScale, isConverged);
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
		// printf("%0.4f  ", par[i]);
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
	// printf("%.2f\n", nloglike);
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
	upar.Add("Normalization", 0, 0.1); // Needed for later when fitting a more general spectrum

	// Conditions on parameters
	upar.SetLimits("Translation", -1, 1);
	upar.SetLimits("Scaling", 0.8, 1.2);
	upar.Fix("Normalization");

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
	ROOT::Minuit2::FunctionMinimum calMinimumSx = simplex(1000, 10.);


	return calMinimumSx;
}

// Data structure conversions
void ConvertCalFitDataToTree(std::vector<CalibrationFitData_t> const &vCalData, TTree &t){

	// Define tree variables
	double trueShift, trueScale, fitShift, fitScale;
	bool isConverged;

	// Create branches
	t.Branch("trueShift",   &trueShift);
	t.Branch("trueScale",   &trueScale);
	t.Branch("fitShift",    &fitShift);
	t.Branch("fitScale",    &fitScale);
	t.Branch("isConverged", &isConverged);
	
	// Write data to tree
	for( auto cal : vCalData){

		trueShift   = cal.trueShift;
		trueScale   = cal.trueScale;
		fitShift    = cal.fitShift;
		fitScale    = cal.fitScale;
		isConverged = cal.isConverged;
		std::cout << cal.trueShift << "\t" << cal.fitShift << std::endl;

		t.Fill();
	}
}

// Monte Carlo Functions
TH1D * GeneratePoissonBinHisto(TH1D & histo, unsigned seed, std::string name){

	// Create new histogram
	int nbins   = histo.GetNbinsX();
	double xmin = histo.GetXaxis()->GetXmin();
	double xmax = histo.GetXaxis()->GetXmax();
	TH1D * poissFluc = new TH1D(name.c_str(), name.c_str(), nbins, xmin, xmax);

	// Create random number generator
	std::default_random_engine generator;

	if(seed){
		generator.seed(seed);
	}else{
		generator.seed(std::chrono::high_resolution_clock::now().time_since_epoch().count());
	}

	for (int i = 1; i <= nbins; ++i)
	{
		double xc = histo.GetBinCenter(i);
		double mu = histo.GetBinContent(i);
		std::poisson_distribution<int> pois(mu);

		// Compute bin value. Set bin and error value
		double poissBinValue = pois(generator);
		poissFluc->SetBinContent(i, poissBinValue);
		poissFluc->SetBinError(i, sqrt((double) poissBinValue));
	}

	return poissFluc;

}

void MonteCarloCalibrationFit(ParticleCollection& simulation, std::vector<CalibrationFitData_t> &vCalData, unsigned seed){

	// Define relevant variables
	std::array<double, npar> params = nominalParameters;
	int ntrials = vCalData.size();
	TH1D hMC("histoMC", "histoMC", 200, 0, 60);
	TF1 * fPartialChargeMC = new TF1("fpcc", fpcc, 0, 675, npar);
	
	// Create random number generator
	std::default_random_engine generator;
	if(seed){
		generator.seed(seed);
	}else{
		generator.seed(std::chrono::high_resolution_clock::now().time_since_epoch().count());
	}
	std::uniform_real_distribution<double> randomShiftDistribution(-0.5, 0.5);
	std::uniform_real_distribution<double> randomScaleDistribution(0.9, 1.1);

	// Generate monte carlo data
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	std::vector<TH1D*> vDataMC(ntrials);
	// #pragma omp parallel for
	for (int i = 0; i < ntrials; ++i)
	{
		printf("Trial %i\n", i);

		// generate random fit parameters
		double shiftMC = randomShiftDistribution(generator);
		double scaleMC = randomScaleDistribution(generator);

		// Adjust parameters with monte carlo values
		params[npolynomial]     = shiftMC; 
		params[npolynomial + 1] = scaleMC;
		vCalData[i].trueShift   = shiftMC;
		vCalData[i].trueScale   = scaleMC;

		// TH1D * hMC = new TH1D(("histoMC" + std::to_string(i)).c_str(), "histoMC", 200, 0, 60);
		fPartialChargeMC->SetParameters(params.data());
		// TF1 *fPartialChargeMC = new TF1(("fpcc" + std::to_string(i)).c_str(), fpcc, 0, 675, npar);
		// fPartialChargeMC->SetParameters(params.data());
		simulation.ApplyPartialChargeModel(&hMC, fPartialChargeMC);
		vDataMC[i] = GeneratePoissonBinHisto(hMC, seed, "test_" + std::to_string(i));
		// delete hMC;
	}
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time to generate data: " << std::chrono::duration_cast<std::chrono::seconds>(end-start).count() << " s" << std::endl;

	// Perform fits on MC data
	// #pragma omp parallel for
	for (int i = 0; i < ntrials; ++i)
	{
		printf("Trial %i\n", i);
		ROOT::Minuit2::FunctionMinimum calminMC = PerformCalibrationFit(*vDataMC[i], simulation);

		// Parse fit results into data structure
		vCalData[i].fitShift = calminMC.UserParameters().Value(0);
		vCalData[i].fitScale = calminMC.UserParameters().Value(1);
		vCalData[i].isConverged = calminMC.IsValid();

		// std::cout << vCalData[i].trueShift << "\t" << vCalData[i].fitShift << std::endl;
	}


	delete fPartialChargeMC;
}

