#ifndef FLAG_H
#define FLAG_H

#include <vector>
#include "common.h"
#include "cube.h"
#include "cube.h"

extern point4 disk_points[20];

class flag
{
  public:
    flag() {};

    void draw();
    void init(vector<vec4> polecolors, vector<vec4> flagcolors, GLint nmodel_view, int cubeinitialpoint, bool ninit);
    // void set_mv(mat4 flagloc);//not using this method for setting flag location anymore
    void set_pos(vec4 loc);


    bool collide(point4 loc);


  private:
    mat4 mv;
    cube flagpole;
    cube flagflag;
    float base_x, base_y, base_z;//location of base of flag

    int disk_initialpoint;
    

};






#endif