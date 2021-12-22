#ifndef SLOPE_H
#define SLOPE_H

#include "common.h"

extern point4 slope_vertices[6];

class slope {
    public:
      slope() {};

    void set_mv(float nminx, float nminy, float nminz, float nmaxx, float nmaxy, float nmaxz, float orientation);

    void init(vector<vec4> colors, GLint nmodel_view,int nInitial_Point, bool ninit);


    //location: center of sphere, radius defaults to 1.0
    bool collide(point4 location, float radius = 1.0);

    //handles collision events for ball with this object, this assumes that if the ball continues it will collide
    void handle_collision();  

    void draw();


    private:
    float minx, miny, minz, maxx, maxy, maxz;
    float orientation;
    mat4 mv;
    GLint model_view;
    int Initial_Point;
    int NumVertices;
};


#endif