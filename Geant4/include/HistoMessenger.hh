#ifndef HISTOMESSENGER_HH_
#define HISTOMESSENGER_HH_

#include "globals.hh"
#include "G4UImessenger.hh"

class HistoManager;
class G4UIdirectory;
class G4UIcmdWithAString;

class HistoMessenger: public G4UImessenger
{
public:
	HistoMessenger(HistoManager* );
	virtual ~HistoMessenger();

	virtual void SetNewValue(G4UIcommand*, G4String);

private:
	HistoManager*      fHistoManager;

	G4UIdirectory*             fHistoDir;

	G4UIcmdWithAString*        fRootFileNameCmd;
	G4UIcmdWithAString*        fRootTreeNameCmd;
};

#endif /* HISTOMESSENGER_HH_ */
