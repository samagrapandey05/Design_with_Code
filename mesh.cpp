//
//  mesh.cpp
//  Design with Code
//
//

#include "mesh.hpp"
#include <iostream>
#include<vector>
#include <unordered_set>
#include <queue>
#include <algorithm>

using namespace std;
    
    //METHODS
    //Make pointmap for planar surface
    void Mesh::createPlanePoints(Point p1, Point p2, Point p3, double length, double width){ //starting point is P1
        unordered_set<Point*> surface;
        //Two vectors
        double x1, y1, z1; //p1 to p2
        double x2, y2, z2; //p1 to p3
        x1 = p2.x - p1.x;
        y1 = p2.y - p1.y;
        z1 = p2.z - p1.z;
        double normalization1 = sqrt(pow(x1, 2) + pow(y1, 2) + pow(z1, 2));
        x1 /= normalization1;
        y1 /= normalization1;
        z1 /= normalization1;
        
        x2 = p3.x - p1.x;
        y2 = p3.y - p1.y;
        z2 = p3.z - p1.z;
        double normalization2 = sqrt(pow(x2, 2) + pow(y2, 2) + pow(z2, 2));
        x2 /= normalization2;
        y2 /= normalization2;
        z2 /= normalization2;
        
        for(double i = 0; i < length; i += resolution){
            for (double j = 0; j < width; j += resolution){
                //find new point; proceed i*vector1 + j*vector2 from p1
                double newX = p1.x + x1 * i + x2 * j;
                double newY = p1.y + y1 * i + y2 * j;
                double newZ = p1.z + z1 * i + z2 * j;
                Point* newPoint = new Point(newX, newY, newZ);
                
                //add new point to surface
                surface.insert(newPoint);
                
                //find all neighbors of newPoint, if they exist, add newPoint to its adjacent and add it to newPoint's adjacent
                array<array<double, 3>, 8> neighbors = {{
                    {p1.x + x1 * (i + resolution) + x2 * (j), p1.y + y1 * (i + resolution) + y2 * (j), p1.z + z1 * (i + resolution) + z2 * (j)},
                    {p1.x + x1 * (i - resolution) + x2 * (j), p1.y + y1 * (i - resolution) + y2 * (j), p1.z + z1 * (i - resolution) + z2 * (j)},
                    {p1.x + x1 * (i) + x2 * (j + resolution), p1.y + y1 * (i) + y2 * (j + resolution), p1.z + z1 * (i) + z2 * (j + resolution)},
                    {p1.x + x1 * (i) + x2 * (j - resolution), p1.y + y1 * (i) + y2 * (j - resolution), p1.z + z1 * (i) + z2 * (j - resolution)},
                    {p1.x + x1 * (i + resolution) + x2 * (j + resolution), p1.y + y1 * (i + resolution) + y2 * (j + resolution), p1.z + z1 * (i + resolution) + z2 * (j + resolution)},
                    {p1.x + x1 * (i - resolution) + x2 * (j + resolution), p1.y + y1 * (i - resolution) + y2 * (j + resolution), p1.z + z1 * (i - resolution) + z2 * (j + resolution)},
                    {p1.x + x1 * (i + resolution) + x2 * (j - resolution), p1.y + y1 * (i + resolution) + y2 * (j - resolution), p1.z + z1 * (i + resolution) + z2 * (j - resolution)},
                    {p1.x + x1 * (i - resolution) + x2 * (j - resolution), p1.y + y1 * (i - resolution) + y2 * (j - resolution), p1.z + z1 * (i - resolution) + z2 * (j - resolution)}
                }};
                neighbors_points.insert(make_pair(newPoint, unordered_set<Point*>()));
                for (array<double, 3> neighbor : neighbors){
                    double neiX = neighbor[0];
                    double neiY = neighbor[1];
                    double neiZ = neighbor[2];
                    Point nei(neiX, neiY, neiZ);
                    if(neighbors_points.contains(&nei)){
                        neighbors_points[&nei].insert(newPoint);
                        neighbors_points[newPoint].insert(&nei);
                    }
                }
            }
        }
        
        /*if(normal[2] != 0){
            for(double i = 0; i < length; i += resolution){
                for(double j = 0; j < width; j += resolution){
                    //cout << i << j << endl;
                    double newX = startingPoint[0] + i; double newY = startingPoint[1] + j;
                    double newZ = (((-1) * (i * normal[0] + j * normal[1])) / normal[2]) + startingPoint[2];
                    Point* p = new Point(newX, newY, newZ);
                    surface.insert(p);
                }
            }
        }*/
        shape_points.push_back(surface);
        starting_points.push_back(&p1);
    }
    //Make pointmap for cube
    void Mesh::createCubePoints(double length) {
        Point lowerLeft(0, 0, 0);
        Point lowerRight(1, 0, 0);
        Point topLeft(0, 1, 0);
        Point topRight(1, 1, 0);
        Point upperLowerLeft(0, 0, 1);
        Point upperLowerRight(1, 0, 1);
        Point upperTopLeft(0, 1, 1);
        Point upperTopRight(1, 1, 1);
        createPlanePoints(lowerLeft, lowerRight, topLeft, length, length);
        createPlanePoints(lowerLeft, lowerRight, upperLowerLeft, length, length);
        createPlanePoints(lowerLeft, topLeft, upperLowerLeft, length, length);
        createPlanePoints(lowerRight, topRight, upperLowerRight, length, length);
        createPlanePoints(topRight, topLeft, upperTopRight, length, length);
        createPlanePoints(upperTopRight, upperTopLeft, upperLowerRight, length, length);
        
    }
    //Make pointmap for spherical surface
    //Copy certain surface
    //Translate certain surface to a new location
    //Hollow Extrude (ie. extend edge points in normal direction, have duplicate of edge
    //Convert current pointmap to an STL File and export
    void Mesh::export_stl(string filename){
        //ppintmap to STl: BFS from the first three points at every surface, have adjacency mapping between adjacent points, proceed with BFS creating a triangle with the third vertex being whatever is adjacent to both vertices already in a facet
        //for each surface
        for(int i = 0;i < shape_points.size(); i++){
            unordered_set<Point*> surface = shape_points[i];
            Point* start = starting_points[i];
            unordered_set<Facet*> facets;
            queue<pair<Point*, Point*>> q;
            
            
            //create queue of point pairs, the third point constitutes the Facet
            unordered_set<Point*>::iterator it = neighbors_points[start].begin();
            pair<Point*, Point*> initial_pair = make_pair(start, *it);
            q.push(initial_pair);
            
            //perform BFS, adding to facets
            while(q.size() > 0){
                //pop pair
                pair<Point*, Point*> curPoints = q.front();
                q.pop();
                //find third point
                Point* firstPoint = curPoints.first;
                Point* secondPoint = curPoints.second;
                vector<Point*> candidates;
                set_difference(neighbors_points[firstPoint].begin(), neighbors_points[firstPoint].end(), neighbors_points[secondPoint].begin(), neighbors_points[secondPoint].end(), back_inserter(candidates));
                if(candidates.size() == 0){
                    continue;
                }
                Point* thirdPoint = candidates[0];
                //create a new facet
                Facet* newFacet = new Facet();
                newFacet->vertex1 = firstPoint;
                newFacet->vertex2 = secondPoint; //find out how to make it in CCW order
                newFacet->vertex3 = thirdPoint;
                newFacet->normal = calculate_normal(firstPoint, secondPoint, thirdPoint);
                //hash the
            }
        }
    }
    
    array<double, 3> Mesh::calculate_normal(Point* p1, Point* p2, Point* p3){
        array<double, 3> v1 = {p1->x - p2->x, p1->y - p2->y, p1->z - p1->z};
        array<double, 3> v2 = {p1->x - p3->x, p1->y - p3->y, p1->z - p3->z};
        return {v1[1] * v2[2] - v1[2] * v1[1], -(v1[0] * v2[2] - v1[2] * v2[0]), v1[0] * v2[1] - v1[1] * v2[0]};
    }
    
    
    
    void Mesh::createPlane(double length, double width, double startingPoint[3], double normalVector[3])
    {
        //create head facet
        head = new Facet();
        for(int i = 0; i < 3; i++){
            head->normal[i] = normalVector[i];
        }
        //vertex 1 at starting point
        for(int i = 0; i < 3; i++){
            head->vertex1[i] = startingPoint[i];
        }
        //vertex 2 in positive x
        for(int i = 0; i < 3; i++){
            if(i == 0){
                head->vertex2[i] = startingPoint[i] + resolution;
            }
            else if (i == 1){
                head->vertex2[i] = startingPoint[i];
            }
            else{
                double diffX = head->vertex2[0] - startingPoint[0];
                double diffY = head->vertex2[1] - startingPoint[1];
                double diffZ = ((-1) * (diffX * normalVector[0] + diffY * normalVector[1])) / normalVector[2];
                head->vertex2[i] = diffZ + startingPoint[2];
            }
        }
        //vertex 3 in positive 3 direction
        for(int i = 0; i < 3; i++){
            if(i == 0){
                head->vertex3[i] = startingPoint[i];
            }
            else if (i == 1){
                head->vertex3[i] = startingPoint[i] + resolution;
            }
            else{
                double diffX = head->vertex3[0] - startingPoint[0];
                double diffY = head->vertex3[1] - startingPoint[1];
                double diffZ = ((-1) * (diffX * normalVector[0] + diffY * normalVector[1])) / normalVector[2];
                head->vertex2[i] = diffZ + startingPoint[2];
            }
        }
        //add to private variables
        idx_map.insert(make_pair(0,head));
        //extrude the head facet in the positive x until it reaches length + startingPoint[0]
        
    }
    double Mesh::distanceBnPoints(double p1[3], double p2[3]){
        return sqrt(pow((p1[0] - p2[0]), 2) + pow((p1[1] - p2[1]), 2) + pow((p1[2] - p2[2]), 2));
    }
    vector<Facet*> Mesh::extrude_point(int idx, vector<Facet*> surface, array<double, 3> direction, double maxLength, double startPoint[3], double curLength){
        //add idx to data structures, if adding another triangle in that directions fits the length, call the function again
        Facet* original = idx_map[idx];
        surface.push_back(original);
        //find lengthInDirection
        for(int i = 0; i < 3; i++){
            //curFacetDistance = max(curFacetDistance, distanceBnPoints(startPoint, double *p2))
        }
        
        //find next facet
        
        //add next facet to adjacency list
        adj_List.insert(make_pair<int, array<int, 3>>(0, {NULL, NULL, NULL}));
        
        //call extrude on next facet
        
        //return the surface
        return surface;
    }
    
    void Mesh::hollow_extrude(double length, double normalX, double normalY, double normalZ){
        
    }
    
    void Mesh::solid_extrude(double length, double normalVector[3]){
        
    }
    
    void Mesh::rotate(double angle, double xRange[2], double yRange[2], double zRange[2]){
        
    }
    
    void Mesh::solid_rotate(double angle, double xRange[2], double yRange[2], double zRange[2]){
        
    }
    
    void Mesh::translation(double xTranslation, double yTranslation, double zTranslation, double xRange[2], double yRange[2], double zRange[2]){
        
    }
    
    void Mesh::add(Mesh otherMesh){
        
    }
    
    void Mesh::subtract(Mesh otherMesh){
        
    }
    
    void Mesh::import_stl(string filename){
        
    }
};
