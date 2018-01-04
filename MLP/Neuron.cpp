#include "stdafx.h"
#include "Neuron.h"
#include <iostream>

using namespace std;

Neuron::Neuron(int inputSize, bool isBias) : isBias(isBias)
{
	randomWeights(inputSize);
	deltaBias = 0;
}

void Neuron::randomWeights(int inputSize)
{
	for (int i = 0; i < inputSize; i++) {
		weights.push_back(Calculations::dRand(WEIGHT_MIN, WEIGHT_MAX));
		deltaWeightsPrev.push_back(0);
	}
	if (isBias) bias = Calculations::dRand(WEIGHT_MIN, WEIGHT_MAX);
}

double Neuron::calculate(vector<double> &input)
{
	double output = 0.0;

	for (int i = 0; i < input.size(); i++)
		output += weights[i] * input[i];

	if (isBias) output += bias;

	output = Calculations::activation(output);

	return output;
}

void Neuron::learn(std::vector<double>& input, double error)
{
	double deltaCurrent = 0.0;

	for (int i = 0; i < input.size(); i++)
	{
		deltaCurrent = (LEARNING_RATE * error * Calculations::activation(input[i])) + (MOMENTUM * deltaWeightsPrev[i]);
		weights[i] += deltaCurrent;
		deltaWeightsPrev[i] = deltaCurrent;
	}

	if (isBias)
	{
		deltaCurrent = LEARNING_RATE * error;
		bias += deltaCurrent + (MOMENTUM * deltaBias);
		deltaBias = deltaCurrent;
	}
}

vector<double> Neuron::getWeights()
{
	return weights;
}
