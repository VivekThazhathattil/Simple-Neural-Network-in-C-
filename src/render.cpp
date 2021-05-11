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
//		std::cout << bodyIdx << std::endl;
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
	if(env.checkGenExpiration()){
		env.resetEnv();
		resetRender();
	}
	env.checkBodyDeath();
	env.checkPelletConsumption();
	env.changeBodyPosition();
	showNearBodyLines();
}

void Render::drawNDisplay(){
	/* draw pellets */
	for( unsigned pelletIdx = 0; pelletIdx < m_pellets.size(); ++pelletIdx)
		m_window.draw( m_pellets[ pelletIdx ] );

	/* draw connection lines */
	for( unsigned lineIdx = 0; lineIdx < m_srcVertex.size(); ++lineIdx){
		sf::Vertex vertices[2] = {\
			m_srcVertex[lineIdx],\
			m_destVertex[lineIdx]\
		};
		m_window.draw(vertices ,2, sf::Lines);
	}

	/* draw circles */
	for( unsigned circleIdx = 0; circleIdx < m_circles.size(); ++circleIdx)
		m_window.draw( m_circles[ circleIdx ] );


	m_window.display();
}

void Render::resetRender(){
}

void Render::showNearBodyLines(){
//	/* get lines from bodies */
//	m_lines.clear();
//	sf::RectangleShape line;
//	Position pos0, pos1;
//	for( unsigned i = 0; i < env.bodies.size(); ++i){
//		pos0 = env.bodies[i].getPosition();
//		for(unsigned j = 0; j < 3; ++j){
//			if( env.bodies[i].nearBodyLoc[j].dist != INT_MAX){
//				pos1 = {(int)env.bodies[i].nearBodyLoc[j].x, (int)env.bodies[i].nearBodyLoc[j].y};
//				line.setPosition(sf::Vector2f(pos0.x, pos0.y));
//	//			line.setOrigin(sf::Vector2f(0.5*(pos1.x - pos0.x), 0.5*(pos1.y - pos0.y)));
//				line.setSize(sf::Vector2f(env.bodies[i].nearBodyLoc[j].dist, 1));
//	//			std::cout << "nearBodyLoc Dist: " << env.bodies[i].nearBodyLoc[j].dist << std::endl;
//				float rad_rot = atan( float(pos1.y-pos0.y) / (pos1.x - pos0.x));
//				line.setRotation(rad_rot*180/M_PI);
//				unsigned *colors = env.bodies[i].getColor();
//				line.setFillColor(sf::Color(colors[0], colors[1], colors[2]));
//				m_lines.push_back(line);
//			}
//		}
//	}
//	/* get lines from walls */
//	/* get lines from pellets */
//
	m_srcVertex.clear();
	m_destVertex.clear();

	Position pos0, pos1;
	for( unsigned i = 0; i < env.bodies.size(); ++i ){
		pos0 = env.bodies[i].getPosition();
		for( unsigned j = 0; j < 3; ++j){
			pos1 = {env.bodies[i].nearBodyLoc[j].x, env.bodies[i].nearBodyLoc[j].y};
			m_srcVertex.push_back(sf::Vector2f(pos0.x,pos0.y));
			m_destVertex.push_back(sf::Vector2f(pos1.x, pos1.y));
		}
	}
}
