#pragma once

#include <GL/gl.h>
#include <iostream>

#include "vector.h"

class Matrix4 {
public:
    Matrix4();
    Matrix4(const GLfloat* data);
    Matrix4(const Matrix4& other);

    Matrix4& operator=(const Matrix4& other);
    Matrix4 operator*(const Matrix4& other) const;
    Matrix4 operator*(float scalar) const;
    Matrix4 operator+(const Matrix4& other) const;
    Matrix4 operator-(const Matrix4& other) const;
    Matrix4 operator/(float scalar) const;
    Matrix4 operator-() const;

    void operator*=(const Matrix4& other);

    static Matrix4 identity();

    static Matrix4 translation(const Vector3& translation);
    static Matrix4 translation(const GLfloat &x, const GLfloat &y, const GLfloat &z);
    static Matrix4 rotation(const GLfloat &angle, const Vector3& axis);
    static Matrix4 rotation(const GLfloat &angle, const GLfloat &x, const GLfloat &y, const GLfloat &z);
    static Matrix4 scale(const Vector3& scale);
    static Matrix4 scale(const GLfloat &x, const GLfloat &y, const GLfloat &z);

    static Matrix4 lookAt(const Vector3& eye, const Vector3& center, const Vector3& up);
    static Matrix4 frustum(const GLfloat &left, const GLfloat &right, const GLfloat &bottom, const GLfloat &top, const GLfloat &z_near, const GLfloat &z_far);

    friend std::ostream &operator<<(std::ostream &os, const Matrix4 &matrix);

    GLfloat data[16];
};
