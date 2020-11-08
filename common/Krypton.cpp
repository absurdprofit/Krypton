/*
Author: Nathan Johnson
Date: August 11, 2020
Email: nahtanjohn1l@gmail.com


The contents of this file represents Krypton Core. Fine details on how apps will eventually be built haven't
been finalised just yet, but the idea is that Krypton.h will be what manages a Krypton application. It will
draw to the canvas, process events, access web APIs, so on and so forth. It will also be where our UI data is
"retained" in some arbitrary data structure such as a widget tree (as implemented in Flutter).

TODO:
1.KryptonTimer
2. KryptonEvents
3. KryptonLayout engine
4. Stroke text
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

    static Text text(_contextData);
    _text = &text;
}

std::function<void()> loop;
void main_loop()
{
    loop();
}

void Krypton::Run()
{

    running = true;


    loop = [&] {
        if (!running)
        {
            SDL_Quit();
        }

        SDL_Event e;

        while(SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_KEYDOWN:
                    std::cout << "Key Down" << std::endl;
                    break;
                
                case SDL_MOUSEBUTTONDOWN:
                    std::cout << "Mouse Button Down" << std::endl;
                    break;
                
                case SDL_QUIT:
                    Clean();
                    break;
            }
        }

        //clear the colour buffer
        glClear(GL_COLOR_BUFFER_BIT);

        Update();

        //round draw call
        //bind index and vertex buffer on every tick
        glBindBuffer(GL_ARRAY_BUFFER, _geometry_data->VBO());
        glBufferSubData(GL_ARRAY_BUFFER, 0, _geometry_data->verticesSize() * sizeof(Vertex), (const void *)_geometry_data->verticesData());

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _geometry_data->IBO());
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, _geometry_data->indicesSize() * sizeof(GLuint), (const void *)_geometry_data->indicesData());
        glDrawElements(GL_TRIANGLES, _geometry_data->numIndices(), GL_UNSIGNED_INT, NULL);

        //quad draw call
        //bind index and vertex buffer on every tick
        // glBindBuffer(GL_ARRAY_BUFFER, _geometry_data->VBO());
        // glBufferSubData(GL_ARRAY_BUFFER, 0, _geometry_data->numQuadVertices() * sizeof(Vertex), (const void *)_geometry_data->quadVerticesData());

        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _geometry_data->IBO());
        // glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, _geometry_data->quadIndicesSize() * sizeof(GLuint), (const void *)_geometry_data->quadIndicesData());
        // glDrawElements(GL_TRIANGLES, _geometry_data->numQuadIndices(), GL_UNSIGNED_INT, NULL);
        
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
    
    _geometry_data->numIndices(0);
    _geometry_data->numVertices(0);

    Element circle(_geometry_data, _contextData->width / 2.0f, _contextData->height / 2.0f, 0.0f, 80.0f, 80.0f);
    circle.colour(1.0f, 0.0f, 0.0f, 1.0f);
    circle.topLeftRadius(40.0f, 0.0f);
    circle.topRightRadius(40.0f, 0.0f);
    circle.bottomRightRadius(40.0f, 0.0f);
    circle.bottomLeftRadius(40.0f, 0.0f);
    circle.Render();

    Element square(_geometry_data, 50.0f, 50.0f, 0.0f, 40.0f, 40.0f);
    square.rgba(255, 107, 158, 255);
    square.topLeftRadius(10.0f, 0.0f);
    square.topRightRadius(20.0f, 0.0f);
    square.bottomRightRadius(20.0f, 0.0f);
    square.bottomLeftRadius(0.0f, 0.0f);
    square.Render();

    Element footer(_geometry_data, _contextData->width / 2.0f, _contextData->height - 20.0f, 0.0f, _contextData->width, 40.0f);
    footer.colour(0X2ee6dc);
    footer.Render();

    Element Char(_geometry_data, _text->Characters[((int)'J') - 33], 90.0f, 90.0f, -20.0f);
    Char.colour(0X5C257F);
    Char.scale(3.0);
    Char.Render();
    glUseProgram(*_contextData->programObjects);
}

void Krypton::Clean()
{
    running = false;
    free(_contextData->programObjects);
    SDL_Quit();
}
#endif