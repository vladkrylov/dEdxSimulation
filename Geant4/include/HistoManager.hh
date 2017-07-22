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
/// \file electromagnetic/TestEm8/include/HistoManager.hh
/// \brief Definition of the HistoManager class
//
// $Id: HistoManager.hh 79241 2014-02-20 16:03:35Z gcosmo $
//
//---------------------------------------------------------------------------
//
// ClassName:   HistoManager
//
// Description: Singleton class to make analysis and build histograms.
//              User cannot access to the constructor.
//              The pointer of the only existing object can be got via
//              HistoManager::GetPointer() static method.
//              The first invokation of this static method makes
//              the singleton object.
//
// Author:      V.Ivanchenko 01.09.2010
//
//----------------------------------------------------------------------------
//

#ifndef HistoManager_h
#define HistoManager_h 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include <vector>
#include <string>

#include "globals.hh"
#include "G4DataVector.hh"
#include "G4StatDouble.hh"

#include <TTree.h>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class G4Step;
class G4ElectronIonPair;
class HistoMessenger;

class TFile;
class TH1D;

class HistoManager
{

public:
  // With description

  static HistoManager* GetPointer();

private:

  HistoManager();

public: // Without description

  ~HistoManager();

  void BeginOfRun();
  void EndOfRun();

  void BeginOfEvent();
  void EndOfEvent();

  void InitializeROOT();
  void AddEnergy(G4double edep, const G4Step*);
  void AddPrimaryElectron(G4double energy);

  inline void SetTreeName(const std::string& name);
  inline void SetRootFileName(const std::string& name);

private:

  // MEMBERS
  static HistoManager* fManager;
  HistoMessenger* fMessenger;

  G4ElectronIonPair* fElIonPair;

  // ROOT
  G4double fTotEdep;
  G4double fNeEstimated;
  G4double fNeCounted;
  std::vector<G4double> fEnergyOfPrim;
  TFile*   fRootFile;
  TTree*   fDetectorTree;
  std::string fTreeName;
  std::string fRootFileName;
};

inline void HistoManager::SetTreeName(const std::string& name)
{
  fTreeName = name;
}

inline void HistoManager::SetRootFileName(const std::string& name)
{
  fRootFileName = name;
}

#endif
