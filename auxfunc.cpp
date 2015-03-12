#include "auxfunc.h"
#include <iostream>

bool Auxfunc::compare (double i, double j) 
{
  return (i==j);
}

void Auxfunc::displayHelpMessage() 
// Displays a help message, if the user calls the function with argument -h
{
	std::cout << std::endl 
		<< "This is spherecounter." << std::endl 
		<< "An algorithm that counts spheres in LIGGGHTS-"
		<< "dumpfiles." << std::endl
		<< "Spherecounter counts spheres layer wise. The Layers either can be" 
		<< "given by the [u]ser (option -u), or can be [a]utomatically created."
		<< "Options -a and -u are mutually exclusive." << std::endl
		<< "The direction in which the spheres are to be counted is set to the"
		<< "z-direction as standard. Other directions can be counted by"
		<< "calling the [d]irection option." 
		<< std::endl << std::endl 
		<< "Usage: counter -option argument inputfile outputfile"
		<< std::endl << "(More than one option can be given!)" << std::endl
		<< std::endl << "Options:" << std::endl 
		<< " -u: [U]ser-given lis of layers (mutually exclusive with -a!)." 
		<< std::endl << "     Expects argument of type String, in the form of:"
		<< " \"l1b1,l1b2-l2b1,l2b2-l3b1,l3b2;...\"" << std::endl
		<< "     l1b1 = layer 1 border 1,"
		<< " l1b2 = layer 1 border 2; etc..." 
		<< std::endl << std::endl
		<< " -a: [A]utomatic generation of layers"
		<< " (mutually exclusive with -u!)." << std::endl
		<< "     Expects argument of type int," 
		<< " giving the number of desired layers." << std::endl
		<< "     The layers are automatically spread  evenly across the sample." 
		<< std::endl << std::endl 
		<< " -d: [D]irection in which the spheres  are to be counted:" 
		<< std::endl << "     Expects argument of type char. Default is 'z'." 
		<< std::endl
		<< "     Valid options: 'x', 'y', and 'z'"
		<< " for the respective directions." << std::endl;
}

void Auxfunc::unknownOptionMessage() 
// Dispalys a message, if an unknown Argument is given by the user during 
// function call 
{
	std::cerr << std::endl 
		<< "Encountered unknown argument!" << std::endl << std::endl
		<< "Usage: counter -option argument inputfile outputfile"
		<< std::endl << "(More than one option can be given!)" << std::endl
		<< std::endl << "Options:" << std::endl 
		<< " -u: [U]ser-given lis of layers (mutually exclusive with -a!)." 
		<< std::endl << "     Expects argument of type String, in the form of:"
		<< " \"l1b1,l1b2-l2b1,l2b2-l3b1,l3b2-...\"" << std::endl
		<< "     l1b1 = layer 1 border 1,"
		<< " l1b2 = layer 1 border 2; etc..." 
		<< std::endl << std::endl
		<< " -a: [A]utomatic generation of layers"
		<< " (mutually exclusive with -u!)." << std::endl
		<< "     Expects argument of type int," 
		<< " giving the number of desired layers." << std::endl
		<< "     The layers are automatically spread  evenly across the sample." 
		<< std::endl << std::endl 
		<< " -d: [D]irection in which the spheres  are to be counted:" 
		<< std::endl << "     Expects argument of type char. Default is 'z'." 
		<< std::endl
		<< "     Valid options: 'x', 'y', and 'z'"
		<< " for the respective directions." << std::endl;
}

