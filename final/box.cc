#include "box.h"
#include <algorithm>

vector<vec4> box_colors{vec4(0.0,0.5,0.0,1.0), vec4(0.0,0.5,0.0,1.0), vec4(0.0,0.5,0.0,1.0), vec4(0.0,0.5,0.0,1.0), vec4(0.0,0.5,0.0,1.0), vec4(0.0,0.5,0.0,1.0)};

bool box::boxTextureInit = false;
int box::boxInitialPoint = -1;

void box::init(float nminx, float nminy, float nminz, float nmaxx, float nmaxy, float nmaxz) {
    minx = nminx;
    miny = nminy;
    minz = nminz;
    maxx = nmaxx;
    maxy = nmaxy;
    maxz = nmaxz;

    float scalex = maxx - minx;
    float scaley = maxy - miny;
    float scalez = maxz - minz;

    float locx = (maxx + minx) / 2.0;
    float locy = (maxy + miny) / 2.0;
    float locz = (maxz + minz) / 2.0;

    if (boxTextureInit) {
      boxTexture.init(box_colors, model_view, boxInitialPoint, true);
    }
    else {
      boxInitialPoint = points.size();
      boxTexture.init(box_colors, model_view, boxInitialPoint, false);
      boxTextureInit = true;
    }

    boxTexture.set_mv(Translate(locx,locy,locz)*Scale(scalex, scaley, scalez));
}

bool box::collide(point4 loc, float radius) {
  //COLLISION OF SPHERE AND AABB
  float xtemp = std::max(minx, std::min(loc.x, maxx)) - loc.x;
  float ytemp = std::max(miny, std::min(loc.y, maxy)) - loc.y;
  float ztemp = std::max(minz, std::min(loc.z, maxz)) - loc.z;

  float distsquared = xtemp*xtemp + ytemp*ytemp + ztemp*ztemp;

  return( (distsquared < (radius*radius) ) );
}

void box::set_mv(float nminx, float nminy, float nminz, float nmaxx, float nmaxy, float nmaxz) {
    minx = nminx;
    miny = nminy;
    minz = nminz;
    maxx = nmaxx;
    maxy = nmaxy;
    maxz = nmaxz;

    float scalex = maxx - minx;
    float scaley = maxy - miny;
    float scalez = maxz - minz;

    float locx = (maxx + minx) / 2.0;
    float locy = (maxy + miny) / 2.0;
    float locz = (maxz + minz) / 2.0;

    boxTexture.set_mv(Translate(locx,locy,locz)*Scale(scalex, scaley, scalez));
}

void box::handle_collision() {
  if (by < miny ) {//bottom
    if (bz < minz) {//left
      if (bx < minx) {//below
        vx = -bounce_loss*vx;
        vy = -bounce_loss*vy;
        vz = -bounce_loss*vz;
      }
      else if (bx > maxx) {//above
        vx = -bounce_loss*vx;
        vy = -bounce_loss*vy;
        vz = -bounce_loss*vz;
      }
      else {//middle
        vy = -bounce_loss*vy;
        vz = -bounce_loss*vz;
      }
    }
    else if (bz > maxz) {//right
      if (bx < minx) {//below
        vx = -bounce_loss*vx;
        vy = -bounce_loss*vy;
        vz = -bounce_loss*vz;
      }
      else if (bx > maxx) {//above
        vx = -bounce_loss*vx;
        vy = -bounce_loss*vy;
        vz = -bounce_loss*vz;
      }
      else {//middle
        vy = -bounce_loss*vy;
        vz = -bounce_loss*vz;
      }
    }
    else {//center
      if (bx < minx) {//below
        vx = -bounce_loss*vx;
        vy = -bounce_loss*vy;
      }
      else if (bx > maxx) {//above
        vx = -bounce_loss*vx;
        vy = -bounce_loss*vy;
      }
      else {//middle
        vy = -bounce_loss*vy;
      }
    }
  }
  else if (by > maxy) {//top
    if (bz < minz) {//left
      if (bx < minx) {//below
        vx = -bounce_loss*vx;
        vy = -bounce_loss*vy;
        vz = -bounce_loss*vz;
      }
      else if (bx > maxx) {//above
        vx = -bounce_loss*vx;
        vy = -bounce_loss*vy;
        vz = -bounce_loss*vz;
      }
      else {//middle
        vy = -bounce_loss*vy;
        // vz = -bounce_loss*vz;
      }
    }
    else if (bz > maxz) {//right
      if (bx < minx) {//below
        vx = -bounce_loss*vx;
        vy = -bounce_loss*vy;
        vz = -bounce_loss*vz;
      }
      else if (bx > maxx) {//above
        vx = -bounce_loss*vx;
        vy = -bounce_loss*vy;
        vz = -bounce_loss*vz;
      }
      else {//middle
        vy = -bounce_loss*vy;
        // vz = -bounce_loss*vz;
      }
    }
    else {//center
      if (bx < minx) {//below
        // vx = -bounce_loss*vx;
        vy = -bounce_loss*vy;
      }
      else if (bx > maxx) {//above
        // vx = -bounce_loss*vx;
        vy = -bounce_loss*vy;
      }
      else {//middle
        vy = -bounce_loss*vy;

        //Special case: Ball landing on top of the box: this allows it to roll
        if (vy < .01 && vy > -.01) {//if magnitude of vy is low enough just roll
          vy = 0;
          by = maxy + 1.0;
        }


      }
    }
  }
  else {//level
    if (bz < minz) {//left
      if (bx < minx) {//below
        vx = -bounce_loss*vx;
        vz = -bounce_loss*vz;
      }
      else if (bx > maxx) {//above
        vx = -bounce_loss*vx;
        vz = -bounce_loss*vz;
      }
      else {//middle
        vz = -bounce_loss*vz;
      }
    }
    else if (bz > maxz) {//right
      if (bx < minx) {//below
        vx = -bounce_loss*vx;
        vz = -bounce_loss*vz;
      }
      else if (bx > maxx) {//above
        vx = -bounce_loss*vx;
        vz = -bounce_loss*vz;
      }
      else {//middle
        vz = -bounce_loss*vz;
      }
    }
    else {//center
      if (bx < minx) {//below
        vx = -bounce_loss*vx;
      }
      else if (bx > maxx) {//above
        vx = -bounce_loss*vx;
      }
      else {//middle
        //do nothing this is technically inside the box which shouldn't happen
        //just do nothing and hope for the best
      }
    }
  }
}

void box::draw() {
  boxTexture.draw();
}