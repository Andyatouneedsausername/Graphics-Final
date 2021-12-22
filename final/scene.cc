//
// Perspective view of several objects using LookAt() and Perspective()
//
// Colors are assigned to each vertex and then the rasterizer interpolates
//   those colors across the triangles.
//
// Modified by David Chelberg
// last-modified: Fri Nov  1 11:42:30 2019
// Added classes for sphere, cube
// Many mods throughout, including using vectors for points, colors,
// and normals.
//
// last-modified: Fri Oct 28 14:06:29 2016
// Added an animated rotation to the code.
// Separated the transform for the camera (camera_view) from the
//   object (model_view).
// Added code to draw a "door"
// Added code to draw a moving second cube (animated moving back and
//   forth in a straight line).
// Added code to account for elapsed time in the animations.
//
// Modified by David Chelberg
// last-modified: Wed Oct  2 10:54:10 2019
// Moved more common code to common.cc and common.h

#include <Angel.h>
#include "common.h"
#include "cube.h"
#include "sphere.h"
#include "flag.h"
#include "arrow.h"
#include "box.h"
#include "slope.h"

int current_level = 0;
GLuint TIME_DELAY=1000/60;
// Implementing a door that opens by swinging
GLfloat doorAngle = 0.0;       // Angle of cube's door's rotation.
GLfloat doorAngleIncr = 30.0;   // Amount to increment Angle of cube's
			       // door's rotation.
GLfloat trans = 0.0;   // trans of 2nd cube.
GLfloat transinc = 1.0;// trans increment of 2nd cube.


sphere mysphere;
flag myflag;
cube walls[4];
cube myfloor;
arrow myarrow;


void advance_level();

int number_of_slopes = 1;
const int TOTAL_SLOPES = 1;
int number_of_boxes = 1;
const int TOTAL_BOXES = 1;
box mybox;
slope myslope;
slope secondslope;
box secondbox;

//----------------------------------------------------------------------------
// OpenGL initialization
void init()
{
  // Create a vertex array object
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // Create and initialize a buffer object
  GLuint buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);

  // Load shaders and use the resulting shader program
  GLuint program = InitShader("vshader41.glsl", "fshader41.glsl");
  glUseProgram(program);

  model_view = glGetUniformLocation(program, "model_view");
  camera_view = glGetUniformLocation(program, "camera_view");
  projection = glGetUniformLocation(program, "projection");
  shade_loc = glGetUniformLocation(program, "shade");
  fog_loc = glGetUniformLocation(program, "fog");
  fogfactor_loc = glGetUniformLocation(program, "fogfactor");
  dark_loc = glGetUniformLocation(program, "dark");
    
  // Pass the inital values
  glUniform1f(fogfactor_loc, fogfactor);
  glUniform1f(fog_loc, fogp);
  glUniform1f(dark_loc, darkp);

  // First set up all the models

  mysphere.init(vec4(0.8, 0.8, 0.8, 1), model_view, points.size(), false);

  //flagpole init
  vector<vec4> polecolors{vec4(0.1,0.1,0.1,1), vec4(0.1,0.1,0.1,1), vec4(0.1,0.1,0.1,1),
                            vec4(0.1,0.1,0.1,1), vec4(0.1,0.1,0.1,1), vec4(0.1,0.1,0.1,1)};
  vector<vec4> flagcolors{vec4(1.0,0.0,0.0,1), vec4(1.0,0.0,0.0,1), vec4(1.0,0.0,0.0,1),
                          vec4(1.0,0.0,0.0,1), vec4(1.0,0.0,0.0,1), vec4(1.0,0.0,0.0,1)};
  myflag.init(polecolors, flagcolors, model_view, points.size(), false);
  myflag.set_pos(vec4(20,5.0,0.0,1.0));
  //flag init
  vec4 arrowcolor = vec4(1.0,0.0,0.0,1); 
  myarrow.init(arrowcolor, model_view, points.size(), false);

  //wall objects init
  vector<vec4> wall_colors{vec4(0.5,1.0,0.5), vec4(0.5,1.0,0.5), vec4(0.5,1.0,0.5), vec4(0.5,1.0,0.5), vec4(0.5,1.0,0.5), vec4(0.5,1.0,0.5)};
  int initial = points.size();//all walls use the same model
  walls[0].init(wall_colors, model_view, initial, false);
  walls[1].init(wall_colors, model_view, initial, true);//don't need to re-init points
  initial = points.size();
  //Color of second pair of walls
  vector<vec4> more_wall_colors{vec4(0.4,1.0,0.5), vec4(0.4,1.0,0.5), vec4(0.4,1.0,0.5), vec4(0.4,1.0,0.5), vec4(0.4,1.0,0.5), vec4(0.4,1.0,0.5)};
  walls[2].init(more_wall_colors,model_view, initial, false);
  walls[3].init(more_wall_colors, model_view, initial, true);

  //Floor init
  vector<vec4> floor_color{vec4(0.6,1.0,0.6),vec4(0.6,1.0,0.6), vec4(0.6,1.0,0.6), vec4(0.6,1.0,0.6), vec4(0.6,1.0,0.6), vec4(0.6,1.0,0.6)};
  myfloor.init(floor_color, model_view, points.size(), false);
  myfloor.set_mv(Scale(map_max*1.99, 0.2, 1.99*map_max)*Translate(0,-0.5,0));

  //Move each wall into place
  walls[0].set_mv(Translate(map_max,0,0)*Scale(1.0, 2.0*map_max, 2.0*map_max)*Translate(0.5, 0.4, 0));
  walls[1].set_mv(Translate(-map_max,0,0)*Scale(1.0, 2.0*map_max, 2.0*map_max)*Translate(-0.5, 0.4, 0));
  walls[2].set_mv(Translate(0,0, map_max)*Scale( 2.0*map_max, 2.0*map_max, 1)*Translate( 0, 0.4, 0.5));
  walls[3].set_mv(Translate(0,0, -map_max)*Scale( 2.0*map_max, 2.0*map_max, 1)*Translate( 0, 0.4, -0.5));


  //init box
  vector<vec4> slope_colors{vec4(0.0,0.5,0.0,1.0), vec4(0.0,0.5,0.0,1.0), vec4(0.0,0.5,0.0,1.0), vec4(0.0,0.5,0.0,1.0), vec4(0.0,0.5,0.0,1.0)};
  // myslope[0].init(slope_colors, model_view, points.size(), false);
  myslope.init(slope_colors, model_view, points.size(), false);

  //Move slope to location in first level
  myslope.set_mv(-10, 0, -10, 10, 5, 10, 0.0 );
  //Move box to location  in first level
  mybox.init(10,0,-10,30,5,10);

  secondslope.init(slope_colors, model_view, points.size(), false);
  secondslope.set_mv(10, 5, -20, 30, 10, -10, -90.0 );//This location is in the second level as the slope is not added to game until then
  secondbox.init(10,0,-10,30,10,10);
  




  // Now send the data to the GPU
  // set up vertex arrays
  GLuint vPosition = glGetAttribLocation(program, "vPosition");
  glEnableVertexAttribArray(vPosition);
  glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(0));

  GLuint vColor = glGetAttribLocation(program, "vColor"); 
  glEnableVertexAttribArray(vColor);
  glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(points.size()*sizeof(vec4)));
  glBufferData(GL_ARRAY_BUFFER, points.size()*sizeof(vec4) + colors.size()*sizeof(vec4),
	       NULL, GL_STATIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, points.size()*sizeof(vec4), points[0]);
  glBufferSubData(GL_ARRAY_BUFFER, points.size()*sizeof(vec4), colors.size()*sizeof(vec4), colors[0]);

  glEnable(GL_DEPTH_TEST);
  // glClearColor(1.0, 1.0, 1.0, 1.0);
     glClearColor(0.2, 0.2, 1.0, 1.0); 
}

//----------------------------------------------------------------------------
extern "C" void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


  point4  eye(bx-cam_horizontal_dist*cos(phi), by + cam_vertical_dist, bz- cam_horizontal_dist*sin(phi), 1.0);
  point4  at(bx, by + 3, bz, 1.0);//camera will be focused on the ball's current position

  // point4  eye(mvx, mvy, mvz, 1.0);
  // point4  at(mvx+radius*sin(theta)*cos(phi), mvy+radius*cos(theta), mvz+radius*sin(theta)*sin(phi), 1.0);
  vec4    up(0.0, 1.0, 0.0, 0.0);

  mat4  cv = LookAt(eye, at, up);
  glUniformMatrix4fv(camera_view, 1, GL_TRUE, cv);

  mat4  p = Perspective(fovy, aspect, zNear, zFar);
  glUniformMatrix4fv(projection, 1, GL_TRUE, p);

  // Don't shade the cube by distance
  glUniform1i(shade_loc, false);
  //glUniform1i(shade_loc, true);
  mat4 mv;
  // mat4 mv = RotateZ(angle);
  // mycube.set_mv(mv);
  // mycube.draw();
  //draw flag
  // mv = Translate(2.0, 0.0, 0.0)*RotateY(90.0);
  // myflag.set_mv(mv);
  
  myflag.draw();

  
  mybox.draw();
  
  
  myslope.draw();
  
  if (current_level > 0) {
    secondslope.draw();//these objects are not in the first level
    secondbox.draw();
  }
  

  myfloor.draw();
  // walls[0].draw(); 
  for (int i = 0; i < 4; i++) {
    walls[i].draw();//draw each wall
  } 

  //if ball is not moving draw shot arrow
  if (!ball_moving) {
    mv = Translate(bx, by, bz)*RotateY( -1.0*(phi/DegreesToRadians) - 90)*RotateX(-90 - theta/DegreesToRadians)*Translate(0.0,0.0,1.5)*Scale(1.0,1.0,radius);
    myarrow.set_mv(mv);//If ball has stopped moving, render arrow to indicate player should move again
    myarrow.draw();
  }
  

  glUniform1i(shade_loc, true);

  mv = Translate(bx, by, bz);
  mysphere.set_mv(mv);
  mysphere.draw();

  
  glutSwapBuffers();
}

// Simple animation
GLint lasttime=0;
extern "C" void timer(GLint ignore)
{
  // Added code to account for glutElapsedTime
  GLint time = glutGet(GLUT_ELAPSED_TIME);
  GLint dt = time-lasttime;

 

  
  //ball motion handling
  if (ball_moving) {
    
    //change in x, y, and z this timestep
    float dx = vx*dt;
    float dy = vy*dt;
    float dz = vz*dt;


    
    if (mybox.collide(point4(bx + dx, by + dy, bz + dz, 1.0)) ) {
      mybox.handle_collision();
      dx = vx*dt;
      dy = vy*dt;//update changes
      dz = vz*dt;
    }
    

    
    if (myslope.collide(point4(bx + dx, by + dy, bz + dz, 1.0)) ) {
      myslope.handle_collision();
      dx = vx*dt;
      dy = vy*dt;//update changes
      dz = vz*dt;
    }
    

    if (current_level > 0) {
      if (secondbox.collide(point4(bx + dx, by + dy, bz + dz, 1.0)) ) {
      secondbox.handle_collision();
      dx = vx*dt;
      dy = vy*dt;//update changes
      dz = vz*dt;
      }

      if (secondslope.collide(point4(bx + dx, by + dy, bz + dz, 1.0)) ) {
        secondslope.handle_collision();
        dx = vx*dt;
        dy = vy*dt;//update changes
        dz = vz*dt;
      }
      
    
    }
    

    if (myflag.collide(point4(bx+dx,by+dy,bz+dz,1.0) ) ) {
      //End level and move to next
      advance_level();
    }
    
    //Collision With Walls of map and floor
    if ((bx + dx) > (map_max- 1.0) || (bx + dx) < (-map_max + 1) ) {
      //move in opposite direction
      vx = -bounce_loss*vx;
      dx = vx*dt;
    }
    bx += dx;   
    if ((bz +dz) > (map_max- 1.0) || (bz + dz) < (-map_max + 1) ) {
      
      vz = -bounce_loss*vz;
      dz = vz*dt;
    }
    bz += dz;
    if ( (by + dy) < 0.8 ) {
      //move in opposite direction
      vy = -bounce_loss*vy;

      if (vy < .01 && vy > -.01) {//if magnitude of vy is low enough just roll
        vy = 0;
        by = .8;
      }
      dy = vy*dt;

    }
    by += dy;

    vy -= .0000098*dt;//gravity







    //friction
    float ds = sqrt(dx*dx + dy*dy + dz*dz);

    if (abs(ds) > 0.001) {
      
      float loss_x = (dx/ds) * friction_constant;
      if ( abs(loss_x) > abs(dx) ) {
        vx = 0;
      }
      else {
        vx -= loss_x;
      }


      float loss_y = (dy/ds) * friction_constant;
      if ( abs(loss_y) > abs(dy) ) {
        vy = 0;
      }
      else {
        vy -= loss_y;
      }


      float loss_z = (dz/ds) * friction_constant;
      if ( abs(loss_z) > abs(dz) ) {
        vz = 0;
      }
      else {
        vz -= loss_z;
      }
    }
    else {
      vy = vx = vz = 0;
      ball_moving = false;
    }
    
    
    

    
  }



  // Do the animation code here in idle, not in display.
  //  Code for moving 2nd cube
  trans += transinc/1000.0*dt;
  if (trans > 1.5) {
    trans = 1.5;
    transinc*=-1;
  }
  if (trans < -1.5) {
    trans = -1.5;
    transinc*=-1;
  }

  lasttime=time;

  glutPostRedisplay();
  glutTimerFunc(TIME_DELAY, timer, 0);
}


//----------------------------------------------------------------------------
int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(1024, 1024);
  glutCreateWindow("Golf Game");

  glewInit();

  init();

  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(special);
  glutTimerFunc(TIME_DELAY, timer, 0);
  glutReshapeFunc(reshape);

  glutMainLoop();
  return(EXIT_SUCCESS);
}


void advance_level() {
  ball_moving = false;
  switch (current_level) {
    case 0:
      current_level++;
      bx = bz = -20;
      by = 0.8;
      number_of_slopes = 2;
      number_of_boxes = 1;
      mybox.set_mv(10, 0, -30, 30, 5, -10);
      
      myslope.set_mv(0,0,-30,10,5, -10, 0.0);
      
      myflag.set_pos(vec4(20,10,0,1));

    break;

    case 1:
      current_level++;
      bx = -23;
      by = 11.2;
      bz = 0;

      mybox.set_mv(-30,0,-10,-10,10,10);
      myslope.set_mv(-15, 10, -10,-10,15,10,0.0);
    break;

    case 2:
      std::cout << "Total Number of Shots:" << shot_count << "\n";
      exit(EXIT_SUCCESS);



    default:
    //do nothing if no level is prepared
    break;
  }
}
