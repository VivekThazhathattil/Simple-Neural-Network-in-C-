#include "include/body.h"

Body::radiusIncreaseRate = 1;
Body::velocityDecreaseRate = 1;

Body::Body(const float vel, const float rad, const std::vector<unsigned> &topology) : net(topology){
	setVelocity(vel);
	setRadius(rad);
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
		m_color[colorIdx] = std::rand()%256; // from 0 to 255; 
		/*don't worry if color matches your bg; we'll have label to differentiate the sneaky one.*/
}
