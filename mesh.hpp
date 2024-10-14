//
//  mesh.hpp
//  Design with Code
//
//

#ifndef mesh_hpp
#define mesh_hpp

#include <stdio.h>
#include <iostream>
#include<vector>
#include <unordered_set>
#include <queue>
#include <algorithm>

using namespace std;
struct Point {
    double x;
    double y;
    double z;
    Point(double X, double Y, double Z) : x(X), y(Y), z(Z) {}
    bool operator==(const Point& otherPoint) const
      {
        if (this->x == otherPoint.x && this->y == otherPoint.y && this->z == otherPoint.z) return true;
        else return false;
      }

      struct HashFunction
      {
        size_t operator()(const Point& point) const
        {
          size_t xHash = std::hash<int>()(point.x);
          size_t yHash = std::hash<int>()(point.y) << 1;
          size_t zHash = std::hash<int>()(point.z) << 2;
          return xHash ^ yHash ^ zHash;
        }
      };
};

struct Facet {
    array<double, 3> normal;
    Point* vertex1;
    Point* vertex2;
    Point* vertex3;
    
    bool operator==(const Facet& f) const
    {
        for (int i = 0; i < 3; i++){
            if(normal[i] != f.normal[i]){
                return false;
            }
            if(vertex1[i] != f.vertex1[i]){
                return false;
            }
            if(vertex2[i] != f.vertex2[i]){
                return false;
            }
            if(vertex3[i] != f.vertex3[i]){
                return false;
            }
        }
        return true;
    }
};
class Mesh {
private:
    
    vector<vector<Facet*>> shape; //each vector is a surface
    unordered_map<int, Facet*> idx_map; //maps index to Facet
    unordered_map<int, array<int, 3>> adj_List; //maps index to three adjacent indices
    string name;
    double resolution; //x-y distance bn points
    Facet* head;
    
    //Pointmap variables
    vector<unordered_set<Point*>> shape_points; //vector of surfaces; boundary points are in multiple surfaces
    vector<Point*> starting_points; //starting points corresponding to surfaces in shape_points
    unordered_map<Point*, unordered_set<Point*>> neighbors_points;
    
public:
    //constructors
    Mesh(string shapeName, double shapeResolution) : name(shapeName), resolution(shapeResolution) {}
    Mesh(string shapeName, vector<vector<Facet*>> shapeVector, double shapeResolution, Facet* shapeHead) : name(shapeName), shape(shapeVector), resolution(shapeResolution), head(shapeHead) {}
    
    //mutators, accessors
    vector<vector<Facet*>> getShape() { return shape; }
    string getName(){ return name; }
    void setResolution(double input_resolution){ resolution = input_resolution; }
    double getResolution() { return resolution; }
    vector<unordered_set<Point*>> getPointMap(){ return shape_points;}
    
    //METHODS
    //Make pointmap for planar surface
    void createPlanePoints(Point p1, Point p2, Point p3, double length, double width);
    //Make pointmap for cube
    void createCubePoints(double length);
    //Make pointmap for spherical surface
    //Copy certain surface
    //Translate certain surface to a new location
    //Hollow Extrude (ie. extend edge points in normal direction, have duplicate of edge
    //Convert current pointmap to an STL File and export
    void export_stl(string filename);
    
    array<double, 3> calculate_normal(Point* p1, Point* p2, Point* p3);
    
    
    
    void createPlane(double length, double width, double startingPoint[3], double normalVector[3]);
    double distanceBnPoints(double p1[3], double p2[3]);
    vector<Facet*> extrude_point(int idx, vector<Facet*> surface, array<double, 3> direction, double maxLength, double startPoint[3], double curLength);
    
    void hollow_extrude(double length, double normalX, double normalY, double normalZ);
    
    void solid_extrude(double length, double normalVector[3]);
    
    void rotate(double angle, double xRange[2], double yRange[2], double zRange[2]);
    
    void solid_rotate(double angle, double xRange[2], double yRange[2], double zRange[2]);
    
    void translation(double xTranslation, double yTranslation, double zTranslation, double xRange[2], double yRange[2], double zRange[2]);
    
    void add(Mesh otherMesh);
    
    void subtract(Mesh otherMesh);
    
    void import_stl(string filename);
    
};

#endif /* mesh_hpp */
