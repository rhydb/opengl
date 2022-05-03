#include <GL/glew.h>

#include "shader.h"
#include "utils.h"

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

