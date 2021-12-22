#include "slope.h"
#include <algorithm>

point4 slope_vertices[6] = {
  point4(-0.5, -0.5, -0.5, 1.0),//bottom left point 0
  point4(0.5, -0.5, -0.5, 1.0),//bottom left corner 1
  point4(0.5, -0.5, 0.5, 1.0),//bottom right corner 2
  point4(-0.5, -0.5, 0.5, 1.0),//bottom right point 3
  point4(0.5, 0.5, -0.5, 1.0),//upper left point    4
  point4(0.5, 0.5, 0.5, 1.0)//upper right point     5
};


void slope_add_triangle(int a, int b, int c, vec4& color) {
  colors.push_back(color); points.push_back(slope_vertices[a]);
  colors.push_back(color); points.push_back(slope_vertices[b]);
  colors.push_back(color); points.push_back(slope_vertices[c]);
}

void slope_add_square(int a, int b, int c, int d, vec4 color) {
  slope_add_triangle(a,b,c,color);
  slope_add_triangle(a,c,d,color);
}


void init_slope(vector<vec4> ncolors) {
  slope_add_square(0,1,2,3, ncolors[0]);
  slope_add_square(1,2,4,5,ncolors[1]);
  slope_add_square(0,3,5,4,0.8*ncolors[2]);
  slope_add_triangle(0,1,4,ncolors[3]);
  slope_add_triangle(2,3,5,ncolors[4]);
}

void slope::draw()
{
  glUniformMatrix4fv(model_view, 1, GL_TRUE, mv);
  glDrawArrays(GL_TRIANGLES, Initial_Point, NumVertices);
}

void slope::set_mv(float nminx, float nminy, float nminz, float nmaxx, float nmaxy, float nmaxz, float norientation) {
    minx = nminx;
    miny = nminy;
    minz = nminz;
    maxx = nmaxx;
    maxy = nmaxy;
    maxz = nmaxz;
    orientation = norientation;

    float scalex = maxx - minx;
    float scaley = maxy - miny;
    float scalez = maxz - minz;


    float locx = (maxx + minx) / 2.0;
    float locy = (maxy + miny) / 2.0;
    float locz = (maxz + minz) / 2.0;

    mv = Translate(locx,locy,locz)*Scale(scalex, scaley, scalez)*RotateY(norientation);
}

void slope::init(vector<vec4> ncolors, GLint nmodel_view,int nInitial_Point, bool ninit)
{
  if (!ninit) {
    init_slope(ncolors);
  }
  NumVertices=24;

  model_view=nmodel_view;
  Initial_Point=nInitial_Point;
}

//assuming ball collides with bounding box and it completely over the slope does it collide
//this works because of similar triangles and a lot of trig
bool collide_on_slope(float x, float y, float w, float h, float r) {
  float dist_squared = ((y - (h*x/w) )*(y - (h*x/w) ) )/ ( ((h*h)/(w*w)) + 1);
  return( dist_squared < r*r); 
}

bool slope::collide(point4 loc, float radius) {
  //COLLISION OF SPHERE AND AABB
  float xtemp = std::max(minx, std::min(loc.x, maxx)) - loc.x;
  float ytemp = std::max(miny, std::min(loc.y, maxy)) - loc.y;
  float ztemp = std::max(minz, std::min(loc.z, maxz)) - loc.z;

  float distsquared = xtemp*xtemp + ytemp*ytemp + ztemp*ztemp;

  if (distsquared < (radius*radius) ) {
    //if inside AABB
    if (loc.y < miny) {
      return(true);
    }
    else {
      if (orientation == 0.0) {
        if (loc.z > maxz || loc.z < minz) {
          //We already know the sphere collides with AABB that contains slope so can determine radius of circle that would make contact with corner
          //if beyond the edge 
          float slope_y = (loc.x - minx)*(maxy-miny)/(maxx-minx);
          if (loc.y > slope_y) {//       x               y          w          h           r
            return(collide_on_slope(loc.x - minx, loc.y - miny, maxx-minx, maxy-miny, loc.y - slope_y));
          }
          else {
            return(true);
          }
        }
        else {
          return(collide_on_slope(loc.x - minx, loc.y - miny, maxx-minx, maxy-miny, radius));
        }
        
      }
      else if (orientation == -90.0) {
        if (loc.x > maxx || loc.x < minx) {
          float slope_y = (loc.z - minz)*(maxy-miny)/(maxz-minz);
          if (loc.y > slope_y) {//       x               y          w          h           r
            return(collide_on_slope(loc.z - minz, loc.y - miny, maxz-minz, maxy-miny, loc.y - slope_y));
          }
          else {
            return(true);
          }
        }
        else {
          return(collide_on_slope(loc.z - minz, loc.y - miny, maxz-minz, maxy-miny, radius));
        }
      }
      else if (orientation == 90.0) {
        if (loc.x > maxx || loc.x < minx) {
          float slope_y = (maxz -loc.z)*(maxy-miny)/(maxz-minz);
          if (loc.y > slope_y) {//       x               y          w          h           r
            return(collide_on_slope(maxz -loc.z, loc.y - miny, maxz-minz, maxy-miny, loc.y - slope_y));
          }
        }
        else {
          return(collide_on_slope(maxz -loc.z, loc.y - miny, maxz-minz, maxy-miny, radius));
        }
        
      }
      else if (orientation == 180.0) {
        if (loc.z > maxz || loc.z < minz) {
          float slope_y = (maxx- loc.x)*(maxy-miny)/(maxx-minx);
          if (loc.y > slope_y) {//       x               y          w          h           r
            return(collide_on_slope(maxx- loc.x, loc.y - miny, maxx-minx, maxy-miny, loc.y - slope_y));
          }
          else {
            return(true);
          }
        }
        else {
          return(collide_on_slope(maxx- loc.x, loc.y - miny, maxx-minx, maxy-miny, radius));
        }
      }
    }
  }
  return(false);
}

//assuming that collision will occur on next timestep, handle collision
void slope::handle_collision() {
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
  else {
    float slope_y;
    vec4 normal;//normal vector for slope
    if (orientation == 0.0) {
      slope_y = (bx - minx)*(maxy-miny)/(maxx-minx);
      normal = normalize(vec4(miny, maxx, 0, 1) - vec4(maxy,minx,0,1));
    }
    else if (orientation == 90.0) {
      slope_y = (maxz - bz)*(maxy-miny)/(maxz-minz);
      normal = normalize(vec4(0, maxz, maxy, 1) - vec4(0,minz,miny,1));
    }
    else if (orientation == -90) {
      slope_y = (bz- minz)*(maxy-miny)/(maxz-minz);
      normal = normalize(vec4(0, maxz, miny, 1) - vec4(0,minz,maxy,1));
    }
    else if (orientation == 180) {
      slope_y = (maxx - bx)*(maxy-miny)/(maxx-minx);
      normal = normalize(vec4(maxy, maxx, 0, 1) - vec4(miny,minx,0,1));
    }
    
    vec4 veloctiy = vec4(vx, vy, vz, 0.0);
    vec4 perpindicular = dot(veloctiy, normal)*normal;
    vec4 parallel = veloctiy - perpindicular;
    parallel = 0.9*parallel;//scale down parllel a bit 
    if ((by) > slope_y) {//top
      if (bz < minz) {//left
        if (bx < minx) {//below
          veloctiy = -bounce_loss*perpindicular + parallel;
          vx = veloctiy.x;
          vy = veloctiy.y;
          vz = veloctiy.z;
        }
        else if (bx > maxx) {//above
          veloctiy = -bounce_loss*perpindicular + parallel;
          vx = veloctiy.x;
          vy = veloctiy.y;
          vz = veloctiy.z;
        }
        else {//middle
          veloctiy = -bounce_loss*perpindicular + parallel;
          vx = veloctiy.x;
          vy = veloctiy.y;
          vz = veloctiy.z;
        }
      }
      else if (bz > maxz) {//right
        if (bx < minx) {//below
          veloctiy = -bounce_loss*perpindicular + parallel;
          vx = veloctiy.x;
          vy = veloctiy.y;
          vz = veloctiy.z;
        }
        else if (bx > maxx) {//above
          veloctiy = -bounce_loss*perpindicular + parallel;
          vx = veloctiy.x;
          vy = veloctiy.y;
          vz = veloctiy.z;
        }
        else {//middle
          veloctiy = -bounce_loss*perpindicular + parallel;
          vx = veloctiy.x;
          vy = veloctiy.y;
          vz = veloctiy.z;
        }
      }
      else {//center
        if (bx < minx) {//below
          veloctiy = -bounce_loss*perpindicular + parallel;
          vx = veloctiy.x;
          vy = veloctiy.y;
          vz = veloctiy.z;
        }
        else if (bx > maxx) {//above
          veloctiy = -bounce_loss*perpindicular + parallel;
          vx = veloctiy.x;
          vy = veloctiy.y;
          vz = veloctiy.z;
        }
        else {//middle
          veloctiy = -bounce_loss*perpindicular + parallel;
          vx = veloctiy.x;
          vy = veloctiy.y;
          vz = veloctiy.z;
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
          vx = -bounce_loss*vx;
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
}
