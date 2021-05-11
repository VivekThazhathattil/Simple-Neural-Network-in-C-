#include "env.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include "consts.h"
#include <iostream>
#include <cmath>

class Render{
	private:
		Env env;
		sf::RenderWindow m_window;
		std::vector<sf::CircleShape> m_circles;
		std::vector<sf::CircleShape> m_pellets;
		std::vector<sf::Vertex> m_srcVertex;
		std::vector<sf::Vertex> m_destVertex;

		void updateState();
		void drawNDisplay();
		void resetRender();
		void showNearBodyLines();
	public:
		Render(const std::vector<unsigned> &topology, const unsigned numBodies,\
	       			const Position &TL, const Position &BR);
		~Render();
		void runSimulation();
};
