#include "include/render.h"
#include <vector>

int main(){
	
	std::vector<unsigned> topology;
	topology.push_back(3);
	topology.push_back(2);
	topology.push_back(1);

	unsigned numBodies = 30;
	Position TL = {200, 0};
	Position BR = {800,700};

	Render r(topology, numBodies, TL, BR);
	r.runSimulation();
	return 0;
}
