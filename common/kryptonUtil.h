#ifndef __KRYPTON_UTIL__
#define __KRYPTON_UTIL__
#ifdef __EMSCRIPTEN__

#define KryptonRaiseError() \
    std::cerr << "Error at line: " << __LINE__ << "\nIn file: " << __FILE__ << "\nIn function: " << __func__ << std::endl

#include <SDL.h>
#define GL_GLEXT_PROTOTYPES 1
#include <SDL_opengles2.h>

#else
#include <SDL2/SDL.h>
#define GL_GLEXT_PROTOTYPES 1
#include <SDL2/SDL_opengles2.h>
#endif

typedef struct ContextData
{
    int width;
    int height;
    SDL_Window *wnd;
    SDL_GLContext *glContext;
    SDL_Renderer *renderer;
    GLint programObjectCounter = 0;
    GLuint *programObjects;
    GLchar *windowName;
} ContextData;

typedef struct
{
    GLuint *programObject;
    void *attributeData;
    GLchar *attributeName;
    GLint location;
    GLint dimensions;
    GLint size;
    GLenum type;
    GLenum target;
    GLenum usage;
} GLattrib;

GLint CheckStatus(GLenum type, const GLuint *object);
GLuint LoadShader(GLenum type, const char *shaderName);
GLuint *LoadProgram(GLuint *shaders);
GLvoid LoadAttribute(GLattrib *attribute);
GLint Init(ContextData *ContextData);
GLvoid _resize(ContextData *ContextData, int w, int h);

#endif