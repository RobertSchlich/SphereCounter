#ifndef CONTAINER_H
#define CONTAINER_H

#include <vector>
// To use vector template.
#include "sphere.h"
#include "layer.h"

class Container 
{
private:
	double m_xTop;
	double m_xBottom;
	double m_yTop;
	double m_yBottom;	
	double m_zTop;
	double m_zBottom;
	// A set of doubles to hold the boundaries in x, y, and z direction.
	std::vector<Sphere> spheres;
	// A vector to hold all spheres of the experiment
	std::vector<Layer> layers;
	// A vector to hold all layers of the experiment
	std::vector<double> uniqueRadii;
	// A vector that will hold all unique radii of the spheres
public:
	Container();
	// Default constructor
	Container(double xTop, double xBottom, double yTop, 
			  double yBottom, double zTop, double zBottom);
	// Constructor with given boundaries
	void addSphere(Sphere newsphere);
	// A method to add a new sphere to the sphere vector
	void addLayer(Layer newlayer);
	// A method to add a new layer to the layer vector
	double getTop(char direction);
	// A method to return the top value of a given direction x, y, or z
	double getBottom(char direction);
	// A method to return the bottom value of a given direction x, y, or z
	void createInfoVectors();
	// A method to count Spheres and create the info Arrays inside of all
	// spheres and layer
	void writeOutputFile(char* outputFileName);
	// A method to write the output
	int whereIsUniqueRadius(double Radius);
	// A method to find the position of a Radius inside the uniqueRadii vektor
};

#endif