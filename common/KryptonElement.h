#ifndef KRYPTON_ELEMENT_H
#define KRYPTON_ELEMENT_H
#include "KryptonMath.h"
#include "KryptonText.h"

enum OFFSET
{
    TOP_LEFT_CORNER,
    TOP_RIGHT_CORNER,
    BOTTOM_LEFT_CORNER,
    BOTTOM_RIGHT_CORNER
};

class Element
{

private:
    Geometry* _geometry_data;
    Vertex vert;
    float width;
    float height;
    float _scale = 1.0f;
    Character _characterTextureData;
    float borderTopLeftRadiusX = 0.0f;
    float borderTopRightRadiusX = 0.0f;
    float borderBottomLeftRadiusX = 0.0f;
    float borderBottomRightRadiusX = 0.0f;
    float borderTopLeftRadiusY = 0.0f;
    float borderTopRightRadiusY = 0.0f;
    float borderBottomLeftRadiusY = 0.0f;
    float borderBottomRightRadiusY = 0.0f;

public:
    Element(Geometry*, float, float, float, float, float);
    Element(Geometry*, Character, float, float, float);
    void topLeftRadius(float, float);
    void topRightRadius(float, float);
    void bottomLeftRadius(float, float);
    void bottomRightRadius(float, float);
    void DrawCorner(float, float, float, float, vec4f, OFFSET);
    void CreateQuad(Vertex v1, Vertex v2, Vertex v3, Vertex v4);
    void colour(float, float, float, float);
    void colour(int);
    void rgba(float, float, float, float);
    void scale(float scale);

    void Render();
};
#endif