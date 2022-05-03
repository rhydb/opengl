#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "loader.h"

// TODO: keep tracks of created vao's, and free them later on

static int
createvao()
{
    int vaoid;
    glGenVertexArrays(1, &vaoid);
    glBindVertexArray(vaoid);
    return vaoid;
}

static void
storedatainattriblist(int attribno, float data[], int size)
{
    int vboid;
    glGenBuffers(1, &vboid);
    glBindBuffer(GL_ARRAY_BUFFER, vboid);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

static void
bindindicesbuffer(int indices[], int size)
{
    int vboid;
    glGenBuffers(1, &vboid);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboid);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
}

rawmodel_t
loadtovao(float positions[], int possize, int indices[], int indicessize)
{
    int vaoid = createvao();
    bindindicesbuffer(indices, indicessize);
    storedatainattriblist(0, positions, possize);
    glBindVertexArray(0);
    return (rawmodel_t){
        .vaoid = vaoid, 
        .vertcount = indicessize / sizeof indices[0]
    };
}

void
rendermodel(rawmodel_t model)
{
    glBindVertexArray(model.vaoid);
    glEnableVertexAttribArray(0);
    glDrawElements(GL_TRIANGLES, model.vertcount, GL_UNSIGNED_INT, 0);
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
} 