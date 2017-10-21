/*
 * LRun.h
 *
 *  Created on: Feb 12, 2017
 *      Author: vlad
 */

#ifndef LRUN_H_
#define LRUN_H_

#include <iostream>
#include <vector>
#include <string>

#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>

#include <LHit.h>
#include <LTrack.h>

class LRun {
public:
	LRun(double myEnergy, int nev, const std::string& particle = "e-", double gap = 0.1 /*cm*/);
	virtual ~LRun();

	void SetOutFile(TFile& f);
	void Generate();
	void Analyze();

	double dEPer1mmFitted;
	double dETruncPer1mmFitted;
	double dETruncPer1mmFittedError;

private:
	bool fGenerated;
	TFile* savefile;
	TDirectory* savedir;
	TTree* detectorTree;
	int printStep;
	int nEvents;
	double energy;
	std::string primParticle;

	std::vector<double> fPosX, fPosY, fPosZ;
	std::vector<int> fNePerCluster;
	std::vector<double> fdEPerCluster;
	double fdEPerTrack;
	int fNePerTrack, fClPerTrack;
	std::vector<double> fdEPerGap;
	std::vector<int> fNePerGap, fClPerGap;

	// Geometry
	double chamberLength;
	double dl;  // length of the track intervals for dE/dx calculation

	std::vector<LTrack*>* tracks;

	std::string GenerateDirName();
	double Mean(const std::vector<double>& vec);
};

#endif /* LRUN_H_ */
