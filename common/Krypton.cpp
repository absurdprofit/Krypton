/*
Author: Nathan Johnson
Date: August 11, 2020
Email: nahtanjohn1l@gmail.com


The contents of this file represents Krypton Core. Fine details on how apps will eventually be built haven't
been finalised just yet, but the idea is that Krypton.h will be what manages a Krypton application. It will
draw to the canvas, process events, access web APIs, so on and so forth. It will also be where our UI data is
"retained" in some arbitrary data structure such as a widget tree (as implemented in Flutter).
*/
#ifndef __KRYPTON_H__
#define __KRYPTON_H__

#include "Krypton.h"
#include "KryptonMath.h"
#include <functional>
#include <iostream>
#include <emscripten.h>
#include <array>

Krypton::Krypton(ContextData* ContextData) {
    if (!Init(ContextData))
        exit(0);

    _contextData = ContextData;

    //allocate vertex and index buffers
    glGenBuffers(1, &_batchIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _batchIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numVertices * 6 * sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &_batchVBO);
    glBindBuffer(GL_ARRAY_BUFFER, _batchVBO);
    glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

    //attach vertex attributes
    int loc = glGetAttribLocation(*_contextData->programObjects, "a_position");
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, position));

    loc = glGetAttribLocation(*_contextData->programObjects, "a_color");
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, colour));

    //populate create _indices
    GLuint offset = 0;
    _indices.resize(numVertices * 6);
    for (int i = 0; i < numVertices; i+=6) {
        _indices[i + 0] = 0 + offset;
        _indices[i + 1] = 1 + offset;
        _indices[i + 2] = 2 + offset;

        _indices[i + 3] = 2 + offset;
        _indices[i + 4] = 3 + offset;
        _indices[i + 5] = 1 + offset;

        offset += 4;
    }
}

std::function<void()> loop;
void main_loop() {
    loop();
}
void Krypton::Draw() {

    running = true;

    loop = [&] {
        if (!running) {
            SDL_Quit();
        }
        //clear the colour buffer
        glClear(GL_COLOR_BUFFER_BIT);

        Update();

        glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, NULL);
        SDL_GL_SwapWindow(_contextData->wnd);
    };
    #ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(main_loop, 0, true);
    #else
    while (true)
        main_loop();
    #endif
}

//create a green quad with position (x, y), height h and width w
static std::array<Vertex, 4> createQuad(vec3f position, float w, float h) {
    Vertex v1;
    v1.position ={ position.x - w, position.y + h, position.z };
    v1.colour ={ 0.0f, 1.0f, 0.0f, 1.0f };
    Vertex v2;
    v2.position ={ position.x + w, position.y + h, position.z };
    v2.colour ={ 0.0f, 1.0f, 0.0f, 1.0f };
    Vertex v3;
    v3.position ={ position.x - w, position.y - h, position.z };
    v3.colour ={ 0.0f, 1.0f, 0.0f, 1.0f };
    Vertex v4;
    v4.position ={ position.x + w, position.y - h, position.z };
    v4.colour ={ 0.0f, 1.0f, 0.0f, 1.0f };

    return { v1, v2, v3, v4 };
}

void Krypton::Update() {

    Vertex v1;
    v1.position ={ 0.0f, (GLfloat)_contextData->height - ((GLfloat)_contextData->height / 3), 0.0f };
    v1.colour ={ 0.0f, 0.0f, 1.0f, 1.f };
    Vertex v2;
    v2.position ={ 0.0f, (GLfloat)_contextData->height, 0.0f };
    v2.colour ={ 0.0f, 0.0f, 1.0f, 1.0f };
    Vertex v3;
    v3.position ={ (GLfloat)_contextData->width, (GLfloat)_contextData->height - ((GLfloat)_contextData->height / 3), 0.0f };
    v3.colour ={ 0.0f, 0.0f, 1.0f, 1.0f };
    Vertex v4;
    v4.position ={ (GLfloat)_contextData->width, (GLfloat)_contextData->height, 0.0f };
    v4.colour ={ 0.0f, 0.0f, 1.0f, 1.0f };

    Vertex v5;
    v5.position ={ 0.0f, 0.0f, 0.0f };
    v5.colour ={ 1.0f, 0.0f, 0.0f, 1.0f };
    Vertex v6;
    v6.position ={ (GLfloat)_contextData->width, 0.0f, 0.0f };
    v6.colour ={ 1.0f, 0.0f, 0.0f, 1.0f };
    Vertex v7;
    v7.position ={ 0.0f, (GLfloat)_contextData->height / 3, 0.0f };
    v7.colour ={ 1.0f, 0.0f, 0.0f, 1.0f };
    Vertex v8;
    v8.position ={ (GLfloat)_contextData->width, (GLfloat)_contextData->height / 3, 0.0f };
    v8.colour ={ 1.0f, 0.0f, 0.0f, 1.0f };

    float x = (GLfloat)_contextData->width / 2;
    float y = (GLfloat)_contextData->height / 2;
    float z = 0;
    float w = 10.0f;
    float h = 10.0f;

    vec3f position = {x, y, z };
    auto quad = createQuad(position, w, h);



    _vertices ={
        v1, v2, v3, v4,
        v5, v6, v7, v8,
        quad[0], quad[1], quad[2], quad[3]
    };

    numIndices = 3 * 6;


    //bind index and vertex buffer on every tick
    glBindBuffer(GL_ARRAY_BUFFER, _batchVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, _vertices.size() * sizeof(Vertex), (const void*)_vertices.data());

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _batchIBO);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, _indices.size() * sizeof(GLuint), (const void*)_indices.data());

    glUseProgram(*_contextData->programObjects);

}

void Krypton::Clean() {
    running = false;
    free(_contextData->programObjects);
}
#endif