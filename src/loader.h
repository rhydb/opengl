#pragma once

typedef struct rawmodel rawmodel_t;
struct rawmodel {
    int vaoid;
    int vertcount;
    int textureid;
};

rawmodel_t loadtovao(float positions[], int possize, int indices[], int indicessize);
int loadtexture(const char* file);
void rendermodel(rawmodel_t model);