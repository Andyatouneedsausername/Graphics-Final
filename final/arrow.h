#ifndef ARROW_H
#define ARROW_H

#include <vector>
#include "common.h"

extern point4 arrow_vertices[14]; 

class arrow {
  public:
  arrow() {};
  void draw();
  void init( vec4& color, GLint nmodel_view, int nInitial_point, bool ninit);
  void set_mv(mat4 nmv){
    mv = nmv;
  }

  private:
    mat4 mv;
    GLint model_view;
    int initial_point;
    int num_vertices;
};


#endif
