#include "DamicParticleInteraction.h"

ParticleInteraction::ParticleInteraction() : eventid(0), nsteps(0) {};

ParticleInteraction::ParticleInteraction(std::vector<Position> position, std::vector<double> energy, std::vector<int> trackid, int eventid ){
	this->position = position;
	this->energy = energy;
	this->trackid = trackid;
	this->eventid = eventid;
	this->nsteps = energy.size();
}

ParticleInteraction::~ParticleInteraction(){};

double ParticleInteraction::GetTotalEnergy(){
	double totalEnergy = 0;
	for(auto e : energy){
		totalEnergy += e;
	}
	return totalEnergy;
}
std::vector<double> ParticleInteraction::GetX(){
	// Returns a vector of the x component of alls step interactios
	std::vector<double> x(nsteps);
	for (int i = 0; i < nsteps; ++i)
	{
		x[i] = position[i].x;
	}

	return x;
}

std::vector<double> ParticleInteraction::GetY(){
	// Returns a vector of the y component of alls step interactios
	std::vector<double> y(nsteps);
	for (int i = 0; i < nsteps; ++i)
	{
		y[i] = position[i].y;
	}

	return y;
}

std::vector<double> ParticleInteraction::GetZ(){
	// Returns a vector of the z component of alls step interactios
	std::vector<double> z(nsteps);
	for (int i = 0; i < nsteps; ++i)
	{
		z[i] = position[i].z;
	}

	return z;
}

ParticleCollection::ParticleCollection(){};
ParticleCollection::ParticleCollection(std::vector<ParticleInteraction> col){
	for (auto interaction : col)
	{
		collection.push_back(interaction);
	}
}

ParticleCollection::~ParticleCollection(){};

void ParticleCollection::ReadROOTFile(const char * filename){
	// Reads information from the Geant4 Root tree and parses into Particle collection

	TFile f(filename);
	TTree * t = (TTree *)f.Get("siData");

	// Set relevant Branches
	double x, y, z, energy;
	int trackid, eventid, prevEventId;

	std::vector<Position> vPosition;
	std::vector<double> vEnergy;
	std::vector<int> vTrackid;

	t->SetBranchAddress("x", &x);
	t->SetBranchAddress("y", &y);
	t->SetBranchAddress("z", &z);
	t->SetBranchAddress("energy", &energy);
	t->SetBranchAddress("EventID", &eventid);
	t->SetBranchAddress("TrackID", &trackid);

	prevEventId = 0;

	for (int i = 0; i < t->GetEntries(); ++i)
	{
		t->GetEntry(i);

		if(eventid != prevEventId && prevEventId !=0){
			// Create a particle interaction and append to the collection
			ParticleInteraction particle(vPosition, vEnergy, vTrackid, eventid);
			collection.push_back(particle);

			vPosition.clear();
			vEnergy.clear();
			vTrackid.clear();
		}

		vPosition.push_back(Position(x, y, z * um + zoffset));
		vEnergy.push_back(energy * keV);
		vTrackid.push_back(trackid);
		prevEventId = eventid;

	}
}

void ParticleCollection::PlotEnergyProjection(TH1D * hSpectrum){

	// Iterate over all interactions and histogram the total energy
	for(auto interaction : collection){
		hSpectrum->Fill(interaction.GetTotalEnergy());
	}
}

void ParticleCollection::ApplyPartialChargeModel(TH1D * hCorrSpectrum, TF1 * fPartialCharge){

	// Iterate over every particle interaction, applying fPartialCharge Correction
	for(auto interaction : collection){
		double totalCorrectedEnergy = 0;
		std::vector<double> z = interaction.GetZ();
		std::vector<double> energy = interaction.GetEnergy();

		// Iterate over each step position and apply partial charge correction
		for (int i = 0; i < interaction.GetNSteps(); ++i)
		{
			totalCorrectedEnergy += fPartialCharge->Eval(z[i]) * energy[i];
		}

		hCorrSpectrum->Fill(totalCorrectedEnergy);
	}
}
