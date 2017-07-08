#include "DetectorConstruction.hpp"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4ProductionCuts.hh"

G4VPhysicalVolume* DetectorConstruction::Construct()
{
	G4NistManager* nist = G4NistManager::Instance();

	// Drift and quenching gases, add your own here if you want to use it
	// Helium
	G4Material* he = nist->FindOrBuildMaterial("G4_He");

	// Argon
	G4Material* ar = nist->FindOrBuildMaterial("G4_Ar");

	// Xenon
	G4Material* xe = nist->FindOrBuildMaterial("G4_Xe");

	// Neon
	G4Material* ne = nist->FindOrBuildMaterial("G4_Ne");

	// quenching gases
	G4Element* elH = nist->FindOrBuildElement("H");
	G4Element* elO = nist->FindOrBuildElement("O");
	G4Element* elF = nist->FindOrBuildElement("F");
	G4Element* elC = nist->FindOrBuildElement("C");

	// CO2
	G4Material* co2 = new G4Material("CO2", 1.977*kg/m3, 2);
	co2->AddElement(elC, 1);
	co2->AddElement(elO, 2);

	// CH4 - Methane
	G4Material* ch4 = new G4Material("CH4", 0.7174*kg/m3, 2);
	ch4->AddElement(elC, 1);
	ch4->AddElement(elH, 4);

	// C3H8 - Propane
	G4Material* c3h8 = new G4Material("C3H8", 2.005*kg/m3, 2);
	c3h8->AddElement(elC, 3);
	c3h8->AddElement(elH, 8);

	// C4H10 - iso-Butane
	G4Material* c4h10 = new G4Material("isoC4H10", 2.67*kg/m3, 2);
	c4h10->AddElement(elC, 4);
	c4h10->AddElement(elH, 10);

	// CF4 - Tetrafluoromethane
	G4Material* cf4 = new G4Material("CF4", 3.72*kg/m3, 2);
	cf4->AddElement(elC, 1);
	cf4->AddElement(elF, 4);


	// Detector definition
	G4bool checkOverlaps = true;

	// Detector
	G4double sizeX_detector = 1.4*4*cm;
	G4double sizeY_detector = 80.*mm;
	G4double sizeZ_detector = 17.*mm;

	// World
	G4double sizeX_world = sizeX_detector + 10*mm;
	G4double sizeY_world = sizeY_detector + 1*mm;
	G4double sizeZ_world  = sizeZ_detector + 1*mm;

	G4Material* mat_air = nist->FindOrBuildMaterial("G4_AIR");
	G4Material* mat_vacuum = new G4Material("Vacuum", 1.e-5*g/cm3, 1, kStateGas, STP_Temperature, 2.e-2*bar);
	mat_vacuum->AddMaterial(mat_air, 1.);

	G4Box* solid_world = new G4Box("World", .5*sizeX_world, .5*sizeY_world, .5*sizeZ_world);
	fLogicWorld = new G4LogicalVolume(solid_world, mat_vacuum, "World");
	fPhysWorld = new G4PVPlacement(0, G4ThreeVector(), fLogicWorld, "World", 0, false, 0, checkOverlaps);
	
	// General detector values
	G4double pressure = 1.*bar;
	G4double temperature = STP_Temperature + 20*kelvin;

	G4Material* nobleGas = he;
	G4Material* quencherGas = c4h10;
	G4double noblePart = 80*perCent;
	G4double quencherPart = 1 - noblePart;

	G4double Mn = nobleGas->GetMassOfMolecule();
	G4double Mq = quencherGas->GetMassOfMolecule();
	G4double nobleMassFrac = noblePart*Mn / (noblePart*Mn + quencherPart*Mq);
	G4double quencherMassFrac = 1 - nobleMassFrac;
	G4cout << "\n========== m_He = " << nobleMassFrac << "; m_iB = " << quencherMassFrac << " ==========" << G4endl;


	G4ThreeVector pos_detector = G4ThreeVector((sizeX_world-sizeX_detector)/2., 0., 0.);
	G4double composition_density = (nobleGas->GetDensity()*noblePart + quencherGas->GetDensity()*quencherPart);
	G4Material* gas_composition = new G4Material("GasComposition", composition_density, 2, kStateGas, temperature, pressure);
	gas_composition->AddMaterial(nobleGas, nobleMassFrac);
	gas_composition->AddMaterial(quencherGas, quencherMassFrac);
	fGasMat = gas_composition;
	G4cout << "\n========== Gas density = " << fGasMat->GetDensity() / (g/cm3) << " g/cm3 ==========" << G4endl;

	solid_detector = new G4Box("GasDetector", .5*sizeX_detector, .5*sizeY_detector, .5*sizeZ_detector);
	fLogicDetector = new G4LogicalVolume(solid_detector, fGasMat, "GasDetector");
	G4VisAttributes* visatt_detector = new G4VisAttributes(G4Colour(1., 1., 1.));
	visatt_detector->SetForceWireframe(true);
	fLogicDetector->SetVisAttributes(visatt_detector);
	fPhysDetector = new G4PVPlacement(0, pos_detector, fLogicDetector, "GasDetector", fLogicWorld, false, 0, checkOverlaps);

	// defined gas detector region
	G4double cut = sizeX_detector;
	G4ProductionCuts* fGasDetectorCuts   = new G4ProductionCuts();
	fGasDetectorCuts->SetProductionCut(cut,"gamma");
	fGasDetectorCuts->SetProductionCut(cut,"e-");
	fGasDetectorCuts->SetProductionCut(cut,"e+");
	fGasDetectorCuts->SetProductionCut(cut,"proton");

	G4Region* fRegGasDet = new G4Region("GasDetector");
	fRegGasDet->SetProductionCuts(fGasDetectorCuts);
	fRegGasDet->AddRootLogicalVolume(fLogicDetector);

	if(0.0 == fGasMat->GetIonisation()->GetMeanEnergyPerIonPair()) {
		SetPairEnergy(20*eV);
	}

	return fPhysWorld;
}

void DetectorConstruction::SetPairEnergy(G4double val)
{
   if(val > 0.0) fGasMat->GetIonisation()->SetMeanEnergyPerIonPair(val);
}

G4ThreeVector DetectorConstruction::GetSourcePosition()
{
	return (fPhysWorld->GetObjectTranslation() + fPhysDetector->GetObjectTranslation())/2
			- G4ThreeVector(solid_detector->GetXHalfLength(), 0., 0.);
}




