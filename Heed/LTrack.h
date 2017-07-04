/*
 * LTrack.h
 *
 *  Created on: Feb 12, 2017
 *      Author: vlad
 */

#ifndef LTRACK_H_
#define LTRACK_H_

#include <iostream>
#include <vector>

#include <LHit.h>

class LTrack {
public:
	LTrack();
	virtual ~LTrack();

	void Init();
	void AddHit(LHit* h);

	std::vector<LHit*>* hits;
};

#endif /* LTRACK_H_ */
