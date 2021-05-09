#include <vector>
#include <iostream>
#include <cassert>
#include "neuron.h"

typedef std::vector<Neuron> Layer;

class Net{
	private:
		std::vector<Layer> m_layers; //m_layers[layerNum][neuronNum]
		double m_error;
		double m_recentAverageError;
		static double m_recentAverageSmoothingFactor;
		
	public:
		Net(const std::vector<unsigned> &topology);
		~Net();

		void feedForward(const std::vector<double> &inputVals);
		void backProp(const std::vector<double> &targetVals);
		void getResults(std::vector<double> &resultVals) const;
		double getRecentAverageError(void) const { return m_recentAverageError; }

};
