#include "include/render.h"

Render::Render(const std::vector<unsigned> &topology, const unsigned numBodies,\
	       	const Position &TL, const Position &BR) : m_window(sf::RenderWindow(sf::VideoMode(WINDOW_SIZE_X, WINDOW_SIZE_Y),\
			       	"NEAT Illustration", sf::Style::Close)), env(topology, numBodies, TL, BR){
	
	createCircles();
	createPellets();

	/* flags for showing connection lines*/
	showBodyLines = false;
	showWallLines = false;
	showPelletLines = false;
	restartEnv = false;
	showInfo = false;

	/* get the TL and BR */
	this->TL = TL;
	this->BR = BR;
	
	if (!font.loadFromFile("res/arial.ttf")){
		std::cerr << "Unable to load font!" << std::endl;
		exit(1);
	}

}

Render::~Render() {}

void Render::createCircles(){
	/* create circles */
	for (unsigned bodyIdx = 0; bodyIdx < env.numBodies; ++bodyIdx){
		m_circles.push_back(sf::CircleShape(env.initRad));
		unsigned *colors = env.bodies[ bodyIdx ].getColor();
		m_circles.back().setFillColor(sf::Color(colors[0], colors[1], colors[2]));
		m_circles.back().setOrigin(sf::Vector2f(env.bodies[bodyIdx].getRadius()/2, env.bodies[bodyIdx].getRadius()/2));
		Position pos = env.bodies[bodyIdx].getPosition();
//		std::cout << bodyIdx << std::endl;
		m_circles.back().setPosition(sf::Vector2f(pos.x, pos.y));
	}
}

void Render::createPellets(){
	/* create pellets */
	for (unsigned pelletIdx = 0; pelletIdx < env.numPellets; ++pelletIdx){
		m_pellets.push_back(sf::CircleShape(env.pelletWt));
		m_pellets.back().setFillColor(sf::Color::White); // for time being, we'll set the color to white
		m_pellets.back().setOrigin(sf::Vector2f(env.pelletWt, env.pelletWt));
		m_pellets.back().setPosition(sf::Vector2f(env.pelletPos[pelletIdx].x, env.pelletPos[pelletIdx].y));
	}
}

void Render::runSimulation(){
	m_window.setPosition(sf::Vector2i(200,50));
	while(m_window.isOpen()){
		sf::Event e;
		while(m_window.pollEvent(e)){
			switch(e.type){
				case sf::Event::Closed:
					m_window.close();
					break;
				case sf::Event::KeyPressed:
					if( e.key.code == sf::Keyboard::B )
						showBodyLines = !showBodyLines;
					else if (e.key.code == sf::Keyboard::P)
						showPelletLines = !showPelletLines;
					else if (e.key.code == sf::Keyboard::W)
						showWallLines = !showWallLines;
					else if (e.key.code == sf::Keyboard::R)
						restartEnv = !restartEnv;
					else if (e.key.code == sf::Keyboard::I){
						showInfo = !showInfo;
					}
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

	if(restartEnv){
		restartEnv = !restartEnv;
		env.resetEnv();
		resetRender();
	}
	env.getBodyInputs(TL,BR);
	env.killOffBoundBodies();
	env.checkPelletConsumption();
	env.changeBodyPosition();
	updateCircles();

	if(showBodyLines)
		showNearBodyLines();
	if(showWallLines)
		showNearWallLines();
	if(showPelletLines)
		showNearPelletLines();
	if(showInfo)
		showDetailedInfo();
}

void Render::drawNDisplay(){
	/* draw pellets */
	for( unsigned pelletIdx = 0; pelletIdx < m_pellets.size(); ++pelletIdx)
		m_window.draw( m_pellets[ pelletIdx ] );

	/* draw body connection lines */
	if(showBodyLines){
		for( unsigned lineIdx = 0; lineIdx < m_srcBodyVertex.size(); ++lineIdx){
			sf::Vertex vertices[2] = {\
				m_srcBodyVertex[lineIdx],\
				m_destBodyVertex[lineIdx]\
			};
			m_window.draw(vertices ,2, sf::Lines);
		}
	}

	/* draw wall connection lines */
	if(showWallLines){
		for( unsigned lineIdx = 0; lineIdx < m_srcWallVertex.size(); ++lineIdx){
			sf::Vertex vertices[2] = {\
				m_srcWallVertex[lineIdx],\
				m_destWallVertex[lineIdx]\
			};
			m_window.draw(vertices ,2, sf::Lines);
		}
	}

	/* draw pellet connection lines*/
	if(showPelletLines){
		for( unsigned lineIdx = 0; lineIdx < m_srcPelletVertex.size(); ++lineIdx){
			sf::Vertex vertices[2] = {\
				m_srcPelletVertex[lineIdx],\
				m_destPelletVertex[lineIdx]\
			};
			m_window.draw(vertices ,2, sf::Lines);
		}
	}

	/* draw circles */
	for( unsigned circleIdx = 0; circleIdx < m_circles.size(); ++circleIdx)
		if(env.bodies[circleIdx].aliveStatus)
			m_window.draw( m_circles[ circleIdx ] );

	/* draw info text */
	if(showInfo){
		unsigned textIdx = 0;
		for(unsigned i = 0; i < env.bodies.size(); ++i){
			if(env.bodies[i].aliveStatus){
				m_window.draw(m_info[textIdx]);
				++textIdx;
			}	
		}
	}

	m_window.display();
}

void Render::resetRender(){
	m_circles.clear();
	m_pellets.clear();
	m_srcBodyVertex.clear();
	m_destBodyVertex.clear();
	m_srcPelletVertex.clear();
	m_destPelletVertex.clear();
	m_srcWallVertex.clear();
	m_destWallVertex.clear();

	createCircles();
	createPellets();
}

void Render::showNearBodyLines(){
	/* get lines from nearby bodies */
	m_srcBodyVertex.clear();
	m_destBodyVertex.clear();

	Position pos0, pos1;
	for( unsigned i = 0; i < env.bodies.size(); ++i ){
		if(env.bodies[i].aliveStatus){
			pos0 = env.bodies[i].getPosition();
			for( unsigned j = 0; j < 3; ++j){
				/* check if the body being connected to is alive; if not don't draw connection line */
				if(env.bodies[ env.bodies[i].nearBodyLoc[j].idx ].aliveStatus && env.bodies[i].nearBodyLoc[j].dist != INT_MAX){
					pos1 = {env.bodies[i].nearBodyLoc[j].x, env.bodies[i].nearBodyLoc[j].y};
					m_srcBodyVertex.push_back(sf::Vector2f(pos0.x,pos0.y));
					m_destBodyVertex.push_back(sf::Vector2f(pos1.x, pos1.y));
				}
			}
		}
	}
}

void Render::showNearWallLines(){
	/* get lines from walls */
	m_srcWallVertex.clear();
	m_destWallVertex.clear();

	Position pos0, pos1;
	for(unsigned i = 0; i < env.bodies.size(); ++i){
		if(env.bodies[i].aliveStatus){
			pos0 = env.bodies[i].getPosition();
	
	 	       /* left wall */
	 	       pos1 = {.x=TL.x, .y=pos0.y};
	 	       m_srcWallVertex.push_back(sf::Vector2f(pos0.x,pos0.y));
	 	       m_destWallVertex.push_back(sf::Vector2f(pos1.x,pos1.y));
	
	 	       /* right wall */
	 	       pos1 = {.x = BR.x, .y = pos0.y};
	 	       m_srcWallVertex.push_back(sf::Vector2f(pos0.x,pos0.y));
	 	       m_destWallVertex.push_back(sf::Vector2f(pos1.x,pos1.y));
	 	       
	 	       /* top wall */
	 	       pos1 = {.x = pos0.x, .y = TL.y};
	 	       m_srcWallVertex.push_back(sf::Vector2f(pos0.x,pos0.y));
	 	       m_destWallVertex.push_back(sf::Vector2f(pos1.x,pos1.y));
	
	 	       /* bottom wall */
	 	       pos1 = {.x = pos0.x,.y = BR.y};
	 	       m_srcWallVertex.push_back(sf::Vector2f(pos0.x,pos0.y));
	 	       m_destWallVertex.push_back(sf::Vector2f(pos1.x,pos1.y));
		}	
	}
}

void Render::showNearPelletLines(){
	/* get lines from pellets */
	m_srcPelletVertex.clear();
	m_destPelletVertex.clear();

	Position pos0, pos1;
	for( unsigned i = 0; i < env.bodies.size(); ++i ){
		if(env.bodies[i].aliveStatus){
			pos0 = env.bodies[i].getPosition();
			for( unsigned j = 0; j < 3; ++j){
				pos1 = {env.bodies[i].nearPelletLoc[j].x, env.bodies[i].nearPelletLoc[j].y};
				m_srcPelletVertex.push_back(sf::Vector2f(pos0.x,pos0.y));
				m_destPelletVertex.push_back(sf::Vector2f(pos1.x, pos1.y));
			}
		}
	}
}

void Render::updateCircles(){
	for(unsigned i = 0; i < env.bodies.size(); ++i){
		Position pos = env.bodies[i].getPosition();
		m_circles[i].setPosition(sf::Vector2f(pos.x, pos.y));
	}
}

void Render::showDetailedInfo(){
	sf::Text text;
	text.setFont(font);
	m_info.clear();
	for (unsigned bodyIdx = 0; bodyIdx < env.bodies.size(); ++bodyIdx){
		if(env.bodies[bodyIdx].aliveStatus){
			/* velocity scaled to 100x below */
			text.setString(\
					"Position: (" + std::to_string((int)env.bodies[bodyIdx].getPosition().x) + ", " +\
				       	std::to_string((int)env.bodies[bodyIdx].getPosition().y) +  ")\n" +\
					"Velocity: (" + std::to_string((int)(env.bodies[bodyIdx].getVelocityMag()*\
								env.bodies[bodyIdx].getVelocityDir().x*100)) + "," +\
							std::to_string((int)(env.bodies[bodyIdx].getVelocityMag()*\
								env.bodies[bodyIdx].getVelocityDir().y*100)) + ")\n" \
					);
			text.setPosition(sf::Vector2f(env.bodies[bodyIdx].getPosition().x,\
						env.bodies[bodyIdx].getPosition().y));
	//		std::cout << "(" << env.bodies[bodyIdx].getPosition().x << "," << env.bodies[bodyIdx].getPosition().y << ")\n";
			text.setCharacterSize(12);
			text.setFillColor(sf::Color::White);
			m_info.push_back(text);
		}	
	}

}
