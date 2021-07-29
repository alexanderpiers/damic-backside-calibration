#ifndef DAMICPARTICLEINTERACTION_H
#define DAMICPARTICLEINTERACTION_H

// Standard headers
#include <vector>
#include <algorithm>
#include <numeric>
#include <iostream>

// ROOT headers
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TF1.h"


#define keV 1000
#define um 1000
#define zoffset 0

struct Position
{
	Position() : x(0), y(0), z(0) {};
	Position(double x, double y, double z) : x(x), y(y), z(z){};

	double x, y, z;
};

class ParticleInteraction
{
public:
	ParticleInteraction();
	ParticleInteraction(std::vector<Position> pos, std::vector<double> energy, std::vector<int> trackid, int eventid );
	~ParticleInteraction();

	// Setters
	void SetPosition(std::vector<Position> position){ this->position = position; };
	void SetTrackID(std::vector<int> trackid){ this->trackid = trackid; };
	void SetRunID(int eventid){ this->eventid = eventid; };
	void SetNSteps(int nsteps){ this->nsteps = nsteps; };


	// Getters
	std::vector<double> GetX();
	std::vector<double> GetY();
	std::vector<double> GetZ();
	double GetTotalEnergy();
	std::vector<double> GetEnergy() { return energy; };
	std::vector<Position> GetPosition(){ return position; };
	std::vector<int> GetTrackID(){ return trackid; }
	Position GetWeightedPosition();
	int GetRunID(){ return eventid; };
	int GetNSteps() { return nsteps; };

private:
	std::vector<Position> position;
	std::vector<double> energy;
	std::vector<int> trackid;
	int eventid;
	int nsteps;
};

class ParticleCollection
{
public:
	ParticleCollection();
	ParticleCollection(std::vector<ParticleInteraction> );
	ParticleCollection(const ParticleCollection & parcol);
	~ParticleCollection();

	void ReadROOTFile(const char * filename);
	void PlotEnergyProjection(TH1D * hSpectrum);
	void ApplyPartialChargeModel(TH1D * hCorrSpectrum, TF1 * fPartialCharge) const;
	void GenerateEZInteraction(TH2D * h2);


	std::vector<ParticleInteraction> collection;
	
};

#endif

