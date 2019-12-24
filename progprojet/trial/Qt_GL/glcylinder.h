#ifndef GLCYLINDER_H
#define GLCYLINDER_H

#include <QGLBuffer>
#include <QGLShaderProgram>

class GLCylinder
{
public:
 GLCylinder()
   : vbo(QGLBuffer::VertexBuffer), ibo(QGLBuffer::IndexBuffer)
 {height =1.0;
 radius=1.0;}

  void initialize(GLuint slices = 26, GLuint stacks = 26);

  void draw(QGLShaderProgram& program, int attributeLocation);

  void bind();
  void release();
	void set_dimension(double r, double h){
		height = h; radius = r;
	}

private:
  QGLBuffer vbo, ibo;
  GLuint vbo_sz;
  GLuint ibo_sz[3];
	double height;
	double radius;
};

#endif // GLCylinder_H
