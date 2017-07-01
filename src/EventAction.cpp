#include "EventAction.hpp"
#include "Run.hpp"

#include "G4Event.hh"
#include "G4RunManager.hh"

EventAction::EventAction(OutputManager* outManager) :
G4UserEventAction()
, fOutputManager(outManager)
{}

EventAction::~EventAction() {}

void EventAction::BeginOfEventAction(const G4Event*)
{
	fOutputManager->StartNewEvent();
}

void EventAction::EndOfEventAction(const G4Event*)
{
	fOutputManager->FillEvent();
}
