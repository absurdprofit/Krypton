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
#include "KryptonElement.h"
#include <functional>
#include <iostream>
#include <emscripten.h>
#include <array>

Krypton::Krypton(ContextData *ContextData)
{
    if (!Init(ContextData))
        exit(0);

    _contextData = ContextData;

    static Geometry geometry_data(_contextData);
    _geometry_data = &geometry_data;
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
        glBindBuffer(GL_ARRAY_BUFFER, _geometry_data->VBO());
        glBufferSubData(GL_ARRAY_BUFFER, 0, _geometry_data->numQuadVertices() * sizeof(Vertex), (const void *)_geometry_data->quadVerticesData());

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _geometry_data->IBO());
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, _geometry_data->quadIndicesSize() * sizeof(GLuint), (const void *)_geometry_data->quadIndicesData());
        glDrawElements(GL_TRIANGLES, _geometry_data->numQuadIndices(), GL_UNSIGNED_INT, NULL);

        //round draw call
        //bind index and vertex buffer on every tick
        glBindBuffer(GL_ARRAY_BUFFER, _geometry_data->VBO());
        glBufferSubData(GL_ARRAY_BUFFER, 0, _geometry_data->roundVerticesSize() * sizeof(Vertex), (const void *)_geometry_data->roundVerticesData());

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _geometry_data->IBO());
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, _geometry_data->roundIndicesSize() * sizeof(GLuint), (const void *)_geometry_data->roundIndicesData());
        glDrawElements(GL_TRIANGLES, _geometry_data->numRoundIndices(), GL_UNSIGNED_INT, NULL);

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
    _geometry_data->numQuadIndices(0);
    _geometry_data->numQuadVertices(0);
    _geometry_data->numRoundIndices(0);
    _geometry_data->numRoundVertices(0);

    Element circle(_geometry_data, _contextData->width / 2.0f, _contextData->height / 2.0f, 0.0f, 40.0f, 40.0f);
    circle.topLeftRadius(40.0f, 0.0f);
    circle.topRightRadius(40.0f, 0.0f);
    circle.bottomRightRadius(40.0f, 0.0f);
    circle.bottomLeftRadius(40.0f, 0.0f);
    circle.Render();

    Element square(_geometry_data, 50.0f, 50.0f, 0.0f, 40.0f, 40.0f);
    square.topLeftRadius(10.0f, 0.0f);
    square.topRightRadius(20.0f, 0.0f);
    square.bottomRightRadius(20.0f, 0.0f);
    square.bottomLeftRadius(0.0f, 0.0f);
    square.Render();

    Element footer(_geometry_data, _contextData->width / 2.0f, _contextData->height - 40.0f, 0.0f, _contextData->width, 40.0f);
    footer.Render();

    glUseProgram(*_contextData->programObjects);
}

void Krypton::Clean()
{
    running = false;
    free(_contextData->programObjects);
}
#endif