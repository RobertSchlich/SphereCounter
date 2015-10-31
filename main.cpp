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
#include <iostream>
// Input- Output Stream
#include <iomanip>
// IO Manipulation
#include <fstream>
// Reading/Writing in Files
#include <string>
// Strings
#include <sstream>
// Stringsstream; necessary for processing of Layerlist
#include <cstdlib>
#include <cmath>
#include <algorithm>
// z.B. für sort etc.
#include <getopt.h>
// für getopt
#include <cerrno>
// Für Fehlerkontrolle bei strtod

#include "sphere.h"
#include "layer.h"
#include "auxfunc.h"    
// Auxilliary functions
#include "container.h"
// Includes of custom classes

using namespace std;

int main(int argc, char* argv[]) 
{
    
    /* -----------------------------------------------------------------
       Processing of the command line arguments
    ----------------------------------------------------------------- */
    int c;
    string uarg = "0";
    
    // Some flags to store user-defined behavior
    int uflag = 0;
    int aflag = 0;
    int aarg = 0;
    // Output is set to [s]hort by default
    char outputOption = 's';
    
    // This is where getop_long stores the option index
    int option_index = 0;
    extern int optind;
    
    // Some standard variables (will be changed if user sets the accoring options)
    char direction = 'z';
    // WrapTreatment has the following options: 
    // [i]gnore, [e]xtra-layer, add-to[n]ext
    char wrapTreatment = 'n';
    
    std::cout << endl;
    
    // While loop to process all command-line arguments
    while (1) 
    {
        int this_option_optind = optind ? optind : 1;
        // Definition of long options
        static struct option long_options[] = 
        {
            {"direction",       required_argument,  0,  'd'},
            {"auto-layer",      required_argument,  0,  'a'},
            {"user-defined",    required_argument,  0,  'u'},
            {"output",          required_argument,  0,  'o'},
            {"long",            no_argument,        0,  'l'},
            {"help",            no_argument,        0,  'h'},
        // Options regarding wrap Treatment    
            {"extra-layer",     no_argument,        0,  'e'},    
            {"add-to-next",     no_argument,        0,  'n'},
            {"ignore",          no_argument,        0,  'i'}
            
        };
        
        // Process options and safe results in c
        c = getopt_long (argc, argv, "ienco:a:d:u:", long_options, &option_index);
        
        // Getop returns -1 if all known arguments have been processed
        // So in that case the while loop can be cancelled.
        if (c == -1)
            break;
        
        // Check which option has been found by getopt and react accordingly
        switch(c) 
        {
            // [a]utomatc layer generation    
            case 'a':
                // Check for mutually exclusive option -u
                if (uflag == 1) 
                {
                    std::cerr << "Options -a and -l  are mutually exclusive!";
                    exit (0);
                }
                else 
                {
                    aflag = 1;
                    // Convert the argument to an int
                    aarg = atoi(optarg);
                    // Check if the argument was a valid integer
                    if (aarg == 0) {
                        cerr << "Option -a requires an argument of type integer" 
                        << "which defines the number of desired layers.";
                        exit (0);
                    } else
                        std::cout << "Automatic layer definition of " << aarg 
                            << " layers was requested." << endl;
                }
                break;
                
            // [u]ser-given layer generation
            case 'u':
                // Check for mutually exclusive option -a
                if (aflag == 1) 
                {
                    cerr << "Options -a and -l  are mutually exclusive!";
                    exit (0);
                }
                else  
                {
                    cout << "User defined layer definition was requested:" 
                    <<endl << "The given defintion is: "
                    << "'" << optarg << "'" << endl;
                    uflag = 1;
                    // Save the argument of option -u
                    uarg = optarg;
                }
                break;
                
            // [s]hort output option
            case 'o':
                cout << "Selected output option: ";
                if(optarg[0] == 'l')
                {
                    outputOption = optarg[0];
                    cout << "long." << endl;
                } 
                else if(optarg[0] == 's') 
                {
                    outputOption = optarg[0];
                    cout << "short." << endl;
                } 
                else if(optarg[0] == 'u') 
                {
                    outputOption = optarg[0];
                    cout << "ultra short." << endl;
                } 
                else if(optarg[0] == 'r')
                {
                    outputOption = optarg[0];
                    cout << "R." << endl;
                }
                else
                {
                        outputOption ='s';
                        cout << "UNKNOWN!!! Output option is set to short!" <<endl;
                }
                break;
                
            // [d]irection specified by user
            case 'd':
                if (optarg[0] == 'x' || optarg[0] == 'y' || optarg[0] == 'z') 
                    direction = optarg[0];
                else
                {
                    cerr << "Invalid direction option!" << endl
                        << "Only 'x', 'y', and 'z'are valid direction options.";
                    cerr << "\nProceeding with direction option 'z'.\n";
                    direction = 'z';
                }
                break;
                
            // [e]xtra layer
            case 'e':
                wrapTreatment = 'e';
                break;
                
            // add-to-[n]ext
            case 'n':
                wrapTreatment = 'n';
                break;
            
            // [i]gnore
            case 'i':
                wrapTreatment = 'i';
                break;
                
            // [h]elp
            case 'h':
                Auxfunc::displayHelpMessage();
                exit(0);
                break;
                
            // Unknown option
            case '?':
                // Error message in case of an unknown argument
                Auxfunc::unknownOptionMessage();
                exit(0);
        }
    }
    
    cout << "Counting spheres in " << direction << "-direction!" 
        << endl;
    cout << "Method of wrap Treatment: " << wrapTreatment
        << endl << endl;
        
    /* Reading the filenames of in- and output ---------------------- */
    
    char * infiledirectory;
    char * outfiledirectory;
    string defaultoutfiledirectory = "spherecounter.out";
    
    // Set infiledirectory to the first argument after the options
    infiledirectory = argv[optind];
    
    // Check if another argument has been given
    if (argv[optind+1])
        // If so, use it as outfiledirectory
        outfiledirectory = argv[optind+1];
    else
        // If not use standard filename spherecounter.out as outfiledirectory
        outfiledirectory = &defaultoutfiledirectory[0];
    
    
    /* -----------------------------------------------------------------
       Opening the Input file
    ----------------------------------------------------------------- */
    
    ifstream infile(infiledirectory);
    
    // Error message if inputfile cannot be opened
    if (!infile.is_open()) 
    {
        cout << "Inputfile '" << infiledirectory << "' could not be opened." << endl;
        cout << "Usage: counter -options inputfile outputfile" << endl;
        exit(0);
    } else {
        cout << "Successfully opened inputfile '" << infiledirectory << "'."
            << endl;
    }
        
    // Skip inputfile until the number of spheres is reached
    string currentline = "abc";
    while (currentline != "ITEM: NUMBER OF ATOMS") getline(infile, currentline);
    int numberofspheres = 0;        
    infile >> numberofspheres;            // Read the number of spheres
    cout << "Inputfile specified " << numberofspheres << " spheres."
        << endl << endl;
    getline(infile, currentline);        
    
    // Read the boundary conditions of the Simulation
    getline(infile, currentline);
    char boundx = currentline[18];
    char boundy = currentline[21];
    char boundz = currentline[24];
    
    // Read the borders of the test setup out of the input file
    double x1, x2;
    infile >> x1;
    infile >> x2;
    double y1, y2;
    infile >> y1;
    infile >> y2;
    double z1, z2;
    infile >> z1;
    infile >> z2;
    // Create mainContainer with the direction, the xyz-borders, and the xyz-boundary conditions.
    Container mainContainer(direction, x1, x2, boundx, y1, y2, boundy, z1, z2, boundz);
    
    // Check if wrap treatment options, set to ignore if the boundaries 
    // in the observed direction is fixed
    if (mainContainer.getBoundary(direction) == 'f' 
        &&  wrapTreatment != 'i')
        std::cout << "You have chosen wrap treatment despite of fixed boundaries." << std::endl;
    // Check if user-defined layers were given, in that case wrap treatment options are ignored!
    if (uflag == 1) 
	{
        wrapTreatment = 'i';
		std::cout << "Warning: Wrap treatment options are ignored with user given layer definitions!" << std::endl;
	}
    // Set wrap for mainContainer. 
    mainContainer.setWraptreatment(wrapTreatment);
    
    
    // Skip inputfile until listing of sphere characteristics
    string searchme("ITEM: ATOMS");
    while (currentline.compare(0, searchme.length(), searchme)) 
      getline(infile, currentline);
    
    // Identify order of atom-defintions
    string segment;
    stringstream defintionorderss(currentline);
    vector<string> definitionorder;
    while(std::getline(defintionorderss, segment, ' '))
    {
      definitionorder.push_back(segment);
    }
    
    int idpos, xpos, ypos, zpos, radiuspos;
    for (int i = 0; i < definitionorder.size(); i++)
    {
        if (definitionorder[i] == "id")
            idpos = i - 2;
        else if (definitionorder[i] == "x")
            xpos = i - 2;
        else if (definitionorder[i] == "y")
            ypos = i - 2;
        else if (definitionorder[i] == "z")
            zpos = i - 2;
        else if (definitionorder[i] == "radius")
            radiuspos = i - 2;
    }
    
    int defintionlength = definitionorder.size() - 2;
    
    string trash;
    int id;
    double x, y, z, radius;
    Sphere currentsphere;
    // Read the spheres and save tham in the spheres[] array
    for (int i = 0; i < numberofspheres; i++)
    {
        for (int j = 0; j < defintionlength; j++)
        {
            if (j == idpos)
                infile >> id;
            else if (j == xpos)
                infile >> x;
            else if (j == ypos)
                infile >> y;
            else if (j == zpos)
                infile >> z;
            else if (j == radiuspos)
                infile >> radius;
            else
                infile >> trash;
        }
        currentsphere.setSphere(id, x, y, z, radius);
        mainContainer.addSphere(currentsphere);
    }
    
    // Close inputfile
    infile.close();
    
    
    /* -------------------------------------------------------------------------
       Read the layers
    ------------------------------------------------------------------------- */
    
    /* Count the number of layers ------------------------------------------- */
    
    int numberoflayers = 0;
    
    // User defined layers, count the number of tokens of the option argument
    if (uflag == 1) 
    {
        //Transform the argument of option -l to a stringstream
        stringstream countstream(uarg);
        string layerlisttoken;
        
        // Count how many times countstream is divided by the character '-'
        while (getline(countstream, layerlisttoken, '-')) 
        {
            numberoflayers++;
        }
    }
    
    // Automatic layer definition, set numberoflayers to the option argument
    if (aflag == 1) 
    {
        numberoflayers = aarg;
    }
    
    /* Define the layers ---------------------------------------------------- */

    // Option -u: User defined layers
    if (uflag == 1) 
    {
        
        double border[2];
        stringstream layerliststream(uarg);
        string layerlisttoken;
        int count = 0;
        
        while (getline(layerliststream, layerlisttoken, '-')) 
        {
            
            stringstream singlelayerstream(layerlisttoken);
            string singlelayertoken;
            for(int i = 0; i < 2; i++) 
            {
                
                if (getline(singlelayerstream, singlelayertoken, ',')) 
                {
                    const char * c = singlelayertoken.c_str();
                    char * e;
                    border[i] = strtod(c, &e);
                    if (*e != '\0' || errno != 0 ) 
                    { 
                        // String wurde nicht komplett ausgelesen 
                        // bzw. overflow oder underflow fehlermeldung
                        cerr << endl 
                            << "The argument for option -l could not be read."
                            << endl
                            << "Option -l expects argument of type string "
                            << "\"l1b1,l1b2-l2b1,l2b2;..\"" 
                            << endl
                            << "Where: l1b1 = layer 1 border 1," 
                            << "l1b2 = layer 1 border 2; etc..." 
                            << endl;
                        exit (0);
                    }
                } else 
                {
                    cerr << "Faulty formating of the argument of -l:\n";
                    cerr << uarg << endl;
                    exit (0);
                }
            }
            Layer currentlayer(border[0], border[1]);
            mainContainer.addLayer(currentlayer);
            count++;
            cout << "Created user-defined layer " << count << "."
                << " Bottom border: " << currentlayer.getBottom()
                << ", top border: " << currentlayer.getTop() 
                << endl ;
        }
        cout << endl;
    }
    
    // Option -a: automatically generated layers
    if (aflag == 1) 
    {
        double top = mainContainer.getTop(direction);
        double bottom = mainContainer.getBottom(direction);
        double totalheight = top - bottom;
        double heightoflayer = totalheight / numberoflayers;
        for (int i = 0; i < numberoflayers; i++)
        {
            Layer currentlayer(bottom+i*heightoflayer, 
                               bottom+(i+1)*heightoflayer);
            mainContainer.addLayer(currentlayer);
            cout << "Created automatically-defined layer " << i+1 << "."
                << " Bottom border: " << currentlayer.getBottom()
                << ", top border: " << currentlayer.getTop() 
                << endl;
        }
        cout << endl;
    }
    bool routput = 0;
    if (outputOption == 'r')
        routput = 1;
        
    cout << "Start counting spheres." << endl;
    mainContainer.createInfoVectors(routput);
    cout << " Chosen output option: " << outputOption << endl;
    mainContainer.writeOutputFile(outfiledirectory, infiledirectory, outputOption);

    return 0;
}
