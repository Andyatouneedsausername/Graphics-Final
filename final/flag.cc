#include "flag.h"

const mat4 poletransform = Scale(0.03, 1.5, 0.03);//these transform two cubes into a flag
const mat4 flagtransform = Scale(0.5, 0.2, 0.005)*Translate(0.5,3.2,0);

point4 disk_points[20];

void flag::draw() {
  flagpole.draw();
  flagflag.draw();

  mv = Translate(base_x,base_y,base_z)*Scale(4.0,4.0,4.0);
  glUniformMatrix4fv(model_view, 1, GL_TRUE, mv);
  glDrawArrays(GL_TRIANGLE_FAN, disk_initialpoint, 20);
}

void flag::init(vector<vec4> polecolors, vector<vec4> flagcolors, GLint nmodel_view, int cubeinitialpoint, bool ninit) {
  disk_initialpoint = points.size();
  float dtheta = 2.0*3.14159 / 18.0;

  disk_points[0] = vec4(0,0,0,1); 
  points.push_back(disk_points[0]); 
  colors.push_back(vec4(0,0,0,1));


  for (int i = 1; i < 20; i++) {
    disk_points[i] = vec4(0.5*cos(i*dtheta), 0.0, 0.5*sin(i*dtheta), 1.0 );
    points.push_back(disk_points[i]); colors.push_back(vec4(0.0,0.0,0.0,1.0));
  }


  flagpole.init(polecolors, nmodel_view, points.size(), ninit );
  flagflag.init(flagcolors, nmodel_view, points.size(), ninit );
  flagpole.set_mv(poletransform);//default to flag at origin
  flagflag.set_mv(flagtransform);
}

// void flag::set_mv(mat4 flagloc) {
//   flagpole.set_mv(flagloc*poletransform);
//   flagflag.set_mv(flagloc*flagtransform);
// }

bool flag::collide(point4 loc) {
  float r_squared = (bx-base_x)*(bx-base_x) + (by-base_y)*(by-base_y) + (bz-base_z)*(bz-base_z);
  return (r_squared < 2.0);
}

void flag::set_pos(vec4 loc) {
  mv = Translate(loc.x, loc.y + 6.6, loc.z)*RotateY(90.0)*Scale(10,10,10);
  flagpole.set_mv(mv*poletransform);
  flagflag.set_mv(mv*flagtransform);
  base_x = loc.x; base_y = loc.y + .1; base_z = loc.z;//copy location to base location
}

