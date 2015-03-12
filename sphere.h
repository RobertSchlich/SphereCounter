#ifndef SPHERE_H
#define SPHERE_H

#include <vector>
// To use vector template.
#include "layer.h"

class Sphere 
{
private:
	static constexpr double PI = 3.14159265358979323846; 
	int m_id;
	double m_x;
	double m_y;
	double m_z;
	double m_radius;
	std::vector<double> percentInEachLayer;
	// A vector to hold information about what percentage of the sphere is 
	// located inside of each layer. The number of values stored is to be equal
	// to the number of layers defined in the container.
	// A double chechsum, to count the percentage of the sphere contained in ALL
	// of the layers. Because user defined layers may overlap, the checksum may 
	// have values > 1. Also if the LIGGGHTS run was set to have periodic 
	// boundaries and a sphere extends over the edge of the experiments its 
	// checksum may be < 1.
public:
	Sphere();	
	// Default constructor
	Sphere(int id, double x, double y, double z, double radius);
	void setSphere(int id, double x, double y, double z, double radius);
	// A method to 'set up' a sphere after it has been constructed
	double getChecksum();
	double getRadius();
	double getVolume();
	int getID();
	double percentinlayer(Layer layer);
	void setPercentInLayer(double percentInThisLayer);
	double getPercentInLayer(int numberOfLayer);
};

#endif
