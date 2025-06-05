#include "vector.h"

#include <cmath>

Vector3::Vector3() {
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
}

Vector3::Vector3(const GLfloat &x, const GLfloat &y, const GLfloat &z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

Vector3::Vector3(const Vector3& other) {
    x = other.x;
    y = other.y;
    z = other.z;
}

Vector3& Vector3::operator=(const Vector3& other) {
    if (this != &other) {
        x = other.x;
        y = other.y;
        z = other.z;
    }
    return *this;
}

Vector3 Vector3::operator*(float scalar) const {
    return Vector3(x * scalar, y * scalar, z * scalar);
}

Vector3 Vector3::operator+(const Vector3& other) const {
    return Vector3(x + other.x, y + other.y, z + other.z);
}

Vector3 Vector3::operator-(const Vector3& other) const {
    return Vector3(x - other.x, y - other.y, z - other.z);
}

Vector3 Vector3::operator/(float scalar) const {
    return Vector3(x / scalar, y / scalar, z / scalar);
}

Vector3 Vector3::operator-() const {
    return Vector3(-x, -y, -z);
}

void Vector3::operator*=(float scalar) {
    x *= scalar;
    y *= scalar;
    z *= scalar;
}

void Vector3::operator/=(float scalar) {
    x /= scalar;
    y /= scalar;
    z /= scalar;
}

Vector3 Vector3::zero() {
    return Vector3(0.0f, 0.0f, 0.0f);
}

Vector3 Vector3::one() {
    return Vector3(1.0f, 1.0f, 1.0f);
}

void Vector3::normalize() {
    GLfloat length = std::sqrt(x * x + y * y + z * z);
    if (length > 0.0f) {
        x /= length;
        y /= length;
        z /= length;
    }
}

Vector3 Vector3::normalized() const {
    Vector3 result = *this;
    result.normalize();
    return result;
}

Vector3 Vector3::cross(const Vector3& other) const {
    return Vector3(
        y * other.z - z * other.y,
        z * other.x - x * other.z,
        x * other.y - y * other.x
    );
}

GLfloat Vector3::dot(const Vector3& other) const {
    return x * other.x + y * other.y + z * other.z;
}

std::ostream &operator<<(std::ostream &os, const Vector3 &vector) {
    os << "Vector3(" << vector.x << ", " << vector.y << ", " << vector.z << ")";
    return os;
}

Vector4::Vector4() {
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
    w = 1.0f;
}

Vector4::Vector4(const GLfloat &x, const GLfloat &y, const GLfloat &z, const GLfloat &w) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

Vector4::Vector4(const Vector4& other) {
    x = other.x;
    y = other.y;
    z = other.z;
    w = other.w;
}

Vector4& Vector4::operator=(const Vector4& other) {
    if (this != &other) {
        x = other.x;
        y = other.y;
        z = other.z;
        w = other.w;
    }
    return *this;
}

Vector4 Vector4::operator*(float scalar) const {
    return Vector4(x * scalar, y * scalar, z * scalar, w * scalar);
}

Vector4 Vector4::operator+(const Vector4& other) const {
    return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
}

Vector4 Vector4::operator-(const Vector4& other) const {
    return Vector4(x - other.x, y - other.y, z - other.z, w - other.w);
}

Vector4 Vector4::operator/(float scalar) const {
    return Vector4(x / scalar, y / scalar, z / scalar, w / scalar);
}

Vector4 Vector4::operator-() const {
    return Vector4(-x, -y, -z, -w);
}

void Vector4::operator*=(float scalar) {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    w *= scalar;
}

void Vector4::operator/=(float scalar) {
    x /= scalar;
    y /= scalar;
    z /= scalar;
    w /= scalar;
}

Vector4 Vector4::zero() {
    return Vector4(0.0f, 0.0f, 0.0f, 0.0f);
}

Vector4 Vector4::one() {
    return Vector4(1.0f, 1.0f, 1.0f, 1.0f);
}

void Vector4::normalize() {
    GLfloat length = std::sqrt(x * x + y * y + z * z + w * w);
    if (length > 0.0f) {
        x /= length;
        y /= length;
        z /= length;
        w /= length;
    }
}

Vector4 Vector4::normalized() const {
    Vector4 result = *this;
    result.normalize();
    return result;
}

Vector4 Vector4::cross(const Vector4& other) const {
    return Vector4(
        y * other.z - z * other.y,
        z * other.x - x * other.z,
        x * other.y - y * other.x,
        0.0f
    );
}

std::ostream &operator<<(std::ostream &os, const Vector4 &vector) {
    os << "Vector4(" << vector.x << ", " << vector.y << ", " << vector.z << ", " << vector.w << ")";
    return os;
}