#include "KryptonMath.h"

Geometry::Geometry(ContextData* _contextData)
{
    GLuint size = ((_maxVertices / (_numRoundSegments + 1)) * ((_numRoundSegments - 1) * 3) > (_maxVertices / 4) * 6 ? (_maxVertices / (_numRoundSegments + 1)) * ((_numRoundSegments - 1) * 3) : (_maxVertices / 4) * 6);

    //allocate vertex and index buffers
    glGenBuffers(1, &_batchIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _batchIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &_batchVBO);
    glBindBuffer(GL_ARRAY_BUFFER, _batchVBO);
    glBufferData(GL_ARRAY_BUFFER, _maxVertices * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

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
    _quad_vertices.resize(_maxVertices, fill);
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
    _round_vertices.resize(_maxVertices, fill);
    offset = 1;
    int centre = 0;
    int count = 0;
    for (int i = 0; i < size; i += (_numRoundSegments - 1) * 3)
    {
        for (int j = i; j < i + ((_numRoundSegments - 1) * 3); j += 3)
        {
            _round_indices[j + 0] = centre;
            _round_indices[j + 1] = offset;
            _round_indices[j + 2] = ++offset;
            count += 3;
        }
        centre = ++offset;
        offset++;
    }
}

const GLuint Geometry::numQuadVertices()
{
    return _numQuadVertices;
}
void Geometry::numQuadVertices(GLuint numVertices)
{
    _numQuadVertices = numVertices;
}
const GLuint Geometry::numRoundVertices()
{
    return _numRoundVertices;
}
void Geometry::numRoundVertices(GLuint numVertices)
{
    _numRoundVertices = numVertices;
}
const GLuint Geometry::maxVertices()
{
    return _maxVertices;
}
const GLuint Geometry::numRoundSegments()
{
    return _numRoundSegments;
}
const GLuint Geometry::numQuadIndices()
{
    return _numQuadIndices;
}
void Geometry::numQuadIndices(GLuint numIndices)
{
    _numQuadIndices = numIndices;
}
const GLuint Geometry::numRoundIndices()
{
    return _numRoundIndices;
}
void Geometry::numRoundIndices(GLuint numIndices)
{
    _numRoundIndices = numIndices;
}
const GLuint Geometry::VBO()
{
    return _batchVBO;
}
const GLuint Geometry::IBO()
{
    return _batchIBO;
}
const GLuint* Geometry::quadIndicesData()
{
    return _quad_indices.data();
}
const GLuint Geometry::quadIndicesSize()
{
    return _quad_indices.size();
}
const GLuint* Geometry::roundIndicesData()
{
    return _round_indices.data();
}
const GLuint Geometry::roundIndicesSize()
{
    return _round_indices.size();
}
const Vertex* Geometry::quadVerticesData()
{
    return _quad_vertices.data();
}
const GLuint Geometry::quadVerticesSize()
{
    return _quad_vertices.size();
}
const Vertex* Geometry::roundVerticesData()
{
    return _round_vertices.data();
}
const GLuint Geometry::roundVerticesSize()
{
    return _round_vertices.size();
}

void Geometry::addQuadVertices(int numVertices)
{
    _numQuadVertices += numVertices;
}
void Geometry::addRoundVertices(int numVertices)
{
    _numRoundVertices += numVertices;
}
void Geometry::addQuadIndices(int numIndices)
{
    _numQuadIndices += numIndices;
}
void Geometry::addRoundIndices(int numIndices)
{
    _numRoundIndices += numIndices;
}
void Geometry::addRoundVertex(int index, Vertex v)
{
    _round_vertices[index] = v;
}
void Geometry::addQuadVertex(int index, Vertex v)
{
    _quad_vertices[index] = v;
}

