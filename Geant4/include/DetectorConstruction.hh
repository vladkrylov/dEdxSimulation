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
/// \file electromagnetic/TestEm8/include/DetectorConstruction.hh
/// \brief Definition of the DetectorConstruction class
//
// $Id: DetectorConstruction.hh 66241 2012-12-13 18:34:42Z gunter $
//
/////////////////////////////////////////////////////////////////////////
//
// TestEm8: Gaseous detector
//
// Created: 31.08.2010 V.Ivanchenko on base of V.Grichine code
//
// Modified:
//
////////////////////////////////////////////////////////////////////////
// 

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4Material;
class G4Region;
class DetectorMessenger;
class TargetSD;
class G4VPhysicalVolume;
class G4LogicalVolume;
class G4ProductionCuts;
class PrimaryGeneratorAction;

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
  
  DetectorConstruction(PrimaryGeneratorAction*);
  virtual ~DetectorConstruction();
     
  virtual G4VPhysicalVolume* Construct();

  void SetGasMaterial (const G4String&);     
  void SetContainerMaterial (const G4String&);     
  void SetWorldMaterial(const G4String&);

  void SetGasThickness(G4double);     
  void SetGasRadius(G4double);          
  void SetContainerThickness(G4double);     

  void SetPairEnergy(G4double);

  G4VPhysicalVolume* GetDetectorVolume() {return fPhysDetector;};
     
private:

  void DefineMaterials();
  G4Material* ConstructGasMixture2(G4Material* gas1, G4Material* gas2, G4int fraq1, G4int fraq2);
  G4Material* He;
  G4Material* Ar;
  G4Material* Kr;
  G4Material* Xe;
  G4Material* CarbonDioxide;
  G4Material* Methane;
  G4Material* Propane;
  G4Material* empty;
  G4Material* C3H8;
  G4Material* IsoBut;

  G4Material*        fGasMat;
  G4double           fGasThickness;
  G4double           fGasRadius;

  G4Material*        fWindowMat;
  G4double           fWindowThick;
 
  G4Material*        fWorldMaterial;
            
  G4VPhysicalVolume* fPhysWorld;
  G4LogicalVolume*   fLogicWorld;
  G4LogicalVolume*   fLogicWind;
  G4LogicalVolume*   fLogicDet;
  G4VPhysicalVolume* fPhysDetector;

  DetectorMessenger* fDetectorMessenger;  
  TargetSD*          fTargetSD;
  G4ProductionCuts*  fGasDetectorCuts;
  G4Region*          fRegGasDet;

  PrimaryGeneratorAction* fPrimaryGenerator;
      
};

#endif

