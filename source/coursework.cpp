#include <iostream>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/io.hpp>

#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/maths.hpp>
#include <common/camera.hpp>
#include <common/model.hpp>
#include <common/light.hpp>

// Function prototypes
void keyboardInput(GLFWwindow *window);

int main( void )
{
    // GLFW / GLEW Setup 

    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    GLFWwindow* window;
    window = glfwCreateWindow(1024, 768, "Computer Graphics Coursework", NULL, NULL);
    
    if( window == NULL ){
        fprintf(stderr, "Failed to open GLFW window.\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        glfwTerminate();
        return -1;
    }
    // -------------------------------------------------------------------------
    // End of window creation
    // =========================================================================

    // Define vertex positions
    static const float vertices[] = {
        // x     y     z      index
        -0.5f, -0.5f, 0.0f,  // 0       
         0.5f, -0.5f, 0.0f,  // 1         
         0.5f,  0.5f, 0.0f,  // 2       
        -0.5f,  0.5f, 0.0f   // 3       
    };

    // Define texture coordinates
    static const float uv[] = {
        // u    v      index
        0.0f,  0.0f,  // 0
        2.0f,  0.0f,  // 1
        2.0f,  2.0f,  // 2
        0.0f,  2.0f,  // 3
    };

    // Define indices
    static const unsigned int indices[] = {
        0, 1, 2,  // lower-right triangle
        0, 2, 3   // upper-left triangle
    };

    // Load and Use Shaders
    GLuint shaderProgram = LoadShaders("vertexShader.glsl", "fragmentShader.glsl");
    glUseProgram(shaderProgram);

    // ================== START OF MATH STUFF ==================
    // 
    // do i know if i actually need this for the assignment??
    // mm. no not really but doesn't hurt to have it

    // Defining vectors
    printf("Vectors and Matrices\n");
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

    // ================== END OF MATH STUFF ==================

    // Create VAO
    GLuint VAO; 
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create VBO 
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); // layout(location = 0)
    glEnableVertexAttribArray(0);

    // vertex colours
    const float colours[] = {
        // R   G     B
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 0.0f
    };

    // Create colour buffer
    unsigned int colourBuffer;
    glGenBuffers(1, &colourBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colourBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colours), colours, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); // layout(location = 1)
    glEnableVertexAttribArray(1);

    // Create texture buffer
    unsigned int uvBuffer;
    glGenBuffers(1, &uvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uv), uv, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0); // layout(location = 2)
    glEnableVertexAttribArray(2);

    // EBO
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Load texture
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Texture wrapping options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    // Texture filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    const char* path = "../assets/cobblestone.png";
    int width, height, nChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nChannels, 0);

    if (data) {
        GLenum format = (nChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data); 
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Texture not loaded. Check the path." << std::endl;
    }

    stbi_image_free(data);
     
    // Input mode
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE); 

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        // inputs
        keyboardInput(window);

        // clear window
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // bind VAO and Texture
        glUseProgram(shaderProgram); 
        glBindVertexArray(VAO); 
        glUniform1i(glGetUniformLocation(shaderProgram, "textureMap"), 0); 
        glActiveTexture(GL_TEXTURE0); 
        glBindTexture(GL_TEXTURE_2D, texture); 

        // make triangles
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, 0);

        // swap buffers + process window events
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }
    
    // Close OpenGL window and terminate GLFW
    glfwTerminate();
    return 0;
}

void keyboardInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
