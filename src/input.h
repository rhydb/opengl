#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>
#define MOUSE_BUTTON_COUNT 3


struct input_t {
    struct {
        float x;
        float y;
        float last_x;
        float last_y;
        float scroll_x;
        float scroll_y;
        bool dragging;
        bool buttons[MOUSE_BUTTON_COUNT];
    } mouse;

    bool keys[350];

};
extern struct input_t input;

void mousebtn_cb(GLFWwindow *win, int btn,  int action,  int mods);
void mousepos_cb(GLFWwindow *win, double x, double y);
void scroll_cb(GLFWwindow *win, double xoff, double yoff);
void key_cb(GLFWwindow *win, int key, int scancode, int action, int mods);