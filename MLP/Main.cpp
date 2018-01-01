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

const vector<double> FIRST_SPECIES = vector<double>{ 1, 0, 0 };
const vector<double> SECOND_SPECIES = vector<double>{ 0, 1, 0 };
const vector<double> THIRD_SPECIES = vector<double>{ 0, 0, 1 };

enum Species {
	First,
	Second,
	Thirs
};

Network& configureForIris();
Network& configureForWine();
Network& configureForSquareRoots();

bool isThatSpecies(vector<double> actual, Species expected);
bool areEqual(vector<double> i1, vector<double> i2);

int main()
{
	srand(time(NULL));

	Network network = configureForSquareRoots();
	//Network network = configureForIris();

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

Network & configureForWine()
{
	int learningCount = 30;
	int wineSpeciesCount = 3;
	vector<vector<double>> learningInputs, learningOutputs, testInputs, testOutputs;
	vector<vector<double>> wineSetFile;

	FileManager::readData("wine.data", wineSetFile);

	for (int i = 1; i < 14; i++) {
		vector<double> column;
		for (int k = 0; k < wineSetFile.size(); k++) {
			column.push_back(wineSetFile[k][i]);
		}
		for (int j = 0; j < wineSetFile.size(); j++) {
			double rozkal = Statystyka::rozkladNormalny(column);
			wineSetFile[j][i] /= rozkal;
		}
	}

	int coKtoryUczacy = 2;
	vector<int> etykiety;
	int a = 0, b = 0, c = 0;

	for (int i = 0; i < wineSetFile.size(); i = i + coKtoryUczacy)
	{
		vector<double> input, output;
		//LEARNING
		for (int k = 1; k < wineSetFile[i].size(); k++)
			input.push_back(wineSetFile[i][k]);

		if (wineSetFile[i][0] == 1.0)
			output = vector<double>{ 1, 0, 0 };
		else if (wineSetFile[i][0] == 2.0)
			output = vector<double>{ 0, 1, 0 };
		else if (wineSetFile[i][0] == 3.0)
			output = vector<double>{ 0, 0, 1 };
		learningInputs.push_back(input);
		learningOutputs.push_back(output);
		//LEARNING

		//TESTPOINTS
		if (i < wineSetFile.size() - 1)
			for (int j = 0; j < 1; j++) {
				for (int k = 1; k < wineSetFile[i + j].size(); k++)
					input.push_back(wineSetFile[i + j][k]);
				testInputs.push_back(input);
				if (wineSetFile[i + j][0] == 1.0)
					a++;
				else if (wineSetFile[i + j][0] == 2.0)
					b++;
				else if (wineSetFile[i + j][0] == 3.0)
					c++;
				//etykiety.push_back(wineSetFile[i + j][0]);
			}
		//TESTPOINTS

	}


	/*for (int i = 0; i < wineSetFile.size() - 1; i += 50) {

	for (int j = i; j < learningCount + i; j++)
	{
	vector<double> input, output;

	for (int k = 1; k < wineSetFile[i].size(); k++)
	input.push_back(wineSetFile[j][k]);

	switch (wineSetFile[j][0] == 1) {
	case 1:
	output = FIRST_SPECIES;
	break;
	case 2:
	output = SECOND_SPECIES;
	break;
	case 3:
	output = THIRD_SPECIES;
	break;
	}

	learningInputs.push_back(input);
	learningOutputs.push_back(output);
	}
	int temp = wineSetFile[i][0];
	int j = i;
	while(wineSetFile[j][0] != temp)
	{
	vector<double> input, output;

	for (int k = 1; k < wineSetFile[i].size(); k++)
	input.push_back(wineSetFile[j][k]);
	testInputs.push_back(input);
	j++;
	}
	temp = wineSetFile[j][0];
	}
	*/
	FileManager::writeData("input.data", learningInputs);


	bool useBias = true;
	Network * network = new Network(learningInputs, learningOutputs, useBias);

	Layer layer1(2, 13, useBias);
	Layer layer2(3, 2, useBias);

	network->addLayer(layer1);
	network->addLayer(layer2);
	network->learn();

	//MACIERZ KONFUZJI

	vector<vector<double>> conf;
	for (int i = 0; i < wineSpeciesCount; i++) {
		vector<double> row;
		row.resize(wineSpeciesCount);
		fill(row.begin(), row.end(), 0);
		conf.push_back(row);
	}

	int temp = 0;
	//for (int i = 0; i < wineSpeciesCount; i++) {

	for (int j = 0; j < a; j++)
	{
		vector<double> iris = network->calculate(testInputs[j]);
		if (isThatSpecies(iris, Species::First))
			conf[0][0]++;
		else if (isThatSpecies(iris, Species::Second))
			conf[0][1]++;
		else if (isThatSpecies(iris, Species::Thirs))
			conf[0][2]++;
	}
	for (int j = a; j < a + b; j++)
	{
		vector<double> iris = network->calculate(testInputs[j]);
		if (isThatSpecies(iris, Species::First))
			conf[1][0]++;
		else if (isThatSpecies(iris, Species::Second))
			conf[1][1]++;
		else if (isThatSpecies(iris, Species::Thirs))
			conf[1][2]++;
	}
	for (int j = a + b; j <= a + b + c; j++)
	{
		vector<double> iris = network->calculate(testInputs[j]);
		if (isThatSpecies(iris, Species::First))
			conf[2][0]++;
		else if (isThatSpecies(iris, Species::Second))
			conf[2][1]++;
		else if (isThatSpecies(iris, Species::Thirs))
			conf[2][2]++;
	}
	//}

	cout << "Macierz konfuzji:\n";
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			cout << conf[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;

	//MACIERZ KONFUZJI


	//MACIERZ B£ÊDU

	for (int i = 0; i < conf.size(); i++) {
		vector<vector<int>> errors;
		vector<int> row;
		int fn = 0, fp = 0, tp = 0, tn = 0;
		for (int j = 0; j < conf.size(); j++) {
			if (i != j) {
				fn += conf[i][j];
				fp += conf[j][i];
			}
		}

		tp = conf[i][i];

		for (int j = 0; j < conf.size(); j++)
			for (int k = 0; k < conf.size(); k++) {
				if (j != i && k != i) {
					tn += conf[j][k];
				}
			}

		row.push_back(tp);
		row.push_back(fn);
		errors.push_back(row);
		row[0] = fp;
		row[1] = tn;
		errors.push_back(row);

		cout << "Macierz bledu:\n";
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				cout << errors[i][j] << " ";
			}
			cout << endl;
		}
		cout << endl;
	}

	//MACIERZ B£ÊDU

	FileManager::writeData("input.data", learningInputs);
	int i = 1;
	for each (vector<double> tester in testInputs)
	{
		//for each(double input in tester)
		//	cout << fixed << setprecision(3) << input << "   ";
		//cout << endl;
		cout << i << " Output:   ";
		if (network->calculate(tester)[0] > 0.5)
			cout << " Iris-setosa";
		else if (network->calculate(tester)[1] > 0.5)
			cout << " Iris-versicolor";
		else if (network->calculate(tester)[2] > 0.5)
			cout << " Iris-virginica";
		//for each(double output in network->calculate(tester))
		//	cout << fixed << setprecision(3) << output << "   ";
		cout << endl;// << endl;
		i++;
	}

	return *network;
}
Network & configureForIris()
{
	int learningCount = 30, testCount = 20; // sum to 50
	int irisSpeciesCount = 3;
	vector<vector<double>> learningInputs, learningOutputs, testInputs, testOutputs;
	vector<vector<double>> irisSetFile;

	FileManager::readData("iris.data", irisSetFile);

	for (int i = 0; i < irisSetFile.size() - 1; i += 50) {

		for (int j = i; j < learningCount + i; j++)
		{
			vector<double> input, output;

			for (int k = 0; k < 4; k++)
				input.push_back(irisSetFile[j][k]);

			for (int k = 4; k < irisSetFile[j].size(); k++)
				output.push_back(irisSetFile[j][k]);

			learningInputs.push_back(input);
			learningOutputs.push_back(output);
		}

		for (int j = i + learningCount; j < learningCount + testCount + i; j++)
		{
			vector<double> input, output;

			for (int k = 0; k < 4; k++)
				input.push_back(irisSetFile[j][k]);
			testInputs.push_back(input);
		}
	}
	FileManager::writeData("input.data", learningOutputs);


	bool useBias = true;
	Network * network = new Network(learningInputs, learningOutputs, useBias);

	Layer layer1(2, 4, useBias);
	Layer layer2(3, 3, useBias);

	network->addLayer(layer1);
	network->addLayer(layer2);
	network->learn();

	//MACIERZ KONFUZJI

	vector<vector<double>> conf;
	for (int i = 0; i < irisSpeciesCount; i++) {
		vector<double> row;
		row.resize(irisSpeciesCount);
		fill(row.begin(), row.end(), 0);
		conf.push_back(row);
	}

	for (int i = 0; i < irisSpeciesCount; i++) {
		for (int j = i * learningCount; j < (i + 1)*learningCount; j++)
		{
			vector<double> iris = network->calculate(learningInputs[j]);
			if (isThatSpecies(iris, Species::First))
				conf[i][0]++;
			else if (isThatSpecies(iris, Species::Second))
				conf[i][1]++;
			else if (isThatSpecies(iris, Species::Thirs))
				conf[i][2]++;
		}
	}

	cout << "Macierz konfuzji:\n";
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			cout << conf[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;

	//MACIERZ KONFUZJI


	//MACIERZ B£ÊDU

	for (int i = 0; i < conf.size(); i++) {
		vector<vector<int>> errors;
		vector<int> row;
		int fn = 0, fp = 0, tp = 0, tn = 0;
		for (int j = 0; j < conf.size(); j++) {
			if (i != j) {
				fn += conf[i][j];
				fp += conf[j][i];
			}
		}

		tp = conf[i][i];

		for (int j = 0; j < conf.size(); j++)
			for (int k = 0; k < conf.size(); k++) {
				if (j != i && k != i) {
					tn += conf[j][k];
				}
			}

		row.push_back(tp);
		row.push_back(fn);
		errors.push_back(row);
		row[0] = fp;
		row[1] = tn;
		errors.push_back(row);

		cout << "Macierz bledu:\n";
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				cout << errors[i][j] << " ";
			}
			cout << endl;
		}
		cout << endl;
	}

	//MACIERZ B£ÊDU

	FileManager::writeData("input.data", learningInputs);
	int i = 1;
	for each (vector<double> tester in testInputs)
	{
		//for each(double input in tester)
		//	cout << fixed << setprecision(3) << input << "   ";
		//cout << endl;
		cout << i << " Output:   ";
		if (network->calculate(tester)[0] > 0.5)
			cout << " Iris-setosa";
		else if (network->calculate(tester)[1] > 0.5)
			cout << " Iris-versicolor";
		else if (network->calculate(tester)[2] > 0.5)
			cout << " Iris-virginica";
		//for each(double output in network->calculate(tester))
		//	cout << fixed << setprecision(3) << output << "   ";
		cout << endl;// << endl;
		i++;
	}

	return *network;
}

Network & configureForSquareRoots()
{
	int learningCount = 20, testCount = 20; // sum to 50
	vector<vector<double>> learningInputs, learningOutputs, testInputs, testOutputs;

	for (int i = 0; i < learningCount; i++) {
		double randomNumber = ((double)rand() / RAND_MAX) * 100;
		vector<double> input, output;
		input.push_back(randomNumber/100.0);
		//input.push_back(randomNumber);
		double sqrtvar = sqrt(randomNumber) / 10.0;
		//int sqrInt = (int)sqrtvar;
		output.push_back(sqrtvar);
		//output.push_back(0.5);
		learningInputs.push_back(input);
		learningOutputs.push_back(output);
	}
	
	for (int i = 0; i < testCount; i++) {
		double randomNumber = ((double)rand() / RAND_MAX) * 100;
		vector<double> input, output;
		input.push_back(randomNumber / 100.0);
		double sqrtvar = sqrt(randomNumber)/10.0;
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
	Layer layer2(3, 3, useBias);
	Layer layer3(1, 3, useBias);

	network->addLayer(layer1);
	network->addLayer(layer2);
	network->addLayer(layer3);
	network->learn();

	for (int i = 0; i < testInputs.size(); i++)
	{
		cout << "wejscie: " << testInputs[i].at(0) << " wyjscie oczekiwane: ";

		cout << testOutputs[i][0] << " ";
		auto output = network->calculate(testInputs[i]);
		cout << " wyjscie: " << output[0];
		cout << endl;
	}

	////MACIERZ KONFUZJI

	//vector<vector<double>> conf;
	//for (int i = 0; i < irisSpeciesCount; i++) {
	//	vector<double> row;
	//	row.resize(irisSpeciesCount);
	//	fill(row.begin(), row.end(), 0);
	//	conf.push_back(row);
	//}

	//for (int i = 0; i < irisSpeciesCount; i++) {
	//	for (int j = i * learningCount; j < (i + 1)*learningCount; j++)
	//	{
	//		vector<double> iris = network->calculate(learningInputs[j]);
	//		if (isThatSpecies(iris, Species::First))
	//			conf[i][0]++;
	//		else if (isThatSpecies(iris, Species::Second))
	//			conf[i][1]++;
	//		else if (isThatSpecies(iris, Species::Thirs))
	//			conf[i][2]++;
	//	}
	//}

	//cout << "Macierz konfuzji:\n";
	//for (int i = 0; i < 3; i++)
	//{
	//	for (int j = 0; j < 3; j++)
	//	{
	//		cout << conf[i][j] << " ";
	//	}
	//	cout << endl;
	//}
	//cout << endl;

	////MACIERZ KONFUZJI


	////MACIERZ B£ÊDU

	//for (int i = 0; i < conf.size(); i++) {
	//	vector<vector<int>> errors;
	//	vector<int> row;
	//	int fn = 0, fp = 0, tp = 0, tn = 0;
	//	for (int j = 0; j < conf.size(); j++) {
	//		if (i != j) {
	//			fn += conf[i][j];
	//			fp += conf[j][i];
	//		}
	//	}

	//	tp = conf[i][i];

	//	for (int j = 0; j < conf.size(); j++)
	//		for (int k = 0; k < conf.size(); k++) {
	//			if (j != i && k != i) {
	//				tn += conf[j][k];
	//			}
	//		}

	//	row.push_back(tp);
	//	row.push_back(fn);
	//	errors.push_back(row);
	//	row[0] = fp;
	//	row[1] = tn;
	//	errors.push_back(row);

	//	cout << "Macierz bledu:\n";
	//	for (int i = 0; i < 2; i++)
	//	{
	//		for (int j = 0; j < 2; j++)
	//		{
	//			cout << errors[i][j] << " ";
	//		}
	//		cout << endl;
	//	}
	//	cout << endl;
	//}

	////MACIERZ B£ÊDU

	//FileManager::writeData("input.data", learningInputs);
	//int i = 1;
	//for each (vector<double> tester in testInputs)
	//{
	//	//for each(double input in tester)
	//	//	cout << fixed << setprecision(3) << input << "   ";
	//	//cout << endl;
	//	cout << i << " Output:   ";
	//	if (network->calculate(tester)[0] > 0.5)
	//		cout << " Iris-setosa";
	//	else if (network->calculate(tester)[1] > 0.5)
	//		cout << " Iris-versicolor";
	//	else if (network->calculate(tester)[2] > 0.5)
	//		cout << " Iris-virginica";
	//	//for each(double output in network->calculate(tester))
	//	//	cout << fixed << setprecision(3) << output << "   ";
	//	cout << endl;// << endl;
	//	i++;
	//}

	return *network;
}

bool isThatSpecies(vector<double> iris, Species species) {
	vector<double> x;
	switch (species) {
	case Species::First:
		x = FIRST_SPECIES;
		break;
	case Species::Second:
		x = SECOND_SPECIES;
		break;
	case Species::Thirs:
		x = THIRD_SPECIES;
		break;
	}
	return areEqual(iris, x);
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
