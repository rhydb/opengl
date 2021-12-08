#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>


struct input_t;
extern struct input_t input;

void mousebtn_cb(GLFWwindow *win, int btn,  int action,  int mods);
void mousepos_cb(GLFWwindow *win, double x, double y);
void scroll_cb(GLFWwindow *win, double xoff, double yoff);
void key_cb(GLFWwindow *win, int key, int scancode, int action, int mods);