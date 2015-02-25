#include <iostream>
#include "layer.h"

Layer::Layer () {
	//Default constructor
	topborder=0.1; 
	bottomborder=0.0; 
}

double Layer::getTop() {
	return topborder;
}

double Layer::getBottom() {
	return bottomborder;
}

Layer::Layer(double border1, double border2) {
	//Constructor
	if (border1 > border2) {
		topborder = border1;
		bottomborder = border2;
	} else if (border2 > border1) {
		topborder = border2;
		bottomborder = border1;
	}
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