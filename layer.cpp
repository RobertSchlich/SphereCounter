#include "layer.h"
#include <iostream>

Layer::Layer () {
	// Default constructor
	topborder = 0.1; 
	bottomborder = 0.0; 
	direction = 'z';
}

Layer::Layer(double border1, double border2, char direc) {
	// Constructor
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

double Layer::getTop() {
	return topborder;
}

double Layer::getBottom() {
	return bottomborder;
}

void Layer::setBorders(double border1, double border2) {
	if (border1 > border2) {
		topborder = border1;
		bottomborder = border2;
	} else if (border2 > border1) {
		topborder = border2;
		bottomborder = border1;
	}
}

char Layer::getDirection(){
	return direction;
}