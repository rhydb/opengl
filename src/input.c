#include "input.h"

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

} input = {
    .mouse = {
        .scroll_x = 0, .scroll_y = 0,
        .x = 0, .y = 0, .last_x = 0, .last_y = 0,
        .dragging = false,
    },
};

void
mousebtn_cb(GLFWwindow *win, int btn,  int action,  int mods)
{
    if (action == GLFW_PRESS) {
        if (btn < MOUSE_BUTTON_COUNT) {
            input.mouse.buttons[btn] = true;
        }
    } else if (action == GLFW_RELEASE) {
        if (btn < MOUSE_BUTTON_COUNT) {
            input.mouse.buttons[btn] = false;
        }
        input.mouse.dragging = true;
    }
}

void
mousepos_cb(GLFWwindow *win, double x, double y)
{
    input.mouse.last_x = input.mouse.x;
    input.mouse.last_y = input.mouse.y;
    input.mouse.x = x;
    input.mouse.y = y;
    input.mouse.dragging = input.mouse.buttons[0] || input.mouse.buttons[1] || input.mouse.buttons[2];
}

void
scroll_cb(GLFWwindow *win, double xoff, double yoff)
{
    input.mouse.scroll_x = xoff;
    input.mouse.scroll_y = yoff;
}

void
key_cb(GLFWwindow *win, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS) {
        input.keys[key] = true;
    } else if (action == GLFW_RELEASE) {
        input.keys[key] = false;
    }
}