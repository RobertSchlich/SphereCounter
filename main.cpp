/* ---------------------------------------------------------------------
   Counter - An algorithm to analyze output Data of LIGGGHTS Software
   
   Counts how many spheres are in a user-defined set of layers.
   Layers are only defined vertically.
   Creates an output file with two tables:
   
    1 Layerinfoarray:
	  A list of the user-defined layers with information about how many 
	  spheres of a certain kind each layer contains. 
	  Spheres are sorted by their discrete radii.

    2 Sphereinfoarray:
      A list of every sphere that is defined in the dump file. The first
      column contains an id, the second column the volume of the sphere.
      Additional columns contain the volume percentage of the sphere 
      that is contained in each of the user defined layers. 
      In the last column all the volume percentages are added up. This 
      column is only meant for verification purposes.
       
   As of: 11.12.2014
   ------------------------------------------------------------------ */
// Includes of standard library headers
#include <iostream>		//Input- Output Stream
#include <iomanip>		//IO Manipulation
#include <fstream>		//Reading/Writing in Files
#include <string>		//Strings
#include <sstream>		//Stringsstream; necessary for processing of Layerlist
#include <cstdlib>		
#include <cmath>		//
#include <algorithm>	//z.B. für sort etc.
#include <unistd.h> 	//für getopt
#include <cerrno>		//Für Fehlerkontrolle bei strtod

// Includes of custom classes
#include "sphere.h"
#include "layer.h"
#include "auxfunc.h"	//Auxilliary functions
#include "container.h"

using namespace std;

/* ---------------------------------------------------------------------
   Main function
--------------------------------------------------------------------- */

int main(int argc, char* argv[]) {
	
	
	/* -----------------------------------------------------------------
	   Processing of the command line arguments
	----------------------------------------------------------------- */
	int opt;
	int uflag = 0;
	string uarg = "0";
	int aflag = 0;
	int aarg = 0;
	char direction = 'z';
	
	cout << endl;
	
	while ((opt = getopt(argc, argv, "u:a:d:")) != -1) {
		switch(opt) {
			case 'a':		// [a]utomatc layer generation
				// Check for mutually exclusive option -u
				if (uflag == 1) {
					cerr << "Options -a and -l  are mutually exclusive!";
					exit (0);
				}
				else {
					cout << "Automatic layer definition was requested." << endl;
					aflag = 1;
					// Convert the argument to an int
					aarg = atoi(optarg);
					// Check if the argument was a valid integer
					if (aarg == 0) {
						cerr << "Option -a requires an argument of type integer" 
						<< "which defines the number of desired layers.";
						exit (0);
					} else 
						cout << aarg << " layers were automatically defined."
							<< endl << endl;
				}
				break;
			case 'u':		//[u]ser-given layer generation
				// Check for mutually exclusive option -a
				if (aflag == 1) {
					cerr << "Options -a and -l  are mutually exclusive!";
					exit (0);
				}
				else  {
					cout << "User defined layer definition was requested:" 
					<<endl << "The given defintion is: "
					<< "'" << optarg << "'" << endl << endl;
					uflag = 1;
					// Save the argument of option -u
					uarg = optarg;
				}
				break;
			case 'd':				//[Direction] specified by user
				if (optarg[0] == 'x' || 
					optarg[0] == 'y' || 
					optarg[0] == 'z') {
					direction = optarg[0];
				}
				else
					cerr << "Only 'x', 'y', and 'z'" 
						<< " are valid direction options!";
					break;
			case '?':
				// Error message in case of an unknown argument
				cerr << endl 
					<< "Encountered unknown argument!" << endl << endl
					<< "Usage: counter -option argument inputfile outputfile"
					<< endl << "(More than one option can be given!)" << endl
					<< endl << "Options:" << endl 					
					<< " -u: [U]ser-given lis of layers"
					<< " (mutually exclusive with -a!)." << endl
					<< "     Argument of type String expected, in the form of:"
					<< " \"l1b1,l1b2;l2b1,l2b2;l3b1,l3b2;...\"" << endl
					<< "     l1b1 = layer 1 border 1,"
					<< " l1b2 = layer 1 border 2; etc..." << endl <<endl
					<< " -a: [A]utomatic generation of layers"
					<< " (mutually exclusive with -u!)." << endl
					<< "     Argument of type int expected," 
					<< " giving the number of desired layers." << endl
					<< "     The layers are automatically spread" 
					<< " evenly across the sample." << endl << endl 
					<< " -d: [D]irection in which the spheres"
					<< " are to be counted:" << endl 
					<< "     Argument of type char expected. Default is 'z'!" 
					<< endl
					<< "     Valid options: 'x', 'y', and 'z' for the"
					<< "respective directions.";
				exit(0);
		}
	}
	
	cout << "Counting spheres in " << direction << "-direction!" 
		<< endl << endl;
		
	/* Reading the filenames of in- and output ---------------------- */
	
	char * infiledirectory;
	char * outfiledirectory;
	
	int i = optind;
	infiledirectory = argv[i];
	outfiledirectory = argv[i+1];
	
	
	/* -----------------------------------------------------------------
	   Opening the Input file
	----------------------------------------------------------------- */
	
	ifstream infile(infiledirectory);
	
	//Error message if inputfile cannot be opened
	if (!infile.is_open()) {
		cout << "Inputfile could not be opened." << endl;
		cout << "Usage: counter -options inputfile outputfile" << endl;
		exit(0);
	}
		
	//Skip inputfile until the number of spheres is reached
	string currentline = "abc";
	while (currentline != "ITEM: NUMBER OF ATOMS") getline(infile, currentline);
	int numberofspheres = 0;		
	infile >> numberofspheres;			//Read the number of spheres
	//Read the borders of the test setup out of the input file
	getline(infile, currentline);		//Skip a line
	getline(infile, currentline);		//Skip a line
	
	double x1, x2;
	infile >> x1;
	infile >> x2;
	double y1, y2;
	infile >> y1;
	infile >> y2;
	double z1, z2;
	infile >> z1;
	infile >> z2;
	Container mainContainer(x1, x2, y1, y2, z1, z2);
	
	//Skip inputfile until listing of sphere characteristics
	while (currentline != 
		"ITEM: ATOMS id type x y z ix iy iz vx vy vz fx fy fz omegax omegay omegaz radius c_contact ") 
	  getline(infile, currentline);
	
	string trash;
	int id;
	double x, y, z, radius;
	Sphere currentsphere;
	//Read the spheres and save tham in the spheres[] array
	for (int i = 0; i < numberofspheres; i++){
		infile >> id;
		infile >> trash;
		infile >> x;
		infile >> y;
		infile >> z;
		infile >> trash;
		infile >> trash;
		infile >> trash;
		infile >> trash;
		infile >> trash;
		infile >> trash;
		infile >> trash;
		infile >> trash;
		infile >> trash;
		infile >> trash;
		infile >> trash;
		infile >> trash;
		infile >> radius;
		infile >> trash;
		currentsphere.setSphere(id, x, y, z, radius);
		mainContainer.addSphere(currentsphere);
	}
	
	//Close inputfile
	infile.close();
	
	
	/* -------------------------------------------------------------------------
	   Read the layers
	------------------------------------------------------------------------- */
	
	/* Count the number of layers ------------------------------------------- */
	
	int numberoflayers = 0;
	
	//User defined layers, count the number of tokens of the option argument
	if (uflag == 1) {
		//Transform the argument of option -l to a stringstream
		stringstream countstream(uarg);
		string layerlisttoken;
		
		//Count how many times countstream is divided by a ';'
		while (getline(countstream, layerlisttoken, ';')) {
			numberoflayers++;
		}
	}
	
	//Automatic layer definition, set numberoflayers to the option argument
	if (aflag == 1) {
		numberoflayers = aarg;
	}
	
	//Set the direction of the layers
	//Layer layers[numberoflayers];
	
	/* Define the layers ---------------------------------------------------- */

	//Option -u: User defined layers
	if (uflag == 1) {
		
		double border[2];
		stringstream layerliststream(uarg);
		string layerlisttoken;
		int count = 0;
		
		while (getline(layerliststream, layerlisttoken, ';')) {
			
			stringstream singlelayerstream(layerlisttoken);
			string singlelayertoken;
			for(int i = 0; i < 2; i++) {
				
				if (getline(singlelayerstream, singlelayertoken, ',')) {
					const char * c = singlelayertoken.c_str();
					char * e;
					border[i] = strtod(c, &e);
					if (*e != '\0' || errno != 0 ) { 
						//String wurde nicht komplett ausgelesen 
						//bzw. overflow oder underflow fehlermeldung
						cerr << endl 
							<< "The argument for option -l could not be read."
							<< endl
							<< "Option -l expects argument of type string "
							<< "\"l1b1,l1b2;l2b1,l2b2;..\"" 
							<< endl
							<< "Where: l1b1 = layer 1 border 1," 
							<< "l1b2 = layer 1 border 2; etc..." 
							<< endl;
						exit (0);
					}
				} else {
					cerr << "Faulty formating of the argument of -l:\n";
					cerr << uarg << endl;
					exit (0);
				}
			}
			Layer currentlayer(border[0], border[1], direction);
			mainContainer.addLayer(currentlayer);
			count++;
		}
	}
	
	// Option -a: automatically generated layers
	if (aflag == 1) {
		double top = mainContainer.getTop(direction);
		double bottom = mainContainer.getBottom(direction);
		double totalheight = top - bottom;
		double heightoflayer = totalheight / numberoflayers;
		cout << endl;
		for (int i = 0; i < numberoflayers; i++){
			Layer currentlayer(bottom+i*heightoflayer, 
							   bottom+(i+1)*heightoflayer, direction);
			mainContainer.addLayer(currentlayer);
		}
	}
	
	mainContainer.writeOutputFile(outfiledirectory);

	return 0;
}
