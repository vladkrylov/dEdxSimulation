#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "EventAction.hh"
#include "DetectorConstruction.hh"

#include "G4UserSteppingAction.hh"
#include "globals.hh"

class G4LogicalVolume;
class HistoManager;

class SteppingAction : public G4UserSteppingAction {
	public:
		SteppingAction(DetectorConstruction*);
		virtual ~SteppingAction();

		// method from the base class
		virtual void UserSteppingAction(const G4Step*);

	private:
		EventAction*  fEventAction;
		DetectorConstruction* fDetector;
		HistoManager* fHisto;
};

#endif
