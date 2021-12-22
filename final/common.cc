#include "common.h"

const GLfloat dr = 5.0 * DegreesToRadians;

bool rotatep=false;          // whether to rotate or not

GLfloat theta = 3.14189/2.0;
GLfloat phi = 0.0;
GLfloat angle = 0.0;

GLfloat left = -1.0, right = 1.0;
GLfloat bottom = -1.0, top = 1.0;

// For perspective camera
GLfloat fovy=45;
GLfloat aspect=1.0;

// Camera and view parameters
GLfloat zNearInit = 11.8, zFarInit=300.0;
GLfloat zNear = zNearInit;
GLfloat zFar = zFarInit;
GLfloat radiusInit=15.0;
GLfloat radius=radiusInit;
bool fogp=false;         // Whether to use fog
bool darkp=false;        // Whether to use dark fog, and clear color
			 // to black.
GLfloat fogfactor=5.0;   // Factor to control for depth factor in
GLfloat fogfactor_init=5.0;// Factor to control for depth factor in
			 // fog equation


GLint  camera_view;// camera-view matrix uniform shader variable location
GLint  model_view; // model-view matrix uniform shader variable locatio
GLint  projection; // projection matrix uniform shader variable location
GLint  shade_loc;  // shade uniform shader variable location
GLint  fog_loc;    // fog uniform shader variable location
GLint  fogfactor_loc; // fog uniform shader variable location
GLint  dark_loc;    // dark uniform shader variable location

int Index = 0;      // Keeps track of which vertex we are on.

// Triangle data for the cube
vector<point4> points;
vector<color4> colors;
vector<color4> normals;

GLfloat mvx=0.0;
GLfloat mvy=0.0;
GLfloat mvz=0.0;

int shot_count = 0;

//Ball data
GLfloat bx = -20.0;
GLfloat by = 0.8;
GLfloat bz = 0.0;
bool ball_moving = false;
GLfloat vx = 0.0;
GLfloat vy = 0.0;
GLfloat vz = 0.0;
GLfloat launch_speed = 0.002*radius;


//camera distance from ball
GLfloat cam_horizontal_dist = 15;
GLfloat cam_vertical_dist = 5;

//Map Size
GLfloat map_max = 30.0;

GLfloat bounce_loss = 0.7;
GLfloat friction_constant = .00005;


GLfloat incr=0.8;      // movement increment
GLfloat incr_init=0.8; // Initial value of movement increment

//----------------------------------------------------------------------------
extern "C" void special(int key, int x, int y)
{
  switch(key) {
  case GLUT_KEY_UP:
    if (theta - .05 > 0) {
      theta -= .05;
    }
    else {
      theta = .05;
    }
    break;
  case GLUT_KEY_DOWN:
    if (theta + .05 < 3.14) {
      theta += .05;
    }
    else {
      theta = 3.14;
    }
    break;
  case GLUT_KEY_LEFT:
    phi -= .05;  
    break;
  case GLUT_KEY_RIGHT:
    phi +=  .05;
    break;
  }
}

//----------------------------------------------------------------------------
extern "C" void reshape(int width, int height)
{
  glViewport(0, 0, width, height);

  aspect = GLfloat(width)/height;
}


//----------------------------------------------------------------------------
extern "C" void keyboard(unsigned char key, int x, int y)
{
  switch(key) {
  case 033: // Escape Key
  case 'q': case 'Q':
    exit(EXIT_SUCCESS);
    break;

  case 13://Enter Key
    shot_count++;
    ball_moving = true;
    vx = launch_speed*cos(phi)*sin(theta);
    vz = launch_speed*sin(phi)*sin(theta);
    vy = launch_speed*cos(theta);
    break;
  case 'W':
  case 'w'://move forward
    mvx+=0.05*cos(phi);
    mvz+=0.05*sin(phi);
    break;

  case 'A':
  case 'a'://move left
    mvx+=0.05*sin(phi);
    mvz-=0.05*cos(phi);
    break;

  case 'S':
  case 's':
    mvx-=0.05*cos(phi);
    mvz-=0.05*sin(phi);
    break;

  case 'D':
  case 'd':
    mvx-=0.05*sin(phi);
    mvz+=0.05*cos(phi);
    break;

  

    // Speed up/slow down movements
  case '+':
    incr*=2.0;
    break;
  case '-':
    incr/=2.0;
    break;

  case 'x': left *= 1.1; right *= 1.1; break;
  case 'X': left /= 1.1; right /= 1.1; break;
    //  case 'y': bottom *= 1.1; top *= 1.1; break;
    //  case 'Y': bottom /= 1.1; top /= 1.1; break;
  case 'z': zNear  *= 1.1; zFar /= 1.1; break;
  case 'Z': zNear /= 1.1; zFar *= 1.1; break;
  case 'Y':
    mvy+=incr;
    break;
  case 'y':
    mvy-=incr;
    break;

  case 'r': radius *= 1.5; launch_speed = .002*radius;  break;
  case 'R': radius /= 1.5; launch_speed = .002*radius; break;

  case 'o': theta += dr; break;
  case 'O': theta -= dr; break;

  case 'p': phi += dr; break;
  case 'P': phi -= dr; break;

  case 'v': 
    fovy-=5; 
    if (fovy < 0) {
      // Min angle of view 1 degree
      fovy = 1;
    }
    break;
  case 'V': fovy+=5; break;
    if (fovy > 179) {
      // Max angle of view 179 degrees
      fovy = 179;
    }
    break;

  case 'l':
  case 'L':
    // By default don't turn on fog.
    darkp=!darkp;
    glUniform1i(dark_loc, darkp);
    if (darkp) {
      glClearColor(0,0,0,1);
    } else {
      glClearColor(1,1,1,1);
    }
    break;

  case 'b':
    fogfactor*=1.2;
    glUniform1f(fogfactor_loc, fogfactor);
    break;
  case 'B':
    fogfactor/=1.2;
    glUniform1f(fogfactor_loc, fogfactor);
    break;
    
  case 'f':
  case 'F':
    // By default don't turn on fog.
    fogp=!fogp;
    glUniform1i(fog_loc, fogp);
    break;

  case ' ':  // reset values to their defaults
    rotatep=!rotatep;
    incr=incr_init;
    left = -1.0;
    right = 1.0;
    bottom = -1.0;
    top = 1.0;
    zNear = zNearInit;
    zFar = zFarInit;
    mvx = 0.0;
    mvy = 0.0;
    mvz = 0.0;
    radius = radiusInit;
    theta  = 3.14169 / 2.0;
    phi    = 0.0;
    fogfactor=fogfactor_init;


    ball_moving = false;
    
    bx = -20.0;
    by = 0.8;
    bz = 0.0;
    
    vx = 0.0;
    vy = 0.0;
    vz = 0.0;




    break;
  }

  glutPostRedisplay();
}

// Simple animation
extern "C" void idle()
{
  // Code to animate cube goes here.
  if (rotatep) {
    angle+=1;
  }
  glutPostRedisplay();
}

