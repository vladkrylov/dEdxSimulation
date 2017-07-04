/*
 * LHit.h
 *
 *  Created on: Feb 12, 2017
 *      Author: vlad
 */

#ifndef LHIT_H_
#define LHIT_H_

class LHit {
public:
	LHit();
	virtual ~LHit();

	double X;
	double Y;
	double Z;
	double t;

	double Ch;  // can be a charge of hit or the energy of electron
	int nE;
};

#endif /* LHIT_H_ */
