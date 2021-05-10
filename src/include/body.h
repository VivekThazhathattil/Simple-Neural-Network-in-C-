/* this class defines the body i.e the circles. 
 * Each body will have its own set of properties and behavior, hence the need for a separate class for them.
 * The properties include a size (radius), a velocity (both of which are the same initially, a color (just to differentiate the body from others
*/

#include "net.h"
#include "position.h"
#include <cstdlib> // rand, srand
#include <time.h> // time
#include <vector>
#include <iostream>

class Body{
	private:
		float m_radius; // mass and radius used interchangebly
		float m_velocity;
		Position m_pos; // each body has a position
		unsigned m_color[3]; // for storing the rgb values
		unsigned m_bodyIdx; // to identify the body by its number
		Net net; // neural network for the body

		void setColor();
		void setSpawnPosition(const Position &TL, const Position &BR);

	public:
		Body(const float vel, const float rad, const std::vector<unsigned> &topology, const Position &TL, const Position &BR);
		~Body();

		static double radiusIncreaseRate;
		static double velocityDecreaseRate;

		void setVelocity(const float vel) { m_velocity = vel; }
		float getVelocity() const { return m_velocity; }
		void setRadius(const float rad){ m_radius = rad; }
		float getRadius() const { return m_radius; }

		Position getPosition() const { return m_pos; }
		void updateBody(const float pelletWt);
		void setBodyIdx(const float num) { m_bodyIdx = num; }
		unsigned* getColor() { return m_color; }
};
