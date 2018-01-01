#include "stdafx.h"
#include "Layer.h"
#include <iostream>
using namespace std;

Layer::Layer(int neuronCount, int inputsCount, bool bias)
{
	neurons = vector<Neuron>();

	for(int i = 0; i < neuronCount; i++)
		neurons.push_back(Neuron(inputsCount, bias));
}

vector<double> Layer::calculate(std::vector<double> &input)
{
	vector<double> outputs;

	for each (Neuron neuron in neurons) {
		double output = neuron.calculate(input);
		outputs.push_back(output);
	}		

	return outputs;
}

vector<double> Layer::calculate(double input, int index)
{
	vector<double> outputs;
	vector<double> inputs;
	inputs.push_back(input);
	double output = neurons[index].calculate(inputs);
	outputs.push_back(output);

	return outputs;
}

void Layer::learn(std::vector<double> &input, std::vector<double> &errors)
{	
	for (int index = 0; index < neurons.size(); index++)	
		neurons[index].learn(input, errors[index]);
}

vector<vector<double>> Layer::getWeights()
{
	vector<vector<double>> allNeuronsWeights;

	for each (Neuron neuron in neurons) {
		vector<double> weights = neuron.getWeights();
		allNeuronsWeights.push_back(weights);
	}

	return allNeuronsWeights;
}

int Layer::getNumberOfNeurons()
{
	return neurons.size();
}
