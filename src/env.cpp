#include "include/env.h"

Env(const std::vector<unsigned> &topology, const unsigned numBodies, const Position &TL, const Position &BR){
	std::srand(time(0));
	/* generate bodies */
	std::assert(numBodies > 0);
	for (unsigned bodyIdx = 0; bodyIdx < numBodies; ++bodyIdx){
		m_bodies.push_back(Body(m_initVel, m_initRad, topology));	
	}

	/* set walls */	
	setWalls();

	/* set pellets position */
	m_numPellets = rand()%(101 - numBodies) + numBodies; /* number of pellets atleast the same as number of bodies */
	std::assert(TL.x >= 0);
	std::assert(TL.y >= 0);
	std::assert(BR.x > 0);
	std::assert(BR.y > 0);
	for(unsigned pelletNum = 0; pelletNum < m_numPellets; ++pelletNum){
		Position pos = {std::rand()%(BR.x - TL.x) + TL.x, std::rand()%(BR.y - TL.y) + TL.y};
		m_pelletPos.push_back(pos);
	}
	
}

Env::~Env() {}

void Env::setWalls(){
	/* for the time being, define the walls only at the edge of the env
	 * Later use getWalls to setup complex wall cases */
	
	unsigned x0 = 0, x1 = WINDOW_SIZE_X,\
		 y0 = 0, y1 = WINDOW_SIZE_Y;

	m_walls.push_back(x0); 
	m_walls.push_back(x1); 
	m_walls.push_back(y0); 
	m_walls.push_back(y1); 
}

void Env::getWalls_stub(){
	/* After setting up everything else, complete this functions to add custom walls to the env */
}
