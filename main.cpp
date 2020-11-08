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

int main()
{
    int w = EM_ASM_INT({return screen.width});
    int h = EM_ASM_INT({
        return screen.height;
    });

    int run = EM_ASM_INT({
        if (document.getElementById('warning'))
        {
            return 1;
        }
        else
        {
            return 0;
        }
    });
    if (!run)
    {
        ContextData ContextData;
        ContextData.width = w;
        ContextData.height = h;
        ContextData.windowName = (GLchar *)"Skeleton";

        Krypton krypton(&ContextData);
        krypton.Run();
        std::cout << "END" << std::endl;
    }

    return 1;
}

void _clean()
{
    emscripten_cancel_main_loop();
}

EMSCRIPTEN_BINDINGS(my_module)
{
    emscripten::function("clean", &_clean);
}