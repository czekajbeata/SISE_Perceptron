#include "stdafx.h"
#include "Calculations.h"


double Calculations::activation(double x)
{
	//if (x > 0)
	//	return 1;
	//else return 0;

	return 1.0 / (1 + exp(-x));
}

double Calculations::activationDerivative(double x)
{
	return exp(x)/(pow((exp(x)+1), 2));
	//return x * (1.0 - x);
}

double Calculations::dRand(double min, double max)
{
	double d = (double)rand() / RAND_MAX;
	return min + d * (max - min);
}
