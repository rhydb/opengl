#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

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

typedef struct camera_t camera;
struct camera_t {
    float yaw;
    float pitch;
    float movespeed;
    float sensitivity;
    vec3 pos;
    vec3 front;
    vec3 up;
} cam = {
    .pos = (vec3){0.0f, 0.0f, 3.0f},
    .front = (vec3){0.0f, 0.0f, -1.0f},
    .up = {0.0f, 1.0f, 0.0f},
    .yaw = GLM_PI_2, .pitch = 0.0f,
    .movespeed = 2.0f, .sensitivity = 0.1f
};

unsigned int loadshaders(const char* vertfile, const char* fragfile);
void framebuffersize_cb(GLFWwindow* window, int width, int height);

void mousepos(GLFWwindow *win, double x, double y);

int
main(int argc, char const *argv[])
{
    if (!glfwInit()) {
    	die("Failed to init glfw");
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    const int width = 800;
    const int height = 600;
    const char* title = "C Engine";
   

    GLFWwindow* glfw_win  = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!glfw_win) {
        glfwTerminate();
        die("Failed to create GLFW window\n");
    }
    glfwSetFramebufferSizeCallback(glfw_win, framebuffersize_cb);
    glfwSetMouseButtonCallback(glfw_win, mousebtn_cb);
	glfwSetCursorPosCallback(glfw_win, mousepos);
	glfwSetScrollCallback(glfw_win, scroll_cb);
	glfwSetKeyCallback(glfw_win, key_cb);

    glfwMakeContextCurrent(glfw_win);

    GLenum err = glewInit();
    if (err != GLEW_OK)
        die("Failed to init glew: %s\n", glewGetErrorString(err));

    printf("Status: Using GLEW %s & GL %s\n", glewGetString(GLEW_VERSION), glGetString(GL_VERSION));

    glfwSetInputMode(glfw_win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glViewport(0, 0, width, height);
    glfwSwapInterval(1);
    glEnable(GL_DEPTH_TEST);
    
    stbi_set_flip_vertically_on_load(true);  
    
    unsigned int program = loadshaders("vert.glsl", "frag.glsl");

float vertArr[] = {
    // positions            // colors                   // texture coords
    -0.5f, 0.5f, 0.0f,           0.0f, 1.0f, // Top left
    0.5f, 0.5f, 0.0f,            1.0f, 1.0f, // Top right
    -0.5f, -0.5f, 0.0f,          0.0f, 0.0f, // Bottom left
    0.5f, -0.5f, 0.0f,           1.0f, 0.0f, // Bottom right
};
    int elementArr[] = {
        0, 1, 2,
        3, 1, 2,
    };

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,   1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,   1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,   1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,   1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,   1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,   0.0f, 1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,    0.0f, 1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,    0.0f, 1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,    0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,   0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,   0.0f, 1.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,   0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,   0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,   0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,   0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,   0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,   0.0f, 0.0f, 1.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,    1.0f, 1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,    1.0f, 1.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,    1.0f, 1.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,    1.0f, 1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,    1.0f, 1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,    1.0f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,   1.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,    1.0f, 0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,    1.0f, 0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,    1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,   1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,   1.0f, 0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,   0.0f, 1.0f,  1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,    0.0f, 1.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,    0.0f, 1.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,    0.0f, 1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,   0.0f, 1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,   0.0f, 1.0f,  1.0f,
    };
    vec3 cubePositions[] = {
        { 0.0f,  0.0f,  0.0f},
        { 2.0f,  5.0f, -15.0f},
        {-1.5f, -2.2f, -2.5f},
        {-3.8f, -2.0f, -12.3f},
        { 2.4f, -0.4f, -3.5f},
        {-1.7f,  3.0f, -7.5f},
        { 1.3f, -2.0f, -2.5f},
        { 1.5f,  2.0f, -2.5f},
        { 1.5f,  0.2f, -1.5f},
        {-1.3f,  1.0f, -1.5f}
    };
    
    unsigned int vaoID, vboID, eboID;

    glGenVertexArrays(1, &vaoID);
    glGenBuffers(1, &vboID);
    // glGenBuffers(1, &eboID);

    glBindVertexArray(vaoID);

    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    int pos_size = 3;
    int colour_size = 3;
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
                          texture_size,
                          GL_FLOAT,
                          GL_FALSE,
                          vert_size,
                          (void*)((pos_size) * sizeof(float))
                          );
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2,
                          colour_size,
                          GL_FLOAT,
                          GL_FALSE,
                          vert_size,
                          (void*)((pos_size + texture_size) * sizeof(float))
    );
    glEnableVertexAttribArray(2);

    unsigned int texture;
    {
        glGenTextures(1, &texture);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
        int texwidth, texheight, nrChannels;
        unsigned char *data = stbi_load("gus.jpg", &texwidth, &texheight, &nrChannels, STBI_rgb_alpha);

        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texwidth, texheight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            puts("Error loading image");
        }
        stbi_image_free(data);
    }

    unsigned int texture2;
    {
        glGenTextures(1, &texture2);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
        int texwidth, texheight, nrChannels;
        unsigned char *data = stbi_load("awesomeface.png", &texwidth, &texheight, &nrChannels, STBI_rgb_alpha);

        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texwidth, texheight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            puts("Error loading image");
        }
        stbi_image_free(data);
    }

    glUseProgram(program);
    glUniform1i(glGetUniformLocation(program, "u_texture1"), 0);
    glUniform1i(glGetUniformLocation(program, "u_texture2"), 1);

    float dt = 0.0f;
    float lastframe = 0.0f;
    while (!glfwWindowShouldClose(glfw_win)) {
        float currentframe = glfwGetTime();
        dt = currentframe - lastframe;
        lastframe = currentframe;

        glfwPollEvents();

        if (input.keys[GLFW_KEY_W]) {
            vec3 movement;
            glm_vec3_scale(cam.front, cam.movespeed * dt, movement);
            glm_vec3_add(cam.pos, movement, cam.pos);
        }
        if (input.keys[GLFW_KEY_A]) {
            vec3 movement;
            glm_vec3_cross(cam.front, cam.up, movement);
            glm_normalize(movement);
            glm_vec3_scale(movement, cam.movespeed * dt, movement);
            glm_vec3_sub(cam.pos, movement, cam.pos);
        }
        if (input.keys[GLFW_KEY_S]) {
            vec3 movement;
            glm_vec3_scale(cam.front, cam.movespeed * dt, movement);
            glm_vec3_sub(cam.pos, movement, cam.pos);
        }
        if (input.keys[GLFW_KEY_D]) {
            vec3 movement;
            glm_vec3_cross(cam.front, cam.up, movement);
            glm_normalize(movement);
            glm_vec3_scale(movement, cam.movespeed * dt, movement);
            glm_vec3_add(cam.pos, movement, cam.pos);
        }
        if (input.keys[GLFW_KEY_LEFT]) {
            cam.front[0] -= 0.01f;
        }
        if (input.keys[GLFW_KEY_RIGHT]) {
            cam.front[0] += 0.01f;
        }
        static esc_state = 0;
        static esc = 1;
        if (input.keys[GLFW_KEY_ESCAPE]) {
            if (esc_state == 0) {
                esc_state = 1;
                esc ^= 1;
                glfwSetInputMode(glfw_win, GLFW_CURSOR, esc ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
            }
        } else {
            esc_state = 0;
        }
        cam.pos[1] = 0.0f;
        glClearColor(bg.r/255.0f, bg.g/255.0f, bg.b/255.0f, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        glUseProgram(program);
        glBindVertexArray(vaoID);

        mat4 projection;
        glm_perspective(glm_rad(90.0f), (float)width / (float)height, 0.1f, 100.0f, projection);
        glUniformMatrix4fv(glGetUniformLocation(program, "u_projection"), 1, GL_FALSE, projection[0]);

        mat4 view;
        vec3 center;
        glm_vec3_add(cam.pos, cam.front, center);
        glm_lookat(cam.pos, center, cam.up, view);
        glUniformMatrix4fv(glGetUniformLocation(program, "u_view"), 1, GL_FALSE, view[0]);


        // glDrawElements(GL_TRIANGLES, sizeof(elementArr)/sizeof(float), GL_UNSIGNED_INT, 0);
        for (unsigned int i = 0; i < 10; i++) {
            mat4 model = GLM_MAT4_IDENTITY_INIT;
            glm_translate(model, cubePositions[i]);
            float angle = glfwGetTime() +  20.0f * i;
            glm_rotate(model, angle, (vec3){1.0f, 0.3f, 0.5f});
            glUniformMatrix4fv(glGetUniformLocation(program, "u_model"), 1, GL_FALSE, model[0]);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glfwSwapBuffers(glfw_win);
    }
    glDeleteVertexArrays(1, &vaoID);
    glDeleteBuffers(1, &vboID);
    glDeleteBuffers(1, &eboID);
    glfwTerminate();
	return 0;
}

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

void mousepos(GLFWwindow *win, double x, double y)
{
    mousepos_cb(win, x, y);

    float xoffset = input.mouse.x - input.mouse.last_x;
    float yoffset = input.mouse.y - input.mouse.last_y;
    xoffset *= cam.sensitivity;
    yoffset *= cam.sensitivity;
    cam.yaw = fmod(cam.yaw + xoffset, 360.0f);
    cam.pitch -= yoffset;
    if (cam.pitch < -89.0f)
        cam.pitch = -89.0f;
    if (cam.pitch > 89.0f)
        cam.pitch = 89.0f;

    cam.front[0] = cos(glm_rad(cam.yaw)) * cos(glm_rad(cam.pitch));
    cam.front[1] = sin(glm_rad(cam.pitch));
    cam.front[2] = sin(glm_rad(cam.yaw) * cos(glm_rad(cam.pitch)));
    glm_normalize(cam.front);
}