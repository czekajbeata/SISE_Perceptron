#pragma once
#include "Layer.h"

class Learner {
public:
	std::vector<double> learningPoint;
	std::vector<double> learningOutput;
	Learner(std::vector<double> &learningPoint, std::vector<double> &learningOutput)
		: learningPoint(learningPoint), learningOutput(learningOutput) {};
};

class Network
{
	private:
		#pragma region Constants
		const double GLOBAL_ERROR_LIMIT = 0.00000000001;
		const double LEARNING_ERROR = 0.1;
		const int MAX_ITERATION = 10000;
		const int GRAPH_STEP = 100;
		#pragma endregion

		#pragma region Fields
		int iterator = 0;
		std::vector<Layer> layers;
		std::vector<Learner> learners;
		std::vector<std::vector<double>> realOutputs;
		bool isBias;
		double error;
		#pragma endregion

		#pragma region Methods
		std::vector<double> countOutputError(std::vector<double>& expected, std::vector<double>& actual);
		std::vector<double> countLocalErrorForOtherLayers(int layerIndex, std::vector<double>& actual, std::vector<double>& backErrors);
		std::vector<std::vector<double>> errorsEpochsGraphSet;
		std::vector<std::vector<double>> realExpectedGraph;


		double countGlobalError(std::vector<double>& localErrors);
		bool areEqual(std::vector<double> i1, std::vector<double> i2);
		void learnSingleLearner(Learner &learner);
		bool isNetworkLearnt();
		#pragma endregion

	public:
		#pragma region Constructors
		Network(std::vector<std::vector<double>> &lerningPoints, std::vector<std::vector<double>> &lerningOutputs, bool bias);
		#pragma endregion

		#pragma region Network Configuration
		void addLayer(Layer &layer);
		void learn();
		#pragma endregion
	
		#pragma region Value Accessors
		int getIterationCount();
		std::vector<double> calculate(std::vector<double> &input);
		std::vector<std::vector<double>> getWeights();
		std::vector<std::vector<double>> getErrorsEpochsGraphSet();
		std::vector<std::vector<double>> getRealExpectedGraph();
		double getError();
		void setBias(bool bias);
		bool getBias();
		#pragma endregion	
};

