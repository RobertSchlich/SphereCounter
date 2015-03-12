#include "container.h"

#include <iostream>
#include <algorithm>
#include <iomanip>
// IO Manipulation
#include <fstream>
// Reading/Writing to/from Files

#include "auxfunc.h"

Container::Container() 
// Standard constructor
{
	m_xTop = 0;
	m_xBottom = 0;
	m_yTop = 0;
	m_yBottom = 0;
	m_zTop = 0;
	m_zBottom = 0;
}

Container::Container(double x1, double x2, double y1, double y2, 
					 double z1, double z2) 
// Constructor
{
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
	
	std::cout << "Read box-boundaries from inputfile:" << std::endl
		<< "  x: " << m_xBottom << "-" << m_xTop << std::endl
		<< "  y: " << m_yBottom << "-" << m_yTop << std::endl
		<< "  z: " << m_zBottom << "-" << m_zTop << std::endl << std::endl;
}

void Container::addSphere(Sphere newsphere) 
// A method to add a sphere to the sphere vector
{
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

void Container::addLayer(Layer newlayer)
// A method to add a layer to the layer vector
{
	layers.push_back(newlayer);
}

double Container::getTop(char direction)
// A method to get the top boundary of the experiment for the desired direciton
{
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

double Container::getBottom(char direction)
// A method to get the bottom boundary of the experiment for the desired 
// direction
{
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

void Container::createInfoVectors()
// A method that should create the info arrays inside each sphere and each layer
// contained in the container.
// This is where the magic happens! :)
{
	int numberOfUniqueRadii = uniqueRadii.size();
	//First initialize the SphereVector inside of every layer
	for(int l = 0; l < layers.size(); ++l) 
	{
		layers[l].initSphereVector(numberOfUniqueRadii);
	}
	
	// Iterate through all spheres with Integer 'm'
	for(int m = 0; m < spheres.size(); ++m) 
	{
		
		// Iterate through all layers with Integer 'n'
		for(int n = 0; n < layers.size(); ++n) 
		{
			
			// Check what percentage of the current sphere(m) is in the current 
			// layer (n)
			double percentinlayer = spheres[m].percentinlayer(layers[n]);
			// Find the position of the radius of the sphere inside the
			// uniqueRadii vector
			int radiusPosition = whereIsUniqueRadius( spheres[m].getRadius());
			
			spheres[m].setPercentInLayer(percentinlayer);
			layers[n].addSphere(percentinlayer, radiusPosition);
		}
	}
}

void Container::writeOutputFile(char* outfiledirectory) 
{
	// Variables for output formating
	std::cout.setf(std::ios::right);
	int width = 16;
	
	std::ofstream outfile(outfiledirectory);
	outfile.precision(10);
	
	outfile << "Spheres were counted in the " << layers[0].getDirection() 
		<< "-direction." << std::endl << std::endl;
	
	// Write a describtion of the layers to the outputfile
	for (int i = 0; i < layers.size(); i++) 
	{
		outfile << "Layer #" << i+1 << ":" << std::endl;
		outfile << "\tBottom border:\t" << layers[i].getBottom() << std::endl;
		outfile << "\tTop border:\t\t" << layers[i].getTop() << std::endl;
	}
	outfile << std::endl;
	
	
	/* -------------------------------------------------------------------------
		For every layer , write how many spheres of each radius are inside
	--------------------------------------------------------------------------*/
	
	outfile << "Information about the layers:" << std::endl;
	outfile << "-----------------------------" << std::endl;
	outfile << std::setw(8) << "Radius";
	// Write the header of the Table
	for (int i = 0; i < layers.size(); i++) 
	{
		outfile << std::setw(width-1) << "# in layer " << i+1;
	}
	outfile << std::setw(width-1) << "Total ";
	outfile << std::endl;
	// Draw a line with '-'
	outfile << std::setw(8+(layers.size()+1)*width) << std::setfill('-') << "--" 
			<< std::endl << std::setfill(' ');

	// Write to file the number of Spheres inside each layer sorted by
	// unique radius of the spheres
	for (int i = 0;  i < uniqueRadii.size(); i++) 
	{
		double totalNumber = 0;
		outfile << std::setw(8) << uniqueRadii[i];
		for (int j = 0; j < layers.size(); j++) 
		{
			outfile << std::setw(width) << layers[j].getNumberOfSpheres(i);
			totalNumber += layers[j].getNumberOfSpheres(i);
		}
		outfile << std::setw(width) << totalNumber;
		outfile << std::endl;
	}
	outfile << std::endl << std::endl;
	
	
	/* -------------------------------------------------------------------------
		For every sphere, write to what percentage they are in which layer
	--------------------------------------------------------------------------*/
	
	outfile << "Information about the spheres:" << std::endl;
	outfile << "------------------------------" << std::endl;
	// Write the header of the table
	outfile << std::setw(6) << "Nr:";
	outfile << std::setiosflags(std::ios::fixed) << std::setw(width+1) 
			<< "Volume:";
	for (int i = 0; i< layers.size(); i++) {
		outfile << std::setw(width-2) << "% in layer " << i+1 << ":";
	}
	outfile << std::setiosflags(std::ios::fixed) << std::setw(width) 
			<< "Checksum:";
	outfile << std::endl;
	// Draw a line of '-'
	outfile << std::setw(6+(layers.size()+2)*width) << std::setfill('-') << "--" 
			<< std::endl << std::setfill(' ');
	// Start writing information for each sphere
	for (int i = 0; i < spheres.size(); i++) {
		// Write id and volume of sphere [i]
		outfile << std::setiosflags(std::ios::fixed) 
				<< std::setw(6) << spheres[i].getID() 
				<< std::setw(width) << spheres[i].getVolume();
		for (int j = 0; j < layers.size(); j++) {
			// Write what percentage of sphere[i] is inside layer [j]
			outfile << std::setiosflags(std::ios::fixed) << std::setw(width) 
					<< spheres[i].getPercentInLayer(j);
		}
		// Write the checksum to the outfile
		outfile << std::setw(width) << spheres[i].getChecksum();
		outfile << std::endl;
		outfile.flush();
	}
	
	outfile.close();
}

int Container::whereIsUniqueRadius(double searchedRadius) 
{
	for (int i =  0;  i < uniqueRadii.size(); ++i) {
		if (searchedRadius == uniqueRadii[i]) {
			return i;
			break;
		}	
	}
}


