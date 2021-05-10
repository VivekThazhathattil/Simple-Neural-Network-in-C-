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
	std::srand(time(0));
	for( unsigned colorIdx = 0; colorIdx < 3; ++colorIdx)
		m_color[colorIdx] = rand()%256; // from 0 to 255; 
		/*don't worry if color matches your bg; we'll have label to differentiate the sneaky one.*/
}

void Body::setSpawnPosition(const Position &TL, const Position &BR){ // apparently this is the same for all the bodies!
	srand(time(NULL));
	m_pos = { rand()%(BR.x - TL.x) + TL.x + m_radius,\
	       	 rand()%(BR.y - TL.y) + TL.y + m_radius }; 
	std::cout << rand() <<  " " << rand() << std::endl;
}
