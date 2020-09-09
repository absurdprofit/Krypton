#include "KryptonUtil.h"
#include "KryptonMath.h"
#include <vector>

enum OFFSET {
    TOP_LEFT_CORNER,
    TOP_RIGHT_CORNER,
    BOTTOM_LEFT_CORNER,
    BOTTOM_RIGHT_CORNER
};

struct Element {
    Vertex vert;
    vec3f position;
    float width;
    float height;
    float borderTopRightRadius = 0.0f;
    float borderTopLeftRadius = 0.0f;
    float borderBottomRightRadius = 0.0f;
    float borderBottomLeftRadius = 0.0f;

    Element(float x=0.0f, float y=0.0f, float z=0.0f, float w=0.0f, float h=0.0f) {
        position.x = x;
        position.y = y;
        position.z = z;
        width = w;
        height = h;
        vert.position = position;
        vert.colour = { 1.0f, 0.0f, 0.0f, 1.0f };
    }
};

class Krypton {
public:
    Krypton(ContextData* ContextData);
    void Draw();
    void Update();
    void Clean();
    void DrawCorner(float, float, float, OFFSET);
    void DrawElement(Element);
    void CreateQuad(Vertex, Vertex, Vertex, Vertex);

private:
    ContextData* _contextData;
    GLuint numQuadVertices = 0;
    GLuint numRoundVertices = 0;
    const GLuint maxVertices = 15000;
    const GLuint numRoundSegments = 499;
    GLuint numQuadIndices = 0;
    GLuint numRoundIndices = 0;
    GLuint _batchVBO;
    GLuint _batchIBO;
    std::vector<GLuint> _quad_indices;
    std::vector<Vertex> _quad_vertices;
    std::vector<Vertex> _round_vertices;
    std::vector<GLuint> _round_indices;
    bool running = false;
};