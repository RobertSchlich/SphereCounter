#include "container.h"

#include <iostream>
#include <algorithm>
#include <iomanip>		//IO Manipulation
#include <fstream>		//Reading/Writing in Files

#include "auxfunc.h"

// Standard constructor
Container::Container() {
	m_xTop = 0;
	m_xBottom = 0;
	m_yTop = 0;
	m_yBottom = 0;
	m_zTop = 0;
	m_zBottom = 0;
}

// Constructor
Container::Container(double x1, double x2, double y1, double y2, 
					 double z1, double z2) {
	if (x1 > x2) {
		m_xTop = x1;
		m_xBottom = x2;
	} else {
		m_xTop = x2;
		m_xBottom = x1;
	}
	if (y1 > y2) {
		m_yTop = y1;
		m_yBottom = y2;
	} else {
		m_yTop = y2;
		m_yBottom = y1;
	}
	if (z1 > z2) {
		m_zTop = z1;
		m_zBottom = z2;
	} else {
		m_zTop = z2;
		m_zBottom = z1;
	}
	
	std::cout << "Created a new container." << std::endl
		<< "  x: " << m_xBottom << "-" << m_xTop << std::endl
		<< "  y: " << m_yBottom << "-" << m_yTop << std::endl
		<< "  z: " << m_zBottom << "-" << m_zTop << std::endl;
}

// A method to add a sphere to the sphere vector
void Container::addSphere(Sphere newsphere) {
	spheres.push_back(newsphere);
	// Check if the radius of the sphere is already registered in 
	// the uniqueradii vector
	bool isnewradius = true;
	// First check if uniqueRadii has elements
	if (uniqueRadii.begin() != uniqueRadii.end()) { 
		// If uniqueRadii has elements, iterate over all of them
		for(auto it = uniqueRadii.begin(); it != uniqueRadii.end(); ++it) {
			// If the radius of the current sphere matches an element of 
			// uniqueRadii the radius was allready registered
			// The current radius does not need to be registered and the 
			// iteration can stop.
			if ( newsphere.getRadius() == *it){
				isnewradius = false;
				break;
			}
		}
	} 
	// If it is a new radius, write it to the uniqueRadii vektor
	if (isnewradius == true) {
		uniqueRadii.push_back(newsphere.getRadius());
		// While we're at it, why not sort the uniqueRadii vektor
		// to ensure it's sorted all the time
		std::sort (uniqueRadii.begin(), uniqueRadii.end());
	}
	
}

// A method to add a layer to the layer vector
void Container::addLayer(Layer newlayer){
	layers.push_back(newlayer);
}

// A method to get the top boundary of the experiment for the desired direciton
double Container::getTop(char direction){
	if (direction = 'x')
		return m_xTop;
	else if (direction = 'y')
		return m_yTop;
	else if (direction = 'z')
		return m_zTop;
	else 
		std::cerr << "Invalid direction in call for Container::getTop: '" 
			<< direction << "'" << std::endl
			<< "Valid diretions are 'x','y', or 'z'!";
}

// A method to get the bottom boundary of the experiment for the desired 
// direction
double Container::getBottom(char direction){
	if (direction = 'x')
		return m_xBottom;
	else if (direction = 'y')
		return m_yBottom;
	else if (direction = 'z')
		return m_zBottom;
	else 
		std::cerr << "Invalid direction in call for Container::getBottom: '" 
			<< direction << "'" << std::endl
			<< "Valid diretions are 'x','y', or 'z'!";
}

void Container::writeOutputFile(char* outfiledirectory) {
	/* -----------------------------------------------------------------
	   Layer-wise counting of the spheres
	----------------------------------------------------------------- */
	
	double layerinfoarray [uniqueRadii.size()] [layers.size()];
	double sphereinfoarray [spheres.size()] [layers.size()+2];
	
	for(int m = 0; m < spheres.size(); m++) {
		
		sphereinfoarray[m][0]=spheres[m].getVolume();
		double rowsumofspheres = 0;
		
		for(int n = 0; n < layers.size(); n++) {
				
			double percentinlayer = spheres[m].percentinlayer(layers[n]);
			int radiusposition = whereIsUniqueRadius( spheres[m].getRadius());
			layerinfoarray[radiusposition][n] += percentinlayer;
			sphereinfoarray[m][n+1] = percentinlayer;
			rowsumofspheres += percentinlayer;
			
		}
		
		sphereinfoarray[m][layers.size()+1] = rowsumofspheres;
		
	}
	std::cout.setf(std::ios::right);
	int width = 16;
	
	/* -----------------------------------------------------------------
	   Writing the output file
	----------------------------------------------------------------- */

	std::ofstream outfile(outfiledirectory);
	outfile.precision(10);
	
	outfile << "Spheres were counted in the " << layers[0].getDirection() 
		<< "-direction." << std::endl << std::endl;
	
	//Write a describtion of the layers to the outputfile
	for (int i = 0; i < layers.size(); i++) {
		outfile << "Layer #" << i+1 << ":" << std::endl;
		outfile << "\tBottom border:\t" << layers[i].getBottom() << std::endl;
		outfile << "\tTop border:\t\t" << layers[i].getTop() << std::endl;
	}
	outfile << std::endl;
	
	//Write the layerinfoarray
	outfile << "Layerinfoarray:" << std::endl;
	outfile << "---------------" << std::endl;
	outfile << std::setw(8) << "Radius";
	for (int i = 0; i < layers.size(); i++) 
	{
		outfile << std::setw(width-1) << "#K in Schicht" << i+1;
	}
	outfile << std::endl;
	outfile << std::setw(8+(layers.size())*width) << std::setfill('-') << "--" 
			<< std::endl << std::setfill(' ');
	for (int i = 0;  i < uniqueRadii.size(); i++) 
	{
		outfile << std::setw(8) <<uniqueRadii[i];
		for (int j = 0; j < layers.size(); j++) 
		{
			outfile << std::setw(width) << layerinfoarray[i][j];
		}
		outfile << std::endl;
	}
	outfile << std::endl << std::endl;
	
	//Write the sphereinfoarray
	outfile << "Sphereinfoarray: " << std::endl;
	outfile << "----------------" << std::endl;
	outfile << std::setw(6) << "Nr:";
	outfile << setiosflags(std::ios::fixed) << std::setw(width+1) << "Volumen:";
	for (int i = 0; i< layers.size(); i++) {
		outfile << std::setw(width-2) << "% Schicht " << i+1 << ":";
	}
	outfile << setiosflags(std::ios::fixed) << std::setw(width) << "Prüfsumme:";
	outfile << std::endl;
	outfile << std::setw(6+(layers.size()+2)*width) << std::setfill('-') << "--" 
			<< std::endl << std::setfill(' ');
	for (int i = 0; i < spheres.size(); i++) {
		outfile << setiosflags(std::ios::fixed) << std::setw(6) <<spheres[i].getID();
		for (int j = 0; j < layers.size() + 2 ; j++) {
			outfile << setiosflags(std::ios::fixed) << std::setw(width) 
					<< sphereinfoarray[i][j];
		}
		outfile << std::endl;
		outfile.flush();
	}
	
	outfile.close();
}

int Container::whereIsUniqueRadius(double searchedRadius) {
	for (int i =  0;  i < uniqueRadii.size(); ++i) {
		if (searchedRadius == uniqueRadii[i]) {
			return i;
			break;
		}	
	}
}

