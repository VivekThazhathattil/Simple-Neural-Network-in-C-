#include<vector>
#include <cstdlib>
#include <cmath>
#include "connection.h"


class Neuron{
	private:
		typedef std::vector<Neuron> Layer;

		static double transferFunction(double x);
		static double transferFunctionDerivative(double x);
		static double randomWeight(void) { return std::rand()/double(RAND_MAX); }
		double sumDOW(const Layer &nextLayer);
		double m_outputVal;
		std::vector<Connection> m_outputWeights;
		unsigned m_myIndex;
		double m_gradient;
	public:
		static double eta; // [0.0..1.0] overall training rate
		static double alpha; // [0.0..n] momentum: multiplier of last weight change

		Neuron(unsigned numOutputs, unsigned m_myIndex);
		~Neuron();
		void feedForward(const Layer &prevLayer);
		void setOutputVal(double val) { m_outputVal = val; }
		double getOutputVal() const { return m_outputVal; }
		void calcOutputGradients( double targetVal);
		void calcHiddenGradients( const Layer &nextLayer);
		void updateInputWeights( Layer &prevLayer);
};
