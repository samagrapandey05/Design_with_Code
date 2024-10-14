//
//  boundary_representation.h
//  Design with Code
//
//

#ifndef boundary_representation_h
#define boundary_representation_h
#include <stdio.h>
#include <iostream>
#include<vector>
#include <unordered_set>
#include <queue>
#include <algorithm>
using namespace std;


struct Edge;
struct Shape;
struct Vertex{
    double x;
    double y;
    double z;
    vector<Edge*> edges;
    Shape* shape;
}
struct Edge{
    vector<Vertex*> vertices;
    array<Precurve*, 2> precurves;
}
struct Precurve{
    Edge* edge;
    Precurve* next;
    Precurve* inside;
};
struct Face{
    Precurve* precurves;
};
struct Shell{
    vector<Face*> faces;
    Shell* next;
    Shell* inside;
};
struct Shape{
    Shell* shells;
};
class Boundary_Representation {
private:
    Shape* final_shape;
    
public:
    //constructors
    Boundary_Representation(string shapeName, double shapeResolution);
    
    //mutators, accessors
    Shape* getShape();
    void setShape(Shape* other);
    
    //METHODS
    void createPlane(double length, double width, double startingPoint[3], double normalVector[3]);
    double distanceBnPoints(double p1[3], double p2[3]);
    vector<Facet*> extrude_point(int idx, vector<Facet*> surface, array<double, 3> direction, double maxLength, double startPoint[3], double curLength);
    
    void hollow_extrude(double length, double normalX, double normalY, double normalZ);
    
    void solid_extrude(double length, double normalVector[3]);
    
    void rotate(double angle, double xRange[2], double yRange[2], double zRange[2]);
    
    void solid_rotate(double angle, double xRange[2], double yRange[2], double zRange[2]);
    
    void translation(double xTranslation, double yTranslation, double zTranslation, double xRange[2], double yRange[2], double zRange[2]);
    
    void add(Boundary_Representation other);
    
    void subtract(Boundary_Representation other);
    
    void import_file(string filename);
    
};
#endif /* boundary_representation_h */
