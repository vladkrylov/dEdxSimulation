#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"

#include "HistoManager.hh"
#include "HistoMessenger.hh"

HistoMessenger::HistoMessenger(HistoManager* hMan)
: fHistoManager(hMan)
{
	fHistoDir = new G4UIdirectory("/testem/histo/");
	fHistoDir->SetGuidance("histograms control");

	fRootFileNameCmd = new G4UIcmdWithAString("/testem/histo/setRootFileName",this);
	fRootFileNameCmd->SetGuidance("Better to be an absolute path");
	fRootFileNameCmd->SetDefaultValue("G4.root");  // use relative path here

	fRootTreeNameCmd = new G4UIcmdWithAString("/testem/histo/setRootTreeName",this);
	fRootTreeNameCmd->SetGuidance("TTree name");
	fRootTreeNameCmd->SetDefaultValue("T");
}

HistoMessenger::~HistoMessenger()
{
}

void HistoMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
	if( command == fRootFileNameCmd ) fHistoManager->SetRootFileName(newValue);
	if( command == fRootTreeNameCmd ) fHistoManager->SetTreeName(newValue);
}
