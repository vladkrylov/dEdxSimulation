#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

#include "OutputManager.hpp"

/// Event action class

class EventAction : public G4UserEventAction
{
public:
	EventAction(OutputManager*);
	virtual ~EventAction();

	virtual void BeginOfEventAction(const G4Event* event);
	virtual void EndOfEventAction(const G4Event* event);

private:
	OutputManager* fOutputManager;
};

#endif

		
