#include "stdafx.h"
#include "FileManager.h"
using namespace std;

void FileManager::readData(string filename, std::vector<std::vector<double>>& data)
{
	ifstream file;

	string row;
	double bufor;
	stringstream currentRow;
	vector<double> rowValues;	
	file.open(filename);
	file >> row; // miss first row
	while (!file.eof()) {
		file >> row;
		currentRow << row;

		while (currentRow >> bufor) {
			rowValues.push_back(bufor);
				char peek = currentRow.peek();
				if (peek == ',')
					currentRow.ignore();
		}

		data.push_back(rowValues);
		rowValues.clear();
		currentRow.clear(); // Clearing the stringstream
	}
	file.close();
}


void FileManager::writeData(string filename, vector<vector<double>>& data)
{
	ofstream file;
	file.open(filename);
	if (!file.is_open())
		return;

	for each (vector<double> row in data)
	{
		for each (double value in row)
		{
			file << value << " ";
		}
		file << "\n";
	}

	file.close();
}
