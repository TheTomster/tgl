#pragma once

#include <GL3/gl3w.h>
#include <GL/glfw.h>

typedef struct {
  const char ** filenames;
  unsigned int nfiles;
} tgl_shaderset;

GLuint tgl_make_shader(GLenum type, tgl_shaderset ss);

GLuint tgl_make_program(
    GLuint vertex_shader, GLuint geom_shader, GLuint fragment_shader);
