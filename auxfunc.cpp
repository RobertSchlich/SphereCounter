#include "auxfunc.h"
#include <iostream>

bool Auxfunc::compare (double i, double j) 
{
  return (i==j);
}

void Auxfunc::displayHelpMessage() 
// Displays a help message, if the user calls the function with argument -h
{
    std::cout
        << "spherecounter: count spheres in a liggghts (and possibly lammps) dump file layer-wise." << std::endl
        << std::endl
        << "usage: spherecounter [option] [argument]... [inputfile] [outputfile]" << std::endl
        << "usage: spherecounter [option] [argument]... [inputfile]" << std::endl 
        << "If no outputfile is defined results will be written to 'Spherecounter.out'"<< std::endl 
        << std:: endl
        << "Options:" << std::endl 
        << " -u,  --user-defined      User defined layer definiton" << std::endl
        << "                            Expects argument of type string." << std::endl
        << "                            \"l1b1,l1b2-l2b1,l2b2-...\"" << std::endl
        << "                            l1b1 = layer 1 border 1 etc..." << std::endl
        << " -a,  --auto-layer        Automatic layer definiton" << std::endl
        << "                            Expects argument of type int." << std::endl
        << "                            Number of desired layers. Layers will be spread evenly." << std::endl
        << " -d,  --direction         Alternative sphere counting direction (z is default direction)" << std::endl        
        << "                            Expects argument of type char." << std::endl
        << "                            'x', 'y' or 'z'. Default is 'z'." << std::endl
        << " -o,  --output            Defines the length of the output file." << std::endl
        << "                            Expects argument of type char." << std::endl
        << "                              l: long -- all information about layers and spheres" << std::endl
        << "                                 as well as information about the input file." << std::endl
        << "                              s: short -- all information about layers and spheres" << std::endl
        << "                              u: ultra short -- only information about layers is displayed" << std::endl
        << "Options defining treatment of wrapped spheres:" << std::endl
        << "Those are only valuable with periodic boundaries, otherwise they are ignored." << std::endl
        << "Chose one, only the last will be considered." << std::endl
        << "Default behaviour is to ignore wrapped percentages." << std::endl
        << " -e,  --extra-layer       Two extra layers are added at the top and the bottom of the experiment." << std::endl
        << " -n,  --add-to-next       Wrapped percentages are added to the layer at the other side of the experiment." << std::endl
        << " -i,  --ignore            Wrapped percentages are ignored (this is default behaviour)." << std::endl;
        
}

void Auxfunc::unknownOptionMessage() 
// Dispalys a message, if an unknown Argument is given by the user during 
// function call 
{
    std::cerr << std::endl 
        << "spherecounter: Unknown option." << std::endl
        << "\"spherecounter --help\" gives further information." << std::endl; 
}

