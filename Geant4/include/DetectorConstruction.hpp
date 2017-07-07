#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class G4Material;
class G4Box;

// Detector construction class to define materials and geometry.

class DetectorConstruction : public G4VUserDetectorConstruction {
	public:
		virtual G4VPhysicalVolume* Construct();

		G4VPhysicalVolume* GetShieldVolume() { return fPhysShield; }
		G4VPhysicalVolume* GetDetectorVolume() { return fPhysDetector; }

		void SetPairEnergy(G4double);
		G4ThreeVector GetSourcePosition();

	private:
		G4VPhysicalVolume* 	fPhysWorld;
		G4VPhysicalVolume* 	fPhysShieldCover;
		G4VPhysicalVolume* 	fPhysShield;
		G4VPhysicalVolume* 	fPhysCathode;
		G4VPhysicalVolume* 	fPhysDetector;
		G4LogicalVolume*   	fLogicWorld;
		G4LogicalVolume*   	fLogicShieldCover;
		G4LogicalVolume*   	fLogicShield;
		G4LogicalVolume*   	fLogicCathode;
		G4LogicalVolume*   	fLogicDetector;
		G4Box*				solid_detector;
		G4Material*         fGasMat;
};

#endif

