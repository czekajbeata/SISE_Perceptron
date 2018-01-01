#include "stdafx.h"
#include "gnuplot_i.hpp"
#include "GnuplotManager.h"

void GnuplotManager::configure()
{
	main_plot->set_title(title);
	main_plot->set_xlabel(xLabel);
	main_plot->set_ylabel(yLabel);

	// Style
	main_plot->set_style(style);

	// Grid
	if (gridOn)
		main_plot->set_grid();
}

GnuplotManager::GnuplotManager()
{	
	Gnuplot::set_GNUPlotPath(GNUPLOT_PATH);
	main_plot = new Gnuplot();
	title = "title";
	xLabel = "x";
	yLabel = "y";
	style = "points";
	gridOn = true;
}

void GnuplotManager::plotErrorFile() {

	setXLabel("Liczba epok");
	setYLabel("Blad");
	setTitle("Wykres bledu");
	setStyle("lines");

	configure();

	//main_plot->set_yrange(-0.1, 0.5);
	main_plot->plotfile_xy("errors.data");
	while (!cin.get());
	cin.get();
}

void GnuplotManager::plotOutputsFile() {

	setXLabel("Liczba epok");
	setYLabel("Ilosc dopasowanych");
	setTitle("Wykres dopasowan");
	setStyle("points");

	configure();

	//main_plot->set_yrange(-0.1, 0.5);
	main_plot->plotfile_xy("outputs.data");
	while (!cin.get());
	cin.get();
}

#pragma region Plot Configuration Methods
void GnuplotManager::setTitle(string text)
{
	title = text;
}

void GnuplotManager::setXLabel(string text)
{
	xLabel = text;
}

void GnuplotManager::setYLabel(string text)
{
	yLabel = text;
}

void GnuplotManager::setRange(double min, double max)
{
	main_plot->set_xrange(min, max);
	main_plot->set_yrange(min, max);
}

void GnuplotManager::setStyle(string text)
{
	if (style == "lines" || style == "points")
		style = text;
}

void GnuplotManager::setGridOn(bool on)
{
	gridOn = on;
}
#pragma endregion

