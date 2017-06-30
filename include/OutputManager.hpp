#ifndef OutputManager_h
#define OutputManager_h 1

#include <vector>

#include "G4Track.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"

#include <TTree.h>

class TFile;
class TTree;
class TH1D;

class OutputManager {
	public:
		OutputManager();
		~OutputManager();
	 
		void Initialize();
		void Save();
		void PrintStatistic();
		TTree* GetShieldTree() { return fShieldTree; }
		TTree* GetDetectorTree() { return fDetectorTree; }

		void FillEvent(TTree*, G4Track*);

	private:
		TFile*   fRootFile;

		TTree*   fDetectorTree;

		std::vector<G4double> fPhiVertex, fPhi;
		std::vector<G4double> fThetaVertex, fTheta;
		std::vector<G4double> fT;
		std::vector<G4double> fEkinVertex, fEkin;
		std::vector<G4double> fEloss;
		std::vector<G4double> fZVertex;
		std::vector<G4double> fTrackLength;
		std::vector<G4double> fPx, fPy, fPz;
		std::vector<G4double> fPosX, fPosY, fPosZ;
};

#endif
