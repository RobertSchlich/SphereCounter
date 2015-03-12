#ifndef LAYER_H
#define LAYER_H

#include <vector>
// To use vector template.
class Layer 
{
private:
	double topborder;
	double bottomborder;
	char direction;
	std::vector<double> numberOfSpheresInside;
public:
	Layer();
	// Default constructor
	Layer(double border1, double border2, char direc);
	// Constructor
	double getTop();
	double getBottom();
	void initSphereVector(int numberOfUniqueradii);
	// Initializes the numberOfSpheresInside vector
	void addSphere(double percentInLayer, int positionOfThisRadius);
	// Adds a sphere to the numberOfSpheresInside vector
	double getNumberOfSpheres(int radiusPosition);
	void setBorders(double border1, double border2);	
	// Method to set borders after construction just for the heck of it
	void setDirection(char direc);
	char getDirection();
};

#endif