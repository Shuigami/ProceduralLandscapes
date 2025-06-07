#include "program.h"

#include <iostream>
#include <fstream>

Program::Program() {
    program = glCreateProgram();
}

Program::~Program() {
    glDeleteProgram(program);
}

Program* Program::make_program(const std::string &vertex_shader_path, const std::string &fragment_shader_path) {
    std::string vertex_shader;
    std::string fragment_shader;
    
    std::ifstream vertex_file(vertex_shader_path);
    if (vertex_file.is_open()) {
        std::string line;
        while (std::getline(vertex_file, line)) {
            vertex_shader += line + "\n";
        }
        vertex_file.close();
    } else {
        std::cerr << "Unable to open vertex shader file: " << vertex_shader_path << std::endl;
    }

    std::ifstream fragment_file(fragment_shader_path);
    if (fragment_file.is_open()) {
        std::string line;
        while (std::getline(fragment_file, line)) {
            fragment_shader += line + "\n";
        }
        fragment_file.close();
    } else {
        std::cerr << "Unable to open fragment shader file: " << fragment_shader_path << std::endl;
    }

    Program* program = new Program();
    program->compile_shader(GL_VERTEX_SHADER, vertex_shader.c_str());
    program->compile_shader(GL_FRAGMENT_SHADER, fragment_shader.c_str());
    program->link_program();
    return program;
}

void Program::compile_shader(GLuint shader_type, const char* source) {
    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        GLint log_length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
        log = new char[log_length];
        glGetShaderInfoLog(shader, log_length, nullptr, log);
        std::cerr << "Shader compilation failed, log: " << std::endl << log << std::endl;
        ready = false;
    } else {
        glAttachShader(program, shader);
    }
    glDeleteShader(shader);
}

void Program::link_program() {
    glLinkProgram(program);
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        GLint log_length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
        log = new char[log_length];
        glGetProgramInfoLog(program, log_length, nullptr, log);
        std::cerr << "Program linking failed, log: " << std::endl << log << std::endl;
        ready = false;
    } else {
        ready = true;
    }
}

char* Program::get_log() {
    return log;
}

bool Program::is_ready() {
    return ready;
}

void Program::use() {
    if (ready) {
        glUseProgram(program);
    } else {
        std::cerr << "Program not ready: " << log << std::endl;
    }
}
