// MLP.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <iomanip>

#include "Network.h"
#include "FileManager.h"
#include "GnuplotManager.h"
#include "Statystyka.h"

using namespace std;

Network& configureForSquareRoots();

int main()
{
	srand(time(NULL));

	Network network = configureForSquareRoots();

	FileManager::writeData("weights.data", network.getWeights());
	FileManager::writeData("errors.data", network.getErrorsEpochsGraphSet());
	FileManager::writeData("outputs.data", network.getRealExpectedGraph());

	std::cout << " Ilosc iteracji: " << network.getIterationCount() << endl;
	std::cout << " Blad          : " << fixed << setprecision(10) << network.getError() << endl << endl;


	GnuplotManager gnuplot;
	gnuplot.plotErrorFile();
	gnuplot = GnuplotManager();
	gnuplot.plotOutputsFile();

	cin.get();
	cin.get();
	return 0;
}

Network & configureForSquareRoots()
{
	int learningCount = 30, testCount = 100;
	vector<vector<double>> learningInputs, learningOutputs, testInputs, testOutputs;

	for (int i = 1; i < learningCount; i++) {
		double randomNumber = ((double)rand() / RAND_MAX) * 100;
		vector<double> input, output;
		input.push_back(randomNumber / 100.0);
		double sqrtvar = sqrt(randomNumber) / 10.0;
		output.push_back(sqrtvar);
		learningInputs.push_back(input);
		learningOutputs.push_back(output);
	}

	for (int i = 0; i < testCount; i++) {
		double randomNumber = (i + 1)*(i + 1);
		vector<double> input, output;
		input.push_back(randomNumber / (double)(testCount*testCount));
		double sqrtvar = sqrt(randomNumber) / 100.0;
		int sqrInt = (int)sqrtvar;
		output.push_back(sqrtvar);
		testInputs.push_back(input);
		testOutputs.push_back(output);
	}

	FileManager::writeData("inputs", learningInputs);
	FileManager::writeData("outputs", learningOutputs);
	FileManager::writeData("testInputs", testInputs);
	FileManager::writeData("testOutputs", testOutputs);


	bool useBias = true;
	Network * network = new Network(learningInputs, learningOutputs, useBias);

	Layer layer1(3, 1, useBias);
	Layer layer2(1, 3, useBias);

	network->addLayer(layer1);
	network->addLayer(layer2);
	network->learn();

	vector<vector<double>> output; // wejscie, wyjscie oczekiwane, wyjscie obliczone

	for (int i = 0; i < testInputs.size(); i++)
	{
		vector<double> row;
		auto currentInput = testInputs[i][0];
		auto currentOutput = testOutputs[i][0];
		row.push_back(currentInput*testCount*testCount);
		row.push_back(currentOutput*testCount);
		row.push_back(network->calculate(testInputs[i])[0] * testCount);
		output.push_back(row);

		cout << "wejscie: " << testInputs[i].at(0) * testCount * testCount << " wyjscie oczekiwane: ";
		cout << testOutputs[i][0] * testCount << " ";
		auto output = network->calculate(testInputs[i]);
		cout << " wyjscie: " << output[0] * testCount;
		cout << endl;
	}

	FileManager::writeData("testResults.csv", output);

	return *network;
}

bool areEqual(vector<double> i1, vector<double> i2)
{
	if (i1.size() != i2.size())
		return false;
	for (int i = 0; i < i1.size(); i++) {
		if (round(i1[i]) != round(i2[i]))
			return false;
	}
	return true;
}
