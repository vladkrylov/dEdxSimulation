#include "PrimaryGeneratorAction.hpp"
#include "DetectorConstruction.hpp"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "Randomize.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction()
, fParticleGun(0)
, fEnvelopeBox()
{
	G4int n_particle = 1;
	fParticleGun = new G4ParticleGun(n_particle);

	// default particle kinematic
	G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
	G4ParticleDefinition* particle = particleTable->FindParticle("e-");
	fParticleGun->SetParticleDefinition(particle);
	fParticleGun->SetParticleMomentumDirection(G4ThreeVector(1., 0., 0.));
	fParticleGun->SetParticleEnergy(1.69*MeV);

	fDetector = (DetectorConstruction*)G4RunManager::GetRunManager()->GetUserDetectorConstruction();
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
	delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
	fParticleGun->SetParticlePosition(fDetector->GetSourcePosition());
	fParticleGun->GeneratePrimaryVertex(anEvent);
}

