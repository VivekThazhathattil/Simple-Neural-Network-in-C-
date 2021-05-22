#include "include/env.h"

double Env::initVel = 0.5;
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
//	std::cout << clockTime << " " << numAliveBodies << std::endl;
	if(clockTime > expirationTime || numAliveBodies < 2){
		clockTime = 0;
		return true;
	}	
	updateClockTime();
	return false;
}

void Env::getBodyInputs(const Position& TL, const Position &BR){
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
	for(unsigned i = 0; i < bodies.size(); ++i){
		float dist = getBodyToPelletDist(bodies[i].getPosition().x, bodies[i].getPosition().y ,\
				bodies[i].nearPelletLoc[0].x , bodies[i].nearPelletLoc[0].y);
		if(dist < bodies[i].getRadius() - pelletWt){
			//complete this TODO
		}
	}
}

float Env::getBodyToPelletDist(const float &x0, const float &y0, const float &x1, const float &y1){
return sqrt( (x0-x1)*(x0-x1) + (y0-y1)*(y0-y1));
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

void Env::killOffBoundBodies(){
	/* change alive body status, remove the body from the boundary */
	for(unsigned i = 0; i < bodies.size(); ++i){
		if((bodies[i].getPosition().x > BR.x ||\
		   bodies[i].getPosition().x < TL.x ||\
		   bodies[i].getPosition().y > BR.y ||\
		   bodies[i].getPosition().y < TL.y) &&\
			bodies[i].aliveStatus){
			bodies[i].aliveStatus = false;
			--numAliveBodies;	
		}

	}	
}

void Env::feedInput(){
	std::vector<double> inputVals;
	for(unsigned i = 0; i < bodies.size(); ++i){
		inputVals.clear();
		float normalizer = sqrt(BR.x*BR.x + BR.y*BR.y);
		/* nearBodies locations */
		/* normalize position and radii with sqrt(BR.x^2 + BR.y^2) */
		for(unsigned j = 0; j < 3; ++j){
			inputVals.push_back(float(bodies[i].nearBodyLoc[j].x)/normalizer);
			inputVals.push_back(float(bodies[i].nearBodyLoc[j].y)/normalizer);
		}
		/* near pellet locations */
		for(unsigned j = 0; j < 3; ++j){
			inputVals.push_back(float(bodies[i].nearPelletLoc[j].x)/normalizer);
			inputVals.push_back(float(bodies[i].nearPelletLoc[j].y)/normalizer);
		}
		/* near walls locations */
		for(unsigned j = 0; j < bodies[i].nearWallLoc.size(); ++j){
			inputVals.push_back(float(bodies[i].nearWallLoc[j])/normalizer);
		}
		/* present body position*/
		inputVals.push_back(float(bodies[i].getPosition().x)/normalizer);
		inputVals.push_back(float(bodies[i].getPosition().y)/normalizer);

		/* present body radius */
		inputVals.push_back(float(bodies[i].getRadius())/normalizer);

		/* near bodies radii */
		for(unsigned j = 0; j < 3; ++j){
			inputVals.push_back(float(bodies[bodies[i].nearBodyLoc[j].idx].getRadius())/normalizer);
		}

		/* feed the inputVals to the corresponding function in body */
		bodies[i].feedInput(inputVals);
	}
}

double Env::getOutputFromNN(const unsigned bodyIdx){
	feedInput();
	return bodies[bodyIdx].getOutput();
}

void Env::getBestNN(){

}

void Env::setBestNN(){
	mutateNN(); // for each body
}

void Env::mutateNN(){
}

void Env::outputToDirs(){
	for(unsigned i = 0; i < bodies.size(); ++i){
		double output = getOutputFromNN(i);
//		std::cout << " output raw = " << output << std::endl;
		if (output > 1) 
			output = 1.0;
		else if (output < -1) 
			output = -1.0;
		/* convert this output to usable format i.e. rotation in rad  : [-1..1] -> [-PI..PI]*/
		output = M_PI * output; // convert to radians
		Velocity vel;
		vel.x = cos(output);
		vel.y = sin(output);
//		std::cout << "output = " << output << " vel.x = " << vel.x << " vel.y = " << vel.y << std::endl;
		bodies[i].setVelocityDir(vel);
	}
}
