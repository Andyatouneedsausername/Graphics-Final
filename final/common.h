#ifndef COMMON_H
#define COMMON_H

#include <Angel.h>
#include <vector>
using std::vector;
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
// Globals to control moving around a scene.
extern GLfloat mvx;
extern GLfloat mvy;
extern GLfloat mvz;

extern int shot_count;


extern GLfloat bx;
extern GLfloat by;
extern GLfloat bz;
extern bool ball_moving;
extern GLfloat vx;
extern GLfloat vy;
extern GLfloat vz;
extern GLfloat launch_speed;

extern GLfloat cam_horizontal_dist;
extern GLfloat cam_vertical_dist;


extern GLfloat map_max;

extern GLfloat bounce_loss;
extern GLfloat friction_constant;

extern "C" void special(int key, int x, int y);

typedef Angel::vec4 color4;
typedef Angel::vec4 point4;

extern vector<point4> points;
extern vector<color4> colors;
extern vector<color4> normals;

// Viewing transformation parameters
extern GLfloat radius;
extern GLfloat radiusInit;

extern GLfloat theta;
extern GLfloat phi;

extern const GLfloat dr;      // Degrees to radian conversion

extern bool rotatep;          // whether to rotate or not
extern GLfloat angle;         // Angle of cube rotation.

extern GLint  camera_view;   // camera-view matrix uniform shader variable location
extern GLint  model_view;    // model-view matrix uniform shader variable location
extern GLint  shade_loc;     // shade uniform shader variable location
extern GLint  fog_loc;       // fog uniform shader variable location
extern GLint  fogfactor_loc; // fog uniform shader variable location
extern GLint  dark_loc;      // dark uniform shader variable location
extern GLint  projection;    // projection matrix uniform shader variable location

// Projection transformation parameters
extern GLfloat left, right;
extern GLfloat bottom, top;
extern GLfloat zNear, zFar;
extern GLfloat zNearInit, zFarInit;
extern bool fogp;
extern GLfloat fogfactor;  // Factor to control for depth factor in
			   // fog equation
extern GLfloat fogfactor_init;  // Initial value of factor to control
				// fog for depth
extern bool darkp;

extern GLfloat fovy;
extern GLfloat aspect;

extern int Index;          // Keep track of how many points we generate.

void MyQuad(int a, int b, int c, int d);
void colorcube(vector<vec4> colors);

extern "C" void reshape(int width, int height);
extern "C" void keyboard(unsigned char key, int x, int y);
extern "C" void idle();
void init();

#endif
