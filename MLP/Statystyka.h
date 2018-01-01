#pragma once

#include <math.h>
#include <vector>
#include <algorithm>
#include <iterator>
#include <random>
#include <chrono>


using namespace std;

class Statystyka
{
public:

	static double sredniaPotegowa(vector<double> &data, int rzad);

	static double sredniaArytmetyczna(vector<double> &data);

	static double wariancja(vector<double> &data);

	static double odchylenieStandartowe(vector<double> &data);

	static double rozkladNormalny(vector<double> &data);

private:

	static double sumaPoteg(vector<double> &data, int rzad);

};



