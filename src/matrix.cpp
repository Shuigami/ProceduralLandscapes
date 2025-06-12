#include "matrix.h"

#include <cmath>

Matrix4::Matrix4() {
    for (int i = 0; i < 16; ++i) {
        data[i] = 0.0f;
    }
}

Matrix4::Matrix4(const GLfloat* data) {
    for (int i = 0; i < 16; ++i) {
        this->data[i] = data[i];
    }
}

Matrix4::Matrix4(const Matrix4& other) {
    for (int i = 0; i < 16; ++i) {
        data[i] = other.data[i];
    }
}

Matrix4& Matrix4::operator=(const Matrix4& other) {
    if (this != &other) {
        for (int i = 0; i < 16; ++i) {
            data[i] = other.data[i];
        }
    }
    return *this;
}

Matrix4 Matrix4::operator*(const Matrix4& other) const {
    Matrix4 result;
    // Column-major matrix multiplication
    for (int col = 0; col < 4; ++col) {
        for (int row = 0; row < 4; ++row) {
            result.data[col * 4 + row] = data[0 * 4 + row] * other.data[col * 4 + 0] +
                                        data[1 * 4 + row] * other.data[col * 4 + 1] +
                                        data[2 * 4 + row] * other.data[col * 4 + 2] +
                                        data[3 * 4 + row] * other.data[col * 4 + 3];
        }
    }
    return result;
}

Matrix4 Matrix4::operator*(float scalar) const {
    Matrix4 result;
    for (int i = 0; i < 16; ++i) {
        result.data[i] = data[i] * scalar;
    }
    return result;
}

Matrix4 Matrix4::operator+(const Matrix4& other) const {
    Matrix4 result;
    for (int i = 0; i < 16; ++i) {
        result.data[i] = data[i] + other.data[i];
    }
    return result;
}

Matrix4 Matrix4::operator-(const Matrix4& other) const {
    Matrix4 result;
    for (int i = 0; i < 16; ++i) {
        result.data[i] = data[i] - other.data[i];
    }
    return result;
}

Matrix4 Matrix4::operator/(float scalar) const {
    Matrix4 result;
    for (int i = 0; i < 16; ++i) {
        result.data[i] = data[i] / scalar;
    }
    return result;
}

Matrix4 Matrix4::operator-() const {
    Matrix4 result;
    for (int i = 0; i < 16; ++i) {
        result.data[i] = -data[i];
    }
    return result;
}

void Matrix4::operator*=(const Matrix4& other) {
    *this = *this * other;
}

Matrix4 Matrix4::identity() {
    Matrix4 result;
    for (int i = 0; i < 16; ++i) {
        result.data[i] = (i % 5 == 0) ? 1.0f : 0.0f;
    }
    return result;
}

Matrix4 Matrix4::translation(const Vector3& v) {
    return translation(v.x, v.y, v.z);
}

Matrix4 Matrix4::translation(const GLfloat &x, const GLfloat &y, const GLfloat &z) {
    Matrix4 result = Matrix4::identity();
    result.data[12] = x;
    result.data[13] = y;
    result.data[14] = z;
    return result;
}

Matrix4 Matrix4::rotation(const GLfloat &angle, const Vector3& axis) {
    return rotation(angle, axis.x, axis.y, axis.z);
}

Matrix4 Matrix4::rotation(const GLfloat &angle, const GLfloat &x, const GLfloat &y, const GLfloat &z) {
    Matrix4 result;
    GLfloat c = std::cos(angle);
    GLfloat s = std::sin(angle);
    // Column-major order for OpenGL
    result.data[0] = c + (1 - c) * x * x;
    result.data[1] = (1 - c) * x * y + s * z;
    result.data[2] = (1 - c) * x * z - s * y;
    result.data[3] = 0.0f;

    result.data[4] = (1 - c) * y * x - s * z;
    result.data[5] = c + (1 - c) * y * y;
    result.data[6] = (1 - c) * y * z + s * x;
    result.data[7] = 0.0f;

    result.data[8] = (1 - c) * z * x + s * y;
    result.data[9] = (1 - c) * z * y - s * x;
    result.data[10] = c + (1 - c) * z * z;
    result.data[11] = 0.0f;

    result.data[12] = 0.0f;
    result.data[13] = 0.0f;
    result.data[14] = 0.0f;
    result.data[15] = 1.0f;

    return result;
}

Matrix4 Matrix4::scale(const Vector3& v) {
    return scale(v.x, v.y, v.z);
}

Matrix4 Matrix4::scale(const GLfloat &x, const GLfloat &y, const GLfloat &z) {
    Matrix4 result = Matrix4::identity();
    result.data[0] = x;
    result.data[5] = y;
    result.data[10] = z;
    return result;
}

Matrix4 Matrix4::lookAt(const Vector3& eye, const Vector3& center, const Vector3& up) {
    Vector3 f = (center - eye).normalized();
    Vector3 s = f.cross(up.normalized()).normalized();
    Vector3 u = s.cross(f);

    Matrix4 result;
    result.data[0] = s.x;
    result.data[1] = u.x;
    result.data[2] = -f.x;
    result.data[3] = 0.0f;

    result.data[4] = s.y;
    result.data[5] = u.y;
    result.data[6] = -f.y;
    result.data[7] = 0.0f;

    result.data[8] = s.z;
    result.data[9] = u.z;
    result.data[10] = -f.z;
    result.data[11] = 0.0f;

    result.data[12] = -s.dot(eye);
    result.data[13] = -u.dot(eye);
    result.data[14] = f.dot(eye);
    result.data[15] = 1.0f;

    return result;
}

Matrix4 Matrix4::frustum(const GLfloat &left, const GLfloat &right, const GLfloat &bottom, const GLfloat &top, const GLfloat &z_near, const GLfloat &z_far) {
    Matrix4 result;
    result.data[0] = (2 * z_near) / (right - left);
    result.data[1] = 0.0f;
    result.data[2] = 0.0f;
    result.data[3] = 0.0f;

    result.data[4] = 0.0f;
    result.data[5] = (2 * z_near) / (top - bottom);
    result.data[6] = 0.0f;
    result.data[7] = 0.0f;

    result.data[8] = (right + left) / (right - left);
    result.data[9] = (top + bottom) / (top - bottom);
    result.data[10] = -(z_far + z_near) / (z_far - z_near);
    result.data[11] = -1.0f;

    result.data[12] = 0.0f;
    result.data[13] = 0.0f;
    result.data[14] = -(2 * z_far * z_near) / (z_far - z_near);
    result.data[15] = 0.0f;

    return result;
}

std::ostream &operator<<(std::ostream &os, const Matrix4 &matrix) {
    for (int i = 0; i < 4; ++i) {
        os << "[ ";
        for (int j = 0; j < 4; ++j) {
            os << matrix.data[i * 4 + j] << " ";
        }
        os << "]" << std::endl;
    }
    return os;
}