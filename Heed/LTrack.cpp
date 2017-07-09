/*
 * LTrack.cpp
 *
 *  Created on: Feb 12, 2017
 *      Author: vlad
 */

#include "LTrack.h"

LTrack::LTrack()
{
	Init();
}

LTrack::~LTrack()
{
	for(std::vector<LHit*>::iterator ihit = hits->begin(); ihit != hits->end(); ++ihit)
		delete (*ihit);
	delete hits;
}

void LTrack::Init()
{
	hits = new std::vector<LHit*>();
	hits->clear();
}

void LTrack::AddHit(LHit* h)
{
	hits->push_back(h);
}
