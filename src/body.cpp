#include "include/body.h"

double Body::radiusIncreaseRate = 1.0;
double Body::velocityDecreaseRate = 1.0;

Body::Body(const float vel, const float rad, const std::vector<unsigned> &topology,\
	       	const Position &TL, const Position &BR) : net(topology){
	setVelocity(vel);
	setRadius(rad);
	setSpawnPosition(TL, BR);
	setColor();
}

Body::~Body() {}

void Body::updateBody(const float pelletWt){
	m_radius += radiusIncreaseRate * pelletWt;
	m_velocity -=  velocityDecreaseRate * pelletWt;
}

void Body::setColor(){
	for( unsigned colorIdx = 0; colorIdx < 3; ++colorIdx)
		m_color[colorIdx] = rand()%256; // from 0 to 255; 
		/*don't worry if color matches your bg; we'll have label to differentiate the sneaky one.*/
}

void Body::setSpawnPosition(const Position &TL, const Position &BR){
	m_pos = { rand()%(BR.x - TL.x) + TL.x + m_radius,\
	       	 rand()%(BR.y - TL.y) + TL.y + m_radius }; 
	std::cout << rand() <<  " " << rand() << std::endl;
}

void Body::getNearest3BodyLoc(const std::vector<Body> &bodies, const unsigned currBodyIdx){
	/* set all dist to 0 initially */
	for(unsigned i = 0; i < 3; ++i){
		nearBodyLoc[i].dist = INT_MAX;// calcDist(bodies[i].getPosition().x - m_pos.x, bodies[i].getPosition().y - m_pos.y);
		nearBodyLoc[i].x = -1;
		nearBodyLoc[i].y = -1;
	}

	/* put all the distances: */
	assert(bodies.size() > 3);
	for (unsigned bodyIdx = 0; bodyIdx < bodies.size(); ++bodyIdx){
		unsigned dist = calcDist( bodies[bodyIdx].getPosition().x - m_pos.x, bodies[bodyIdx].getPosition().y - m_pos.y);
		if (dist < nearBodyLoc[0].dist && currBodyIdx != bodyIdx){
			nearBodyLoc[2] = nearBodyLoc[1];
			nearBodyLoc[1] = nearBodyLoc[0];
			nearBodyLoc[0].dist = dist;
			nearBodyLoc[0].x = bodies[bodyIdx].m_pos.x;
			nearBodyLoc[0].y = bodies[bodyIdx].m_pos.y;
			nearBodyLoc[0].idx = bodyIdx;
		}
	}
}

unsigned Body::calcDist(const unsigned m, const unsigned n){
	return m*m + n*n;	
}

void Body::getNearest3PelletsLoc(const std::vector<Position> &pelletPos, const unsigned numPellets){
	/* set all dist to 0 initially */
	for(unsigned i = 0; i < 3; ++i){
		nearPelletLoc[i].dist = INT_MAX;// calcDist(bodies[i].getPosition().x - m_pos.x, bodies[i].getPosition().y - m_pos.y);
		nearPelletLoc[i].x = -1;
		nearPelletLoc[i].y = -1;
		nearPelletLoc[i].idx = i;
	}

	/* put all the distances: */
	assert(numPellets > 3);
	for (unsigned pelletIdx = 0; pelletIdx < numPellets; ++pelletIdx){
		unsigned dist = calcDist( pelletPos[pelletIdx].x - m_pos.x, pelletPos[pelletIdx].y - m_pos.y);
		if (dist < nearPelletLoc[0].dist && currBodyIdx != pelletIdx){
			nearPelletLoc[2] = nearPelletLoc[1];
			nearPelletLoc[1] = nearPelletLoc[0];
			nearPelletLoc[0].dist = dist;
			nearPelletLoc[0].x = pelletPos[pelletIdx].x;
			nearPelletLoc[0].y = pelletPos[pelletIdx].y;
			nearPelletLoc[0].idx = pelletIdx;
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

	for(unsigned i = 0; i < nearWallLoc.size(); ++i){
		assert(nearWallLoc[i] > 0);
	}	
}

