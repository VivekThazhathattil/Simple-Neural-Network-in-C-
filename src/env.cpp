#include "include/env.h"

double Env::initVel = 5;
double Env::initRad = 10;
double Env::pelletWt = 2;


Env::Env( const std::vector<unsigned> &topology, const unsigned numBodies, const Position &TL, const Position &BR ){
	std::srand(time(0));
	/* generate bodies */
	assert(numBodies > 0);
	for (unsigned bodyIdx = 0; bodyIdx < numBodies; ++bodyIdx){
		bodies.push_back(Body(initVel, initRad, topology, TL, BR));	
	}

	/* set walls */	
	setWalls(TL, BR);

	/* set pellets position */
	numPellets = rand()%(101 - numBodies) + numBodies; /* number of pellets atleast the same as number of bodies */
	assert(TL.x >= 0);
	assert(TL.y >= 0);
	assert(BR.x > 0);
	assert(BR.y > 0);
	for(unsigned pelletNum = 0; pelletNum < numPellets; ++pelletNum){
		Position pos = {rand()%(BR.x - TL.x) + TL.x, rand()%(BR.y - TL.y) + TL.y};
		pelletPos.push_back(pos);
	}
	
}

Env::~Env() {}

void Env::setWalls(const Position &TL, const Position &BR){
	/* for the time being, define the walls only at the edge of the env
	 * Later use getWalls to setup complex wall cases 
	 * TODO: find a way to include TL and BR below*/
	
	unsigned x0 = 0, x1 = WINDOW_SIZE_X,\
		 y0 = 0, y1 = WINDOW_SIZE_Y;

	m_walls.push_back(x0); 
	m_walls.push_back(x1); 
	m_walls.push_back(y0); 
	m_walls.push_back(y1); 
}

void Env::getWalls_stub(){
}
