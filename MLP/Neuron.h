#include "Calculations.h"
class Neuron
{
private:

	const double WEIGHT_MIN = -1;
	const double WEIGHT_MAX = 1;
	const double LEARNING_RATE = 0.3;
	const double MOMENTUM = 0.6;

	bool isBias;

	double bias;
	double deltaBias;

	std::vector<double> weights;
	std::vector<double> deltaWeightsPrev;

	void randomWeights(int inputSize);

public:

	Neuron(int inputSize, bool bias);

	double calculate(std::vector<double> &input);
	void learn(std::vector<double> &input, double error);

	std::vector<double> getWeights();
};


