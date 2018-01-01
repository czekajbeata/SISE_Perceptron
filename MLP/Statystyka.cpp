#include "stdafx.h"
#include "Statystyka.h"


double Statystyka::sredniaPotegowa(vector<double> &data, int rzad) {
	double srednia;
	double suma = sumaPoteg(data, rzad);
	srednia = 1 / (double)data.size() * suma;
	srednia = pow(srednia, 1 / (double)rzad);
	return srednia;
}

double Statystyka::sredniaArytmetyczna(vector<double> &data) {
	return sredniaPotegowa(data, 1);
}

double Statystyka::wariancja(vector<double> &data) {
	double srednia = sredniaArytmetyczna(data);
	double wartoscWariancji = 0;
	vector<double> sumaOdchylen;
	for each(double d in data) {
		double odchylenie = d - srednia;
		odchylenie *= odchylenie;
		sumaOdchylen.push_back(odchylenie);
	}
	return(sredniaArytmetyczna(sumaOdchylen));
}

double Statystyka::odchylenieStandartowe(vector<double> &data) {
	return sqrt(wariancja(data));
}

double Statystyka::rozkladNormalny(vector<double>& data)
{
	double srednia = sredniaArytmetyczna(data);
	double odchylenie = odchylenieStandartowe(data);
	normal_distribution<double> distribution(srednia, odchylenie);
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);
	return distribution(generator);
}

double Statystyka::sumaPoteg(vector<double> &data, int rzad) {
	double suma = 0;
	for (int i = 0; i < data.size(); i++) {
		suma += pow(data[i], rzad);
	}
	return suma;
}