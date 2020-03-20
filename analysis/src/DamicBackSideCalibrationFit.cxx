#include "DamicBackSideCalibrationFit.h"

double nominalParameters[npar] = {0.00745023, 0.027043, 0.00371954, 0.0728877, -0.0416052, 0.00999033, -0.0011136, 4.68929e-05, 0};
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

