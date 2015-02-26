#ifndef AUXFUNC_H
#define AUXFUNC_H

class Auxfunc {
private:
public:
	static int find(double array[], int& numberofentries, double searchedfor);
	static bool compare (double i, double j);
	static void swapifneeded(double& a, double& b);
};

#endif
