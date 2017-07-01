#include "OutputManager.hpp"

#include "G4Track.hh"
#include "G4ThreeVector.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include <TFile.h>
#include <TTree.h>

OutputManager::OutputManager() :
fRootFile(0)
, fDetectorTree(0)
, fPhiVertex(0)
, fPhi(0)
, fThetaVertex(0)
, fTheta(0)
, fT(0)
, fEkinVertex(0)
, fEkin(0)
, fEloss(0)
, fZVertex(0)
, fTrackLength(0)
, fPx(0)
, fPy(0)
, fPz(0)
, fPosX(0)
, fPosY(0)
, fPosZ(0)
{ }

OutputManager::~OutputManager() {
	if (fRootFile) delete fRootFile;
}

void OutputManager::Initialize() {
	G4String fileName = "photoconversion.root";
	fRootFile = new TFile(fileName, "RECREATE");

	if(!fRootFile) {
		G4cout << "OutputManager::Initialize: Problem creating the ROOT TFile" << G4endl;
		return;
	}

	fDetectorTree = new TTree("detectorTree", "Conversion");
	fDetectorTree->Branch("phi", &fPhi); // phi angle
	fDetectorTree->Branch("theta", &fTheta); // theta angle to z axis
	fDetectorTree->Branch("EkinVertex", &fEkinVertex);
	fDetectorTree->Branch("Ekin", &fEkin); // kinetic energy
	fDetectorTree->Branch("ZVertex", &fZVertex); // z value of the vertex position (track creation point)
	fDetectorTree->Branch("TrackLength", &fTrackLength);
	fDetectorTree->Branch("PosX", &fPosX); // x position
	fDetectorTree->Branch("PosY", &fPosY); // y position
	fDetectorTree->Branch("PosZ", &fPosZ); // z position
	fDetectorTree->Branch("Px", &fPx); // x momentum
	fDetectorTree->Branch("Py", &fPy); // y momentum
	fDetectorTree->Branch("Pz", &fPz); // z momentum
	fDetectorTree->Branch("t", &fT); // time

	G4cout << "\n----> Output file is: " << fileName << G4endl;
}

void OutputManager::Save() { 
	if (fRootFile) {
		fRootFile->Write();
		fRootFile->Close();
		G4cout << "\n----> Output Tree is saved \n" << G4endl;
	}
}

void OutputManager::FillHit(G4Track* track)
{
	G4ThreeVector pos = track->GetPosition();
	G4ThreeVector dirVertex = track->GetVertexMomentumDirection();
	G4ThreeVector dir = track->GetMomentumDirection();
	fPhiVertex.push_back(dirVertex.getPhi());
	fPhi.push_back(dir.getPhi());
	fThetaVertex.push_back(dirVertex.getTheta());
	fTheta.push_back(dir.getTheta());
	fPx.push_back(dir.x());
	fPy.push_back(dir.y());
	fPz.push_back(dir.z());

	// using garfield++ units here (cm, ns, eV)
	fPosX.push_back(pos.x()/cm);
	fPosY.push_back(pos.y()/cm);
	fPosZ.push_back(pos.z()/cm);
	fT.push_back(track->GetGlobalTime()/ns);
	fEkinVertex.push_back(track->GetVertexKineticEnergy()/eV);
	fEkin.push_back(track->GetKineticEnergy()/eV);
	fEloss.push_back(track->GetVertexKineticEnergy()/eV - track->GetKineticEnergy()/eV);

	fZVertex.push_back(track->GetVertexPosition().z()/cm);
	fTrackLength.push_back(track->GetTrackLength()/cm);
}

void OutputManager::FillEvent()
{
	if (fDetectorTree) fDetectorTree->Fill();
}

void OutputManager::PrintStatistic() {
	G4cout << "--- Tree Stats" << G4endl;
	// TODO replace GetEntries to vector total
	if(fDetectorTree) G4cout << " N_detector = " << fDetectorTree->GetEntries() << G4endl;
	G4cout << "---" << G4endl;
}
