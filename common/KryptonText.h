#ifndef KRYPTON_TEXT_H
#define KRYPTON_TEXT_H
#include <ft2build.h>
#include FT_FREETYPE_H
#include "KryptonUtil.h"
#include "KryptonMath.h"
#include <vector>
#include <iostream>

struct Character {
    float ax = 0.0f; //advance.x
    float ay = 0.0f; //advance.y

    float bw = 0.0f; //bitmap.width
    float bh = 0.0f; //bitmap.rows

    float bl = 0.0f; //bitmap_left
    float bt = 0.0f; //bitmap_top

    float tx = 0.0f; //x offset of glyph in texture coordinates

    vec2f textCoordTopLeft = { 0.0f, 0.0f };
    vec2f textCoordTopRight = { 0.0f, 0.0f };
    vec2f textCoordBottomLeft = { 0.0f, 0.0f };
    vec2f textCoordBottomRight = { 0.0f, 0.0f };
};

class Text
{
public:
    Text(ContextData* _contextData);
    void addFont(const char* path);
    std::vector<Character> Characters;

private:
    const GLuint maxFonts = 10;
    GLuint numFonts;
    FT_Library ft;
    ContextData* _contextData;
};
#endif