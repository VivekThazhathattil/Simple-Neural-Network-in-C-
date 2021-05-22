#include "include/body.h"

double Body::radiusIncreaseRate = 1.0;
double Body::velocityDecreaseRate = 1.0;

Body::Body(const float vel, const float rad, const std::vector<unsigned> &topology,\
	       	const Position &TL, const Position &BR) : net(topology){
	setVelocityMag(vel);
	setRandomVelDir();
	setRadius(rad);
	setSpawnPosition(TL, BR);
	setColor();
	aliveStatus = true;
}

Body::~Body() {}

void Body::updateBody(const float pelletWt){
	m_radius += radiusIncreaseRate * pelletWt;
	m_velocityMag -=  velocityDecreaseRate * pelletWt;
}

void Body::setColor(){
	for( unsigned colorIdx = 0; colorIdx < 3; ++colorIdx)
		m_color[colorIdx] = rand()%256; // from 0 to 255; 
		/*don't worry if color matches your bg; we'll have label to differentiate the sneaky one.*/
}

void Body::setSpawnPosition(const Position &TL, const Position &BR){
	m_pos = { rand()%int(BR.x - TL.x - 10) + TL.x + 5 ,\
	       	 rand()%int(BR.y - TL.y - 10) + TL.y + 5}; 
//	std::cout << rand() <<  " " << rand() << std::endl;
}
void Body::getNearest3BodyLoc(const std::vector<Body> &bodies, const unsigned currBodyIdx){ /* set all dist to 0 initially */
	for(unsigned i = 0; i < 3; ++i){
		nearBodyLoc[i].dist = INT_MAX;// calcDist(bodies[i].getPosition().x - m_pos.x, bodies[i].getPosition().y - m_pos.y);
		nearBodyLoc[i].x = -1;
		nearBodyLoc[i].y = -1;
		nearBodyLoc[i].idx = 0; // TODO:was -1 before, but segfault encounterd. Will figure out later.
	}

	/* put all the distances: */
	assert(bodies.size() > 3);
	for (unsigned bodyIdx = 0; bodyIdx < bodies.size(); ++bodyIdx){
		if(bodies[bodyIdx].aliveStatus){
			unsigned dist = calcDist( bodies[bodyIdx].getPosition().x - m_pos.x, bodies[bodyIdx].getPosition().y - m_pos.y);
	
			for( unsigned j = 0; j < 3; ++j){
				if ( dist <= nearBodyLoc[j].dist && bodyIdx != currBodyIdx){
					for( unsigned k = 2; k > j; --k)
						nearBodyLoc[k] = nearBodyLoc[k-1];
					nearBodyLoc[j].dist = dist;
					nearBodyLoc[j].x = bodies[bodyIdx].m_pos.x;
					nearBodyLoc[j].y = bodies[bodyIdx].m_pos.y;
					nearBodyLoc[j].idx = bodyIdx;
					break;
				}
			}	
		}
	}
}

unsigned Body::calcDist(const unsigned m, const unsigned n){
	return int(sqrt(m*m + n*n));	
}

void Body::getNearest3PelletsLoc(const std::vector<Position> &pelletPos, const unsigned numPellets){
	for(unsigned i = 0; i < 3; ++i){
		nearPelletLoc[i].dist = INT_MAX;// calcDist(bodies[i].getPosition().x - m_pos.x, bodies[i].getPosition().y - m_pos.y);
		nearPelletLoc[i].x = -1;
		nearPelletLoc[i].y = -1;
		nearPelletLoc[i].idx = -1;
	}

	/* put all the distances: */
	assert(numPellets > 3);
	for (unsigned pelletIdx = 0; pelletIdx < numPellets; ++pelletIdx){
		unsigned dist = calcDist( pelletPos[pelletIdx].x - m_pos.x, pelletPos[pelletIdx].y - m_pos.y);
		for( unsigned j = 0; j < 3; ++j){
			if ( dist <= nearPelletLoc[j].dist){
				for( unsigned k = 2; k > j; --k)
					nearPelletLoc[k] = nearPelletLoc[k-1];
				nearPelletLoc[j].dist = dist;
				nearPelletLoc[j].x = pelletPos[pelletIdx].x;
				nearPelletLoc[j].y = pelletPos[pelletIdx].y;
				break;
			}
		}	
	}
}

void Body::getNearest3BodyRadii(const std::vector<Body> &bodies){
	for(unsigned i = 0; i < 3; ++i){
		nearBodyRadii[i] = bodies[nearBodyLoc[i].idx].getRadius();
	}
}

void Body::getShortestDistanceToWalls(const Position &TL, const Position &BR){
	/* for now, just return the distance to each of the four walls */
	nearWallLoc.clear();				
	nearWallLoc.push_back(m_pos.x - TL.x);
	nearWallLoc.push_back(m_pos.y - TL.y);
	nearWallLoc.push_back(BR.x - m_pos.x);
	nearWallLoc.push_back(BR.y - m_pos.y);

//	for(unsigned i = 0; i < nearWallLoc.size(); ++i){
//		//std::cout << "wall loc : " << nearWallLoc[i] << std::endl;
//		assert(nearWallLoc[i] > 0);
//	}	
}
void Body::setRandomVelDir(){
	m_velocityDir.x =  static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	m_velocityDir.y = static_cast <float> (rand()) / static_cast <float> (RAND_MAX); 

	/* normalize both components */
	float tempX = m_velocityDir.x;
	m_velocityDir.x = m_velocityDir.x / sqrt(m_velocityDir.x * m_velocityDir.x + m_velocityDir.y * m_velocityDir.y);
	m_velocityDir.y = m_velocityDir.y / sqrt(tempX * tempX + m_velocityDir.y * m_velocityDir.y);
	switch(rand()%4){
		case 0:
			m_velocityDir.x = -m_velocityDir.x;
			break;
		case 1:
			m_velocityDir.y = -m_velocityDir.y;
			break;
		case 2: 
			m_velocityDir.x = -m_velocityDir.x;
			m_velocityDir.y = -m_velocityDir.y;
			break;
		case 3:
		default: 
			break;
	}
}

void Body::feedInput(const std::vector<double> inputVals){
	net.feedForward(inputVals);
}

double Body::getOutput(){
	std::vector<double> resultVals;
	net.getResults(resultVals);
	return resultVals.back();
	// lets not have a targetVal and backpropagation here
}
