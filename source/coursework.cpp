#include <iostream>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/io.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
    const float vertices[] = {
        // Front
        -1, -1,  1,   1, -1,  1,   1,  1,  1,   -1,  1,  1,
        // Back
        -1, -1, -1,   1, -1, -1,   1,  1, -1,   -1,  1, -1,
        // Left
        -1, -1, -1,  -1, -1,  1,  -1,  1,  1,  -1,  1, -1,
        // Right
         1, -1, -1,   1, -1,  1,   1,  1,  1,   1,  1, -1,
         // Bottom
         -1, -1, -1,   1, -1, -1,   1, -1,  1,  -1, -1,  1,
         // Top
         -1,  1, -1,   1,  1, -1,   1,  1,  1,  -1,  1,  1,
    };

    // Define texture coordinates
    const float uv[] = {
        // 6 faces, 4 vertices per face
        0, 0,  1, 0,  1, 1,  0, 1,  
        0, 0,  1, 0,  1, 1,  0, 1,
        0, 0,  1, 0,  1, 1,  0, 1,
        0, 0,  1, 0,  1, 1,  0, 1,
        0, 0,  1, 0,  1, 1,  0, 1,
        0, 0,  1, 0,  1, 1,  0, 1,
    };

    // Define indices
    const unsigned int indices[] = {
       0, 1, 2,  2, 3, 0,       // front
       4, 5, 6,  6, 7, 4,       // back
       8, 9,10, 10,11, 8,       // left
      12,13,14, 14,15,12,       // right
      16,17,18, 18,19,16,       // bottom
      20,21,22, 22,23,20        // top
    };

    // Load and Use Shaders
    GLuint shaderProgram = LoadShaders("vertexShader.glsl", "fragmentShader.glsl");
    glUseProgram(shaderProgram);

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

    // Create texture buffer
    unsigned int uvBuffer;
    glGenBuffers(1, &uvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uv), uv, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0); // layout(location = 2)
    glEnableVertexAttribArray(2);

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

    int width, height, nChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("../assets/cobblestone.png", &width, &height, &nChannels, 0);
    if (data) {
        GLenum format = (nChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture\n";
    }
    stbi_image_free(data);

    // Tell the shader which texture unit to use
    glUseProgram(shaderProgram); 
    glUniform1i(glGetUniformLocation(shaderProgram, "textureMap"), 0); 

    // Projection/view/model setup (identity for now)
    glm::mat4 model = glm::mat4(1.0f); 
    glm::mat4 view = glm::mat4(1.0f); 
    glm::mat4 proj = glm::mat4(1.0f); 

    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model"); 
    GLuint viewLoc = glGetUniformLocation(shaderProgram, "view"); 
    GLuint projLoc = glGetUniformLocation(shaderProgram, "projection"); 

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); 
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view)); 
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj)); 
     
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

        // Use shader + bind
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        // Bind texture
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
