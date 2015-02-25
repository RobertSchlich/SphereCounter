#ifndef SPHERE_H
#define SPHERE_H
#include "layer.h"

class Sphere {
private:
	static const double PI = 3.14159265358979323846; 
	int m_id;
	double m_x;
	double m_y;
	double m_z;
	double m_radius;
	double m_volume() {return PI * 4/3 * m_radius * m_radius * m_radius;}
public:
	Sphere();	//Default constructor
	Sphere(int id, double x, double y, double z, double radius);
	void setSphere(int id, double x, double y, double z, double radius);
	double getRadius();
	double getVolume();
	int getID();
	double percentinlayer(Layer layer);
};

#endif
