//
// Copyright (c) 2012-2016 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "SharedCommonHeader.h"
#include "../Core/RenderTextQuick.h"
#include "../Core/MeshShapes.h"
#include "../Menus/LanguageTable.h"

// TODO: Fix GL Includes.
#include <gl/GL.h>

MyMeshText::MyMeshText(int maxletters, FontDefinition* pFont)
{
    m_pFont = pFont;

#if _DEBUG
    m_MostLettersAttemptedToDrawThisFrame = 0;
    m_MostLettersAttemptedToDrawEver = 0;
#endif

    // Create buffers and base set of indices.
    VertexFormat_Dynamic_Desc* pVertFormat = g_pVertexFormatManager->GetDynamicVertexFormat( 1, false, false, false, true, 0 );
    CreateOneSubmeshWithBuffers( pVertFormat, (unsigned short)maxletters*4, 2, maxletters*6, true );

    unsigned short* pIndices = GetIndices( true );
    for( unsigned short i=0; i<maxletters; i++ )
    {
        pIndices[i*6 + 0] = i*4 + g_SpriteVertexIndices[0];
        pIndices[i*6 + 1] = i*4 + g_SpriteVertexIndices[1];
        pIndices[i*6 + 2] = i*4 + g_SpriteVertexIndices[2];
        pIndices[i*6 + 3] = i*4 + g_SpriteVertexIndices[3];
        pIndices[i*6 + 4] = i*4 + g_SpriteVertexIndices[4];
        pIndices[i*6 + 5] = i*4 + g_SpriteVertexIndices[5];
    }
    RebuildIndices();
}

MyMeshText::~MyMeshText()
{
}

void MyMeshText::ClearText()
{
    MyAssert( m_SubmeshList.Count() > 0 );

    m_SubmeshList[0]->m_NumVertsToDraw = 0;
    m_SubmeshList[0]->m_NumIndicesToDraw = 0;

#if _DEBUG
    m_MostLettersAttemptedToDrawThisFrame = 0;
#endif
}

Vector2 MyMeshText::GetStringSize(float fontheight, Vector2 size, const char* text, ...)
{
    const char* stringtodraw = text;
    if( g_pLanguageTable != 0 && text[0] == '.' )
        stringtodraw = g_pLanguageTable->LookUp( text );

    char tempbuffer[4096];
    va_list arg;
    va_start( arg, text );
    vsnprintf_s( tempbuffer, sizeof(tempbuffer), _TRUNCATE, stringtodraw, arg );
    va_end(arg);

    return m_pFont->GetBMFont()->GetSize( tempbuffer, fontheight );
}

int MyMeshText::CreateStringWhite(bool concat, float fontheight, float x, float y, unsigned char justificationflags, Vector2 size, const char* text, ...)
{
    MyAssert( g_pRTQGlobals );
    if( g_pRTQGlobals == 0 )
        return 0;

    const char* stringtodraw = text;
    if( g_pLanguageTable != 0 && text[0] == '.' )
        stringtodraw = g_pLanguageTable->LookUp( text );

    char* tempbuffer = g_pRTQGlobals->m_TempBuffer;
    va_list arg;
    va_start( arg, text );
    vsnprintf_s( tempbuffer, sizeof(g_pRTQGlobals->m_TempBuffer), _TRUNCATE, stringtodraw, arg );
    va_end(arg);

    ColorByte color(255, 255, 255, 255);

    return CreateString( concat, fontheight, x, y, 0, 0, justificationflags, color, size, tempbuffer );
}

int MyMeshText::CreateStringColor(bool concat, float fontheight, float x, float y, unsigned char justificationflags, ColorByte color, Vector2 size, const char* text, ...)
{
    MyAssert( g_pRTQGlobals );
    if( g_pRTQGlobals == 0 )
        return 0;

    const char* stringtodraw = text;
    if( g_pLanguageTable != 0 && text[0] == '.' )
        stringtodraw = g_pLanguageTable->LookUp( text );

    char* tempbuffer = g_pRTQGlobals->m_TempBuffer;
    va_list arg;
    va_start( arg, text );
    vsnprintf_s( tempbuffer, sizeof(g_pRTQGlobals->m_TempBuffer), _TRUNCATE, stringtodraw, arg );
    va_end(arg);

    return CreateString( concat, fontheight, x, y, 0, 0, justificationflags, color, size, tempbuffer );
}

int MyMeshText::CreateStringColorAndShadow(bool concat, float fontheight, float x, float y, unsigned char justificationflags, ColorByte color, ColorByte shadowcolor, float shadowoffx, float shadowoffy, Vector2 size, const char* text, ...)
{
    MyAssert( g_pRTQGlobals );
    if( g_pRTQGlobals == 0 )
        return 0;

    const char* stringtodraw = text;
    if( g_pLanguageTable != 0 && text[0] == '.' )
        stringtodraw = g_pLanguageTable->LookUp( text );

    char* tempbuffer = g_pRTQGlobals->m_TempBuffer;
    va_list arg;
    va_start( arg, text );
    vsnprintf_s( tempbuffer, sizeof(g_pRTQGlobals->m_TempBuffer), _TRUNCATE, stringtodraw, arg );
    va_end(arg);

    CreateString( concat, fontheight, x+shadowoffx, y+shadowoffy, 0, 0, justificationflags, shadowcolor, size, tempbuffer );
    concat = true;
    return CreateString( concat, fontheight, x, y, 0, 0, justificationflags, color, size, tempbuffer );
}

int MyMeshText::CreateStringColorAndShadowStyle(bool concat, float fontheight, float x, float y, unsigned char justificationflags, ColorByte color, TextShadowStyles shadowstyle, ColorByte shadowcolor, float shadowoffx, float shadowoffy, Vector2 size, const char* text, ...)
{
    MyAssert( g_pRTQGlobals );
    if( g_pRTQGlobals == 0 )
        return 0;

    const char* stringtodraw = text;
    if( g_pLanguageTable != 0 && text[0] == '.' )
        stringtodraw = g_pLanguageTable->LookUp( text );

    char* tempbuffer = g_pRTQGlobals->m_TempBuffer;
    va_list arg;
    va_start( arg, text );
    vsnprintf_s( tempbuffer, sizeof(g_pRTQGlobals->m_TempBuffer), _TRUNCATE, stringtodraw, arg );
    va_end(arg);

    if( shadowstyle == TextShadowStyle_None )
    {
    }
    else if( shadowstyle == TextShadowStyle_Single )
    {
        CreateString( concat, fontheight, x+shadowoffx, y+shadowoffy, 0, 0, justificationflags, shadowcolor, size, tempbuffer );
        concat = true;
    }
    else if( shadowstyle == TextShadowStyle_Quad )
    {
        CreateString( concat, fontheight, x+shadowoffx, y+shadowoffy, 0, 0, justificationflags, shadowcolor, size, tempbuffer );
        concat = true;
        CreateString( concat, fontheight, x-shadowoffy, y+shadowoffx, 0, 0, justificationflags, shadowcolor, size, tempbuffer );
        CreateString( concat, fontheight, x-shadowoffx, y-shadowoffy, 0, 0, justificationflags, shadowcolor, size, tempbuffer );
        CreateString( concat, fontheight, x+shadowoffy, y-shadowoffx, 0, 0, justificationflags, shadowcolor, size, tempbuffer );
    }
    else if( shadowstyle == TextShadowStyle_Oct )
    {
        CreateString( concat, fontheight, x+shadowoffx, y+shadowoffy, 0, 0, justificationflags, shadowcolor, size, tempbuffer );
        concat = true;
        CreateString( concat, fontheight, x-shadowoffy, y+shadowoffx, 0, 0, justificationflags, shadowcolor, size, tempbuffer );
        CreateString( concat, fontheight, x-shadowoffx, y-shadowoffy, 0, 0, justificationflags, shadowcolor, size, tempbuffer );
        CreateString( concat, fontheight, x+shadowoffy, y-shadowoffx, 0, 0, justificationflags, shadowcolor, size, tempbuffer );

        CreateString( concat, fontheight, x           , y+shadowoffy, 0, 0, justificationflags, shadowcolor, size, tempbuffer );
        CreateString( concat, fontheight, x-shadowoffx, y           , 0, 0, justificationflags, shadowcolor, size, tempbuffer );
        CreateString( concat, fontheight, x           , y-shadowoffy, 0, 0, justificationflags, shadowcolor, size, tempbuffer );
        CreateString( concat, fontheight, x+shadowoffx, y           , 0, 0, justificationflags, shadowcolor, size, tempbuffer );
    }
    else if( shadowstyle == TextShadowStyle_OctPlusPlus )
    {
        CreateString( concat, fontheight, x+shadowoffx, y+shadowoffy, 0, 0, justificationflags, shadowcolor, size, tempbuffer );
        concat = true;
        CreateString( concat, fontheight, x-shadowoffy, y+shadowoffx, 0, 0, justificationflags, shadowcolor, size, tempbuffer );
        CreateString( concat, fontheight, x-shadowoffx, y-shadowoffy, 0, 0, justificationflags, shadowcolor, size, tempbuffer );
        CreateString( concat, fontheight, x+shadowoffy, y-shadowoffx, 0, 0, justificationflags, shadowcolor, size, tempbuffer );

        CreateString( concat, fontheight, x           , y+shadowoffy, 0, 0, justificationflags, shadowcolor, size, tempbuffer );
        CreateString( concat, fontheight, x-shadowoffx, y           , 0, 0, justificationflags, shadowcolor, size, tempbuffer );
        CreateString( concat, fontheight, x           , y-shadowoffy, 0, 0, justificationflags, shadowcolor, size, tempbuffer );
        CreateString( concat, fontheight, x+shadowoffx, y           , 0, 0, justificationflags, shadowcolor, size, tempbuffer );

        CreateString( concat, fontheight, x+shadowoffx*2, y+shadowoffy*2, 0, 0, justificationflags, shadowcolor, size, tempbuffer );
        CreateString( concat, fontheight, x+shadowoffx*3, y+shadowoffy*3, 0, 0, justificationflags, shadowcolor, size, tempbuffer );
    }

    return CreateString( concat, fontheight, x, y, 0, 0, justificationflags, color, size, tempbuffer );
}

int MyMeshText::CreateStringColorAndZ(bool concat, float fontheight, float x, float y, float z, unsigned char justificationflags, ColorByte color, Vector2 size, const char* text, ...)
{
    MyAssert( g_pRTQGlobals );
    if( g_pRTQGlobals == 0 )
        return 0;

    const char* stringtodraw = text;
    if( g_pLanguageTable != 0 && text[0] == '.' )
        stringtodraw = g_pLanguageTable->LookUp( text );

    char* tempbuffer = g_pRTQGlobals->m_TempBuffer;
    va_list arg;
    va_start( arg, text );
    vsnprintf_s( tempbuffer, sizeof(g_pRTQGlobals->m_TempBuffer), _TRUNCATE, stringtodraw, arg );
    va_end(arg);

    return CreateString( concat, fontheight, x, y, 0, 0, justificationflags, color, size, tempbuffer );
}

int MyMeshText::CreateStringColorZAndRot(bool concat, float fontheight, float x, float y, float z, float rotz, unsigned char justificationflags, ColorByte color, Vector2 size, const char* text, ...)
{
    MyAssert( g_pRTQGlobals );
    if( g_pRTQGlobals == 0 )
        return 0;

    const char* stringtodraw = text;
    if( g_pLanguageTable != 0 && text[0] == '.' )
        stringtodraw = g_pLanguageTable->LookUp( text );

    char* tempbuffer = g_pRTQGlobals->m_TempBuffer;
    va_list arg;
    va_start( arg, text );
    vsnprintf_s( tempbuffer, sizeof(g_pRTQGlobals->m_TempBuffer), _TRUNCATE, stringtodraw, arg );
    va_end(arg);

    return CreateString( concat, fontheight, x, y, z, rotz, justificationflags, color, size, tempbuffer );
}

int MyMeshText::CreateStringColorShadowZAndRot(bool concat, float fontheight, float x, float y, float z, float rotz, unsigned char justificationflags, ColorByte color, ColorByte shadowcolor, float shadowoffx, float shadowoffy, Vector2 size, const char* text, ...)
{
    MyAssert( g_pRTQGlobals );
    if( g_pRTQGlobals == 0 )
        return 0;

    const char* stringtodraw = text;
    if( g_pLanguageTable != 0 && text[0] == '.' )
        stringtodraw = g_pLanguageTable->LookUp( text );

    char* tempbuffer = g_pRTQGlobals->m_TempBuffer;
    va_list arg;
    va_start( arg, text );
    vsnprintf_s( tempbuffer, sizeof(g_pRTQGlobals->m_TempBuffer), _TRUNCATE, stringtodraw, arg );
    va_end(arg);

    CreateString( concat, fontheight, x+shadowoffx, y+shadowoffy, z, rotz, justificationflags, shadowcolor, size, tempbuffer );
    concat = true;
    return CreateString( concat, fontheight, x, y, z, rotz, justificationflags, color, size, tempbuffer );
}

int MyMeshText::CreateStringColorShadowStyleZAndRot(bool concat, float fontheight, float x, float y, float z, float rotz, unsigned char justificationflags, ColorByte color, TextShadowStyles shadowstyle, ColorByte shadowcolor, float shadowoffx, float shadowoffy, Vector2 size, const char* text, ...)
{
    MyAssert( g_pRTQGlobals );
    if( g_pRTQGlobals == 0 )
        return 0;

    const char* stringtodraw = text;
    if( g_pLanguageTable != 0 && text[0] == '.' )
        stringtodraw = g_pLanguageTable->LookUp( text );

    char* tempbuffer = g_pRTQGlobals->m_TempBuffer;
    va_list arg;
    va_start( arg, text );
    vsnprintf_s( tempbuffer, sizeof(g_pRTQGlobals->m_TempBuffer), _TRUNCATE, stringtodraw, arg );
    va_end(arg);

    if( shadowstyle == TextShadowStyle_None )
    {
    }
    else if( shadowstyle == TextShadowStyle_Single )
    {
        CreateString( concat, fontheight, x+shadowoffx, y+shadowoffy, z, rotz, justificationflags, shadowcolor, size, tempbuffer );
    }
    else if( shadowstyle == TextShadowStyle_Quad )
    {
        CreateString( concat, fontheight, x+shadowoffx, y+shadowoffy, z, rotz, justificationflags, shadowcolor, size, tempbuffer );
        concat = true;
        CreateString( concat, fontheight, x-shadowoffy, y+shadowoffx, z, rotz, justificationflags, shadowcolor, size, tempbuffer );
        CreateString( concat, fontheight, x-shadowoffx, y-shadowoffy, z, rotz, justificationflags, shadowcolor, size, tempbuffer );
        CreateString( concat, fontheight, x+shadowoffy, y-shadowoffx, z, rotz, justificationflags, shadowcolor, size, tempbuffer );
    }
    else if( shadowstyle == TextShadowStyle_OctPlusPlus )
    {
        CreateString( concat, fontheight, x+shadowoffx, y+shadowoffy, z, rotz, justificationflags, shadowcolor, size, tempbuffer );
        concat = true;
        CreateString( concat, fontheight, x-shadowoffy, y+shadowoffx, z, rotz, justificationflags, shadowcolor, size, tempbuffer );
        CreateString( concat, fontheight, x-shadowoffx, y-shadowoffy, z, rotz, justificationflags, shadowcolor, size, tempbuffer );
        CreateString( concat, fontheight, x+shadowoffy, y-shadowoffx, z, rotz, justificationflags, shadowcolor, size, tempbuffer );

        CreateString( concat, fontheight, x           , y+shadowoffy, z, rotz, justificationflags, shadowcolor, size, tempbuffer );
        CreateString( concat, fontheight, x-shadowoffx, y           , z, rotz, justificationflags, shadowcolor, size, tempbuffer );
        CreateString( concat, fontheight, x           , y-shadowoffy, z, rotz, justificationflags, shadowcolor, size, tempbuffer );
        CreateString( concat, fontheight, x+shadowoffx, y           , z, rotz, justificationflags, shadowcolor, size, tempbuffer );

        CreateString( concat, fontheight, x+shadowoffx*2, y+shadowoffy*2, z, rotz, justificationflags, shadowcolor, size, tempbuffer );
        CreateString( concat, fontheight, x+shadowoffx*3, y+shadowoffy*3, z, rotz, justificationflags, shadowcolor, size, tempbuffer );
    }

    return CreateString( concat, fontheight, x, y, z, rotz, justificationflags, color, size, tempbuffer );
}

int MyMeshText::CreateString(bool concat, float fontheight, float x, float y, float z, float rotz, unsigned char justificationflags, ColorByte color, Vector2 size, const char* text, ...)
{
    //MyAssert( g_pRTQGlobals );
    //if( g_pRTQGlobals == 0 )
    //    return 0;

    MyAssert( m_pFont );
    if( m_pFont->IsFullyLoaded() == false )
        return 0;

    MyAssert( m_pFont->GetBMFont() );

    if( strlen( text ) == 0 )
        return 0;

    const char* stringtodraw = text;
    if( g_pLanguageTable != 0 && text[0] == '.' )
        stringtodraw = g_pLanguageTable->LookUp( text );

    int numlines = 0;

    if( concat == false )
    {
        ClearText();
    }

    bool moretexttocome = true;
    const char* stringpos = stringtodraw;

    while( moretexttocome )
    {
        numlines++;

        char singlelinebuffer[300];
        singlelinebuffer[0] = 0;
        char* singlelinebufferpos = singlelinebuffer;

        // word wrap if width of text is not 0.
        if( size.x > 0 )
        {
            float linewidth = -1;// = GetStringSize( fontheight, Vector2(0,0), singlelinebuffer ).x;
            while( linewidth < size.x &&
                    *stringpos != 0 )
            {
                *singlelinebufferpos = *stringpos;
                singlelinebufferpos++;
                *singlelinebufferpos = 0;
                stringpos++;

                linewidth = GetStringSize( fontheight, Vector2(0,0), singlelinebuffer ).x;

                MyAssert( singlelinebufferpos < singlelinebuffer + 300 );
            }

            int numcharswewentback = 0;
            while( ( *(singlelinebufferpos-1) != ' ' && *stringpos != 0 ) &&
                    singlelinebufferpos > singlelinebuffer )
            {
                singlelinebufferpos--;
                numcharswewentback++;
            }

            if( singlelinebufferpos != singlelinebuffer )
            {
                *singlelinebufferpos = 0;
                stringpos -= numcharswewentback;
            }

            if( *stringpos == 0 )
                moretexttocome = false;

            stringtodraw = singlelinebuffer;
        }
        else
        {
            moretexttocome = false;
        }

        //// don't bother drawing if fontheight is zero... still doing logic above so the currect number of lines will be returned.
        //if( g_pRTQGlobals->m_WordWrapCountLinesOnly )
        //    continue;

        Vertex_XYZUV_RGBA* pVertsToDraw = (Vertex_XYZUV_RGBA*)GetVerts( true );

        int newverts = (int)strlen( stringtodraw ) * 4;
#if _DEBUG
        m_MostLettersAttemptedToDrawThisFrame += newverts/4;
        if( m_MostLettersAttemptedToDrawThisFrame > m_MostLettersAttemptedToDrawEver )
            m_MostLettersAttemptedToDrawEver = m_MostLettersAttemptedToDrawThisFrame;
#endif

        MyAssert( m_SubmeshList.Count() > 0 );

        if( m_SubmeshList[0]->m_NumVertsToDraw + newverts > GetNumVerts() )
        {
#if _DEBUG
            LOGInfo( LOGTag, "TextMesh buffer isn't big enough for string (%s) - %d of %d letters used - most letters needed (%d)\n", stringtodraw, m_SubmeshList[0]->m_NumVertsToDraw/4, GetNumVerts()/4, m_MostLettersAttemptedToDrawEver );
#endif
            //MyAssert( false ); // drawing more than we have room for.
            return 0;
        }

        pVertsToDraw += m_SubmeshList[0]->m_NumVertsToDraw;

        unsigned int textstrlen = m_pFont->GetBMFont()->GenerateVerts( stringtodraw, true, pVertsToDraw, fontheight, GL_TRIANGLES, justificationflags, color );

        m_SubmeshList[0]->m_NumVertsToDraw += (unsigned short)(textstrlen * 4);
        m_SubmeshList[0]->m_NumIndicesToDraw += textstrlen * 6;

        MyMatrix position;
        position.SetIdentity();
        position.Rotate( rotz, 0, 0, 1 );
        position.SetTranslation( x, y - (numlines-1)*fontheight, z );
        //position.SetPosition( x, y - (numlines-1)*g_pRTQGlobals->m_WordWrapLineIncSize, z );
        //position.SetPosition( x, y, z );

        for( unsigned int i=0; i<textstrlen*4; i++ )
        {
            Vector3 out = position * (*(Vector3*)&pVertsToDraw[i].x);
            pVertsToDraw[i].x = out.x;
            pVertsToDraw[i].y = out.y;
            pVertsToDraw[i].z = out.z;
        }
    }

    m_MeshReady = true;

    return numlines;
}
