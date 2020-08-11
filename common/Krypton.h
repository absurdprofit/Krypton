#include "kryptonUtil.h"
#include "KryptonMath.h"
#include <vector>

class Krypton {
public:
    Krypton(ContextData* ContextData);
    void Draw();
    void Update();
    void Clean();

private:
    ContextData* _contextData;
    GLuint numVertices = 1000;
    GLuint numIndices = 0;
    GLuint _batchVBO;
    GLuint _batchIBO;
    std::vector<GLuint> _indices;
    std::vector<Vertex> _vertices;
    bool running = false;
};