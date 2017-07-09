/*
 * LRun.cpp
 *
 *  Created on: Feb 12, 2017
 *      Author: vlad
 */

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
{
	savefile = 0;
	savedir = 0;

	chamberLength = 1.4*4;  // cm

	printStep = nev/5;
	if (printStep == 0) printStep = 1;

	tracks = new std::vector<LTrack*>(nEvents);
	tracks->reserve(nEvents);
}

LRun::~LRun()
{
	delete tracks;
}

void LRun::SetOutFile(TFile& f)
{
	savefile = &f;
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
	gas->SetComposition("he", 80., "isobutane", 20.);
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
		if (iEv % printStep == 0) std::cout << "Generating: " << iEv << "/" << nEvents << " events\n";
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

	savefile->cd();
	savedir = savefile->GetDirectory(GenerateDirName().c_str());
	if(!savedir) {
		savedir = savefile->mkdir(GenerateDirName().c_str());
	}
	savedir->cd();

	/**
	 * Histograms to save
	 */
	TH1F NePerCluster;
	NePerCluster.SetName("NePerCluster");
	NePerCluster.SetBins(2000, 0, 2000);

	TH1F hX;
	hX.SetName("X");
	hX.SetBins(300, 0, chamberLength);
	hX.GetXaxis()->SetTitle("X, cm");

	TH1F hY;
	hY.SetName("Y");
	hY.SetBins(300, -20, 20);
	hY.GetXaxis()->SetTitle("Y, cm");

	TH1F hZ;
	hZ.SetName("Z");
	hZ.SetBins(300, -20, 20);
	hZ.GetXaxis()->SetTitle("Z, cm");

	TH1F dEPerCluster;
	dEPerCluster.SetName("dEPerCluster");
	dEPerCluster.SetBins(3000, 0, 7.5e+03*chamberLength);
	dEPerCluster.GetXaxis()->SetTitle("Cluster deposit energy, eV");

	TH1F dEPerTrack;
	dEPerTrack.SetName("dEPerTrack");
	dEPerTrack.SetBins(150, 0, 8.75e3*chamberLength);
	dEPerTrack.GetXaxis()->SetTitle("Track deposit energy, eV");

	TH1F NePerTrack;
	NePerTrack.SetName("NePerTrack");
	NePerTrack.SetBins(150, 0, 2e+02*chamberLength);
	NePerTrack.GetXaxis()->SetTitle("Track primary electrons");

	TH1F dNePer1mm;
	dNePer1mm.SetName("dNePerGap");
	dNePer1mm.SetBins(300, 0, 20);
	dNePer1mm.GetXaxis()->SetTitle("Primary electrons per gap");

	TH1F dNePer1mmTrunc;
	dNePer1mmTrunc.SetName("dNePerGapTrunc");
	dNePer1mmTrunc.SetBins(300, 0, 20);
	dNePer1mmTrunc.GetXaxis()->SetTitle("Primary electrons per gap, truncated");

	TH1F dEPer1mm;
	dEPer1mm.SetName("dEPerGap");
	dEPer1mm.SetBins(300, 0, 8e2);
	dEPer1mm.GetXaxis()->SetTitle("Deposit energy per gap, eV");

	TH1F dEPer1mmTrunc;
	dEPer1mmTrunc.SetName("dEPerGapTrunc");
	dEPer1mmTrunc.SetBins(300, 0, 8e2);
	dEPer1mmTrunc.GetXaxis()->SetTitle("Deposit energy per gap (truncated), eV");

	int nGaps = chamberLength/dl + 1;

	std::vector<double> nEperGapAlongTrack(nGaps, 0.);
	std::vector<double> meanNePerGap(nEvents, 0.);

	std::vector<double> dEperGapAlongTrack(nGaps, 0.);
	std::vector<double> meandEPerGap(nEvents, 0.);

	for(std::vector<LTrack*>::iterator itr = tracks->begin(); itr != tracks->end(); ++itr) {
		LTrack* track = *itr;
		double esum = 0.;
		double nsum = 0.;
		for(int igap=0; igap<nGaps; ++igap) nEperGapAlongTrack[igap] = 0;
		for(int igap=0; igap<nGaps; ++igap) dEperGapAlongTrack[igap] = 0;

		for(std::vector<LHit*>::iterator ihit = track->hits->begin(); ihit != track->hits->end(); ++ihit) {
			LHit* hit = *ihit;
			hX.Fill(hit->X);
			hY.Fill(hit->Y);
			hZ.Fill(hit->Z);
			NePerCluster.Fill(hit->nE);
			dEPerCluster.Fill(hit->Ch);

			nsum += hit->nE;
			esum += hit->Ch;

			nEperGapAlongTrack[hit->X/dl] += hit->nE;  // hits per gap
			dEperGapAlongTrack[hit->X/dl] += hit->Ch;  // dE per gap
		}
		dEPerTrack.Fill(esum);
		NePerTrack.Fill(nsum);

		int index = itr-tracks->begin();
		meanNePerGap[index] = Mean(nEperGapAlongTrack);
		for(std::vector<double>::iterator igap = nEperGapAlongTrack.begin(); igap != nEperGapAlongTrack.end(); ++igap) {
			dNePer1mm.Fill(*igap);
		}

		meandEPerGap[index] = Mean(dEperGapAlongTrack);
		for(std::vector<double>::iterator igap = dEperGapAlongTrack.begin(); igap != dEperGapAlongTrack.end(); ++igap) {
			dEPer1mm.Fill(*igap);
		}
	}

	/**
	 * Truncation
	 */
	std::sort (meanNePerGap.begin(), meanNePerGap.end());
	for(int i=0; i<0.8*meanNePerGap.size(); ++i) {
		dNePer1mmTrunc.Fill(meanNePerGap[i]);
	}

	std::sort (meandEPerGap.begin(), meandEPerGap.end());
	for(int i=0; i<0.8*meandEPerGap.size(); ++i) {
		dEPer1mmTrunc.Fill(meandEPerGap[i]);
	}

	/**
	 * Save histograms
	 */
	hX.Write();
	hY.Write();
	hZ.Write();
	NePerCluster.Write();
	dEPerCluster.Write();
	dEPerTrack.Write();
	NePerTrack.Write();
	dNePer1mm.Write();
	dNePer1mmTrunc.Write();
	dEPer1mm.Write();
	dEPer1mmTrunc.Write();

	dETruncPer1mmFitted = dEPer1mmTrunc.GetMean();
	dETruncPer1mmFittedError = dEPer1mmTrunc.GetStdDev();
}

std::string LRun::GenerateDirName()
{
	std::ostringstream strs;
	strs << "E=" << energy*1e-6 << "MeV";
//	strs << "gap=" << dl*10 << "mm";

	return strs.str();
}

double LRun::Mean(const std::vector<double>& vec)
{
	return std::accumulate(vec.begin(), vec.end()-1, 0.0) / (vec.size()-1);
}


