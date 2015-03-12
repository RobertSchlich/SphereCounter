#include "layer.h"
#include <iostream>

Layer::Layer () 
// Default constructor
{
	topborder = 0.1; 
	bottomborder = 0.0; 
	direction = 'z';
}

Layer::Layer(double border1, double border2, char direc) 
// Constructor
{
	if (direc == 'x' || direc == 'y' || direc == 'z')
		direction = direc;
	else
		std::cerr << "Error in layer constructor:" << std::endl 
			<< "The given layer direction is invalid!" << std::endl
			<< "Only 'x', 'y', and 'z' are valid directions.";
	
	if (border1 > border2) {
		topborder = border1;
		bottomborder = border2;
	} else if (border2 > border1) {
		topborder = border2;
		bottomborder = border1;
	}
	direction = direc;
}
void tryToModifyArray( const int b[], int x )
{
}

double Layer::getTop() 
{
	return topborder;
}

double Layer::getBottom() 
{
	return bottomborder;
}

void Layer::initSphereVector(int numberOfUniqueRadii)
// This method initializes the numberOfSpheresInside vector, so that it has
// one entry for every sphere radius that appears in the experiment.
{
	numberOfSpheresInside.reserve(numberOfUniqueRadii);
	for (int i=0; i< numberOfUniqueRadii; ++i)
	{
		numberOfSpheresInside[i]=0;
	}
}

void Layer::addSphere(double percentInLayer, int positionOfThisRadius) 
// This method adds the percentage of one sphere to the numberOfSpheresInside
// vector. The percentage is added at the position given by positionOfThisRadius
{
	// Add the percentage of the currently considered sphere at the accoring
	// position
	numberOfSpheresInside[positionOfThisRadius] += percentInLayer;
}

double Layer::getNumberOfSpheres(int radiusPosition) 
// Returns the number of spheres with the radius at radiusPosition
{
	return numberOfSpheresInside[radiusPosition];
}

void Layer::setBorders(double border1, double border2) 
{
	if (border1 > border2) {
		topborder = border1;
		bottomborder = border2;
	} else if (border2 > border1) {
		topborder = border2;
		bottomborder = border1;
	}
}

char Layer::getDirection()
{
	return direction;
}