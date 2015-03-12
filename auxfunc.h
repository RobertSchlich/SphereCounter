#ifndef AUXFUNC_H
#define AUXFUNC_H

class Auxfunc 
// A class that contains a number of auxiliary functions, that do not belong to
// the other classes
{
private:
public:
	static bool compare (double i, double j);
	static void displayHelpMessage();
	static void unknownOptionMessage();
};

#endif