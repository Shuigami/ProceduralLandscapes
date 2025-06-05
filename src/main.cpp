#include "settings.h"
#include "program.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <iostream>

int main(int argc, char** argv) {
    if (!init_glut(argc, argv)) return -1;
    if (!init_glew()) return -1;
    if (!init_gl()) return -1;
    if (!init_shader()) return -1;
    if (!init_object()) return -1;
    if (!init_pov()) return -1;

    glutMainLoop();

    return 0;
}