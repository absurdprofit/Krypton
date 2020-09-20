/*
Author: Nathan Johnson
Date: August 11, 2020
Email: nahtanjohn1l@gmail.com


This library of functions is aimed at abstracting away core functions that setup the application runtime.
All necessary OpenGL context creation code is in functions that can be called contiguously in main to setup
an OpenGL context. The main function of importance is the Init function, as this creates our window, renderer
and shader program.
*/

#include "KryptonUtil.h"
#include "KryptonMath.h"
#include <iostream>
#include <stdio.h>

#ifdef __EMSCRIPTEN__
#include <SDL.h>
#define GL_GLEXT_PROTOTYPES 1
#include <SDL_opengles2.h>

#else
#include <SDL2/SDL.h>
#define GL_GLEXT_PROTOTYPES 1
#include <SDL2/SDL_opengles2.h>
#endif

GLint CheckStatus(GLenum type, const GLuint *object)
{
    GLint successful;
    GLint infoLen = 0;
    char *infoLog = (char *)malloc(sizeof(char) * infoLen);
    if (type == GL_LINK_STATUS)
    {
        glGetProgramiv(*object, GL_LINK_STATUS, &successful);

        if (!successful)
        {
            glGetProgramiv(*object, GL_INFO_LOG_LENGTH, &infoLen);

            if (infoLen > 1)
            {
                glGetProgramInfoLog(*object, infoLen, NULL, infoLog);
                std::cout << "Error linking program: \n"
                    << infoLog << std::endl;

                free(infoLog);
            }
            glDeleteProgram(*object);
            return 0;
        }
        return 1;
    }
    else if (type == GL_COMPILE_STATUS)
    {
        glGetShaderiv(*object, type, &successful);
        if (!successful)
        {
            glGetShaderiv(*object, GL_INFO_LOG_LENGTH, &infoLen);

            if (infoLen > 1)
            {
                glGetShaderInfoLog(*object, infoLen, NULL, infoLog);

                std::cout << "Error compiling shader:\n"
                    << infoLog << std::endl;
                free(infoLog);
            }

            glDeleteShader(*object);
            return 0;
        }
        return 1;
    }
    return -1;
}

// create a shader object, load the shader source, and compile the shader

GLuint LoadShader(GLenum type, const char *shaderName)
{
    GLuint shader;

    char *string;
    long length;
    FILE *f = fopen(shaderName, "rb");
    if (f == NULL)
    {
        std::cout << "Shader couldn't be opened" << std::endl;
    }
    else
    {
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        fseek(f, 0, SEEK_SET);
        string = (char *)calloc(sizeof(char), length + 1);
        if (string)
        {
            fread(string, sizeof(char), length, f);
        }
    }

    //create the shader object
    shader = glCreateShader(type);
    if (shader == 0)
        return 0;

    const GLchar *src = string;
    GLint strLength = -1;

    //load the shader source
    glShaderSource(shader, 1, &src, &strLength);

    //compile the shader
    glCompileShader(shader);

    //check the compile status
    if (CheckStatus(GL_COMPILE_STATUS, &shader))
    {
        return shader;
    }
    else
    {
        return 0;
    }

    free(string);
}

// shaders[0] vertex shader, shaders[1] fragment shader
//
GLuint* LoadProgram(GLuint *shaders)
{
    //create the program object
    GLuint* programObject = (GLuint*)calloc(sizeof(GLuint), 1);
    *programObject = glCreateProgram();

    if (*programObject == 0)
        return 0;

    glAttachShader(*programObject, shaders[0]);
    glAttachShader(*programObject, shaders[1]);

    //link the program
    glLinkProgram(*programObject);

    //check the link status
    if (CheckStatus(GL_LINK_STATUS, programObject))
    {
        return programObject;
    }
    else
    {
        return 0;
    }
}

//initialize shader and program the object
int Init(ContextData* ContextData)
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 16);
    //create window and opengl rendering context/renderer
    static SDL_Window *wnd(SDL_CreateWindow(ContextData->windowName, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ContextData->width, ContextData->height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN));
    ContextData->wnd = wnd;

    static SDL_GLContext glContext = SDL_GL_CreateContext(wnd);
    ContextData->glContext = &glContext;

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetSwapInterval(0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    
    static SDL_Renderer *renderer = SDL_CreateRenderer(wnd, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
    ContextData->renderer = renderer;
    

    //load the vertex/fragment shaders
    GLuint vertexShader;
    GLuint fragmentShader;
    vertexShader = LoadShader(GL_VERTEX_SHADER, "shaders/shader.vert");
    fragmentShader = LoadShader(GL_FRAGMENT_SHADER, "shaders/shader.frag");

    GLuint *shaders = (GLuint *)calloc(sizeof(GLuint), 2);
    shaders[0] = vertexShader;
    shaders[1] = fragmentShader;


    ContextData->programObjects = LoadProgram(shaders);
    ContextData->programObjectCounter += 1;

    //free shaders, since no garbage collection as of August 11, 2020
    free(shaders);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    //set the viewport
    glViewport(0, 0, ContextData->width, ContextData->height);

    //use the program object
    glUseProgram(*ContextData->programObjects);

    //set the ortho matrix in the vertex shader
    Matrix<float, 4, 4> orthoMatrix = glOrtho(ContextData->height, 0.0f, 0.0f, ContextData->width, 0.0f, 1000.0f);
    int uniformLoc = glGetUniformLocation(*ContextData->programObjects, "projectionMatrix");
    glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, orthoMatrix.data());
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);
    glEnable(GL_SAMPLE_COVERAGE);
    glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
    return 1;
}

GLvoid _resize(ContextData* ContextData, int w, int h) {
    ContextData->width = w;
    ContextData->height = h;
    SDL_SetWindowSize(ContextData->wnd, ContextData->width, ContextData->height);
    glViewport(0, 0, ContextData->width, ContextData->height);
}