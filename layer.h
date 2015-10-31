#ifndef LAYER_H
#define LAYER_H

#include <vector>
// To use vector template.
class Layer
{
private:
    static constexpr double PI = 3.14159265358979323846;
    double topborder;
    double bottomborder;
    double porosity;
    std::vector<double> numberOfSpheresInsideX;
    std::vector<double> numberOfSpheresInsideY;
    std::vector<double> numberOfSpheresInsideZ;
public:
    Layer();
    // Default constructor
    Layer(double border1, double border2);
    // Constructor
    double getTop() const;
    double getBottom() const;
    double getSize() const;
    double calculatePorosity(const std::vector<double>&, double, double, char direction);
    double getPorosity() const;
    void initSphereVector(int numberOfUniqueradii, char direction);
    // Initializes the numberOfSpheresInside vector
    void addSphere(double percentInLayer, int positionOfThisRadius, char direction);
    // Adds a sphere to the numberOfSpheresInside vector
    double getNumberOfSpheres(int radiusPosition, char direction);
    // Method to set borders after construction just for the heck of it
    void setBorders(double border1, double border2);
    // Method to correct the nubmerOfSpheresInside vector
    void consumeLayer(Layer &otherLayer);
    int getNumberOfUniqueRadii(char direction);
};

#endif
