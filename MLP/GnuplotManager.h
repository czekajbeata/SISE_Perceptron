#pragma once
#define GNUPLOT_PATH "C:\\gnuplot\\bin"
#include <vector>
#include <string>
#include <algorithm> 

class Gnuplot;

using namespace std;

class GnuplotManager
{
private:	
	Gnuplot *main_plot;
	string title;
	string xLabel;
	string yLabel;
	string style;
	bool gridOn;

	void configure();

public:
	GnuplotManager();	

	void plotErrorFile();

	void plotOutputsFile();

	void setTitle(string text);
	void setXLabel(string text);
	void setYLabel(string text);
	void setRange(double min, double max);

	// Choose "lines" or "points"
	void setStyle(string text);

	void setGridOn(bool on);
};

