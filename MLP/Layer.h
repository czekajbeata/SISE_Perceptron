#include "Neuron.h"

class Layer
{
	private:

		std::vector<Neuron> neurons;

	public:

		Layer(int neuronCount, int inputsCount, bool bias);		

		std::vector<double> calculate(std::vector<double> &input);
		std::vector<double> calculate(double input, int index);
		void learn(std::vector<double> &input, std::vector<double> &errors);
		std::vector<std::vector<double>> getWeights();
		int getNumberOfNeurons();
};

