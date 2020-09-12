#include "KryptonElement.h"


Element::Element(Geometry* geometry_data, float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f, float h = 0.0f)
{
    position.x = x;
    position.y = y;
    position.z = z;
    width = w;
    height = h;
    vert.position = position;
    vert.colour = {1.0f, 0.0f, 0.0f, 1.0f};
    _geometry_data = geometry_data;
}

void Element::Render()
{
    vec4f colour = {0.0f, 1.0f, 0.0f, 1.0f};
    Vertex v1;
    Vertex v2;
    Vertex v3;
    Vertex v4;
    float v1X = position.x - width + borderTopLeftRadiusX;
    float v1Y = position.y - height + borderTopLeftRadiusY;
    float v2X = position.x + width - borderTopRightRadiusX;
    float v2Y = position.y - height + borderTopRightRadiusY;
    float v3X = position.x - width + borderBottomLeftRadiusX;
    float v3Y = position.y + height - borderBottomLeftRadiusY;
    float v4X = position.x + width - borderBottomRightRadiusX;
    float v4Y = position.y + height - borderBottomRightRadiusY;
    //top left
    v1.position = {v1X, v1Y, position.z};
    v1.colour = colour;
    //top right
    v2.position = {v2X, v2Y, position.z};
    v2.colour = colour;
    //bottom left
    v3.position = {v3X, v3Y, position.z};
    v3.colour = colour;
    //bottom right
    v4.position = {v4X, v4Y, position.z};
    v4.colour = colour;
    CreateQuad(v1, v2, v3, v4);
    //top left
    DrawCorner(v1X, v1Y, borderTopLeftRadiusX, borderTopLeftRadiusY, TOP_LEFT_CORNER);
    //top right
    DrawCorner(v2X, v2Y, borderTopRightRadiusX, borderTopRightRadiusY, TOP_RIGHT_CORNER);
    //bottom left
    DrawCorner(v3X, v3Y, borderBottomLeftRadiusX, borderBottomLeftRadiusY, BOTTOM_LEFT_CORNER);
    //bottom right
    DrawCorner(v4X, v4Y, borderBottomRightRadiusX, borderBottomRightRadiusY, BOTTOM_RIGHT_CORNER);

    //top quad
    //top left
    v1.position = { v1X, position.y - height };
    //top right
    v2.position = { v2X, position.y - height };
    //bottom left
    v3.position = { v1X, v1Y };
    //bottom right
    v4.position = { v2X, v2Y };

    CreateQuad(v1, v2, v3, v4);


    //left quad
    //top left
    v1.position = { position.x - width, v1Y };
    //top right
    v2.position = { v1X, v1Y };

    //bottom left
    v3.position = { position.x - width, v3Y };
    //bottom right
    v4.position = { v3X, v3Y };

    CreateQuad(v1, v2, v3, v4);

    //right quad
    //top left
    v1.position = { v2X, v2Y };
    //top right
    v2.position = { position.x + width, v2Y };
    //bottom left
    v3.position = { v4X, v4Y };
    //bottom right
    v4.position = { position.x + width, v4Y };

    CreateQuad(v1, v2, v3, v4);

    //bottom quad
    //top left
    v1.position = { v3X, v3Y };
    //top right
    v2.position = { v4X, v4Y };
    //bottom left
    v3.position = { v3X, position.y + height };
    //bottom right
    v4.position = { v2X, position.y + height };

    CreateQuad(v1, v2, v3, v4);
}

void Element::topLeftRadius(float rx, float ry = 0.0f)
{
    if (borderTopLeftRadiusY == 0.0f && rx > 0.0f)
    {
        borderTopLeftRadiusX = rx;
        borderTopLeftRadiusY = rx;
    }
    else
    {
        borderTopLeftRadiusX = rx;
        borderTopLeftRadiusY = ry;
    }
}
void Element::topRightRadius(float rx, float ry = 0.0f)
{
    if (borderTopRightRadiusY == 0.0f && rx > 0.0f)
    {
        borderTopRightRadiusX = rx;
        borderTopRightRadiusY = rx;
    }
    else
    {
        borderTopRightRadiusX = rx;
        borderTopRightRadiusY = ry;
    }
}
void Element::bottomLeftRadius(float rx, float ry = 0.0f)
{
    if (borderBottomLeftRadiusY == 0.0f && rx > 0.0f)
    {
        borderBottomLeftRadiusX = rx;
        borderBottomLeftRadiusY = rx;
    }
    else
    {
        borderBottomLeftRadiusX = rx;
        borderBottomLeftRadiusY = ry;
    }
}
void Element::bottomRightRadius(float rx, float ry = 0.0f)
{
    if (borderBottomRightRadiusY == 0.0f && rx > 0.0f)
    {
        borderBottomRightRadiusX = rx;
        borderBottomRightRadiusY = rx;
    }
    else
    {
        borderBottomRightRadiusX = rx;
        borderBottomRightRadiusY = ry;
    }
}

void Element::DrawCorner(float cx, float cy, float rx, float ry, OFFSET offset)
{
    const GLuint numRoundSegments = _geometry_data->numRoundSegments();
    const GLuint numRoundVertices = _geometry_data->numRoundVertices();
    Vertex c;
    c.position = {cx, cy, 0.0f};
    c.colour = {0.0f, 1.0f, 0.0f, 1.0f};

    _geometry_data->addRoundVertex(numRoundVertices, c);
    //populate vertices
    float tau;
    switch (offset)
    {
    case TOP_LEFT_CORNER:
        tau = 3.1415926f;
        break;

    case TOP_RIGHT_CORNER:
        tau = 3 * 3.1415926f / 2.0f;
        break;

    case BOTTOM_LEFT_CORNER:
        tau = 3.1415926f / 2.0f;
        break;

    case BOTTOM_RIGHT_CORNER:
        tau = 0.0f;
        break;
    }
    if (rx > 0.0f || ry > 0.0f)
    {
        for (int i = 0; i < numRoundSegments; i++)
        {
            float theta = (3.1415926f / 2.0f) * (float)i / (float)numRoundSegments;
            float x = rx * cosf(tau + theta);
            float y = ry * sinf(tau + theta);
            //https://lh3.googleusercontent.com/proxy/_v_q758R5xyNSdwLzvUBaCm1I2k5nCXGYW1tVfGbCcLqJGxrvZWDjpWJsXP92CX9VfMT4_KWUcRVC1xCe6kZ_NyBQw
            Vertex v;
            v.position = {cx + x, cy + y, 0.0f};
            v.colour = {0.0f, 1.0f, 0.0f, 1.0f};
            _geometry_data->addRoundVertex(numRoundVertices + i + 1, v);
        }
        //number of indices
        _geometry_data->addRoundIndices((numRoundSegments - 1) * 3);
        //number of vertices
        _geometry_data->addRoundVertices(numRoundSegments + 1);
    }
}

//create a green quad with position (x, y), height h and width w
void Element::CreateQuad(Vertex v1, Vertex v2, Vertex v3, Vertex v4)
{
    const GLuint numQuadVertices = _geometry_data->numQuadVertices();

    // _quad_vertices[numQuadVertices] = v1;
    _geometry_data->addQuadVertex(numQuadVertices, v1);
    // _quad_vertices[numQuadVertices + 1] = v2;
    _geometry_data->addQuadVertex(numQuadVertices + 1, v2);
    // _quad_vertices[numQuadVertices + 2] = v3;
    _geometry_data->addQuadVertex(numQuadVertices + 2, v3);
    // _quad_vertices[numQuadVertices + 3] = v4;
    _geometry_data->addQuadVertex(numQuadVertices + 3, v4);

    // numQuadIndices += 6;
    _geometry_data->addQuadIndices(6);
    // numQuadVertices += 4;
    _geometry_data->addQuadVertices(4);
}