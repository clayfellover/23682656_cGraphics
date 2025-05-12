#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#include <common/maths.hpp>
#include <cmath>

// QUATERNION!!!!!
Quaternion::Quaternion(){}
Quaternion::Quaternion(const float w, const float x, const float y, const float z)
{
    this->w = w;
    this->x = x;
    this->y = y;
    this->z = z;
}

// QUATERNION MATRIX!!!!! (rotation)
glm::mat4 Quaternion::matrix()
{
    float s = 2.0f / (w * w + x * x + y * y + z * z);
    float xs = x * s, ys = y * s, zs = z * s;
    float xx = x * xs, xy = x * ys, xz = x * zs;
    float yy = y * ys, yz = y * zs, zz = z * zs;
    float xw = w * xs, yw = w * ys, zw = w * zs;

    glm::mat4 rotate;
    rotate[0][0] = 1.0f - (yy + zz);
    rotate[0][1] = xy + zw;
    rotate[0][2] = xz - yw;
    rotate[1][0] = xy - zw;
    rotate[1][1] = 1.0f - (xx + zz);
    rotate[1][2] = yz + xw;
    rotate[2][0] = xz + yw;
    rotate[2][1] = yz - xw;
    rotate[2][2] = 1.0f - (xx + yy);

    return rotate;
}

// CAMERA!!!!
Quaternion::Quaternion(const float pitch, const float yaw)
{
    float cosPitch = cos(0.5f * pitch);
    float sinPitch = sin(0.5f * pitch);
    float cosYaw = cos(0.5f * yaw);
    float sinYaw = sin(0.5f * yaw);

    this->w = cosPitch * cosYaw;
    this->x = sinPitch * cosYaw;
    this->y = cosPitch * sinYaw;
    this->z = sinPitch * sinYaw;
}

// SLERP!!!!!!
Quaternion Maths::SLERP(Quaternion q1, Quaternion q2, const float t)
{
    // Calculate cos(theta)
    float cosTheta = q1.w * q2.w + q1.x * q2.x + q1.y * q2.y + q1.z * q2.z;

    // If q1 and q2 are close together return q2 to avoid divide by zero errors
    if (cosTheta > 0.9999f)
        return q2;

    // Avoid taking the long path around the sphere by reversing sign of q2
    if (cosTheta < 0)
    {
        q2 = Quaternion(-q2.w, -q2.x, -q2.y, -q2.z);
        cosTheta = -cosTheta;
    }

    // Calculate SLERP
    Quaternion q;
    float theta = acos(cosTheta);
    float a = sin((1.0f - t) * theta) / sin(theta);
    float b = sin(t * theta) / sin(theta);
    q.w = a * q1.w + b * q2.w;
    q.x = a * q1.x + b * q2.x;
    q.y = a * q1.y + b * q2.y;
    q.z = a * q1.z + b * q2.z;

    return q;
}

// TRANSLATION!!
glm::mat4 Maths::translate(const glm::vec3& v)
{
    glm::mat4 translate(1.0f);
    translate[3][0] = v.x;
    translate[3][1] = v.y;
    translate[3][2] = v.z;

    return translate;
}

// SCALE TRANSLATION!!
glm::mat4 Maths::scale(const glm::vec3& v)
{
    glm::mat4 scale(1.0f);
    scale[0][0] = v.x;
    scale[1][1] = v.y;
    scale[2][2] = v.z;

    return scale;
}

// ROTATION!!!!
glm::mat4 Maths::rotate(const float& angle, glm::vec3 v)
{
    v = glm::normalize(v);
    float c = cos(0.5f * angle);
    float s = sin(0.5f * angle);
    Quaternion q(c, s * v.x, s * v.y, s * v.z);

    return q.matrix();
}

// RADIANS!!
float Maths::radians(float degrees)
{
    return degrees * (M_PI / 180.0f); 
}


// ================== START OF MATH STUFF ==================

// note: i moved it here so i would have more space in main file. as you saw, i needed it

// Defining vectors
/* printf("Vectors and Matrices\n");
glm::vec3 a, b;
a = glm::vec3(3.0f, 0.0f, 4.0f);
b[0] = 1.0f, b[1] = 2.0f, b[2] = 3.0f;

printf("Defining vectors:\n");
std::cout << "a = " << a << std::endl;
std::cout << "b = " << b << std::endl;

// Vector length
printf("\nVector length:\n");
printf("length(a) = %0.3f\n", glm::length(a));
printf("length(b) = %0.3f\n", glm::length(b));

// Normalising vectors
glm::vec3 aHat, bHat;
aHat = glm::normalize(a);
bHat = b / glm::length(b);

printf("\nNormalising vectors:\n");
std::cout << "aHat = " << aHat << std::endl;
std::cout << "bHat = " << bHat << std::endl;
printf("length(aHat) = %0.3f\n", glm::length(aHat));
printf("length(bHat) = %0.3f\n", glm::length(bHat));

// Arithmetic operations on vectors
printf("\nArithmetic operations on vectors:\n");
std::cout << "a + b = " << a + b << std::endl;
std::cout << "a - b = " << a - b << std::endl;

// The identity matrix
printf("\nThe identity matrix:\n");
glm::mat4 I;
std::cout << "I = " << glm::transpose(I) << "\n" << std::endl; 

// Inverse matrices
printf("\nInverse matrices:\n");
glm::mat2 invA = glm::inverse(A);
glm::mat2 invB = glm::inverse(B);

std::cout << "invA = " << glm::transpose(invA) << "\n" << std::endl;
std::cout << "invB = " << glm::transpose(invB) << "\n" << std::endl;
std::cout << "invA * A = " << glm::transpose(A * invA) << "\n" << std::endl;
std::cout << "invB * B = " << glm::transpose(B * invB) << "\n" << std::endl; */

// ================== END OF MATH STUFF ==================