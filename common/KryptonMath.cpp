#include "KryptonMath.h"
#include <iostream>
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

    loc = glGetAttribLocation(*_contextData->programObjects, "a_textCoords");
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void *)offsetof(Vertex, textCoords));

    loc = glGetAttribLocation(*_contextData->programObjects, "a_textColour");
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void *)offsetof(Vertex, textColour));
    

    loc = glGetUniformLocation(*_contextData->programObjects, "u_texture");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glUniform1i(loc, 0);

    _vertices.resize(_maxVertices);
}
const GLuint Geometry::numVertices()
{
    return _numVertices;
}
void Geometry::numVertices(GLuint numVertices)
{
    _numVertices = numVertices;
}
const GLuint Geometry::maxVertices()
{
    return _maxVertices;
}
const GLuint Geometry::numRoundSegments()
{
    return _numRoundSegments;
}
const GLuint Geometry::numIndices()
{
    return _numIndices;
}
void Geometry::numIndices(GLuint numIndices)
{
    _numIndices = numIndices;
}
const GLuint Geometry::VBO()
{
    return _batchVBO;
}
const GLuint Geometry::IBO()
{
    return _batchIBO;
}
const GLuint* Geometry::indicesData()
{
    return _indices.data();
}
const GLuint Geometry::indicesSize()
{
    return _indices.size();
}
const Vertex* Geometry::verticesData()
{
    return _vertices.data();
}
const GLuint Geometry::verticesSize()
{
    return _vertices.size();
}
void Geometry::addVertices(int numVertices)
{
    _numVertices += numVertices;
}
void Geometry::addIndices(int numIndices)
{
    _indices.resize(_numIndices + numIndices);
    _numIndices += numIndices;
}
void Geometry::addVertex(int index, Vertex v)
{
    _vertices[index] = v;
}
void Geometry::addIndex(int _index, GLuint index) {
    _indices[_index] = index;
}
const GLuint Geometry::lastIndex() {
    return _indices[_numIndices - (_numIndices > 0 ? 1 : 0)];
}