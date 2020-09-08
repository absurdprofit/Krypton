/*
Author: Nathan Johnson
Date: August 11, 2020
Email: nahtanjohn1l@gmail.com


The contents of this file represents Krypton Core. Fine details on how apps will eventually be built haven't
been finalised just yet, but the idea is that Krypton.h will be what manages a Krypton application. It will
draw to the canvas, process events, access web APIs, so on and so forth. It will also be where our UI data is
"retained" in some arbitrary data structure such as a widget tree (as implemented in Flutter).

TODO:
1. Implement second buffer for rounded geometry.
2. Implement draw call for rounded buffer.
3. Implement basic shape made of 5 quads that can depict a square, a circle, or a rectangle.
    --- -------- ---
    |c1|    1   | c2|
    --- -------- ---
    |  |        |   |
    | 3|    2   | 4 |
    |  |        |   |
    --- -------- ---
    |c3|    5   | c4|
    --- -------- ---
    Quads 1-5 make up main structure of the element.
    Sections marked c1-c4 are possible rounded corners. If radius is 0, c1 and c2 are apart of quad 1,
    c3 and c4 are apart of quad 5.
*/
#ifndef __KRYPTON_H__
#define __KRYPTON_H__

#include "Krypton.h"
#include "KryptonMath.h"
#include <functional>
#include <iostream>
#include <emscripten.h>
#include <array>

Krypton::Krypton(ContextData *ContextData)
{
    if (!Init(ContextData))
        exit(0);

    _contextData = ContextData;

    GLuint size = ((maxVertices / (numRoundSegments + 1)) * ((numRoundSegments - 1) * 3) > (maxVertices / 4) * 6 ? (maxVertices / (numRoundSegments + 1)) * ((numRoundSegments - 1) * 3) : (maxVertices / 4) * 6);

    //allocate vertex and index buffers
    glGenBuffers(1, &_batchIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _batchIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &_batchVBO);
    glBindBuffer(GL_ARRAY_BUFFER, _batchVBO);
    glBufferData(GL_ARRAY_BUFFER, maxVertices * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

    //attach vertex attributes
    int loc = glGetAttribLocation(*_contextData->programObjects, "a_position");
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void *)offsetof(Vertex, position));

    loc = glGetAttribLocation(*_contextData->programObjects, "a_color");
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void *)offsetof(Vertex, colour));

    //populate create _quad_indices
    GLuint offset = 0;
    //number of quads * 6 indices that make up a quad
    _quad_indices.resize(size, 0);
    Vertex fill;
    _quad_vertices.resize(maxVertices, fill);
    for (int i = 0; i < size; i += 6)
    {
        _quad_indices[i + 0] = 0 + offset;
        _quad_indices[i + 1] = 1 + offset;
        _quad_indices[i + 2] = 2 + offset;

        _quad_indices[i + 3] = 2 + offset;
        _quad_indices[i + 4] = 3 + offset;
        _quad_indices[i + 5] = 1 + offset;

        offset += 4;
    }

    //number of rounded corners * number for indices
    //took way too fucking long
    _round_indices.resize(size, 0);
    _round_vertices.resize(maxVertices, fill);
    offset = 1;
    int centre = 0;
    int count = 0;
    for (int i = 0; i < size; i += (numRoundSegments - 1) * 3)
    {
        for (int j = i; j < i + ((numRoundSegments - 1) * 3); j += 3)
        {
            _round_indices[j + 0] = centre;
            _round_indices[j + 1] = offset;
            _round_indices[j + 2] = ++offset;
            count+=3;
        }
        centre = ++offset;
        offset++;
    }
}

void Krypton::DrawCorner(float cx, float cy, float r, float offset)
{
    Vertex c;
    c.position = {cx, cy, 0.0f};
    c.colour = {1.0f, 0.0f, 0.0f, 1.0f};

    _round_vertices[numRoundVertices] = c;
    //populate vertices
    for (int i = 0; i < numRoundSegments; i++)
    {
        float theta = (3.1415926f / 2.0f) * (float)i / (float)numRoundSegments;
        float x = r * cosf(offset + theta);
        float y = r * sinf(offset + theta);
        //https://lh3.googleusercontent.com/proxy/_v_q758R5xyNSdwLzvUBaCm1I2k5nCXGYW1tVfGbCcLqJGxrvZWDjpWJsXP92CX9VfMT4_KWUcRVC1xCe6kZ_NyBQw
        Vertex v;
        v.position = {cx + x, cy + y, 0.0f};
        v.colour = {1.0f, 0.0f, 0.0f, 1.0f};
        _round_vertices[numRoundVertices + i + 1] = v;
    }
    //number of indices
    numRoundIndices += (numRoundSegments - 1) * 3;
    numRoundVertices += numRoundSegments + 1;
}

//create a green quad with position (x, y), height h and width w
void Krypton::CreateQuad(vec3f position, float w, float h)
{
    Vertex v1;
    v1.position = {position.x - w, position.y + h, position.z};
    v1.colour = {0.0f, 1.0f, 0.0f, 1.0f};
    Vertex v2;
    v2.position = {position.x + w, position.y + h, position.z};
    v2.colour = {0.0f, 1.0f, 0.0f, 1.0f};
    Vertex v3;
    v3.position = {position.x - w, position.y - h, position.z};
    v3.colour = {0.0f, 1.0f, 0.0f, 1.0f};
    Vertex v4;
    v4.position = {position.x + w, position.y - h, position.z};
    v4.colour = {0.0f, 1.0f, 0.0f, 1.0f};

    _quad_vertices[numQuadVertices] = v1;
    _quad_vertices[numQuadVertices + 1] = v2;
    _quad_vertices[numQuadVertices + 2] = v3;
    _quad_vertices[numQuadVertices + 3] = v4;

    numQuadIndices += 6;
    numQuadVertices += 4;
}

void Krypton::DrawElement(Element element)
{
}

std::function<void()> loop;
void main_loop()
{
    loop();
}

void Krypton::Draw()
{

    running = true;


    loop = [&] {
        if (!running)
        {
            SDL_Quit();
        }

        //clear the colour buffer
        glClear(GL_COLOR_BUFFER_BIT);

        Update();

        //quad draw call
        //bind index and vertex buffer on every tick
        glBindBuffer(GL_ARRAY_BUFFER, _batchVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, numQuadVertices * sizeof(Vertex), (const void *)_quad_vertices.data());

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _batchIBO);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, _quad_indices.size() * sizeof(GLuint), (const void *)_quad_indices.data());
        glDrawElements(GL_TRIANGLES, numQuadIndices, GL_UNSIGNED_INT, NULL);

        //round draw call
        //bind index and vertex buffer on every tick
        glBindBuffer(GL_ARRAY_BUFFER, _batchVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, _round_vertices.size() * sizeof(Vertex), (const void *)_round_vertices.data());

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _batchIBO);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, _round_indices.size() * sizeof(GLuint), (const void *)_round_indices.data());
        glDrawElements(GL_TRIANGLES, numRoundIndices, GL_UNSIGNED_INT, NULL);

        SDL_GL_SwapWindow(_contextData->wnd);
    };
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(main_loop, 0, true);
#else
    while (true)
        main_loop();
#endif
}

void Krypton::Update()
{
    numQuadIndices = 0;
    numQuadVertices = 0;
    numRoundIndices = 0;
    numRoundVertices = 0;

    vec3f position2 = {0.0f, 0.0f};
    CreateQuad(position2, 50, 50);

    vec3f position = {_contextData->width / 2.0f, _contextData->height / 2.0f};
    CreateQuad(position, 50, 50);
    vec3f position3 = {_contextData->width / 2.0f + 50.0f, _contextData->height / 2.0f};
    CreateQuad(position3, 50, 50);
    vec3f position4 = {_contextData->width / 2.0f, _contextData->height / 2.0f + 50.0f};
    CreateQuad(position4, 50, 50);
    vec3f position5 = {_contextData->width / 2.0f, _contextData->height / 2.0f - 50.0f};
    CreateQuad(position5, 50, 50);
    vec3f position6 = {_contextData->width / 2.0f - 50.0f, _contextData->height / 2.0f};
    CreateQuad(position6, 50, 50);

    DrawCorner(_contextData->width / 2.0f + 50.0f, _contextData->height / 2.0f + 50.0f, 50.0f, 0.0f);
    DrawCorner(_contextData->width / 2.0f - 50.0f, _contextData->height / 2.0f + 50.0f, 50.0f, 3.1415926f / 2.0f);
    DrawCorner(_contextData->width / 2.0f + 50.0f, _contextData->height / 2.0f - 50.0f, 50.0f, 3 * 3.1415926f / 2.0f);
    DrawCorner(_contextData->width / 2.0f - 50.0f, _contextData->height / 2.0f - 50.0f, 50.0f, 3.1415926f);

    

    glUseProgram(*_contextData->programObjects);
}

void Krypton::Clean()
{
    running = false;
    free(_contextData->programObjects);
}
#endif