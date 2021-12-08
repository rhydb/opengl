#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include "stb_image.h"
#include "utils.h"
#include "input.h"

struct color_t {
    float r;
    float g;
    float b;
} bg = {
    .r = 128, .g = 128, .b = 128
};

unsigned int
loadshaders(const char* vertfile, const char* fragfile)
{
    GLuint vertID = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragID = glCreateShader(GL_FRAGMENT_SHADER);

    char *vertsrc = 0;
    char *fragsrc = 0;
    readfile(vertfile, &vertsrc);
    readfile(fragfile, &fragsrc);

    GLint result = GL_FALSE;
    int loglen;

    printf("Compiling shader: %s\n", vertfile);
    char const * vertsrcptr = vertsrc;
    glShaderSource(vertID, 1, &vertsrcptr, NULL);
    glCompileShader(vertID);

    glGetShaderiv(vertID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vertID, GL_INFO_LOG_LENGTH, &loglen);
    if (loglen > 0) {
        char log[loglen+1];
        glGetShaderInfoLog(vertID, loglen, NULL, log);
        printf("%s\n", log);
    }

    printf("Compiling shader: %s\n", fragfile);
    char const * fragsrcptr = fragsrc;
    glShaderSource(fragID, 1, &fragsrcptr, NULL);
    glCompileShader(fragID);

    glGetShaderiv(fragID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(fragID, GL_INFO_LOG_LENGTH, &loglen);
    if (loglen > 0) {
        char log[loglen+1];
        glGetShaderInfoLog(fragID, loglen, NULL, log);
        printf("%s\n", log);
    }

    printf("Linking program\n");
    GLuint programid = glCreateProgram();
    glAttachShader(programid, vertID);
    glAttachShader(programid, fragID);
    glLinkProgram(programid);

    glGetProgramiv(programid, GL_LINK_STATUS, &result);
    glGetProgramiv(programid, GL_INFO_LOG_LENGTH, &loglen);
    if (loglen > 0) {
        char log[loglen+1];
        glGetShaderInfoLog(programid, loglen, NULL, log);
        printf("Linking error: %s\n", log);
    }

    glDetachShader(programid, vertID);
    glDetachShader(programid, fragID);
    glDeleteShader(vertID);
    glDeleteShader(fragID);

    free(vertsrc);
    free(fragsrc);

    return programid;
}

void framebuffersize_cb(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int
main(int argc, char const *argv[])
{
    if (!glfwInit()) {
    	die("Failed to init glfw");
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    const int width = 1280;
    const int height = 720;
    const char* title = "C Engine";
   

    GLFWwindow* glfw_win  = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!glfw_win) {
        glfwTerminate();
        die("Failed to create GLFW window\n");
    }

    glfwSetFramebufferSizeCallback(glfw_win, framebuffersize_cb);
    glfwSetMouseButtonCallback(glfw_win, mousebtn_cb);
	glfwSetCursorPosCallback(glfw_win, mousepos_cb);
	glfwSetScrollCallback(glfw_win, scroll_cb);
	glfwSetKeyCallback(glfw_win, key_cb);

    glfwMakeContextCurrent(glfw_win);

    GLenum err = glewInit();
    if (err != GLEW_OK)
        die("Failed to init glew: %s\n", glewGetErrorString(err));

    printf("Status: Using GLEW %s & GL %s\n", glewGetString(GLEW_VERSION), glGetString(GL_VERSION));

    glViewport(0, 0, width, height);
    glfwSwapInterval(1);
    
    stbi_set_flip_vertically_on_load(true);  
    
    unsigned int program = loadshaders("vert.glsl", "frag.glsl");

float vertArr[] = {
    // positions            // colors                   // texture coords
    -0.5f, 0.5f, 0.0f,      0.0f, 1.0f, 0.0f, 1.0f,     0.0f, 1.0f, // Top left
    0.5f, 0.5f, 0.0f,       0.0f, 0.0f, 1.0f, 1.0f,     1.0f, 1.0f, // Top right
    -0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 0.0f, 1.0f,     0.0f, 0.0f, // Bottom left
    0.5f, -0.5f, 0.0f,      1.0f, 0.0f, 0.0f, 1.0f,     1.0f, 0.0f, // Bottom right
};
    int elementArr[] = {
        0, 1, 2,
        3, 1, 2,
    };
    
    unsigned int vaoID, vboID, eboID;

    glGenVertexArrays(1, &vaoID);
    glGenBuffers(1, &vboID);
    glGenBuffers(1, &eboID);

    glBindVertexArray(vaoID);

    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertArr), vertArr, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elementArr), elementArr, GL_STATIC_DRAW);

    int pos_size = 3;
    int colour_size = 4;
    int texture_size = 2;
    int vert_size = (pos_size + colour_size + texture_size) * (int)sizeof(float);

    glVertexAttribPointer(0, // position is 0 in shader, this must match the layout in the shader
                          pos_size, // size, how many vertices
                          GL_FLOAT, // type
                          GL_FALSE, // normalised?
                          vert_size, // stride
                          (void*)0 // offset
    );
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,
                          colour_size,
                          GL_FLOAT,
                          GL_FALSE,
                          vert_size,
                          (void*)(pos_size * sizeof(float))
    );
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2,
                          texture_size,
                          GL_FLOAT,
                          GL_FALSE,
                          vert_size,
                          (void*)((pos_size + colour_size) * sizeof(float))
                          );
    glEnableVertexAttribArray(2);

    unsigned int texture;
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   
    int texwidth, texheight, nrChannels;
    unsigned char *data = stbi_load("brickwall.jpg", &texwidth, &texheight, &nrChannels, 0);

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texwidth, texheight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        puts("Error loading image");
    }
    stbi_image_free(data);

    while (!glfwWindowShouldClose(glfw_win)) {
        glfwPollEvents();

        glClearColor(bg.r/255.0f, bg.g/255.0f, bg.b/255.0f, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUseProgram(program);
        glBindVertexArray(vaoID);

        glDrawElements(GL_TRIANGLES, sizeof(elementArr)/sizeof(float), GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(glfw_win);
    }
    glDeleteVertexArrays(1, &vaoID);
    glDeleteBuffers(1, &vboID);
    glDeleteBuffers(1, &eboID);
    glfwTerminate();
	return 0;
}