//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
/// \file electromagnetic/TestEm8/src/HistoManager.cc
/// \brief Implementation of the HistoManager class
//
// $Id: HistoManager.cc 79241 2014-02-20 16:03:35Z gcosmo $
//
//---------------------------------------------------------------------------
//
// ClassName:   HistoManager
//
// Author:      V.Ivanchenko 01.09.2010
//
//----------------------------------------------------------------------------
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include <string>
#include <algorithm>

#include "HistoManager.hh"
#include "G4UnitsTable.hh"
#include "G4Step.hh"
#include "G4LossTableManager.hh"
#include "G4ElectronIonPair.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4RunManager.hh"
#include "PrimaryGeneratorAction.hh"
#include "HistoMessenger.hh"

#include <TFile.h>
#include <TTree.h>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

HistoManager* HistoManager::fManager = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

HistoManager* HistoManager::GetPointer()
{
  if(!fManager) {
    fManager = new HistoManager();
  }
  return fManager;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

HistoManager::HistoManager()
: fElIonPair(0)
, fRootFile(0)
, fDetectorTree(0)
, fTreeName("")
, fMessenger(0)
{
  // normalisation to PAI
  // fFactorALICE = 325;

  // normalisation to Opt0
  //fFactorALICE = 275;

  fElIonPair = G4LossTableManager::Instance()->ElectronIonPair();
  fMessenger = new HistoMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

HistoManager::~HistoManager()
{
  if (fRootFile) delete fRootFile;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void HistoManager::BeginOfRun()
{
  InitializeROOT();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void HistoManager::EndOfRun()
{
  if (fRootFile) {
	fRootFile->Write();
	fRootFile->Close();
	G4cout << "\n----> Output Tree is saved \n" << G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void HistoManager::BeginOfEvent()
{
  fSecondariesEtot = 0.0;
  fTotalEdep = 0.0;
  fIonizEdep = 0.0;
  fNeCounted = 0;
  fEkinLost = 0.;
  fEnergyOfSecond.clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void HistoManager::EndOfEvent()
{
  fSecondariesEtot = std::accumulate(fEnergyOfSecond.begin(), fEnergyOfSecond.end(), 0.);
  
  if (fDetectorTree) {
    G4double enUnits = keV;
	fSecondariesEtot /= enUnits;
	fTotalEdep /= enUnits;
	fIonizEdep /= enUnits;
	fEkinLost /= enUnits;
	fDetectorTree->Fill();
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::AddTotalEdep(G4double edep)
{
  fTotalEdep += edep;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::AddIonizationEdep(G4double edep)
{
  fIonizEdep += edep;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::AddEkinLost(G4double ekin)
{
	fEkinLost += ekin;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void HistoManager::InitializeROOT()
{
	G4String filename = "G4.root";
	fRootFile = new TFile(filename, "RECREATE");

	if(!fRootFile) {
		G4cout << "OutputManager::Initialize: Problem creating the ROOT TFile" << G4endl;
		return;
	}

	if (fTreeName.empty()) {
		G4double en = ((PrimaryGeneratorAction*)G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction())->GetParticleEnergy();
		std::stringstream s;
		s <<  std::fixed << std::setprecision(2) << "E=" << en/MeV << "MeV";
		fTreeName = s.str();
	}

	fDetectorTree = new TTree(fTreeName.c_str(), "GasDetector");
	fDetectorTree->Branch("SecTotalEn", &fSecondariesEtot);
	fDetectorTree->Branch("TotalEDep", &fTotalEdep);
//	fDetectorTree->Branch("IonizEDep", &fIonizEdep);
//	fDetectorTree->Branch("NSec_Estimated", &fNeEstimated);
	fDetectorTree->Branch("fEkinLost", &fEkinLost);
	fDetectorTree->Branch("NePerTrack_Counted", &fNeCounted);
	fDetectorTree->Branch("SecondaryElecronEnergy", &fEnergyOfSecond);
}

void HistoManager::AddSecondaryElectron(G4double energy)
{
	fNeCounted++;
	fEnergyOfSecond.push_back(energy);
}

