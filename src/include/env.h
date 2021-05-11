/* This class sets up the environment for multiple NNs to interact in the same space 
 * One needs to specify the number of bodies required in the environment while creating an env object*/
// TODO: try making different nets with random number of neurons and see the nets interact

#include "body.h"
#include "consts.h"
#include <vector>
#include <cassert>
#include "position.h"
#include <cstdlib> /* srand, rand */
#include <time.h> /* time */
#include <iostream>

class Env{
	private:
		std::vector<unsigned> m_walls; // needs a better way to define the walls rather than with just unsigned 
		unsigned clockTime;
		unsigned expirationTime;

		void setWalls(const Position &TL, const Position &BR); // this function sets up the walls in the env
		void getWalls_stub();
		void updateClockTime() { ++clockTime; }
	public:
		unsigned numPellets;
		std::vector<Body> bodies;
		std::vector<Position> pelletPos;
		unsigned numAliveBodies;

		static double initVel; // initial velocity of the bodies
		static double initRad; // initial radius of the bodies
		static double pelletWt; // for now, let it be constant. Later we can see how the variation in pelletWt affects the system

		std::vector<unsigned> topology;
		Position TL;
		Position BR;
		unsigned numBodies;

		Env(const std::vector<unsigned> &topology, const unsigned numBodies, const Position &TL, const Position &BR);
		/* TL = top left position and BR = botom right position 
		 * makes it easy to setup the env bound rectangle */
		~Env();
		bool checkGenExpiration();
		void getBodyInputs(const Position& TL, const Position &BR);
		void checkPelletConsumption();
		void changeBodyPosition(); 
		void resetEnv();
		void killOffBoundBodies();
};
