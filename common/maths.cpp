#include <common/maths.hpp>

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

// ROTATION1!!!
glm::mat4 Maths::rotate(float angle)
{
    // Convert angle to radians
    float radians = glm::radians(angle);

    // Create rotation matrix
    glm::mat4 rotation(1.0f);

    rotation[0][0] = glm::cos(radians);   // cos(theta)
    rotation[0][1] = -glm::sin(radians);  // -sin(theta)
    rotation[1][0] = glm::sin(radians);   // sin(theta)
    rotation[1][1] = glm::cos(radians);   // cos(theta)

    return rotation;
}


// ================== START OF MATH STUFF ==================
// 
// do i know if i actually need this for the assignment??
// mm. no not really but doesn't hurt to have it
// moved it here until further notice. i just needed it out the way whilst i work and stuff

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
std::cout << "I = " << glm::transpose(I) << "\n" << std::endl; */

// ================== END OF MATH STUFF ==================