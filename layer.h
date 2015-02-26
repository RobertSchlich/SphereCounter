#ifndef LAYER_H
#define LAYER_H

class Layer {
private:
	double topborder;
	double bottomborder;
	char direction;
public:
	Layer();	//Default constructor
	Layer(double border1, double border2, char direc); //Constructor
	double getTop();
	double getBottom();
	void setBorders(double border1, double border2);	//Method to set borders
	//after construction just for the heck of it
	void setDirection(char direc);
	char getDirection();
};

#endif