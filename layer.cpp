#include "layer.h"
#include <iostream>

Layer::Layer () 
// Default constructor
{
    topborder = 0.1; 
    bottomborder = 0.0; 
}

Layer::Layer(double border1, double border2) 
// Constructor
{
    if (border1 > border2) {
        topborder = border1;
        bottomborder = border2;
    } else if (border2 > border1) {
        topborder = border2;
        bottomborder = border1;
    }
}
void tryToModifyArray( const int b[], int x )
{
}

double Layer::getTop() const
{
    return topborder;
}

double Layer::getBottom() const
{
    return bottomborder;
}

double Layer::getSize() const
{
    return topborder - bottomborder;
}

double Layer::calculatePorosity(const std::vector<double>& uniqueRadii, double length, double width, char direction)
{
    double totalVol = length * width * (topborder - bottomborder);
    double solidVol = 0;
    for (int i = 0; i < uniqueRadii.size(); i++)
    {
        if (direction == 'z')
        {
            solidVol +=  PI * 4/3 * uniqueRadii[i] * uniqueRadii[i] * uniqueRadii[i] * numberOfSpheresInsideZ[i];
        } 
        else if (direction == 'x')
        {
            solidVol +=  PI * 4/3 * uniqueRadii[i] * uniqueRadii[i] * uniqueRadii[i] * numberOfSpheresInsideX[i];
        } 
        else
        {
            solidVol +=  PI * 4/3 * uniqueRadii[i] * uniqueRadii[i] * uniqueRadii[i] * numberOfSpheresInsideY[i];
        } 
        
    }
    porosity = ( totalVol - solidVol ) / totalVol;
}

double Layer::getPorosity() const
{
    return porosity;
}

void Layer::initSphereVector(int numberOfUniqueRadii, char direction)
// This method initializes the numberOfSpheresInside vector, so that it has
// one entry for every sphere radius that appears in the experiment.
{
    if (direction == 'z')
    {
        numberOfSpheresInsideZ.reserve(numberOfUniqueRadii);
        for (int i=0; i< numberOfUniqueRadii; ++i)
        {
            numberOfSpheresInsideZ[i]=0;
        }
    } 
    else if (direction == 'x')
    {
        numberOfSpheresInsideX.reserve(numberOfUniqueRadii);
        for (int i=0; i< numberOfUniqueRadii; ++i)
        {
            numberOfSpheresInsideX[i]=0;
        }
    } else
    {
        numberOfSpheresInsideY.reserve(numberOfUniqueRadii);
        for (int i=0; i< numberOfUniqueRadii; ++i)
        {
            numberOfSpheresInsideY[i]=0;
        }
    } 
}

void Layer::addSphere(double percentInLayer, int positionOfThisRadius, char direction) 
// This method adds the percentage of one sphere to the numberOfSpheresInside
// vector. The percentage is added at the position given by positionOfThisRadius
{
    // Add the percentage of the currently considered sphere at the accoring
    // position
    if (direction == 'z')
    {
        numberOfSpheresInsideZ[positionOfThisRadius] += percentInLayer;
    } 
    else if (direction == 'x')
    {
        numberOfSpheresInsideX[positionOfThisRadius] += percentInLayer;
    } 
    else
    {
        numberOfSpheresInsideY[positionOfThisRadius] += percentInLayer;
    } 
}

double Layer::getNumberOfSpheres(int radiusPosition, char direction) 
// Returns the number of spheres with the radius at radiusPosition
{
    if (direction == 'z')
    {
        return numberOfSpheresInsideZ[radiusPosition];
    } 
    else if (direction == 'x')
    {
        return numberOfSpheresInsideX[radiusPosition];
    } 
    else
    {
        return numberOfSpheresInsideY[radiusPosition];
    } 
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

int Layer::getNumberOfUniqueRadii(char direction) 
{
    if (direction == 'z')
    {
        return numberOfSpheresInsideZ.size();
    } 
    else if (direction == 'x')
    {
        return numberOfSpheresInsideX.size();
    } 
    else
    {
        return numberOfSpheresInsideY.size();
    } 
}
