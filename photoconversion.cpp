#include "DetectorConstruction.hpp"
#include "ActionInitialization.hpp"
#include "OutputManager.hpp"
#include "PhysicsList.hpp"

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"
#include "QBBC.hh"
#include "FTFP_BERT.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "Randomize.hh"

int main(int argc, char** argv) {
	// Choose the Random engine
	G4Random::setTheEngine(new CLHEP::RanecuEngine);
	
	// Construct the default run manager
//	#ifdef G4MULTITHREADED
//		G4MTRunManager* runManager = new G4MTRunManager;
//	#else
		G4RunManager* runManager = new G4RunManager;
//	#endif

	// Set mandatory initialization classes
//	runManager->SetUserInitialization(new PhysicsList);
	runManager->SetUserInitialization(new FTFP_BERT);
	DetectorConstruction* detectorConstruction = new DetectorConstruction();
	runManager->SetUserInitialization(detectorConstruction);

	// User action initialization
	runManager->SetUserInitialization(new ActionInitialization(detectorConstruction));
	
	// Initialize visualization
	G4VisManager* visManager = new G4VisExecutive;
	// G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
	// G4VisManager* visManager = new G4VisExecutive("Quiet");
	visManager->Initialize();

	// Process macro or start UI session
	G4UImanager* UImanager = G4UImanager::GetUIpointer();
	UImanager->ApplyCommand("/control/execute ../vis.mac");
	//[[[end]]]
	
	delete visManager;
	delete runManager;
}

