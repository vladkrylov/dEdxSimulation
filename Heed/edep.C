#include <iostream>
#include <vector>
#include <numeric>

#include <TCanvas.h>
#include <TROOT.h>
#include <TApplication.h>
#include <TH1F.h>
#include <TFile.h>
#include <TTree.h>

#include "MediumMagboltz.hh"
#include "SolidBox.hh"
#include "GeometrySimple.hh"
#include "ComponentConstant.hh"
#include "Sensor.hh"
#include "TrackHeed.hh"
#include "Plotting.hh"
#include "Random.hh"

using namespace Garfield;

int main(int argc, char * argv[])
{
	randomEngine.Seed(123456);
	TApplication app("app", &argc, argv);
//	plottingEngine.SetDefaultStyle();

	// Cluster coordinates
	double xc = 0., yc = 0., zc = 0., tc = 0.;
	// Number of electrons produced in a collision
	int nc = 0;
	// Energy loss in a collision
	double ec = 0.;
	// Dummy variable (not used at present)
	double extra = 0.;
	// Total energy loss along the track
	double esum = 0.;
	double egap = 0.;
	// Total number of electrons produced along the track
	int nsum = 0;
	// Total number of clusters produced along the track
	int ncl = 0;

	// output .root file
	TFile f("Results.root","recreate");
	TTree *tClusters = new TTree("Clusters", "ClustersInfo");
	tClusters->Branch("X", &xc, "X/D");
	tClusters->Branch("Y", &yc, "Y/D");
	tClusters->Branch("Z", &zc, "Z/D");
	tClusters->Branch("Nelectrons", &nc, "Nelectrons/I");
	tClusters->Branch("dE", &ec, "dE/D");

	TTree *tTracks = new TTree("Tracks", "TracksInfo");
	tTracks->Branch("dE", &esum, "dE/D");
	tTracks->Branch("Nelectrons", &nsum, "Nelectrons/I");
	tTracks->Branch("NClusters", &ncl, "NClusters/I");
	tTracks->Branch("dEGapMean", &egap, "dEGapMean/D");

	// Histograms
	TH1::StatOverflows(kTRUE);
	TH1F* hElectrons = new TH1F("hElectrons", "Number of electrons",
							  200, 0, 200);
	TH1F* hEdep = new TH1F("hEdep", "Energy Loss",
						 100, 0., 10.);

	// Make a medium
	MediumMagboltz* gas = new MediumMagboltz();
	gas->SetComposition("he", 80., "isobutane", 20.);
	gas->SetTemperature(293.15);
	gas->SetPressure(760.);

	// Detector geometry
	// Gap [cm]
	const double width = 1.4*4*3;
	double dl = 0.2;  // cm
	int nGaps = width/dl + 1;
	std::vector<double> nEperGap1Track(nGaps, 0.);

	SolidBox* box = new SolidBox(width / 2., 0., 0., width / 2., 10., 10.);
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
	track->SetParticle("e-");
	track->SetEnergy(2e6);

	const int nEvents = 1000;
	track->EnableDebugging();

	for (int i = 0; i < nEvents; ++i) {
		if (i == 1) track->DisableDebugging();
		if (i % 100 == 0) std::cout << i << "/" << nEvents << "\n";
		// Initial position and direction
		double x0 = 0., y0 = 0., z0 = 0., t0 = 0.;
		double dx0 = 1., dy0 = 0., dz0 = 0.;
		track->NewTrack(x0, y0, z0, t0, dx0, dy0, dz0);

		ncl = 0;
		esum = 0;
		nsum = 0;

		for(int igap=0; igap<nGaps; ++igap) nEperGap1Track[igap] = 0;

		// Loop over the clusters.
		while (track->GetCluster(xc, yc, zc, tc, nc, ec, extra)) {
			ncl++;
			esum += ec;
			nsum += nc;
			tClusters->Fill();

			nEperGap1Track[xc/dl] += nc/(10*dl);  // hits per mm
		}
		egap = std::accumulate(nEperGap1Track.begin(), nEperGap1Track.end()-1, 0.0) / (nGaps-1);

		tTracks->Fill();

		hElectrons->Fill(nsum);
		hEdep->Fill(esum * 1.e-3);
	}
	tClusters->Write();
	tTracks->Write();

	f.Close();
}
