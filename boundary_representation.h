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
/*Vertex contains (x,y,z), pointer to all adjacent edges*/
struct Vertex{
    double x;
    double y;
    double z;
    vector<Edge*> edges;
};
struct Edge{
    array<Vertex*, 2> vertices;
    function<array<double, 2>(double)> f;
};

struct Face{
    vector<Edge*> boundaries;
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
    Boundary_Representation(){}
    
    //mutators, accessors
    Shape* getShape(){return final_shape;}
    void setShape(Shape* other){final_shape = other;};
    
    //METHODS - 2D
    void create2DEdge(double domainStart, double domainEnd, function<array<double, 2>(double)> func);
    void createRectangle(double length, double width);
    void createCircle();
    
    //METHODS - 3D
    void extrude(double x, double y, double z); //move every vertex to the new (x,y,z)
    
    void rotate(double radians); //rotate the entire shape
        
    void translation(double x, double y, double z); //translate the entire shape
    
    void add(Boundary_Representation other); //combine shapes
    
    void subtract(Boundary_Representation other); //subtract the commonality with other
    
    void intersection(Boundary_Representation other); //subtract part not common with other
    
    //METHOS - I/O
    void export_file(string filename);
    void import_file(string filename);
    
};
#endif /* boundary_representation_h */
