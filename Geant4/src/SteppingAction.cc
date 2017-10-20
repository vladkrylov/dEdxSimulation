#include "SteppingAction.hh"

#include "EventAction.hh"
#include "DetectorConstruction.hh"
#include "HistoManager.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "G4VProcess.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"

SteppingAction::SteppingAction(DetectorConstruction* detector) :
G4UserSteppingAction(),
fDetector(detector),
fHisto(HistoManager::GetPointer())
{}

SteppingAction::~SteppingAction() {}

void SteppingAction::UserSteppingAction(const G4Step* step)
{
//	G4VPhysicalVolume*  preVolume = step-> GetPreStepPoint()->GetTouchableHandle()->GetVolume();
//	G4VPhysicalVolume* postVolume = step->GetPostStepPoint()->GetTouchableHandle()->GetVolume();
//	HistoManager* m = HistoManager::GetPointer();
//
//	G4VPhysicalVolume* detectorVolume = fDetector->GetDetectorVolume();
//
//	G4Track* track = step->GetTrack();
//	const G4ParticleDefinition* particle = track->GetParticleDefinition();
//
//	// gas conversion
//	if (preVolume == detectorVolume) {
//		if (track->GetCurrentStepNumber() == 1) { // creation
//			if (track->GetParentID() == 1) { // only secondaries
//				if (particle->GetParticleType() == "lepton"  // only electrons
//					&& track->GetCreatorProcess()->GetProcessName() == "eIoni")
//				{
//					G4cout << "Here" << G4endl;
//					m->AddTotalEdep(step->GetTotalEnergyDeposit()/keV);
//					track->GetTotalEnergy();
//					track->SetTrackStatus(fStopAndKill); // kill track
//				}
//			}
//		}
//	}

	// according to Dorothea Pfeiffer presentation
	G4VPhysicalVolume* volumePresent = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
	G4VPhysicalVolume* volumeNext = step->GetPostStepPoint()->GetTouchableHandle()->GetVolume();
	G4VPhysicalVolume* volumeDet = fDetector->GetDetectorVolume();

	fHisto->AddTotalEdep(step->GetTotalEnergyDeposit());

	G4Track* track = step->GetTrack();
	// Number of secondaries and their kinetic energy
	if (track->GetDefinition()->GetParticleName() == "e-"
		&& track->GetParentID() == 1  // only secondaries
		&& volumePresent == volumeDet
		)
	{
		if (track->GetCurrentStepNumber() == 1) {  // creation
			fHisto->AddSecondaryElectron(step->GetPreStepPoint()->GetKineticEnergy());
		}
	}

	if (track->GetParentID() == 1
		&& step->GetPostStepPoint()->GetStepStatus() == fGeomBoundary
		&& volumePresent != volumeNext
		&& volumeNext != volumeDet
		&& track->GetDefinition()->GetParticleName() == "e-")
	{
		fHisto->AddEkinLost(step->GetPostStepPoint()->GetKineticEnergy());
//		track->SetTrackStatus(fStopAndKill);
	}

}
