#pragma once

#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

class FileManager
{
public:
	static void readData(string filename, std::vector<std::vector<double>> &data);
	static void writeData(string filename, std::vector<std::vector<double>> &data);
	//static void prepareFileToPlot(string filename, MultidimentionalPointsContainer &data);
	//static void prepareFileToPlot(string filename, vector<Centroid> &data);
};
