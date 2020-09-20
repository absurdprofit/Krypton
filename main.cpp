/*
Athor: Nathan Johnson
Date: August 11, 2020
Email: nahtanjohn1l@gmail.com

Main entry point of our application, however when users start a new Krypton project and are writing
their own application, they will not use main() as their application entry point. As is the case with a
win32 application. Krypton behind the scenes will be the main entry point of the application and the user will
be given an entry point that gets invoked by Krypton's main entry point.
*/

#include <emscripten/bind.h>
#include <emscripten/html5.h>
#include <emscripten/val.h>
#include <emscripten.h>
#include <iostream>
#include "common/Krypton.h"


int main() {
    int w = EM_ASM_INT({
        return screen.width
    });
    int h = EM_ASM_INT({
        return screen.height;
    });
    ContextData ContextData;
    ContextData.width = w;
    ContextData.height = h;
    ContextData.windowName = (GLchar*)"Skeleton";
    
    Krypton krypton(&ContextData);
    krypton.Draw();

    std::cout << "END" << std::endl;

    return 1;
}

int _clean(int x, int y, int w, int h)
{
    exit(0);
}

bool _click(float x, float y, int w, int h) {
    // if (x <= (w / 2)) {
    //     x /= (w / 2);
    //     x -= 1;
    // } else {
    //     x -= (w / 2);
    //     x /= (w / 2);
    // }
    // if (y <= (h / 2)) {
    //     y /= (h / 2);
    //     y -= 1;
    // } else {
    //     y -= (h / 2);
    //     y /= (h / 2);
    // }
    // std::cout << x << ", " << y << std::endl;
    // if ((x <= 0.5 && x >= 0.0) && (-y <= 0.5 && -y >= 0.0)) {
    //     std::cout << "Square Clicked!" << std::endl;
    //     return true;
    // } else {
    //     return false;
    // }
    return false;
}

//semantics for registering callbacks, this will all be done behind the scenes

EMSCRIPTEN_BINDINGS(my_module)
{
    emscripten::function("clean", &_clean);
    emscripten::function("click", &_click);
}