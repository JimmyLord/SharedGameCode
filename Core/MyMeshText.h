//
// Copyright (c) 2012-2014 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#ifndef __MyMeshText_H__
#define __MyMeshText_H__

#include "RenderTextQuick.h"

class MyMeshText : public MyMesh
{
public:
    FontDefinition* m_pFont;

#if _DEBUG
    unsigned int m_MostLettersAttemptedToDrawThisFrame;
    unsigned int m_MostLettersAttemptedToDrawEver;
#endif

public:
    MyMeshText(int maxletters, FontDefinition* pFont, GameCore* pGameCore);
    virtual ~MyMeshText();

    void ClearText();

    Vector2 GetStringSize(float fontheight, Vector2 size, const char* text, ...);

    int CreateStringWhite(bool concat, float fontheight, float x, float y, unsigned char justificationflags, Vector2 size, const char* text, ...);
    int CreateStringColor(bool concat, float fontheight, float x, float y, unsigned char justificationflags, ColorByte color, Vector2 size, const char* text, ...);
    int CreateStringColorAndShadow(bool concat, float fontheight, float x, float y, unsigned char justificationflags, ColorByte color, ColorByte shadowcolor, float shadowoffx, float shadowoffy, Vector2 size, const char* text, ...);
    int CreateStringColorAndShadowStyle(bool concat, float fontheight, float x, float y, unsigned char justificationflags, ColorByte color, TextShadowStyles shadowstyle, ColorByte shadowcolor, float shadowoffx, float shadowoffy, Vector2 size, const char* text, ...);

    int CreateStringColorAndZ(bool concat, float fontheight, float x, float y, float z, unsigned char justificationflags, ColorByte color, Vector2 size, const char* text, ...);
    int CreateStringColorZAndRot(bool concat, float fontheight, float x, float y, float z, float rotz, unsigned char justificationflags, ColorByte color, Vector2 size, const char* text, ...);
    int CreateStringColorShadowZAndRot(bool concat, float fontheight, float x, float y, float z, float rotz, unsigned char justificationflags, ColorByte color, ColorByte shadowcolor, float shadowoffx, float shadowoffy, Vector2 size, const char* text, ...);
    int CreateStringColorShadowStyleZAndRot(bool concat, float fontheight, float x, float y, float z, float rotz, unsigned char justificationflags, ColorByte color, TextShadowStyles shadowstyle, ColorByte shadowcolor, float shadowoffx, float shadowoffy, Vector2 size, const char* text, ...);

    int CreateString(bool concat, float fontheight, float x, float y, float z, float rotz, unsigned char justificationflags, ColorByte color, Vector2 size, const char* text, ...);
};

#endif //__MyMeshText_H__
