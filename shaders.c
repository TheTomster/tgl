#include "shaders.h"

#include <GL3/gl3w.h>
#include <GL/glfw.h>

#include <stdlib.h>
#include <stdio.h>

static char *load_file(const char *filename, GLint *length);
static int compile_shader(GLuint shader);

// The buffer returned by load_file is malloc'd and needs to be free'd by the
// caller.
static char *load_file(const char *filename, GLint *length) {
  FILE *fd = fopen(filename, "r");
  if (fd == 0) {
    fprintf(stderr, "Unable to open %s for reading.\n", filename);
    return NULL;
  }
  GLint len;
  fseek(fd, 0, SEEK_END);
  len = ftell(fd);
  fseek(fd, 0, SEEK_SET);
  char *buffer = malloc(len + 1);
  len = fread(buffer, 1, len, fd);
  fclose(fd);
  buffer[len] = '\0';
  if (length != NULL) {
    *length = len;
  }
  return buffer;
}

static int compile_shader(GLuint shader) {
  glCompileShader(shader);

  GLint compiled;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
  if (!compiled) {
    fprintf(stderr, "Failed to compile shader:\n");
    GLint log_length;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
    char *log = malloc(log_length + 1);
    glGetShaderInfoLog(shader, log_length + 1, NULL, log);
    fprintf(stderr, "%s", log);
    free(log);
    glDeleteShader(shader);
    return 0;
  }
  return 1;
}

GLuint tgl_make_shader(GLenum type, struct tgl_shaderset ss) {
  GLuint shader;
  shader = glCreateShader(type);

  const char *sources[ss.nfiles];
  for (int i = 0; i < ss.nfiles; i++) {
    sources[i] = load_file(ss.filenames[i], NULL);
  }
  glShaderSource(shader, ss.nfiles, sources, NULL);
  int ok = compile_shader(shader);

  for (int i = 0; i < ss.nfiles; i++) {
    free((void *)sources[i]);
  }

  if (!ok)
    return 0;
  return shader;
}

GLuint tgl_make_program(
    GLuint vertex_shader, GLuint geom_shader, GLuint fragment_shader) {
  GLuint program = glCreateProgram();
  if (program == 0) {
    fprintf(stderr, "Error creating program\n");
    return 0;
  }
  if (geom_shader != 0) {
    glAttachShader(program, geom_shader);
  }
  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);
  glLinkProgram(program);

  GLint linked;
  glGetProgramiv(program, GL_LINK_STATUS, &linked);
  if (!linked) {
    fprintf(stderr, "Failed to link program\n");
    GLint log_length;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
    char *log = malloc(log_length + 1);
    glGetProgramInfoLog(program, log_length + 1, NULL, log);
    fprintf(stderr, "%s", log);
    free(log);
    glDeleteProgram(program);
    return 0;
  }
  return program;
}
