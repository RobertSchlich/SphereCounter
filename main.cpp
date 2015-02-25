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
#include <sstream>		//Stringsstream; necessary for processing of the Layerlist
#include <cstdlib>		
#include <cmath>		//
#include <algorithm>	//z.B. für sort etc.
#include <unistd.h> 	//für getopt
#include <cerrno>		//Für Fehlerkontrolle bei strtod
// Includes of custom classes
#include "sphere.h"
#include "layer.h"


using namespace std;

/* ------------------------------------------------------------------ */

int find(double array[], int& numberofentries, double searchedfor) {
	for (int i = 0; i < numberofentries; i++) {
		if (array[i] == searchedfor) {
			return i;
			break;
		}
	}
	cout << "Der radius konnte nicht gefunden werden\n";
	return 0;
}

/* ------------------------------------------------------------------ */

bool compare (double i, double j) {
  return (i==j);
}
/* ------------------------------------------------------------------ */

void swapifneeded(double& a, double& b){  //compares both arguments and makes sure the first argument is the bigger one
	if (a > b) {
		double cache = a;
		a = b;
		b = cache;
	}
}

/* ---------------------------------------------------------------------
   Main function
--------------------------------------------------------------------- */

int main(int argc, char* argv[]) {
	
	
	/* -----------------------------------------------------------------
	   Processing of the command line arguments
	----------------------------------------------------------------- */
	int opt;
	int lflag = 0;
	string larg = "0";
	int aflag = 0;
	int aarg = 0;
	
	while ((opt = getopt(argc, argv, "l:a:")) != -1) {
		switch(opt) {
			case 'a':				//[a]utomatc layer generation
				if (lflag == 1) {
					cerr << "Options -a and -l  are mutually exclusive!";
					exit (0);
				}
				else {
					aflag = 1;
					aarg = atoi(optarg);
					if (aarg == 0) {
						cerr << "Option -a requires an argument of type integer" 
						<< "which defines the number of desired layers.";
						exit (0);
					}
				}
				break;
			case 'l':				//[L]ayer definition given by the user
				if (aflag == 1) {
					cerr << "Options -a and -l  are mutually exclusive!";
					exit (0);
				}
				else  {
					lflag = 1;
					larg = optarg;
				}
				break;
			case '?':
				cout << "\nUsage: counter -option argument inputfile outputfile\n";
				cout << "Options:\n";
				cout << "   -l: (mutually exclusive with -a) manual list of layers\n";
				cout << "       expects argument of type string in the form of \"l1b1,l1b2;l2b1,l2b2;l3b1,l3b2;....\"\n";
				cout << "       l1b1 = layer 1 border 1, l1b2 = layer 1 border 2; etc...\n";
				cout << "   -a: (mutually exclusive with -l) automatic generation of layers\n";
				cout << "       expects argument of type int giving the number of desired layers\n";
				cout << "       The layers are automatically spread equally across the sample\n";
				exit(0);
		}
	}
	
	/* Reading the filenames of in- and output ---------------------- */
	
	const char * infiledirectory;
	const char * outfiledirectory;
	
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
	infile >> numberofspheres;			//Readg the number of spheres
	Sphere spheres[numberofspheres];	//Create the sphere array
	
	//Read the borders of the test setup out of the input file
	getline(infile, currentline);		//Skip a line
	getline(infile, currentline);		//Skip a line
	
	double startx = 0;
	double endx = 0;
	infile >> startx;
	infile >> endx;
	swapifneeded(startx, endx);	//Call function swapifneeded which makes sure that startx is the bigger value
	
	double starty = 0;
	double endy = 0;
	infile >> starty;
	infile >> endy;
	swapifneeded(starty, endy);
	
	double startz = 0;
	double endz = 0;
	infile >> startz;
	infile >> endz;
	swapifneeded(startz, endz);

	
	//Skip inputfile until listing of sphere characteristics
	while (currentline != "ITEM: ATOMS id type x y z ix iy iz vx vy vz fx fy fz omegax omegay omegaz radius c_contact ") 
	  getline(infile, currentline);
	
	string trash;
	int id;
	double x, y, z, radius;
	
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
		spheres[i].setSphere(id, x, y, z, radius);
	}
	
	//Close inputfile
	infile.close();
	
	
	/* -----------------------------------------------------------------
	   Read the layers
	----------------------------------------------------------------- */
	
	/* Count the number of layers ----------------------------------- */
	
	int numberoflayers = 0;
	
	//User defined layers, count the number of tokens of the option argument
	if (lflag == 1) {
		//Transform the argument of option -l to a stringstream
		stringstream countstream(larg);
		string layerlisttoken;
		
		//Count how many times countstream can be divided along ';'
		while (getline(countstream, layerlisttoken, ';')) {
			numberoflayers++;
		}
	}
	
	//Automatic layer definition, set numberoflayers to the option argument
	if (aflag == 1) {
		numberoflayers = aarg;
	}
	
	Layer layers[numberoflayers];
	
	
	/* Option -l: User defined layers ------------------------------- */

	if (lflag == 1) {
		
		double border[2];
		stringstream layerliststream(larg);
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
					if (*e != '\0' || errno != 0 ) { //String wurde nicht komplett ausgelesen bzw. overflow oder underflow fehlermeldung
						cerr << "\nThe argument for option -l could not be read.\n";
						cerr << "Option -l expects argument of type string \"l1b1,l1b2;l2b1,l2b2;..\"\n";
						cerr << "Where: l1b1 = layer 1 border 1, l1b2 = layer 1 border 2; etc...\n";
						exit (0);
					}
				} else {
					cerr << "Faulty formating of the argument of -l:\n";
					cerr << larg << endl;
					exit (0);
				}
			}
			layers[count].setBorders(border[0],border[1]);	
			count++;
		}
	}
	
	
	/* Option -a: automatically generated layers -------------------- */
	
	if (aflag == 1) { 
		double totalheight = endz - startz;
		double heightoflayer = totalheight / numberoflayers;
		cout << endl;
		for (int i = 0; i < numberoflayers; i++){
			layers[i].setBorders(startz+i*heightoflayer, startz+(i+1)*heightoflayer);
		}
	}
	
	
	/* -----------------------------------------------------------------
	   Counting of unique radius values
	----------------------------------------------------------------- */
	
	//Create a sorted radius array
	double sortedradii[numberofspheres];
	for (int i = 0; i < numberofspheres; i++) {
		sortedradii[i] = spheres[i].getRadius();
	}
	sort(sortedradii, sortedradii + numberofspheres);
	unique(sortedradii, sortedradii + numberofspheres, compare);
	
	//Count unique values out of the radius array
	int numberofuniqueradii = 1;
	int numberofradii = sizeof(sortedradii)/sizeof(double);
	for (int i = 0; i < numberofradii; i++) {
		if (sortedradii[i] < sortedradii[i+1])
			numberofuniqueradii += 1;
		else
			break;
	}
	
	//Create an array with the unique radii values
	double uniqueradii[numberofuniqueradii];
	for (int i = 0; i < numberofuniqueradii; i++) {
		uniqueradii[i] = sortedradii[i];
	}
	cout << endl;
	
	
	/* -----------------------------------------------------------------
	   Layer-wise counting of the spheres
	----------------------------------------------------------------- */
	
	double layerinfoarray [numberofuniqueradii] [numberoflayers];
	double sphereinfoarray [numberofspheres] [numberoflayers+2];
	
	for(int m = 0; m < numberofspheres; m++) {
		
		sphereinfoarray[m][0]=spheres[m].getVolume();
		double rowsumofspheres = 0;
		
		for(int n = 0; n < numberoflayers; n++) {
				
			double percentinlayer = spheres[m].percentinlayer(layers[n]);
			int radiusposition = find(uniqueradii, numberofuniqueradii, spheres[m].getRadius());
			layerinfoarray[radiusposition][n] += percentinlayer;
			sphereinfoarray[m][n+1] = percentinlayer;
			rowsumofspheres += percentinlayer;
			
		}
		
		sphereinfoarray[m][numberoflayers+1] = rowsumofspheres;
		
	}
	cout.setf(ios::right);
	int width = 16;
	
	
	/* -----------------------------------------------------------------
	   Writing the output file
	----------------------------------------------------------------- */

	ofstream outfile(outfiledirectory);
	outfile.precision(10);
	
	//Write a describtion of the layers to the outputfile
	for (int i = 0; i < numberoflayers; i++) {
		outfile << "Layer #" << i+1 << ":" << endl;
		outfile << "\tBottom border:\t" << layers[i].getBottom() << endl;
		outfile << "\tTop border:\t\t" << layers[i].getTop() << endl;
	}
	outfile << endl;
	
	//Write the layerinfoarray
	outfile << "Layerinfoarray:" << endl;
	outfile << "---------------" << endl;
	outfile << setw(8) << "Radius";
	for (int i = 0; i < numberoflayers; i++) 
	{
		outfile << setw(width-1) << "#K in Schicht" << i+1;
	}
	outfile << endl;
	outfile << setw(8+(numberoflayers)*width) << setfill('-') << "--" << endl << setfill(' ');
	for (int i = 0;  i < numberofuniqueradii; i++) 
	{
		outfile << setw(8) <<uniqueradii[i];
		for (int j = 0; j < numberoflayers; j++) 
		{
			outfile << setw(width) << layerinfoarray[i][j];
		}
		outfile << endl;
	}
	outfile << endl << endl;
	
	//Write the sphereinfoarray
	outfile << "Sphereinfoarray: " << endl;
	outfile << "----------------" << endl;
	outfile << setw(6) << "Nr:";
	outfile << setiosflags(ios::fixed) << setw(width+1) << "Volumen:";
	for (int i = 0; i< numberoflayers; i++) {
		outfile << setw(width-2) << "% Schicht " << i+1 << ":";
	}
	outfile << setiosflags(ios::fixed) << setw(width) << "Prüfsumme:";
	outfile << endl;
	outfile << setw(6+(numberoflayers+2)*width) << setfill('-') << "--" << endl << setfill(' ');
	for (int i = 0; i < numberofspheres; i++) {
		outfile << setiosflags(ios::fixed) << setw(6) <<spheres[i].getID();
		for (int j = 0; j < numberoflayers + 2 ; j++) {
			outfile << setiosflags(ios::fixed) << setw(width) << sphereinfoarray[i][j];
		}
		outfile << endl;
		outfile.flush();
	}
	
	outfile.close();
	
	return 0;
}
