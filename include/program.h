#pragma once

#include <string>

#include <GL/glew.h>

class Program {
public:
  Program();
  ~Program();

  static Program* make_program(const std::string &vertex_shader, const std::string &fragment_shader);

  char *get_log();
  bool is_ready();
  void use();
  GLuint get_program() { return program; }

private:
  GLuint program;
  bool ready;
  char *log;
  void compile_shader(GLuint shader, const char* source);
  void link_program();
};