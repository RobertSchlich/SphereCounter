#ifndef LAYER_H
#define LAYER_H

class Layer {
private:
	double topborder;
	double bottomborder;
public:
	Layer();	//Default constructor
	Layer(double border1, double border2); //Constructor
	double getTop();
	double getBottom();
	void setBorders(double border1, double border2);	//Method to set borders
	//after construction just for the heck of it
};

#endif