/* This class sets up the environment for multiple NNs to interact in the same space 
 * One needs to specify the number of bodies required in the environment while creating an env object*/
// TODO: try making different nets with random number of neurons and see the nets interact

#include "body.h"
#include "consts.h"
#include <vector>
#include <cassert>
#include "position.h"
#include <stdlib.h> /* srand, rand */
#include <time.h> /* time */

class Env{
	private:
		std::vector<unsigned> m_walls; // needs a better way to define the walls rather than with just unsigned 
		std::vector<Body> m_bodies;
		std::vector<Position> m_pelletPos;

		static const double m_initVel; // initial velocity of the bodies
		static const double m_initRad; // initial radius of the bodies
		static const double m_pelletWt; // for now, let it be constant. Later we can see how the variation in pelletWt affects the system
		unsigned m_numPellets;


		void setWalls(); // this function sets up the walls in the env
		void getWalls_stub();

	public:
		Env(const std::vector<unsigned> &topology, const unsigned numBodies, const Position &TL, const Position &BR);
		/* TL = top left position and BR = botom right position 
		 * makes it easy to setup the env rectangle */
		~Env();
};
