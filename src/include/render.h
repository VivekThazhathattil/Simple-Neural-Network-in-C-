#include "env.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include "consts.h"
#include <iostream>
#include <cmath>
#include <string>

class Render{
	private:
		Env env;
		sf::RenderWindow m_window;
		std::vector<sf::CircleShape> m_circles;
		std::vector<sf::CircleShape> m_pellets;

		std::vector<sf::Vertex> m_srcBodyVertex;
		std::vector<sf::Vertex> m_destBodyVertex;
		std::vector<sf::Vertex> m_srcPelletVertex;
		std::vector<sf::Vertex> m_destPelletVertex;
		std::vector<sf::Vertex> m_srcWallVertex;
		std::vector<sf::Vertex> m_destWallVertex;
		std::vector<sf::Text> m_info;
		sf::Font font;

		bool showBodyLines;
		bool showWallLines;
		bool showPelletLines;
		bool showInfo;
		bool restartEnv;
		
		void updateState();
		void updateCircles();
		void drawNDisplay();
		void resetRender();

		void showNearBodyLines();
		void showNearPelletLines();
		void showNearWallLines();
		void showDetailedInfo();

		Position TL;
		Position BR;

		void createCircles();
		void createPellets();
	public:
		Render(const std::vector<unsigned> &topology, const unsigned numBodies,\
	       			const Position &TL, const Position &BR);
		~Render();
		void runSimulation();
};
