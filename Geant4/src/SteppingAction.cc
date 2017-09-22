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
fDetector(detector) {}

SteppingAction::~SteppingAction() {}

void SteppingAction::UserSteppingAction(const G4Step* step) {
//	G4VPhysicalVolume*  preVolume = step-> GetPreStepPoint()->GetTouchableHandle()->GetVolume();
//	G4VPhysicalVolume* postVolume = step->GetPostStepPoint()->GetTouchableHandle()->GetVolume();
//
//	G4VPhysicalVolume* detectorVolume = fDetector->GetDetectorVolume();
//
//	G4Track* track = step->GetTrack();
//	const G4ParticleDefinition* particle = track->GetParticleDefinition();
//	Run* run = static_cast<Run*>(G4RunManager::GetRunManager()->GetNonConstCurrentRun());

//	// gas conversion
//	if (preVolume == detectorVolume) {
//		if (track->GetCurrentStepNumber() == 1) { // creation
//			if (track->GetParentID() == 1) { // only secondaries
//				if (particle->GetParticleType() == "lepton"  // only electrons
//					&& track->GetCreatorProcess()->GetProcessName() == "eIoni")
//				{
//					HistoManager* m = HistoManager::GetPointer();
//					m->AddPrimaryElectron(step->GetTotalEnergyDeposit()/eV);
//					track->GetTotalEnergy();
//					track->SetTrackStatus(fStopAndKill); // kill track
//				}
//			}
//		}
//	}
}
