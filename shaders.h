#pragma once

#include <GL3/gl3w.h>
#include <GL/glfw.h>

struct tgl_shaderset {
  const char **filenames;
  unsigned int nfiles;
};

GLuint tgl_make_shader(GLenum type, struct tgl_shaderset ss);

GLuint tgl_make_program(
    GLuint vertex_shader, GLuint geom_shader, GLuint fragment_shader);
