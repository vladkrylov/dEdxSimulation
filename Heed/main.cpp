/*
 * main.cpp
 *
 *  Created on: Feb 13, 2017
 *      Author: vlad
 */

#include "LRun.h"

#include <TGraphErrors.h>

using namespace std;

double K2E(double kineticEnergy);
double E2K(double energy);

int main(int argc, char * argv[])
{
	double eStarEnrgs[] = {0.10, 0.125, 0.15, 0.175, 0.20, 0.25, 0.30, 0.35, 0.40, 0.45, 0.50, 0.55, 0.60, 0.70, 0.80, 0.90, 1, 1.25, 1.5, 1.75, 2, 2.5, 3, 3.5, 4, 4.5, 5};
	size_t nEStar = sizeof(eStarEnrgs) / sizeof(double);

	TFile f("Results.root","recreate");
	vector<double> energies;
	for(size_t i=0; i<nEStar; i+=1) {
		energies.push_back(K2E(eStarEnrgs[i]*1e6));
	}
//	energies.push_back(K2E(1.69*1e6));

	vector<double> gaps;
	for(double gap=0.05; gap<1.; gap*=2) {
		gaps.push_back(gap);
	}

	vector<double>* iterVec = &energies;

	vector<double> dE(iterVec->size(), 0);
	vector<double> dEerr(iterVec->size(), 0);

	for(size_t i=0; i<iterVec->size(); ++i) {
		double energy = iterVec->at(i);
		LRun run(energy, 1500, "e-", 0.3);
		run.SetOutFile(f);

		cout << "======== Doing the analysis for E = " << energy*1e-6 << " MeV" << endl;
		run.Generate();
		run.Analyze();

		dE[i] = run.dETruncPer1mmFitted;
		dEerr[i] = run.dETruncPer1mmFittedError;
	}

	for(size_t i=0; i<iterVec->size(); ++i) {
		cout << energies[i] << "    " << dE[i] << endl;
	}

	TGraphErrors* g = new TGraphErrors(iterVec->size(), iterVec->data(), dE.data(), 0, dEerr.data());
	f.cd();
	g->Draw("ALP");
	g->SetMarkerStyle(20);
	g->SetMarkerSize(2);
	g->SetMarkerColor(kBlue);
	g->SetLineColor(kGreen);
	g->SetLineWidth(2);
	g->GetXaxis()->SetTitle("Gap, cm");
//	g->GetXaxis()->SetTitle("Energy, eV");
	g->GetYaxis()->SetTitle("dE/dx, eV/mm");

	g->Write("dE/gap");

	f.Close();
}

double K2E(double kineticEnergy)
{
	return kineticEnergy + 0.511e6;
}

double E2K(double energy)
{
	return energy - 0.511e6;
}

