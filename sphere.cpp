#include <iostream>
#include <cmath>  
#include "sphere.h"

Sphere::Sphere() {
	//Default constructor
	m_id = 0;
	m_x = 0;
	m_y = 0;
	m_z = 0;
	m_radius =0;
}

Sphere::Sphere(int id, double x, double y, double z, double radius) {
	//Constructor
	if (id < 0) {
		std::cerr << "Negative IDs are not allowed;"
			<< " ID set to '0'! Please revise your input.";
		m_id = 0;
	} else 
		m_id = id;

	if (x < 0) {
		std::cerr << "Negative x values are not allowed;"
			<< " x set to '0'! Please revise your input.";
		m_x = 0;
	} else
		m_x = x;

	if (y < 0) {
		std::cerr << "Negative < values are not allowed;"
			<< " y set to '0'! Please revise your input.";
		m_y = 0;
	} else
		m_y = y;

	if (z < 0) {
		std::cerr << "Negative z values are not allowed;"
			<< " z set to '0'! Please revise your input.";
		m_z = 0;
	} else
		m_z = z;

	if (radius < 0) {
		std::cerr << "Negative radii are not allowed;"
			<< " radius set to '0'! Please revise your input.";
		m_radius = 0;
	} else
		m_radius = radius;
}

void Sphere::setSphere(int id, double x, double y, double z, double radius) {
	// Method to 'set up' a sphere after it has been constructed
	if (id < 0) {
		std::cerr << "Negative IDs are not allowed;"
			<< " ID set to '0'! Please revise your input.";
		m_id = 0;
	} else 
		m_id = id;

	if (x < 0) {
		std::cerr << "Negative x values are not allowed;"
			<< " x set to '0'! Please revise your input.";
		m_x = 0;
	} else
		m_x = x;

	if (y < 0) {
		std::cerr << "Negative < values are not allowed;"
			<< " y set to '0'! Please revise your input.";
		m_y = 0;
	} else
		m_y = y;

	if (z < 0) {
		std::cerr << "Negative z values are not allowed;"
			<< " z set to '0'! Please revise your input.";
		m_z = 0;
	} else
		m_z = z;

	if (radius < 0) {
		std::cerr << "Negative radii are not allowed;"
			<< " radius set to '0'! Please revise your input.";
		m_radius = 0;
	} else
		m_radius = radius;
}

double Sphere::getRadius(){
	return m_radius;
}

double Sphere::getVolume(){
	return m_volume();
}

int Sphere::getID(){
	return m_id;
}

double Sphere::percentinlayer(Layer layer){
// Method to check how many percent of a Sphere are inside the layer given as
// argument
	
	double deltatop;
	double deltabottom;
	
	// Check in which direction the layer is oriented and calculate
	// deltatop and deltabottom accordingly
	if (layer.getDirection() == 'z'){
		deltatop = layer.getTop() - m_z;
		deltabottom = layer.getBottom() - m_z;
	} else if (layer.getDirection() == 'x'){
		deltatop = layer.getTop() - m_x;
		deltabottom = layer.getBottom() - m_x;
	} else {
		deltatop = layer.getTop() - m_y;
		deltabottom = layer.getBottom() - m_y;
	}
		
	if (deltatop < 0) { 
		// The sphere is above the layer
		if (fabs(deltatop) > m_radius) { 
			// The sphere is not cut
			return 0;		
		} else if(fabs(deltabottom) < m_radius) { 
			// The sphere is cut twice
			double htot = m_radius + deltatop;
			double hout = m_radius + deltabottom;
			return 0.25 * ( ( 3 * ( htot * htot - hout * hout ) ) / 
				( m_radius * m_radius ) - 
				( htot * htot * htot - hout * hout * hout ) / 
				( m_radius * m_radius * m_radius ) ); 
		} else { 
			// The sphere is cut once
			double hin = m_radius + deltatop;
			return 0.25 * hin * hin * ( 3 / ( m_radius * m_radius ) - hin / 
			( m_radius * m_radius * m_radius ) );
		}
	} else if (deltabottom > 0) { 
		// The sphere is below the layer
		if (deltabottom > m_radius) { 
			// The sphere is not cut
			return 0;
		} else if (deltatop < m_radius) { 
			// The sphere is cut twice
			double htot = m_radius - deltabottom;
			double hout = m_radius - deltatop;
			return 0.25 * ( ( 3 * ( htot * htot - hout * hout ) ) / 
				( m_radius * m_radius ) - 
				( htot * htot * htot - hout * hout * hout ) / 
				( m_radius * m_radius * m_radius ) ); 
		} else { 
			// The sphere is cut once
			double hin = m_radius - deltabottom;
			return 0.25 * hin * hin * ( 3 / ( m_radius * m_radius ) - 
			hin / ( m_radius * m_radius * m_radius ) );
		}
	} else { 
		// The Sphere is inside the layer
		if (deltatop < m_radius) { 
			// The sphere is cut at the top
			if (fabs(deltabottom) < m_radius) { 
				// The sphere is cut at the top and at the bottom
				double htot = m_radius + deltatop;
				double hout = m_radius + deltabottom;
				return 0.25 * ( ( 3 * ( htot * htot - hout * hout ) ) / 
					( m_radius * m_radius ) - 
					( htot * htot * htot - hout * hout * hout ) / 
					( m_radius * m_radius * m_radius ) ); 
			} else {				
				// The sphere is only cut at the top
				double hin = m_radius + deltatop;
				return 0.25 * hin * hin * ( 3 / ( m_radius * m_radius ) 
				- hin / ( m_radius * m_radius * m_radius ) );	
			}
		} else  { 
			// The Sphere is not cut at the top
			if (fabs(deltabottom) < m_radius) { 
				// The Sphere is cut at the bottom
				double hin = m_radius - deltabottom;
				return 0.25 * hin * hin * ( 3 / ( m_radius * m_radius ) - 
				hin / ( m_radius * m_radius * m_radius ) );
			} else 
				// The sphere is neither cut at the top nor at the bottom
				return 1;
		}
	}
}