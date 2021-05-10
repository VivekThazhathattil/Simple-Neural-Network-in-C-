/* this class defines the body i.e the circles. 
 * Each body will have its own set of properties and behavior, hence the need for a separate class for them.
 * The properties include a size (radius), a velocity (both of which are the same initially, a color (just to differentiate the body from others
*/

#include "net.h"
#include "consts.h"
#include <stdlib.h> // rand, srand
#include <time.h> // time
#include <vector>

class Body{
	private:
		float m_radius; // mass and radius used interchangebly
		float m_velocity;
		static double radiusIncreaseRate;
		static double velocityDecreaseRate;
		unsigned m_color[3]; // for storing the rgb values
		unsigned m_bodyIdx; // to identify the body by its number
		Net net; // neural network for the body

		void setColor();

	public:
		Body(const float vel, const float rad, const std::vector<unsigned> &topology);
		~Body();

		void setVelocity(const float vel) { m_velocity = vel; }
		float getVelocity() const { return m_velocity };
		void setRadius(const float rad){ m_radius = rad; }
		float getRadius() const { return m_radius};

		void updateBody(const float pelletWt);
		void setBodyIdx(const float num) { m_bodyIdx = num; }
};
