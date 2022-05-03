#pragma once

#include <cglm/cglm.h>

unsigned int loadshaders(const char* vertfile, const char* fragfile);

void set_int(unsigned int id, const char *name, int value);
void set_float(unsigned int id, const char *name, float value);
void set_vector2f(unsigned int id, const char *name, float x, float y);
void set_vector3f(unsigned int id, const char *name, float x, float y, float z);
void set_vector4f(unsigned int id, const char *name, float x, float y, float z, float w);
void set_matrix4f(unsigned int id, const char *name, const mat4 *matrix);

