#include "include/render.h"

Render::Render(const std::vector<unsigned> &topology, const unsigned numBodies,\
	       	const Position &TL, const Position &BR) : m_window(sf::RenderWindow(sf::VideoMode(WINDOW_SIZE_X, WINDOW_SIZE_Y),\
			       	"NEAT Illustration", sf::Style::Close)), env(topology, numBodies, TL, BR){
	
	/* create circles */
	for (unsigned bodyIdx = 0; bodyIdx < numBodies; ++bodyIdx){
		m_circles.push_back(sf::CircleShape(env.initRad));
		unsigned *colors = env.bodies[ bodyIdx ].getColor();
		m_circles.back().setFillColor(sf::Color(colors[0], colors[1], colors[2]));
		m_circles.back().setOrigin(sf::Vector2f(env.bodies[bodyIdx].getRadius()/2, env.bodies[bodyIdx].getRadius()/2));
		Position pos = env.bodies[bodyIdx].getPosition();
		std::cout << bodyIdx << std::endl;
		m_circles.back().setPosition(sf::Vector2f(pos.x, pos.y));
	}

	/* create pellets */
	for (unsigned pelletIdx = 0; pelletIdx < env.numPellets; ++pelletIdx){
		m_pellets.push_back(sf::CircleShape(env.pelletWt));
		m_pellets.back().setFillColor(sf::Color::White); // for time being, we'll set the color to white
		m_pellets.back().setOrigin(sf::Vector2f(env.pelletWt, env.pelletWt));
		m_pellets.back().setPosition(sf::Vector2f(env.pelletPos[pelletIdx].x, env.pelletPos[pelletIdx].y));
	}
}
Render::~Render() {}

void Render::runSimulation(){
	m_window.setPosition(sf::Vector2i(100,100));
	while(m_window.isOpen()){
		sf::Event e;
		while(m_window.pollEvent(e)){
			switch(e.type){
				case sf::Event::Closed:
					m_window.close();
					break;
			}	
		}
		m_window.clear();
		updateState();
		drawNDisplay();	
	}
}

void Render::updateState(){
	/* change circles' onscreen  position each timeframe
	 * if pellets consumed, remove them out of the screen
	 * if two or more circles' centres coincide, remove the consumed circles.
	 */
}

void Render::drawNDisplay(){
	/* draw circles */
	for( unsigned circleIdx = 0; circleIdx < m_circles.size(); ++circleIdx)
		m_window.draw( m_circles[ circleIdx ] );

	/* draw pellets */
	for( unsigned pelletIdx = 0; pelletIdx < m_pellets.size(); ++pelletIdx)
		m_window.draw( m_pellets[ pelletIdx ] );

	m_window.display();
}
