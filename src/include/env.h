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
#include <cmath>

class Env{
	private:
		std::vector<unsigned> m_walls; // needs a better way to define the walls rather than with just unsigned 
		unsigned clockTime;
		unsigned expirationTime;

		void setWalls(const Position &TL, const Position &BR); // this function sets up the walls in the env
		void getWalls_stub();
		void updateClockTime() { ++clockTime; }
		float getBodyToPelletDist(const float &x0, const float &y0, const float &x1, const float &y1);
	public:
		unsigned numPellets; // TODO: made the mistake of not defining pellets as an object; find a way around or rewrite!
		std::vector<Position> pelletPos;
		std::vector<bool> pelletAliveStatus;

		std::vector<Body> bodies;
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
		void feedInput();
		double getOutputFromNN(const unsigned bodyIdx);
		void getBestNN();
		void setBestNN();
		void mutateNN();
		void outputToDirs();
};
