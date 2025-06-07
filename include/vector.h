#pragma once

#include <iostream>

#include <GL/glew.h>

class Vector3 {
public:
    Vector3();
    Vector3(const GLfloat &x, const GLfloat &y, const GLfloat &z);
    Vector3(const Vector3& other);

    Vector3& operator=(const Vector3& other);
    Vector3 operator*(float scalar) const;
    Vector3 operator+(const Vector3& other) const;
    Vector3 operator-(const Vector3& other) const;
    Vector3 operator/(float scalar) const;
    Vector3 operator-() const;

    void operator*=(float scalar);
    void operator/=(float scalar);

    static Vector3 zero();
    static Vector3 one();

    void normalize();
    Vector3 normalized() const;
    Vector3 cross(const Vector3& other) const;
    GLfloat dot(const Vector3& other) const;

    friend std::ostream &operator<<(std::ostream &os, const Vector3 &vector);

    GLfloat x;
    GLfloat y;
    GLfloat z;
};

class Vector4 {
public:
    Vector4();
    Vector4(const GLfloat &x, const GLfloat &y, const GLfloat &z, const GLfloat &w);
    Vector4(const Vector4& other);

    Vector4& operator=(const Vector4& other);
    Vector4 operator*(float scalar) const;
    Vector4 operator+(const Vector4& other) const;
    Vector4 operator-(const Vector4& other) const;
    Vector4 operator/(float scalar) const;
    Vector4 operator-() const;

    void operator*=(float scalar);
    void operator/=(float scalar);

    static Vector4 zero();
    static Vector4 one();

    void normalize();
    Vector4 normalized() const;
    Vector4 cross(const Vector4& other) const;

    friend std::ostream &operator<<(std::ostream &os, const Vector4 &vector);

    GLfloat x;
    GLfloat y;
    GLfloat z;
    GLfloat w;
};