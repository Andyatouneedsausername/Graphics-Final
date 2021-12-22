

#include "arrow.h"
#include "common.h"

point4 arrow_vertices[14] = {
  point4(-0.2, 0.1, 0.0, 1.0),//bottom left of arrow 0
  point4(-0.2, 0.1, 0.8, 1.0),//inner left corner    1
  point4(-0.4, 0.1, 0.8, 1.0),//left corner of arrow 2
  point4(0, 0.1, 1.0, 1.0),//top point of arrow      3
  point4(0.4, 0.1, 0.8, 1.0),//right corner of arrow 4
  point4(0.2, 0.1, 0.8, 1.0),//inner right arrow     5
  point4(0.2, 0.1, 0.0, 1.0),//bottom right of arrow 6

  point4(-0.2, -0.1, 0.0, 1.0),//bottom left of arrow7
  point4(-0.2, -0.1, 0.8, 1.0),//inner left corner   8
  point4(-0.4, -0.1, 0.8, 1.0),//left corner of arrow9
  point4(0, -0.1, 1.0, 1.0),//top point of arrow     10
  point4(0.4, -0.1, 0.8, 1.0),//right corner of arrow11
  point4(0.2, -0.1, 0.8, 1.0),//inner right arrow    12
  point4(0.2, -0.1, 0.0, 1.0),//bottom right of arrow13
};

void add_triangle(int a, int b, int c, vec4& color) {
  colors.push_back(color); points.push_back(arrow_vertices[a]);
  colors.push_back(color); points.push_back(arrow_vertices[b]);
  colors.push_back(color); points.push_back(arrow_vertices[c]);
}

void add_square(int a, int b, int c, int d, vec4& color) {
  add_triangle(a,b,c,color);
  add_triangle(a,c,d,color);
}


void init_arrow(vec4& color) {
  //triangles for top of arrow
  add_triangle(2,3,4, color);
  add_square(0,1,5,6,color);
  //bottom of arrow
  add_triangle(9,10,11,color);
  add_square(7,8,12,13, color);
  //rectangles to connect two layers
  add_square(0,1,8,7,color);
  add_square(0,6,13,7,color);
  add_square(5,6,13,12,color);
  add_square(5,4,11,12,color);
  add_square(4,3,10,11,color);
  add_square(2,3,10,9,color);
  add_square(2,1,8,9,color);
}

void arrow::draw() {
  glUniformMatrix4fv(model_view, 1, GL_TRUE, mv);
  glDrawArrays(GL_TRIANGLES, initial_point, num_vertices);
}

void arrow::init( vec4& color, GLint nmodel_view, int nInitial_point, bool ninit) {
  if (!ninit) {
    init_arrow(color);
  }

  num_vertices = 60;

  model_view = nmodel_view;
  initial_point = nInitial_point;
}