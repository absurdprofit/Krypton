#include "KryptonText.h"

void Text::addFont(const char *path)
{
    //load face and create single row texture atlas
    FT_Face face;
    if (FT_New_Face(ft, path, 0, &face))
    {
        KryptonRaiseError();
    }

    FT_Set_Pixel_Sizes(face, 0, 96);

    int width = 0;
    int height = 0;


    for (int i = 0; i < 128; i++)
    {
        if (FT_Load_Char(face, (char)i, FT_LOAD_RENDER))
        {
            KryptonRaiseError();
            continue;
        }
        width += face->glyph->bitmap.width;

        if (face->glyph->bitmap.rows > height)
        {
            height = face->glyph->bitmap.rows;
        }
    }

    GLuint texture;
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //disable byte alignment

    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width, height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, nullptr);
    //set texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int x = 0;
    for (int i = 33; i < 128; i++)
    {
        if (FT_Load_Char(face, (char)i, FT_LOAD_RENDER))
        {
            KryptonRaiseError();
            continue;
        }

        if (face->glyph->bitmap.buffer == NULL)
        {
            std::cout << (char)i << ": NULL" << std::endl;
        }

        glTexSubImage2D(
            GL_TEXTURE_2D,
            0,
            x,
            0,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            GL_LUMINANCE,
            GL_UNSIGNED_BYTE,
            (const void *)face->glyph->bitmap.buffer);

        Character character;
        character.ax = face->glyph->advance.x >> 6;
        character.ay = face->glyph->advance.y >> 6;

        character.bw = face->glyph->bitmap.width;
        character.bh = face->glyph->bitmap.rows;

        character.bl = face->glyph->bitmap_left;
        character.bt = face->glyph->bitmap_top;

        character.tx = (float)x / width;

        character.textCoordTopLeft = {character.tx, 0.0f};
        character.textCoordTopRight = {character.tx + (character.bw / width), 0.0f};
        character.textCoordBottomLeft = {character.tx, character.bh / height};
        character.textCoordBottomRight = {character.tx + (character.bw / width), character.bh / height};

        Characters.push_back(character);
        x += face->glyph->bitmap.width;
    }


    FT_Done_Face(face);
    FT_Done_FreeType(ft);
    numFonts += 1;
}

Text::Text(ContextData *_contextData)
{
    this->_contextData = _contextData;
    //initialise freetype with default font roboto
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    }

    addFont("assets/fonts/Roboto-Medium.ttf");
}