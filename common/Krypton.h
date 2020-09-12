#include "KryptonUtil.h"
#include "KryptonMath.h"
#include <vector>

class Krypton
{
public:
    Krypton(ContextData *ContextData);
    void Draw();
    void Update();
    void Clean();

private:
    ContextData *_contextData;
    Geometry *_geometry_data;
    bool running = false;
};