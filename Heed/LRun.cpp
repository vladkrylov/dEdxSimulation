/*
 * LRun.cpp
 *
 *  Created on: Feb 12, 2017
 *      Author: vlad
 */

#include "SystemOfUnits.h"
#include "LRun.h"

#include <numeric>
#include <algorithm>

#include <TCanvas.h>
#include <TApplication.h>
#include <TTree.h>

#include <MediumMagboltz.hh>
#include "SolidBox.hh"
#include "GeometrySimple.hh"
#include "ComponentConstant.hh"
#include "Sensor.hh"
#include "TrackHeed.hh"
#include "Plotting.hh"
#include "Random.hh"

using namespace Garfield;

LRun::LRun( double myEnergy,
			int nev,
			const std::string& particle,
			double gap)
: fGenerated(false)
, energy(myEnergy)
, nEvents(nev)
, primParticle(particle)
, dl(gap)
, detectorTree(0)
{
	savefile = 0;
	savedir = 0;

//	chamberLength = 1.4*4;  // cm
	chamberLength = 1.;  // cm

	printStep = nev/10;
	if (printStep == 0) printStep = 1;

	tracks = new std::vector<LTrack*>(nEvents);
	tracks->reserve(nEvents);
}

LRun::~LRun()
{
	delete tracks;
	delete detectorTree;
}

void LRun::SetOutFile(TFile& f)
{
	savefile = &f;
	detectorTree = new TTree(GenerateDirName().c_str(), "Heed");
}

void LRun::Generate()
{
	randomEngine.Seed(123456);
//	plottingEngine.SetDefaultStyle();

	// Cluster coordinates
	double xc = 0., yc = 0., zc = 0., tc = 0.;
	// Number of electrons produced in a collision
	int nc = 0;
	// Energy loss in a collision
	double ec = 0.;
	// Dummy variable (not used at present)
	double extra = 0.;
//	// Total energy loss along the track
//	double esum = 0.;
//	double egap = 0.;
//	// Total number of electrons produced along the track
//	int nsum = 0;
//	// Total number of clusters produced along the track
//	int ncl = 0;

	// Make a medium
	MediumMagboltz* gas = new MediumMagboltz();
//	gas->SetComposition("he", 80., "isobutane", 20.);
	gas->SetComposition("ar", 70., "co2", 30.);
	gas->SetTemperature(293.15);
	gas->SetPressure(760.);

	// Detector geometry
	// Gap [cm]


	SolidBox* box = new SolidBox(chamberLength / 2., 0., 0., chamberLength / 2., 10., 10.);
	GeometrySimple* geo = new GeometrySimple();
	geo->AddSolid(box, gas);

	// Make a component
	ComponentConstant* comp = new ComponentConstant();
	comp->SetGeometry(geo);
	comp->SetElectricField(100., 0., 0.);

	// Make a sensor
	Sensor* sensor = new Sensor();
	sensor->AddComponent(comp);

	// Track class
	TrackHeed* track = new TrackHeed();
	track->SetSensor(sensor);
	track->SetParticle(primParticle.c_str());
	track->SetEnergy(energy);

//	track->EnableDebugging();

	for (int iEv = 0; iEv < nEvents; ++iEv) {
//		if (i == 1) track->DisableDebugging();4
		if ((iEv+1) % printStep == 0) std::cout << "Generating: " << iEv+1 << "/" << nEvents << " events\n";
//		// Initial position and direction
		double x0 = 0., y0 = 0., z0 = 0., t0 = 0.;
		double dx0 = 1., dy0 = 0., dz0 = 0.;
		track->NewTrack(x0, y0, z0, t0, dx0, dy0, dz0);
//
////		for(int igap=0; igap<nGaps; ++igap) nEperGap1Track[igap] = 0;
//
		// Loop over the clusters.
		(*tracks)[iEv] = new LTrack();
		while (track->GetCluster(xc, yc, zc, tc, nc, ec, extra)) {
			LHit* h = new LHit();
			h->X = xc;
			h->Y = yc;
			h->Z = zc;
			h->t = tc;
			h->Ch = ec;
			h->nE = nc;

//			if (nc > 60)
//				std::cout << h.Ch << std::endl;

			(*tracks)[iEv]->AddHit(h);
		}
	}
	fGenerated = true;
}

void LRun::Analyze()
{
	if (!savefile) {
		std::cout << "No .root file was provided to save results. Analysis was not performed." << std::endl;
		return;
	}

	if (!fGenerated) {
		std::cout << "No input data. Analysis was not performed." << std::endl;
		return;
	}

//	savefile->cd();
//	savedir = savefile->GetDirectory(GenerateDirName().c_str());
//	if(!savedir) {
//		savedir = savefile->mkdir(GenerateDirName().c_str());
//	}
//	savedir->cd();
//	if (detectorTree) delete detectorTree;

	detectorTree->Branch("PosX", &fPosX); // x position
	detectorTree->Branch("PosY", &fPosY); // y position
	detectorTree->Branch("PosZ", &fPosZ); // z position
	detectorTree->Branch("dEPerCluster", &fdEPerCluster);
	detectorTree->Branch("NePerCluster", &fNePerCluster);

	detectorTree->Branch("dEPerTrack", &fdEPerTrack);
	detectorTree->Branch("NePerTrack", &fNePerTrack);
	detectorTree->Branch("ClPerTrack", &fClPerTrack);

	detectorTree->Branch("dEPerGap", &fdEPerGap);
	detectorTree->Branch("NePerGap", &fNePerGap);

	/**
	 * data to save
	 */
//	TH1F NePerCluster;
//	NePerCluster.SetName("NePerCluster");
//	NePerCluster.SetBins(2000, 0, 2000);
//
//	TH1F hX;
//	hX.SetName("X");
//	hX.SetBins(300, 0, chamberLength);
//	hX.GetXaxis()->SetTitle("X, cm");
//
//	TH1F hY;
//	hY.SetName("Y");
//	hY.SetBins(300, -20, 20);
//	hY.GetXaxis()->SetTitle("Y, cm");
//
//	TH1F hZ;
//	hZ.SetName("Z");
//	hZ.SetBins(300, -20, 20);
//	hZ.GetXaxis()->SetTitle("Z, cm");
//
//	TH1F dEPerCluster;
//	dEPerCluster.SetName("dEPerCluster");
//	dEPerCluster.SetBins(3000, 0, 7.5e+03*chamberLength);
//	dEPerCluster.GetXaxis()->SetTitle("Cluster deposit energy, eV");
//
//	TH1F dEPerTrack;
//	dEPerTrack.SetName("dEPerTrack");
//	dEPerTrack.SetBins(150, 0, 8.75e3*chamberLength);
//	dEPerTrack.GetXaxis()->SetTitle("Track deposit energy, eV");
//
//	TH1F NePerTrack;
//	NePerTrack.SetName("NePerTrack");
//	NePerTrack.SetBins(150, 0, 2e+02*chamberLength);
//	NePerTrack.GetXaxis()->SetTitle("Track primary electrons");
//
//	TH1F dNePer1mm;
//	dNePer1mm.SetName("dNePerGap");
//	dNePer1mm.SetBins(300, 0, 20);
//	dNePer1mm.GetXaxis()->SetTitle("Primary electrons per gap");
//
//	TH1F dNePer1mmTrunc;
//	dNePer1mmTrunc.SetName("dNePerGapTrunc");
//	dNePer1mmTrunc.SetBins(300, 0, 20);
//	dNePer1mmTrunc.GetXaxis()->SetTitle("Primary electrons per gap, truncated");
//
//	TH1F dEPer1mm;
//	dEPer1mm.SetName("dEPerGap");
//	dEPer1mm.SetBins(300, 0, 8e2);
//	dEPer1mm.GetXaxis()->SetTitle("Deposit energy per gap, eV");
//
//	TH1F dEPer1mmTrunc;
//	dEPer1mmTrunc.SetName("dEPerGapTrunc");
//	dEPer1mmTrunc.SetBins(300, 0, 8e2);
//	dEPer1mmTrunc.GetXaxis()->SetTitle("Deposit energy per gap (truncated), eV");

	int nGaps = chamberLength/dl + 1;

	std::vector<double> nEperGapAlongTrack(nGaps, 0.);
	std::vector<double> meanNePerGap(nEvents, 0.);

	std::vector<double> dEperGapAlongTrack(nGaps, 0.);
	std::vector<double> meandEPerGap(nEvents, 0.);

	for(std::vector<LTrack*>::iterator itr = tracks->begin(); itr != tracks->end(); ++itr) {
		fPosX.clear();
		fPosY.clear();
		fPosZ.clear();
		fdEPerCluster.clear();
		fNePerCluster.clear();

		fdEPerGap.clear();
		fNePerGap.clear();

		fdEPerTrack = 0.;
		fNePerTrack = 0.;

		LTrack* track = *itr;
		fdEPerGap.resize(nGaps, 0.);
		fNePerGap.resize(nGaps, 0.);

		for(std::vector<LHit*>::iterator ihit = track->hits->begin(); ihit != track->hits->end(); ++ihit) {
			LHit* hit = *ihit;

			fPosX.push_back(hit->X);
			fPosY.push_back(hit->Y);
			fPosZ.push_back(hit->Z);
			fNePerCluster.push_back(hit->nE);
			fdEPerCluster.push_back(hit->Ch/keV);

			fNePerTrack += hit->nE;
			fdEPerTrack += hit->Ch/keV;

			fNePerGap[hit->X/dl] += hit->nE;  // hits per gap
			fdEPerGap[hit->X/dl] += hit->Ch/keV;  // dE per gap
		}
		detectorTree->Fill();
	}
	detectorTree->Write();
//	/**
//	 * TODO Truncation
//	 */
//	std::sort (meanNePerGap.begin(), meanNePerGap.end());
//	for(int i=0; i<0.8*meanNePerGap.size(); ++i) {
//		dNePer1mmTrunc.Fill(meanNePerGap[i]);
//	}
//
//	std::sort (meandEPerGap.begin(), meandEPerGap.end());
//	for(int i=0; i<0.8*meandEPerGap.size(); ++i) {
//		dEPer1mmTrunc.Fill(meandEPerGap[i]);
//	}

	/**
	 * Save data
	 */
	dETruncPer1mmFitted = 1;
	dETruncPer1mmFittedError = 1;
}

std::string LRun::GenerateDirName()
{
	std::ostringstream strs;
	strs << "E=" << energy/MeV << "MeV";
//	strs << "gap=" << dl*10 << "mm";

	return strs.str();
}

double LRun::Mean(const std::vector<double>& vec)
{
	return std::accumulate(vec.begin(), vec.end()-1, 0.0) / (vec.size()-1);
}


