#include "stdafx.h"
#include "Network.h"
#include <iostream>
#include <algorithm>

using namespace std;

#pragma region Constructors
Network::Network(vector<vector<double>> &lerningPoints, vector<vector<double>> &lerningOutputs, bool isBias)
{
	this->isBias = isBias;
	for (int i = 0; i < lerningPoints.size(); i++) {
		learners.push_back(Learner(lerningPoints[i], lerningOutputs[i]));
	}
}
#pragma endregion

#pragma region Public
void Network::addLayer(Layer &layer)
{
	layers.push_back(layer);
	realOutputs.push_back(vector<double>());
}

void Network::learn()
{
	int realExpectedGraphStep = MAX_ITERATION/1000;

	do {		
		error = 0;
		if (iterator % realExpectedGraphStep == 0)
		{
			realExpectedGraph.push_back(vector<double>());
			realExpectedGraph.back().resize(2);
			fill(realExpectedGraph.back().begin(), realExpectedGraph.back().end(), 0);
		}
		for each (Learner learner in learners)
		{
			if (iterator % realExpectedGraphStep == 0)
			{
				realExpectedGraph.back()[0] = ((double)iterator);
				if(areEqual(learner.learningOutput, calculate(learner.learningPoint)))
					realExpectedGraph.back()[1]++;
			}
			learnSingleLearner(learner);
		}
		if (realExpectedGraph.back()[1] > 0.85*learners.size())
			realExpectedGraphStep = MAX_ITERATION/10;

		if (iterator % GRAPH_STEP == 0)
			errorsEpochsGraphSet.push_back(vector<double>{(double)iterator, error});

		random_shuffle(learners.begin(), learners.end());
		iterator++;
	} while (isNetworkLearnt());
}

vector<double> Network::calculate(std::vector<double>& input)
{
	vector<double> output = layers[0].calculate(input);
	for (int i = 1; i < layers.size(); i++) {
		output = layers[i].calculate(output);
	}
	return output;
}
int Network::getIterationCount()
{
	return iterator;
}

vector<vector<double>> Network::getWeights()
{
	return layers[layers.size() - 1].getWeights();
}

std::vector<std::vector<double>> Network::getErrorsEpochsGraphSet()
{
	return errorsEpochsGraphSet;
}

std::vector<std::vector<double>> Network::getRealExpectedGraph()
{
	return realExpectedGraph;
}

double Network::getError()
{
	return error;
}

void Network::setBias(bool isBias)
{
	this->isBias = isBias;
}
bool Network::getBias()
{
	return isBias;
}
#pragma endregion

#pragma region Private
bool Network::areEqual(vector<double> i1, vector<double> i2)
{
	if (i1.size() != i2.size())
		return false;
	for (int i = 0; i < i1.size(); i++) {
		if (fabs(i1[i] - i2[i])>(LEARNING_ERROR))
			return false;
	}
	return true;
}

void Network::learnSingleLearner(Learner& learner)
{
	vector<vector<double>> localErrors(layers.size());

	//dla kazdej liczy wyjscie OD POCZATKU - 
	// PROPAGACJA W PRZOD
		realOutputs[0] = layers[0].calculate(learner.learningPoint);
	for (int i = 1; i < layers.size(); i++)
		realOutputs[i] = layers[i].calculate(realOutputs[i - 1]);
	
	// LICZY BLAD NA WYJSCIU
	localErrors[layers.size() - 1] = countOutputError(learner.learningOutput, realOutputs[layers.size() - 1]);
	
	// PROPAGACJA WSTECZ JAK MAMY BLAD NA WYJSCIU
	for (int i = layers.size() - 1; i > 0; i--)
		localErrors[i - 1] = countLocalErrorForOtherLayers(i - 1, realOutputs[i - 1], localErrors[i]);

	// NAUKA
	layers[0].learn(learner.learningPoint, localErrors[0]);
	for (int i = 1; i < layers.size(); i++)
		layers[i].learn(realOutputs[i - 1], localErrors[i]);

	error += countGlobalError(localErrors[layers.size() - 1]);
}

bool Network::isNetworkLearnt() {
	return (error > GLOBAL_ERROR_LIMIT && iterator < MAX_ITERATION);
}

vector<double> Network::countLocalErrorForOtherLayers(int layerIndex, std::vector<double>& actual, std::vector<double>& prevErrors)
{
	int prevLayerIndex = layerIndex + 1;
	vector<double> localErrors(layers[layerIndex].getNumberOfNeurons());	

	for (int thisIndex = 0; thisIndex < localErrors.size(); thisIndex++) {
		double error = 0;
		for (int prevIndex = 0; prevIndex < layers[prevLayerIndex].getNumberOfNeurons(); prevIndex++)
			error += prevErrors[prevIndex] * layers[prevLayerIndex].getWeights()[prevIndex][thisIndex];
		localErrors[thisIndex] = Calculations::activationDerivative(actual[thisIndex]) * error;
	}

	return localErrors;
}

vector<double> Network::countOutputError(vector<double>& expected, vector<double>& actual)
{
	vector<double> localErrors(expected.size());
	for (int i = 0; i < layers.back().getNumberOfNeurons(); i++) 
		localErrors[i] = Calculations::activationDerivative(actual[i]) * (expected[i] - actual[i]);
	return localErrors;
}

double Network::countGlobalError(vector<double>& localErrors)
{
	double globalLayerError = 0;
	for each (double localError in localErrors)
	{
		globalLayerError += pow(localError, 2);
	}
	globalLayerError *= 0.5;
	return globalLayerError;
}
#pragma endregion
