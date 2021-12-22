#ifndef BOX_H
#define BOX_H

#include "common.h"
#include "cube.h"

//Axis Aligned Box
class box {
  public:
    box() {};

    void init(float nminx, float nminy, float nminz, float nmaxx, float nmaxy, float nmaxz);

    void set_mv(float nminx, float nminy, float nminz, float nmaxx, float nmaxy, float nmaxz);
    //location: center of sphere, radius defaults to 1.0
    bool collide(point4 location, float radius = 1.0);

    //handles collision events for ball with this object, this assumes that if the ball continues it will collide
    void handle_collision();  

    void draw();
  private:
    float minx, miny, minz, maxx, maxy, maxz;
    cube boxTexture;
    static bool boxTextureInit;
    static int boxInitialPoint;
};



#endif