#ifndef CONTAINER_H
#define CONTAINER_H

#include <vector>	// To use vector template.
#include "sphere.h"
#include "layer.h"

class Container {
private:
	// A set of doubles to hold the boundaries in x, y, and z direction.
	double m_xTop;
	double m_xBottom;
	double m_yTop;
	double m_yBottom;	
	double m_zTop;
	double m_zBottom;
	// A vector to ultimately hold all spheres of the experiment
	std::vector<Sphere> spheres;
	// A vector to ultimately hold all layers of the experiment
	std::vector<Layer> layers;
	// A vector that will ultimately hold all unique radii of the spheres
	std::vector<double> uniqueRadii;
public:
	// Default constructor
	Container();
	// Constructor with given boundaries
	Container(double xTop, double xBottom, double yTop, 
			  double yBottom, double zTop, double zBottom);
	// A method to add a new sphere to the sphere vector
	void addSphere(Sphere newsphere);	
	// A method to add a new layer to the layer vector
	void addLayer(Layer newlayer);
	// A method to return the top value of a given direction x, y, or z
	double getTop(char direction);
	// A method to return the bottom value of a given direction x, y, or z
	double getBottom(char direction);
	// A method to write the output
	void writeOutputFile(char* outputFileName);
	// A method to find the position of a Radius inside the uniqueRadii vektor
	int whereIsUniqueRadius(double Radius);
};

#endif