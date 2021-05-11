#include "include/env.h"

double Env::initVel = 0.2;
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
		Position pos = {rand()%int(BR.x - TL.x) + TL.x, rand()%int(BR.y - TL.y) + TL.y};
		pelletPos.push_back(pos);
	}

	/* set expiration Time and clock Time */
	expirationTime = EXPIRE_TIME;
	clockTime = 0;	

	/* set the number of alive bodies */
	numAliveBodies = numBodies;

	this->topology = topology;
	this->TL = TL;
	this->BR = BR;
	this->numBodies = numBodies;
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

bool Env::checkGenExpiration(){
	/* gen expires if there is only one circle or if the time expires */
	if(clockTime > expirationTime || numAliveBodies < 2){
		clockTime = 0;
		return true;
	}	
	updateClockTime();
	return false;
}

void Env::checkBodyDeath(const Position& TL, const Position &BR){
	/* check if the body has died: either by collision with the wall or by getting consumed by other bodies */
	for(unsigned i = 0; i < bodies.size(); ++i){
		bodies[i].getNearest3BodyLoc(bodies, i);
		bodies[i].getNearest3PelletsLoc(pelletPos, numPellets);
		bodies[i].getNearest3BodyRadii(bodies);
		bodies[i].getShortestDistanceToWalls(TL, BR);
	}
}

void Env::checkPelletConsumption(){
	/* check if the pellets have been consumed by the bodies; once fully consumed, the pellets never regenerate */
	/* we do not delete the pellets, rather we set the pellet position of consumed pellets to INT_MAX */
}

void Env::changeBodyPosition(){
	/* change each body position wrt to the output from the NN */
	for(unsigned i = 0; i < bodies.size(); ++i){
		Position pos;
		pos.x = bodies[i].getPosition().x + (float)bodies[i].getVelocityMag() * bodies[i].getVelocityDir().x;
		pos.y = bodies[i].getPosition().y + (float)bodies[i].getVelocityMag() * bodies[i].getVelocityDir().y;
		bodies[i].setPosition(pos);
	}
}

void Env::resetEnv(){
	/* this function should refill the pellets, set alive all the bodies, assign all of 'em random positions */
	unsigned numBodies = bodies.size();
	bodies.clear();
	for (unsigned bodyIdx = 0; bodyIdx < numBodies; ++bodyIdx){
		bodies.push_back(Body(initVel, initRad, topology, TL, BR));	
	}


	pelletPos.clear();
	numPellets = rand()%(101 - numBodies) + numBodies; /* number of pellets atleast the same as number of bodies */
	for(unsigned pelletNum = 0; pelletNum < numPellets; ++pelletNum){
		Position pos = {rand()%int(BR.x - TL.x) + TL.x, rand()%int(BR.y - TL.y) + TL.y};
		pelletPos.push_back(pos);
	}

	clockTime = 0;	

	numAliveBodies = numBodies;
}
