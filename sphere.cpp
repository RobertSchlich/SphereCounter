#include <iostream>
#include <cmath>  
#include "sphere.h"

Sphere::Sphere() 
// Default constructor
{
    m_id = 0;
    m_x = 0;
    m_y = 0;
    m_z = 0;
    m_radius =0;
}

Sphere::Sphere(int id, double x, double y, double z, double radius) 
// Constructor
{
    if (id < 0) 
    {
        std::cerr << "Negative IDs are not allowed;"
            << " ID set to '0'! Please revise your input.";
        m_id = 0;
    } else 
        m_id = id;

        m_x = x;

        m_y = y;

        m_z = z;

    if (radius < 0) 
    {
        std::cerr << "Negative radii are not allowed;"
            << " radius set to '0'! Please revise your input.";
        m_radius = 0;
    } else
        m_radius = radius;
}

void Sphere::setSphere(int id, double x, double y, double z, double radius) 
{
    if (id < 0) 
    {
        std::cerr << "Negative IDs are not allowed;"
            << " ID set to '0'! Please revise your input.";
        m_id = 0;
    } else 
        m_id = id;

        m_x = x;

        m_y = y;

        m_z = z;

    if (radius < 0) 
    {
        std::cerr << "Negative radii are not allowed;"
            << " radius set to '0'! Please revise your input.";
        m_radius = 0;
    } else
        m_radius = radius;
}

double Sphere::getChecksum()
// Returns the sum of the percentages to which the sphere is inside of all of 
// the layers.
{
    double checksum = 0;
    for (int i = 0; i < percentInEachLayer.size(); ++i) 
    {
        checksum += percentInEachLayer[i];
    }
    return checksum;
}

double Sphere::getRadius()
{
    return m_radius;
}

double Sphere::getVolume()
{
    return PI * 4/3 * m_radius * m_radius * m_radius;
}

int Sphere::getID()
{
    return m_id;
}

double Sphere::percentinlayer(const Layer &layer, char direction)
// Method to check how many percent of a Sphere are inside the layer given as
// argument
{
    double deltatop;
    double deltabottom;
    
    // Check in which direction the layer is oriented and calculate
    // deltatop and deltabottom accordingly
    if (direction == 'z')
    {
        deltatop = layer.getTop() - m_z;
        deltabottom = layer.getBottom() - m_z;
    } else if (direction == 'x')
    {
        deltatop = layer.getTop() - m_x;
        deltabottom = layer.getBottom() - m_x;
    } else 
    {
        deltatop = layer.getTop() - m_y;
        deltabottom = layer.getBottom() - m_y;
    }
        
    if (deltatop < 0) 
    { 
        // The sphere is above the layer
        if (fabs(deltatop) > m_radius) 
        { 
            // The sphere is not cut
            return 0;        
        } else if(fabs(deltabottom) < m_radius) 
        { 
            // The sphere is cut twice
            double htot = m_radius + deltatop;
            double hout = m_radius + deltabottom;
            return 0.25 * ( ( 3 * ( htot * htot - hout * hout ) ) / 
                ( m_radius * m_radius ) - 
                ( htot * htot * htot - hout * hout * hout ) / 
                ( m_radius * m_radius * m_radius ) ); 
        } else 
        { 
            // The sphere is cut once
            double hin = m_radius + deltatop;
            return 0.25 * hin * hin * ( 3 / ( m_radius * m_radius ) - hin / 
            ( m_radius * m_radius * m_radius ) );
        }
    } else if (deltabottom > 0) 
    { 
        // The sphere is below the layer
        if (deltabottom > m_radius) 
        { 
            // The sphere is not cut
            return 0;
        } else if (deltatop < m_radius) 
        { 
            // The sphere is cut twice
            double htot = m_radius - deltabottom;
            double hout = m_radius - deltatop;
            return 0.25 * ( ( 3 * ( htot * htot - hout * hout ) ) / 
                ( m_radius * m_radius ) - 
                ( htot * htot * htot - hout * hout * hout ) / 
                ( m_radius * m_radius * m_radius ) ); 
        } else 
        { 
            // The sphere is cut once
            double hin = m_radius - deltabottom;
            return 0.25 * hin * hin * ( 3 / ( m_radius * m_radius ) - 
            hin / ( m_radius * m_radius * m_radius ) );
        }
    } else 
    { 
        // The Sphere is inside the layer
        if (deltatop < m_radius) 
        { 
            // The sphere is cut at the top
            if (fabs(deltabottom) < m_radius) 
            { 
                // The sphere is cut at the top and at the bottom
                double htot = m_radius + deltatop;
                double hout = m_radius + deltabottom;
                return 0.25 * ( ( 3 * ( htot * htot - hout * hout ) ) / 
                    ( m_radius * m_radius ) - 
                    ( htot * htot * htot - hout * hout * hout ) / 
                    ( m_radius * m_radius * m_radius ) ); 
            } else 
            {                
                // The sphere is only cut at the top
                double hin = m_radius + deltatop;
                return 0.25 * hin * hin * ( 3 / ( m_radius * m_radius ) 
                - hin / ( m_radius * m_radius * m_radius ) );    
            }
        } else  
        { 
            // The Sphere is not cut at the top
            if (fabs(deltabottom) < m_radius) 
	    { 
                // The Sphere is cut at the bottom
                double hin = m_radius - deltabottom;
                return 0.25 * hin * hin * ( 3 / ( m_radius * m_radius ) - 
                hin / ( m_radius * m_radius * m_radius ) );
            } else 
	    {
                // The sphere is neither cut at the top nor at the bottom
                return 1;
	    }
        }
    }
}

void Sphere::setPercentInLayer(double percentInThisLayer) 
// This method will add a new value to the percentInEachLayer vektor.
// It is to be called by the container, that contains the sphere, for a number
// of times, equal to the number of layers that exist in the container.
{
    percentInEachLayer.push_back(percentInThisLayer);
}

double Sphere::getPercentInLayer(int numberOfLayer) 
{
    return percentInEachLayer[numberOfLayer];
}

void Sphere::postWrapCorrection(int nel) // nel = Number of Extra Layers
{
        
    int nal = percentInEachLayer.size() - 2 * nel; // nal = Number of Actual Layers
    // layerToAddBottomTo is the index of the layer to which the bottom extra-layer is to be added to.
    // Remember in case of overlap treatment an extra-bottomlayer has been added to the experiment by method Container::prepareOverlap.
    // So in case of add-to-next the bottom extra-layer is to be added to the topmost of the actual layers.
    // In case of add-to-current the bottom extra-layer is to be added to the lowermost of the actural layers.
    for(int i = 0; i < nel; i++)
    {
        // Adjust bottom layers
        percentInEachLayer[nal+i] += percentInEachLayer[i];
        // Adjust top layers
        percentInEachLayer[nel+i] += percentInEachLayer[nal+nel+i];
    }
    // Now lets clean up by deleting top and bottom extra-layers.
    // Good thing, they are right at the start and the end of the layers vector, too!
    for (int i = 0; i < nel; i++) 
    {
        percentInEachLayer.erase (percentInEachLayer.begin());
        percentInEachLayer.pop_back();
    }
}