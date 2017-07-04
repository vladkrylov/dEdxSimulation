#ifndef RunAction_h
#define RunAction_h 1

#include "Run.hpp"
#include "OutputManager.hpp"

#include "G4UserRunAction.hh"
#include "globals.hh"

class G4Run;
class G4LogicalVolume;

class RunAction : public G4UserRunAction {
	public:
		RunAction(OutputManager*);
		virtual ~RunAction();

		virtual G4Run* GenerateRun();
		virtual void BeginOfRunAction(const G4Run*);
		virtual void   EndOfRunAction(const G4Run*);

	private:
		OutputManager* fOutputManager;

		Run* fRun;
};

#endif

