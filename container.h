#ifndef CONTAINER_H
#define CONTAINER_H

// To use vector template.
#include <vector>
// For max_element
#include <algorithm>
// To print out the current time and date
#include <ctime>
// For strings
#include <string>

#include "sphere.h"
#include "layer.h"

class Container 
{
private:
    double m_xTop;
    double m_xBottom;
    double m_yTop;
    double m_yBottom;
    double m_zTop;
    double m_zBottom;
    char m_direction;
    // A set of chars to hold the boundaries in x, y, and z direction.
    char m_boundx;    // Boundary condition in x
    char m_boundy;    // Boundary condition in y
    char m_boundz;    // Boundary condition in z
    // Wrap treatment is set to [i]gnore by default
    char m_wraptreatment = 'n'; 
    // A vector to hold all spheres of the experiment
    std::vector<Sphere> spheres;
    std::vector<Sphere> spheresx;
    std::vector<Sphere> spheresy;
    std::vector<Sphere> spheresz;
    // A vector to hold all layers of the experiment
    std::vector<Layer> layers;
    // A vector that will hold all unique radii of the spheres
    std::vector<double> uniqueRadii;
public:
    Container();
    // Default constructor
    Container(char direction, 
            double x1, double x2, char boundx, 
            double y1, double y2, char boundy, 
            double z1, double z2, char boundz);
    // Constructor with given boundaries
    void addSphere(Sphere newsphere);
    // A method to add a new sphere to the sphere vector
    void addLayer(Layer newlayer);
    // A method to add a new layer to the layer vector
    double getTop(char direction);
    // A method to return the top value of a given direction x, y, or z
    double getBottom(char direction);
    // A method to return the bottom value of a given direction x, y, or z
    char getBoundary(char direction);
    // A method to set the wrap treatment option
    void setWraptreatment(char wraptreatment);
    // A method to return the boundary condition of a given direction.
    void prepareWrap(int numberOfExtraLayers, double layersize);
    // A method to prepare the layers vector for extra-layer overlap treatment
    void createInfoVectors(bool routput);
    // A method to count Spheres and create the info Arrays inside of all
    // spheres and layer
    void writeOutputFile(char* outputFileName, char* infiledirectory, char outputOption);
    // A method to write the output
    int whereIsUniqueRadius(double Radius);
    // A method to find the position of a Radius inside the uniqueRadii vektor
};

#endif