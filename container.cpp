#include "container.h"

#include <iostream>
#include <algorithm>
#include <cstring>
#include <math.h>
#include <string>
#include <sstream>
#include <iomanip>
// IO Manipulation
#include <fstream>
// Reading/Writing to/from Files
#include <stdexcept>
// Exception handling

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

Container::Container(char direction,
            double x1, double x2, char boundx, 
            double y1, double y2, char boundy, 
            double z1, double z2, char boundz)
// Constructor
{
    // Check if direction is valid
    if (direction == 'x' || direction == 'y' || direction == 'z') 
        m_direction = direction;
    else 
        std::cerr << "Invalid direction argument for container!";
    
    // Make sure Topborder always has a higher value than bottom border.
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
    
    // Treat 'm' and 's' Style boundary conditions as normal 'f' boundaries
    if(boundx == 'm' || boundx == 's')
        boundx = 'f';
    if(boundy == 'm' || boundy == 's')
        boundy = 'f';
    if(boundz == 'm' || boundz == 's')
        boundz = 'f';
    
    // Check if the given boundary conditions are valid and set them
    if (boundx == 'p' || boundx == 'f') 
        m_boundx = boundx;
    else 
        std::cerr << "Unknown boundary condition for x-direction: '" << boundx << "'." << std::endl
            << "Known boundary conditions are 'p' for periodic"
            << "and 'f' for fixed boundaries.\n";
    
    if (boundy == 'p' || boundy == 'f') 
        m_boundy = boundy;
    else 
        std::cerr << "Unknown boundary condition for y-direction: '" << boundy << "'." << std::endl
            << "Known boundary conditions are 'p' for periodic"
            << "and 'f' for fixed boundaries.\n";
    
    if (boundz == 'p' || boundz == 'f') 
        m_boundz = boundz;
    else 
        std::cerr << "Unknown boundary condition for z-direction: '" << boundz << "'." << std::endl
            << "Known boundary conditions are 'p' for periodic"
            << "and 'f' for fixed boundaries.\n";
    
    // Print out the boundary conditions and borders for the user.
    std::cout << "Read box-boundaries from inputfile:"
        << "\n  x: " << m_xBottom << "--" << m_xTop;
    if (m_boundx == 'p')
        std::cout << " periodic boundaries";
    else if (m_boundx == 'f')
        std::cout << " fixed boundaries";
    else 
        std::cout << " unknown boundary conditions";
    
    std::cout << "\n  y: " << m_yBottom << "--" << m_yTop;
    if (m_boundy == 'p')
        std::cout << " periodic boundaries";
    else if (m_boundy == 'f')
        std::cout << " fixed boundaries";
    else 
        std::cout << " unknown boundary conditions";
    
    std::cout << "\n  z: " << m_zBottom << "--" << m_zTop;
    if (m_boundz == 'p')
        std::cout << " periodic boundaries";
    else if (m_boundz == 'f')
        std::cout << " fixed boundaries";
    else 
        std::cout << " unknown boundary conditions";
    
    std::cout << std::endl << std::endl;
}

void Container::addSphere(Sphere newsphere) 
// A method to add a sphere to the sphere vector. Also checks if the radius of
// the new sphere is a new unique radius and, if so, adds it to the uniqueradii
// vector.
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
    if (direction == 'x') 
    {
        return m_xTop;
    }
    else if (direction == 'y') 
    {
        return m_yTop;
    }
    else if (direction == 'z') {
        return m_zTop;
    }
    else 
        std::cout << "Invalid direction in call for Container::getTop: '" 
            << direction << "'" << std::endl
            << "Valid diretions are 'x','y', or 'z'!";
}

double Container::getBottom(char direction)
// A method to get the bottom boundary of the experiment for the desired 
// direction
{
    if (direction == 'x')
        return m_xBottom;
    else if (direction == 'y')
        return m_yBottom;
    else if (direction =='z')
        return m_zBottom;
    else 
        std::cerr << "Invalid direction in call for Container::getBottom: '" 
            << direction << "'" << std::endl
            << "Valid diretions are 'x','y', or 'z'!";
}

void Container::setWraptreatment(char wraptreatment)
// This method sets wrap treatment to one of the following options
//      [i]gnore
//      [e]xtra-layer
//      add-to-[n]ext
//      add-to-[c]urrent
{
    if (wraptreatment == 'i' || wraptreatment == 'e' 
        || wraptreatment == 'n')
        m_wraptreatment = wraptreatment;
    else 
    {
        std::cerr << "Unknown wraptreatment option. "
            <<"Wraptreatment set to 'i'gnore." 
            << std::endl << std::endl;
        m_wraptreatment = 'i';
    }
        
}
void Container::prepareWrap(int numberOfExtraLayers, double layersize)
// This method prepares the wrap treatment options other than 'ignore'.
// Two additional layers are added to the layers vector to
// count those parts of spheres that are wrapped in the experiment.
// This is only relevant if the experiment has periodic boundaries!
// One layer is added below the experiment and one layer is added on
// top of the experiment. The height of the two layers equals the
// maximum sphere radius of the experiment to make sure all spheres
// of the experiment are fully counted, even if the center of one of
// the spheres with the maxium radius is positiond at the absolute 
// bottom of the experiment.
// How sphere percentages in those two layers are ultimately treated
// depends on the explicit wrapping option.
{
    // Determine top and bottom position of the experiment in counting direction.
    double top;
    double bottom;
    // Get the values and set the layers up, according to the counting-
    // direction
    if (m_direction == 'z')
    {
        bottom = m_zBottom;
        top = m_zTop;
    }
    else if (m_direction == 'x')
    {
        bottom = m_xBottom;
        top = m_xTop;
    } 
    else if (m_direction == 'y')
    {
        bottom = m_yBottom;
        top = m_yTop;
    } 

    // Iterate through the number of new layers needed.
    for (int i = 0; i < numberOfExtraLayers; i++)
    {
        // Set adequate borders for additional layer.
        // Four variables to hold the borders of the current layers.
        double cblt = bottom - i * layersize;     // CurrentBottomLayerTop
        double cblb = bottom - (i+1) * layersize; // CurrentBottomLayerBottom
        double ctlt = top + (i+1) * layersize;    // CurrentTopLayerTop
        double ctlb = top + i * layersize;        // CurrentTopLayerBottom
        Layer newBottomLayer(cblt, cblb);
        Layer newTopLayer(ctlt, ctlb);
        // Add the newly set-up layers to the layers vector.
        auto it = layers.begin();
        layers.insert(it, newBottomLayer);
        layers.push_back(newTopLayer);
        
    }
}


void Container::createInfoVectors(bool routput)
// This method creates the info arrays inside each sphere and each layer
// contained in the container.
// This is where the magic happens! :)
{
    std::cout << "Wraptreatment: " << m_wraptreatment << std::endl;
    // Check if wrap treatment is desired
    int numberOfExtraLayers;
    if(m_wraptreatment == 'e' || m_wraptreatment == 'n') 
    {
        //Get the maximum radius 
        double maxelementIterator = *max_element(uniqueRadii.begin(), uniqueRadii.end());
        // Determine how many extra layers are needed.
        numberOfExtraLayers = (int) std::ceil(maxelementIterator / layers[0].getSize());
        std::cout << "Number of extra layers created for wrap treatment: " << numberOfExtraLayers << std::endl;
        std:: cout << "As far as i can see the maxium radius is : " << maxelementIterator << std::endl;
        prepareWrap(numberOfExtraLayers, layers[0].getSize()); 
        std::cout << "Succesfully prepared wrap!" << std::endl;
    }
    
    int numberOfLayers = layers.size();
    int numberOfSpheres = spheres.size();
    int numberOfUniqueRadii = uniqueRadii.size();
    
    //First initialize the SphereVector inside of every layer
    if (routput == false)
    {
        for(int l = 0; l < numberOfLayers; ++l) 
        {
            layers[l].initSphereVector(numberOfUniqueRadii, m_direction);
        }
    }
    else
    {
        for(int l = 0; l < numberOfLayers; ++l) 
        {
            layers[l].initSphereVector(numberOfUniqueRadii, 'x');
            layers[l].initSphereVector(numberOfUniqueRadii, 'y');
            layers[l].initSphereVector(numberOfUniqueRadii, 'z');
        }
    }
    // Iterate through all spheres with Integer 'm'
    for(int m = 0; m < numberOfSpheres; ++m) 
    {
        
        // Iterate through all layers with Integer 'n'
        for(int n = 0; n < numberOfLayers; ++n) 
        {
            
            // Check what percentage of the current sphere(m) is in the current 
            // layer (n)
            double percentinlayer = 0;
            double percentinlayerx = 0;
            double percentinlayery = 0;
            double percentinlayerz = 0;
            if (routput == false)
            {
                percentinlayer = spheres[m].percentinlayer(layers[n], m_direction);
            } 
            else
            {
                char x = 'x';
                char y = 'y';
                char z = 'z';
                percentinlayerx = spheres[m].percentinlayer(layers[n], x);
                percentinlayery = spheres[m].percentinlayer(layers[n], y);
                percentinlayerz = spheres[m].percentinlayer(layers[n], z);
            }
                    
            // Find the position of the radius of the sphere inside the
            // uniqueRadii vector
            int radiusPosition = whereIsUniqueRadius( spheres[m].getRadius());
            
            spheres[m].setPercentInLayer(percentinlayer);
            if (routput == false)
                layers[n].addSphere(percentinlayer, radiusPosition, m_direction);
            else 
            {
                layers[n].addSphere(percentinlayerx, radiusPosition, 'x');
                layers[n].addSphere(percentinlayery, radiusPosition, 'y');
                layers[n].addSphere(percentinlayerz, radiusPosition, 'z');
            }
        }
    }
    
    std::cout << "Succesfully counted " << numberOfSpheres << " spheres in " << numberOfLayers << " layers." << std::endl;
    
    // Check if add-to-[n]ext or add-to-[c]urrent wrap treatment option
    // are desired. In that case start postWrapCorrection for all spheres
    // and layers
    if (m_wraptreatment == 'n')
    {
        // Start postWrapCorrection for all spheres.
        for(int i = 0; i < numberOfSpheres; ++i) 
        {
            spheres[i].postWrapCorrection(numberOfExtraLayers);
        }
        std::cout << "All spheres performed postWrapCorrection!" << std::endl;
        
        // Now start postWrapCorrection for the layers
        int numberOfActualLayers = layers.size() - 2 * numberOfExtraLayers;
        for (int i = 0; i < numberOfExtraLayers; i++)
        {
            for (int j = 0; j < numberOfUniqueRadii; ++j) {
                if (routput == false)
                {
                    // Adjust bottom layers
                    layers[numberOfActualLayers+i].addSphere(layers[i].getNumberOfSpheres(j, m_direction),j, m_direction);
                    // Adjust top layers
                    layers[numberOfExtraLayers+i].addSphere(layers[numberOfActualLayers+numberOfExtraLayers+i].getNumberOfSpheres(j, m_direction),j, m_direction);
                }
                else
                {
                    char x = 'x';
                    char y = 'y';
                    char z = 'z';
                    // Adjust bottom layers
                    layers[numberOfActualLayers+i].addSphere(layers[i].getNumberOfSpheres(j, x),j, x);
                    layers[numberOfActualLayers+i].addSphere(layers[i].getNumberOfSpheres(j, y),j, y);
                    layers[numberOfActualLayers+i].addSphere(layers[i].getNumberOfSpheres(j, z),j, z);
                    // Adjust top layers
                    layers[numberOfExtraLayers+i].addSphere(layers[numberOfActualLayers+numberOfExtraLayers+i].getNumberOfSpheres(j, x),j, x);
                    layers[numberOfExtraLayers+i].addSphere(layers[numberOfActualLayers+numberOfExtraLayers+i].getNumberOfSpheres(j, y),j, y);
                    layers[numberOfExtraLayers+i].addSphere(layers[numberOfActualLayers+numberOfExtraLayers+i].getNumberOfSpheres(j, z),j, z);
                }
            }
        }
        
        for (int i = 0; i < numberOfExtraLayers; i++) 
        {
            layers.erase (layers.begin());
            layers.pop_back();
        }
            numberOfLayers -= numberOfExtraLayers * 2;
    }

    std::cout << "Post Wrap Correction finished and layer-vector cleaned up!\n";
    
    // Get Porosity of each layer
    double length, width;
    if ( m_direction == 'z')
    {
        length = m_xTop - m_xBottom;
        width = m_yTop - m_yBottom;
    }
    else if ( m_direction == 'y')
    {
        length = m_xTop - m_xBottom;
        width = m_zTop - m_zBottom;
    }
    else if ( m_direction == 'x')
    {
        length = m_yTop - m_yBottom;
        width = m_zTop - m_zBottom;
    }
    for(int i = 0; i < numberOfLayers; ++i) 
    {
        layers[i].calculatePorosity(uniqueRadii, length, width, m_direction);
    }
    
    std::cout << "Porosity has been calculated for each Layer.\n";
    
}

char Container::getBoundary(char direction)
// This method returns the boundary conditions in the given direction
{
    if (direction == 'z') 
    {
        return m_boundz;
    } 
    else if (direction == 'x')
    {
        return m_boundx;
    }
    else if (direction == 'y')
    {
        return m_boundy;
    }
    std::cerr << "What the heck are you talking about?" << std::endl
        << "There is no '" << direction << "'-direction!" << std::endl
        << "I'm returning an 'f' for 'F'uck you!" << std::endl << std::endl;
    return 'f';
}

void Container::writeOutputFile(char* outfiledirectory, char* infiledirectory, char outputOption) 
{
    // Variables for output formating
    std::cout.setf(std::ios::right);
    int width = 16;
    
    int numberOfLayers = layers.size();
    int numberOfSpheres = spheres.size();
    int numberOfUniqueRadii = uniqueRadii.size();

    
    double checkSumSum = 0;
    
    if(outputOption != 'r')
    {
        std::ofstream outfile(outfiledirectory);
        outfile.precision(10);
        std::cout << "Starting to write outputfile.\n";
        if (outputOption == 'l') 
        {
        
            // 
            // Print a timestamp
            time_t rawtime;
            struct tm * timeinfo;
            char buffer[80];
            time (&rawtime);
            timeinfo = localtime(&rawtime);
            outfile<< "Spherecounter outputfile created on ";
            strftime(buffer,80,"%d.%m.%Y %H:%M:%S",timeinfo);
            std::string str(buffer);
            outfile << str << "." << std::endl; 
            
            // Print counting direction
            outfile << "Spheres counted in " << m_direction << "-direction.\n";
            
            // Print wrap treatment option
            outfile << "Treatment of wrapped parts of spheres: ";
            if (m_wraptreatment == 'i')
                outfile << "ignore" << std::endl;
            else if (m_wraptreatment == 'e')
                outfile << "extra-layer" << std::endl;    
            else if (m_wraptreatment == 'n')
                outfile << "add-to-next" << std::endl;   
            outfile << std::endl << std::endl;
            
            // Write a describtion of the layers to the outputfile
            for (int i = 0; i < numberOfLayers; i++) 
            {
                outfile << "Layer #" << i+1 << ":";
                outfile << " Bottom border: " << layers[i].getBottom();
                outfile << "; Top border:" << layers[i].getTop() << std::endl;
            }
            outfile << std::endl;
        }
        
        
        /* -------------------------------------------------------------------------
            For every layer , write how many spheres of each radius are inside
        --------------------------------------------------------------------------*/
        if(outputOption != 'u' && outputOption != 'r')
        {
            outfile << "information about the layers:" << std::endl;
            outfile << "-----------------------------" << std::endl;
        }
        outfile << std::setw(11) << "radius";
        // write the header
        if ( m_wraptreatment != 'e' ) 
        {
            for (int i = 0; i < numberOfLayers; i++) 
            {
                outfile << std::setw(width-2) << "\"# layer " << i+1 << "\"";
            }       
        }
        else
        {
            for (int i = 0; i < numberOfLayers-1; i++) 
            {
                outfile << std::setw(width-2) << "\"# layer " << i << "\"";
            }
            outfile << std::setw(width-2) << "\"# layer n+1" << "\"";
        }
        if (outputOption != 'u')
            outfile << std::setw(width-1) << "Total ";
        
        outfile << std::endl;
        if(outputOption != 'u')
        {
            // Draw a line with '-'
            outfile << std::setw(8+(numberOfLayers+1)*width) << std::setfill('-') << "--" 
                    << std::endl << std::setfill(' ');
        }
        // Write to file the number of Spheres inside each layer sorted by
        // unique radius of the spheres
        for (int i = 0;  i < numberOfUniqueRadii; i++) 
        {
            double totalNumber = 0;
            outfile << std::setw(11) << uniqueRadii[i];
            for (int j = 0; j < numberOfLayers; j++) 
            {
                outfile << std::setw(width) << layers[j].getNumberOfSpheres(i, m_direction);
                totalNumber += layers[j].getNumberOfSpheres(i, m_direction);
            }
            outfile << std::setw(width) << totalNumber;
            outfile << std::endl;
        }

        outfile << std::setfill(' ') << std::right << std::setw(11) << "n:";
        for (int i = 0; i < numberOfLayers; i++)
        {
            std::cout.setf(std::ios::right);
            outfile << std::right << std::setw(width) << layers[i].getPorosity();
        }  
        outfile << std::endl << std::endl;
        
        /* -------------------------------------------------------------------------
            For every sphere, write to what percentage they are in which layer
        --------------------------------------------------------------------------*/
         // A double to add up all checksums
        
        if(outputOption != 'u')
        {
            outfile << std::endl;
            outfile << "Information about the spheres:" << std::endl;
            outfile << "------------------------------" << std::endl;
            // Write the header of the table
            outfile << std::setw(6) << "Nr:";
            outfile << std::setiosflags(std::ios::fixed) << std::setw(width+1) 
                    << "Volume:";
            if ( m_wraptreatment != 'e' ) 
            {
                for (int i = 0; i < numberOfLayers; i++) 
                {
                    outfile << std::setw(width-1) << "# in layer " << i+1;
                }
            }
            else
            {
                for (int i = 0; i < numberOfLayers-1; i++) 
                {
                    outfile << std::setw(width-1) << "# in layer " << i;
                }
                outfile << std::setw(width-1) << "# in layer n+1";
            }
            outfile << std::setiosflags(std::ios::fixed) << std::setw(width) 
                    << "Checksum:";
            outfile << std::endl;
            // Draw a line of '-'
            outfile << std::setw(6+(numberOfLayers+2)*width) << std::setfill('-') << "--" 
                    << std::endl << std::setfill(' ');
                    
            // Start writing information for each sphere
            
            for (int i = 0; i < numberOfSpheres; i++) {
                // Write id and volume of sphere [i]
                outfile << std::setiosflags(std::ios::fixed) 
                        << std::setw(6) << spheres[i].getID() 
                        << std::setw(width) << spheres[i].getVolume();
                for (int j = 0; j < numberOfLayers; j++) {
                    // Write what percentage of sphere[i] is inside layer [j]
                    outfile << std::setiosflags(std::ios::fixed) << std::setw(width) 
                            << spheres[i].getPercentInLayer(j);
                }
                // Write the checksum to the outfile
                double checksum = spheres[i].getChecksum();
                outfile << std::setw(width) << checksum;
                checkSumSum += checksum;
                outfile << std::endl;
                outfile.flush();
            }
            if (outputOption == 'l') {
                outfile << "Total of all checksums: " << checkSumSum << std::endl;
                outfile << "Number of Spheres : " << numberOfSpheres << std::endl;
            }
        } 
        else
        {
            for (int i = 0; i < numberOfSpheres; i++) {
                checkSumSum += spheres[i].getChecksum();
            }
        }
        outfile.close();  

        std::cout << "\nSum of Checksums = " << checkSumSum << "." << std::endl;
            
        // Check if Sum of Checksums equals number of Spheres
        double tolerance = 1.0;
        
        // If wraptreatment is 'ignore' paired with fixed boundary condition
        // apply a slight tolerance to make up for particle-boundary overlap.
        if ( m_wraptreatment == 'i')
        {
            if (m_direction == 'x' && m_boundx == 'f' ||  m_direction == 'y' && m_boundy == 'f' 
                || m_direction == 'z' && m_boundz == 'f')
            {
                tolerance = 1,00000000001;
            }
        }        
        if ( numberOfSpheres / checkSumSum <= tolerance ) 
        {
            std::cout << "All is well! Sum of checksums of all spheres equals number of spheres.";
        }
        else 
            std::cout << "WARNING: The sum of checksums of all spheres does not equal the number of spheres!";    
        std::cout << std::endl;        
    }
    else
    // Create Output specifically created for Interpretation in R
    // Data is split up into two files.
    // One .frame file containing information about the simulation itself
    // and one .data file containing the numbers of spheres in each layer
    {
        
        // Getting the Name of the File without directory and extension
        std::string thisName;
        std::stringstream ifdstream(infiledirectory);
                while (std::getline(ifdstream, thisName, '/'))
                {
                    std::cout << thisName;
                }
        ifdstream.str("");
        ifdstream.clear();
        ifdstream << thisName;
        std::getline (ifdstream, thisName, '.');
        std::cout << "Final infileName: " << thisName << std::endl;
        
        
        // Getting the direcotry and name without the extension for output
        ifdstream.str("");
        ifdstream.clear();
        std::string infileName;
        ifdstream.str(infiledirectory);
        std::getline (ifdstream, infileName, '.');
        
        
        size_t len = infileName.size();

        // Count how many digits are in the number of layers
        int layerDigits = 0;
        int nol = numberOfLayers;
        while(nol)
        {
            nol /= 10;
            layerDigits++;
        }
        
        // Convert the number of layers to a string to add to the filename
        std::string layerNumberString = std::to_string(numberOfLayers);
        
        // Create new string for the names of the outputfiles
        std::string frameOut(infileName);
        std::string dataOut(infileName);
        // strcpy(frameOut, infiledirectory);
        // strcpy(dataOut, infiledirectory);
        
        // Append the number of Layers to the filename of the data output file
        dataOut += "_";
        dataOut += layerNumberString;
        
        // Append suffixes to output filenames
        frameOut += ".frame";
        dataOut += ".data";
        
        // Open the output files
        std::ofstream datafile(dataOut.c_str());
        std::ofstream framefile(frameOut.c_str());
        
        // Writing the frame file
        // Header
        framefile << "Name,nRadii,seed,growth,r1,r2,r3,r4,r5,r6,xlength,ylength,zlength" << std::endl;
        // Actual Data
        framefile << thisName << "," 
                << numberOfUniqueRadii << "," 
                << "seed" << "," 
                << "growth" << ",";
        for (int j = 0 ; j < 6; j++)
        {
            if (j < uniqueRadii.size()) 
            {
                framefile << uniqueRadii[j] << ",";
            }
            else
            {
                framefile << "NA,";
            }
        }
        
        framefile << m_xTop - m_xBottom << "," << m_yTop - m_yBottom << "," << m_zTop - m_zBottom << std::endl;
        // All data has been written, close the frame file.
        framefile.close();
        
        // Writing the data file
        // 1. Header
        datafile << "Name,Richtung,Schichtanzahl,Schicht,Radius,Anzahl" << std::endl;
        
        // 2. Actual Data
        // A for loop, to iterate through all three directions
        for(int i = 0; i < 3; i++)
        {
            for (int l = 0; l < numberOfLayers; l++ )
            {
                for (int j = 0; j < 6; j++)
                {
                    // Write Name of Sample, Direction and Layer
                    if(i == 0) 
                        datafile << thisName << ",x," << numberOfLayers << "," << l+1 << "," << uniqueRadii[j] << ",";
                    else if(i == 1)
                        datafile << thisName << ",y," << numberOfLayers << "," << l+1 << "," << uniqueRadii[j] << ",";
                    else
                        datafile << thisName << ",z," << numberOfLayers << "," << l+1 << "," << uniqueRadii[j] << ",";
                    if (j < numberOfUniqueRadii) 
                    {
                        if(i == 0) 
                            datafile << layers[l].getNumberOfSpheres(j, 'x');
                        else if(i == 1)
                            datafile << layers[l].getNumberOfSpheres(j, 'y');
                        else
                            datafile << layers[l].getNumberOfSpheres(j, 'z');
                    }
                    else
                        datafile << "NA";
                    if( i == 2 && l == (numberOfLayers - 1) && j == 5)
                        datafile.close();
                    else
                        datafile << std::endl;
                }   
            }
        }
    }
    
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


