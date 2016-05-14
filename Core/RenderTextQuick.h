//
// Copyright (c) 2012-2015 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#ifndef __RenderTextQuick_H__
#define __RenderTextQuick_H__

class RenderTextQuickGlobals;

extern RenderTextQuickGlobals* g_pRTQGlobals;

enum TextShadowStyles
{
    TextShadowStyle_None,
    TextShadowStyle_Single,
    TextShadowStyle_Quad,
    TextShadowStyle_Oct,
    TextShadowStyle_OctPlusPlus,
    TextShadowStyle_NumStyles,
};

extern int g_TextShadowStyleLetterCount[TextShadowStyle_NumStyles];

class RenderTextQuickGlobals
{
public:
    bool m_BatchMode;
    TextureDefinition* m_pBatchTexture;
    int m_BatchNumLetters;
    int m_VBONumVerts;
    bool m_VBOsInitialized;
    BufferDefinition* m_pVertexBufferIDImmediate;
    BufferDefinition* m_pVertexBuffer;
    //BufferDefinition* m_pIndexBufferID;

    MaterialDefinition* m_pMaterial;
    MyMatrix* m_pMatViewProj;

    bool m_WordWrap;
    int m_WordWrapWidthLimit;
    int m_WordWrapLineIncSize;
    bool m_WordWrapCountLinesOnly;

    char m_TempBuffer[4096]; // for RenderTextQuick* functions only(not GetStringSize!),
                             // RTQ*'s don't call each other so buffer won't get overwritten.

public:
    RenderTextQuickGlobals();
    virtual ~RenderTextQuickGlobals();

    void SetWrapMode(bool wrap, int widthlimit, int lineincsize, bool countonly);
};

Vector2 GetStringSize(FontDefinition* pFont, float fontheight, const char* text, ...);

int RenderTextQuick(FontDefinition* pFont, float fontheight, float x, float y, unsigned char justificationflags, const char* text, ...);
int RenderTextQuickWithColor(FontDefinition* pFont, float fontheight, float x, float y, unsigned char justificationflags, ColorByte color, const char* text, ...);
int RenderTextQuickWithColorAndShadow(FontDefinition* pFont, float fontheight, float x, float y, unsigned char justificationflags, ColorByte color, ColorByte shadowcolor, float shadowoffx, float shadowoffy, const char* text, ...);
int RenderTextQuickWithColorAndShadowStyle(FontDefinition* pFont, float fontheight, float x, float y, unsigned char justificationflags, ColorByte color, TextShadowStyles shadowstyle, ColorByte shadowcolor, float shadowoffx, float shadowoffy, const char* text, ...);

int RenderTextQuickWithColorAndZ(FontDefinition* pFont, float fontheight, float x, float y, float z, unsigned char justificationflags, ColorByte color, const char* text, ...);
int RenderTextQuickWithColorZAndRot(FontDefinition* pFont, float fontheight, float x, float y, float z, float rotz, unsigned char justificationflags, ColorByte color, const char* text, ...);
int RenderTextQuickWithColorShadowZAndRot(FontDefinition* pFont, float fontheight, float x, float y, float z, float rotz, unsigned char justificationflags, ColorByte color, ColorByte shadowcolor, float shadowoffx, float shadowoffy, const char* text, ...);
int RenderTextQuickWithColorShadowStyleZAndRot(FontDefinition* pFont, float fontheight, float x, float y, float z, float rotz, unsigned char justificationflags, ColorByte color, TextShadowStyles shadowstyle, ColorByte shadowcolor, float shadowoffx, float shadowoffy, const char* text, ...);

int RenderTextQuickWithEverything(FontDefinition* pFont, float fontheight, float x, float y, float z, float rotz, unsigned char justificationflags, ColorByte color, const char* text);

void RenderTextQuickBatchStart(FontDefinition* pFont);
void RenderTextQuickBatchEnd();

#endif //__RenderTextQuick_H__
