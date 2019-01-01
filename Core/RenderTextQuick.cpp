//
// Copyright (c) 2012-2018 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "SharedCommonHeader.h"
#include "../Menus/LanguageTable.h"
#include "../Framework/MyFramework/SourceCommon/Renderers/Renderer_Enums.h"
#include "../Framework/MyFramework/SourceCommon/Renderers/Renderer_Base.h"
#include "RenderTextQuick.h"

// TODO: Fix GL Includes.
#include <gl/GL.h>
#include "../Framework/MyFramework/SourceCommon/GL/GLext.h"

int g_TextShadowStyleLetterCount[TextShadowStyle_NumStyles] =
{
    1,
    2,
    5,
    9,
    11,
};

RenderTextQuickGlobals* g_pRTQGlobals = 0;

RenderTextQuickGlobals::RenderTextQuickGlobals()
{
    MyAssert( g_pMaterialManager );

    m_pMatProj = 0;
    m_pMatView = 0;

    m_BatchMode = false;
    m_pBatchTexture = 0;
    m_BatchNumLetters = 0;
    m_VBONumVerts = 0;
    m_VBOsInitialized = false;
    m_pVertexBufferIDImmediate = 0;
    m_pVertexBuffer = 0;
    //BufferDefinition* m_pIndexBufferID = 0; // TODO: cleanup.

    m_pMaterial = 0;

    m_WordWrap = false;
    m_WordWrapWidthLimit = 0;
    m_WordWrapLineIncSize = 0;
    m_WordWrapCountLinesOnly = false;
};

RenderTextQuickGlobals::~RenderTextQuickGlobals()
{
    SAFE_RELEASE( m_pVertexBufferIDImmediate );
    SAFE_RELEASE( m_pVertexBuffer );

    SAFE_RELEASE( m_pMaterial );
}

void RenderTextQuickGlobals::SetWrapMode(bool wrap, int widthlimit, int lineincsize, bool countonly)
{
    m_WordWrap = wrap;
    m_WordWrapWidthLimit = widthlimit;
    m_WordWrapLineIncSize = lineincsize;
    m_WordWrapCountLinesOnly = countonly;
}

Vector2 GetStringSize(FontDefinition* pFont, float fontheight, const char* text, ...)
{
    const char* stringtodraw = text;
    if( g_pLanguageTable != 0 && text[0] == '.' )
        stringtodraw = g_pLanguageTable->LookUp( text );

    char tempbuffer[4096];
    va_list arg;
    va_start( arg, text );
    vsnprintf_s( tempbuffer, sizeof(tempbuffer), _TRUNCATE, stringtodraw, arg );
    va_end(arg);

    return pFont->GetBMFont()->GetSize( tempbuffer, fontheight );
}

int RenderTextQuick(FontDefinition* pFont, float fontheight, float x, float y, unsigned char justificationflags, const char* text, ...)
{
    const char* stringtodraw = text;
    if( g_pLanguageTable != 0 && text[0] == '.' )
        stringtodraw = g_pLanguageTable->LookUp( text );

    char* tempbuffer = g_pRTQGlobals->m_TempBuffer;
    va_list arg;
    va_start( arg, text );
    vsnprintf_s( tempbuffer, sizeof(g_pRTQGlobals->m_TempBuffer), _TRUNCATE, stringtodraw, arg );
    va_end(arg);

    ColorByte color(255, 255, 255, 255);

    return RenderTextQuickWithEverything( pFont, fontheight, x, y, 0, 0, justificationflags, color, tempbuffer );
}

int RenderTextQuickWithColor(FontDefinition* pFont, float fontheight, float x, float y, unsigned char justificationflags, ColorByte color, const char* text, ...)
{
    const char* stringtodraw = text;
    if( g_pLanguageTable != 0 && text[0] == '.' )
        stringtodraw = g_pLanguageTable->LookUp( text );

    char* tempbuffer = g_pRTQGlobals->m_TempBuffer;
    va_list arg;
    va_start( arg, text );
    vsnprintf_s( tempbuffer, sizeof(g_pRTQGlobals->m_TempBuffer), _TRUNCATE, stringtodraw, arg );
    va_end(arg);

    return RenderTextQuickWithEverything( pFont, fontheight, x, y, 0, 0, justificationflags, color, tempbuffer );
}

int RenderTextQuickWithColorAndShadow(FontDefinition* pFont, float fontheight, float x, float y, unsigned char justificationflags, ColorByte color, ColorByte shadowcolor, float shadowoffx, float shadowoffy, const char* text, ...)
{
    const char* stringtodraw = text;
    if( g_pLanguageTable != 0 && text[0] == '.' )
        stringtodraw = g_pLanguageTable->LookUp( text );

    char* tempbuffer = g_pRTQGlobals->m_TempBuffer;
    va_list arg;
    va_start( arg, text );
    vsnprintf_s( tempbuffer, sizeof(g_pRTQGlobals->m_TempBuffer), _TRUNCATE, stringtodraw, arg );
    va_end(arg);

    RenderTextQuickWithEverything( pFont, fontheight, x+shadowoffx, y+shadowoffy, 0, 0, justificationflags, shadowcolor, tempbuffer );
    return RenderTextQuickWithEverything( pFont, fontheight, x, y, 0, 0, justificationflags, color, tempbuffer );
}

int RenderTextQuickWithColorAndShadowStyle(FontDefinition* pFont, float fontheight, float x, float y, unsigned char justificationflags, ColorByte color, TextShadowStyles shadowstyle, ColorByte shadowcolor, float shadowoffx, float shadowoffy, const char* text, ...)
{
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
        RenderTextQuickWithEverything( pFont, fontheight, x+shadowoffx, y+shadowoffy, 0, 0, justificationflags, shadowcolor, tempbuffer );
    }
    else if( shadowstyle == TextShadowStyle_Quad )
    {
        RenderTextQuickWithEverything( pFont, fontheight, x+shadowoffx, y+shadowoffy, 0, 0, justificationflags, shadowcolor, tempbuffer );
        RenderTextQuickWithEverything( pFont, fontheight, x-shadowoffy, y+shadowoffx, 0, 0, justificationflags, shadowcolor, tempbuffer );
        RenderTextQuickWithEverything( pFont, fontheight, x-shadowoffx, y-shadowoffy, 0, 0, justificationflags, shadowcolor, tempbuffer );
        RenderTextQuickWithEverything( pFont, fontheight, x+shadowoffy, y-shadowoffx, 0, 0, justificationflags, shadowcolor, tempbuffer );
    }
    else if( shadowstyle == TextShadowStyle_OctPlusPlus )
    {
        RenderTextQuickWithEverything( pFont, fontheight, x+shadowoffx, y+shadowoffy, 0, 0, justificationflags, shadowcolor, tempbuffer );
        RenderTextQuickWithEverything( pFont, fontheight, x-shadowoffy, y+shadowoffx, 0, 0, justificationflags, shadowcolor, tempbuffer );
        RenderTextQuickWithEverything( pFont, fontheight, x-shadowoffx, y-shadowoffy, 0, 0, justificationflags, shadowcolor, tempbuffer );
        RenderTextQuickWithEverything( pFont, fontheight, x+shadowoffy, y-shadowoffx, 0, 0, justificationflags, shadowcolor, tempbuffer );

        RenderTextQuickWithEverything( pFont, fontheight, x           , y+shadowoffy, 0, 0, justificationflags, shadowcolor, tempbuffer );
        RenderTextQuickWithEverything( pFont, fontheight, x-shadowoffx, y           , 0, 0, justificationflags, shadowcolor, tempbuffer );
        RenderTextQuickWithEverything( pFont, fontheight, x           , y-shadowoffy, 0, 0, justificationflags, shadowcolor, tempbuffer );
        RenderTextQuickWithEverything( pFont, fontheight, x+shadowoffx, y           , 0, 0, justificationflags, shadowcolor, tempbuffer );

        RenderTextQuickWithEverything( pFont, fontheight, x+shadowoffx*2, y+shadowoffy*2, 0, 0, justificationflags, shadowcolor, tempbuffer );
        RenderTextQuickWithEverything( pFont, fontheight, x+shadowoffx*3, y+shadowoffy*3, 0, 0, justificationflags, shadowcolor, tempbuffer );
    }

    return RenderTextQuickWithEverything( pFont, fontheight, x, y, 0, 0, justificationflags, color, tempbuffer );
}

int RenderTextQuickWithColorAndZ(FontDefinition* pFont, float fontheight, float x, float y, float z, unsigned char justificationflags, ColorByte color, const char* text, ...)
{
    const char* stringtodraw = text;
    if( g_pLanguageTable != 0 && text[0] == '.' )
        stringtodraw = g_pLanguageTable->LookUp( text );

    char* tempbuffer = g_pRTQGlobals->m_TempBuffer;
    va_list arg;
    va_start( arg, text );
    vsnprintf_s( tempbuffer, sizeof(g_pRTQGlobals->m_TempBuffer), _TRUNCATE, stringtodraw, arg );
    va_end(arg);

    return RenderTextQuickWithEverything( pFont, fontheight, x, y, 0, 0, justificationflags, color, tempbuffer );
}

int RenderTextQuickWithColorZAndRot(FontDefinition* pFont, float fontheight, float x, float y, float z, float rotz, unsigned char justificationflags, ColorByte color, const char* text, ...)
{
    const char* stringtodraw = text;
    if( g_pLanguageTable != 0 && text[0] == '.' )
        stringtodraw = g_pLanguageTable->LookUp( text );

    char* tempbuffer = g_pRTQGlobals->m_TempBuffer;
    va_list arg;
    va_start( arg, text );
    vsnprintf_s( tempbuffer, sizeof(g_pRTQGlobals->m_TempBuffer), _TRUNCATE, stringtodraw, arg );
    va_end(arg);

    return RenderTextQuickWithEverything( pFont, fontheight, x, y, z, rotz, justificationflags, color, tempbuffer );
}

int RenderTextQuickWithColorShadowZAndRot(FontDefinition* pFont, float fontheight, float x, float y, float z, float rotz, unsigned char justificationflags, ColorByte color, ColorByte shadowcolor, float shadowoffx, float shadowoffy, const char* text, ...)
{
    const char* stringtodraw = text;
    if( g_pLanguageTable != 0 && text[0] == '.' )
        stringtodraw = g_pLanguageTable->LookUp( text );

    char* tempbuffer = g_pRTQGlobals->m_TempBuffer;
    va_list arg;
    va_start( arg, text );
    vsnprintf_s( tempbuffer, sizeof(g_pRTQGlobals->m_TempBuffer), _TRUNCATE, stringtodraw, arg );
    va_end(arg);

    RenderTextQuickWithEverything( pFont, fontheight, x+shadowoffx, y+shadowoffy, z, rotz, justificationflags, shadowcolor, tempbuffer );
    return RenderTextQuickWithEverything( pFont, fontheight, x, y, z, rotz, justificationflags, color, tempbuffer );
}

int RenderTextQuickWithColorShadowStyleZAndRot(FontDefinition* pFont, float fontheight, float x, float y, float z, float rotz, unsigned char justificationflags, ColorByte color, TextShadowStyles shadowstyle, ColorByte shadowcolor, float shadowoffx, float shadowoffy, const char* text, ...)
{
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
        RenderTextQuickWithEverything( pFont, fontheight, x+shadowoffx, y+shadowoffy, z, rotz, justificationflags, shadowcolor, tempbuffer );
    }
    else if( shadowstyle == TextShadowStyle_Quad )
    {
        RenderTextQuickWithEverything( pFont, fontheight, x+shadowoffx, y+shadowoffy, z, rotz, justificationflags, shadowcolor, tempbuffer );
        RenderTextQuickWithEverything( pFont, fontheight, x-shadowoffy, y+shadowoffx, z, rotz, justificationflags, shadowcolor, tempbuffer );
        RenderTextQuickWithEverything( pFont, fontheight, x-shadowoffx, y-shadowoffy, z, rotz, justificationflags, shadowcolor, tempbuffer );
        RenderTextQuickWithEverything( pFont, fontheight, x+shadowoffy, y-shadowoffx, z, rotz, justificationflags, shadowcolor, tempbuffer );
    }
    else if( shadowstyle == TextShadowStyle_OctPlusPlus )
    {
        RenderTextQuickWithEverything( pFont, fontheight, x+shadowoffx, y+shadowoffy, z, rotz, justificationflags, shadowcolor, tempbuffer );
        RenderTextQuickWithEverything( pFont, fontheight, x-shadowoffy, y+shadowoffx, z, rotz, justificationflags, shadowcolor, tempbuffer );
        RenderTextQuickWithEverything( pFont, fontheight, x-shadowoffx, y-shadowoffy, z, rotz, justificationflags, shadowcolor, tempbuffer );
        RenderTextQuickWithEverything( pFont, fontheight, x+shadowoffy, y-shadowoffx, z, rotz, justificationflags, shadowcolor, tempbuffer );

        RenderTextQuickWithEverything( pFont, fontheight, x           , y+shadowoffy, z, rotz, justificationflags, shadowcolor, tempbuffer );
        RenderTextQuickWithEverything( pFont, fontheight, x-shadowoffx, y           , z, rotz, justificationflags, shadowcolor, tempbuffer );
        RenderTextQuickWithEverything( pFont, fontheight, x           , y-shadowoffy, z, rotz, justificationflags, shadowcolor, tempbuffer );
        RenderTextQuickWithEverything( pFont, fontheight, x+shadowoffx, y           , z, rotz, justificationflags, shadowcolor, tempbuffer );

        RenderTextQuickWithEverything( pFont, fontheight, x+shadowoffx*2, y+shadowoffy*2, z, rotz, justificationflags, shadowcolor, tempbuffer );
        RenderTextQuickWithEverything( pFont, fontheight, x+shadowoffx*3, y+shadowoffy*3, z, rotz, justificationflags, shadowcolor, tempbuffer );
    }

    return RenderTextQuickWithEverything( pFont, fontheight, x, y, z, rotz, justificationflags, color, tempbuffer );
}

int RenderTextQuickWithEverything(FontDefinition* pFont, float fontheight, float x, float y, float z, float rotz, unsigned char justificationflags, ColorByte color, const char* text)
{
    MyAssert( g_pRTQGlobals );
    if( g_pRTQGlobals == 0 )
        return 0;

    if( g_pRTQGlobals->m_pMaterial == 0 )
        g_pRTQGlobals->m_pMaterial = g_pMaterialManager->CreateMaterial();

    const char* stringtodraw = text;
    if( g_pLanguageTable != 0 && text[0] == '.' )
        stringtodraw = g_pLanguageTable->LookUp( text );

    int numlines = 0;

    if( pFont && pFont->GetBMFont() && pFont->GetTexture() && pFont->GetTexture()->GetTextureID() )
    {
        bool moretexttocome = true;
        const char* stringpos = stringtodraw;

        while( moretexttocome )
        {
            numlines++;

            char singlelinebuffer[300];
            singlelinebuffer[0] = 0;
            char* singlelinebufferpos = singlelinebuffer;

            if( g_pRTQGlobals->m_WordWrap == true )
            {
                while( GetStringSize( pFont, fontheight, singlelinebuffer ).x < g_pRTQGlobals->m_WordWrapWidthLimit &&
                       *stringpos != 0 )
                {
                    *singlelinebufferpos = *stringpos;
                    singlelinebufferpos++;
                    *singlelinebufferpos = 0;
                    stringpos++;

                    MyAssert( singlelinebufferpos < singlelinebuffer + 300 );
                }

                while( ( *(singlelinebufferpos-1) != ' ' && *stringpos != 0 ) &&
                       singlelinebufferpos > singlelinebuffer )
                {
                    singlelinebufferpos--;
                    *singlelinebufferpos = 0;
                    stringpos--;
                }

                if( *stringpos == 0 )
                    moretexttocome = false;

                stringtodraw = singlelinebuffer;
            }
            else
            {
                moretexttocome = false;
            }

            // don't bother drawing if fontheight is zero... still doing logic above so the currect number of lines will be returned.
            if( g_pRTQGlobals->m_WordWrapCountLinesOnly )
                continue;

            Vertex_XYZUV_RGBA staticverts[100*6];
            Vertex_XYZUV_RGBA* pAllocatedVerts = 0;


            // Rebuild buffers if they get invalidated.
            {
                //if( g_pRTQGlobals->m_pVertexBufferIDImmediate )
                //{
                //    if( g_pRTQGlobals->m_pVertexBufferIDImmediate->m_Dirty )
                //    {
                //        LOGInfo( LOGTag, "m_pVertexBufferIDImmediate->m_Dirty\n" );
                //        g_pRTQGlobals->m_pVertexBufferIDImmediate->Rebuild(
                //            0, g_pRTQGlobals->m_pVertexBufferIDImmediate->m_DataSize );
                //    }
                //    MyAssert( g_pRTQGlobals->m_pVertexBufferIDImmediate->m_Dirty == false );
                //}
            }

            Vertex_XYZUV_RGBA* pVertToDraws = staticverts;

            int textstrlen = (int)strlen( stringtodraw ); // might be too long with \n's(other), fixed below.
            if( textstrlen >= 100 )
            {
                pAllocatedVerts = MyNew Vertex_XYZUV_RGBA[textstrlen*6];
                pVertToDraws = pAllocatedVerts;
                LOGInfo( LOGTag, "RenderTextQuickWithColorZAndRot() is allocating memory to print %s\n", stringtodraw );
            }


            if( g_pRTQGlobals->m_VBOsInitialized == false )
            {
                MyAssert( g_pRTQGlobals->m_pVertexBufferIDImmediate == 0 );
                MyAssert( g_pRTQGlobals->m_pVertexBuffer == 0 );

                g_pRTQGlobals->m_VBOsInitialized = true;

                g_pRTQGlobals->m_VBONumVerts = 1200*6;

                // create a large buffer...
                g_pRTQGlobals->m_pVertexBufferIDImmediate = g_pBufferManager->CreateBuffer( 0, sizeof(Vertex_XYZUV_RGBA)*100*6, MyRE::BufferType_Vertex, MyRE::BufferUsage_DynamicDraw, true, 2, VertexFormat_XYZUV_RGBA, "RenderTextQuick", "VertsImmediate" );

                g_pRTQGlobals->m_pVertexBuffer = g_pBufferManager->CreateBuffer( 0, sizeof(Vertex_XYZUV_RGBA)*g_pRTQGlobals->m_VBONumVerts, MyRE::BufferType_Vertex, MyRE::BufferUsage_DynamicDraw, false, 2, VertexFormat_XYZUV_RGBA, "RenderTextQuick", "Verts" );
            }

            textstrlen = pFont->GetBMFont()->GenerateVerts( stringtodraw, false, pVertToDraws, fontheight, justificationflags, color );

            MyMatrix position;
            position.SetIdentity();
            position.Rotate( rotz, 0, 0, 1 );
            position.SetTranslation( x, y - (numlines-1)*g_pRTQGlobals->m_WordWrapLineIncSize, z );

            //if( g_pRTQGlobals->m_pVertexBufferIDImmediate->m_Dirty )
            //{
            //    LOGInfo( LOGTag, "m_pVertexBufferIDImmediate->m_Dirty\n" );
            //    g_pRTQGlobals->m_pVertexBufferIDImmediate->Rebuild( 0, g_pRTQGlobals->m_pVertexBufferIDImmediate->m_DataSize );
            //}
            //MyAssert( g_pRTQGlobals->m_pVertexBufferIDImmediate->m_Dirty == false );
            //checkGlError( "MyBindBuffer" );

            if( g_pRTQGlobals->m_BatchMode )
            {
                for( int i=0; i<textstrlen*6; i++ )
                {
                    Vector3 out = position * (*(Vector3*)&pVertToDraws[i].x);
                    pVertToDraws[i].x = out.x;
                    pVertToDraws[i].y = out.y;
                    pVertToDraws[i].z = out.z;
                }

                if( g_pRTQGlobals->m_pBatchTexture != pFont->GetTexture() ||
                    (g_pRTQGlobals->m_BatchNumLetters + textstrlen)*6 > g_pRTQGlobals->m_VBONumVerts )
                {
                    LOGInfo( LOGTag, "RenderTextQuick Batch Forced to render, look into it\n" );
                    RenderTextQuickBatchEnd();
                    RenderTextQuickBatchStart( pFont );
                }

                int offset = sizeof(Vertex_XYZUV_RGBA)*g_pRTQGlobals->m_BatchNumLetters*6;
                int size = sizeof(Vertex_XYZUV_RGBA)*textstrlen*6;
                memcpy( &g_pRTQGlobals->m_pVertexBuffer->m_pData[offset], pVertToDraws, size );

                g_pRTQGlobals->m_BatchNumLetters += textstrlen;

                //if( g_pRTQGlobals->m_VBOsInitialized )
                //{
                //    MyBindBuffer( GL_ARRAY_BUFFER, g_pRTQGlobals->m_pVertexBuffer->m_CurrentBufferID );

                //    //int size = 0;
                //    //glGetBufferParameteriv( GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size );

                //    //glBufferData( GL_ARRAY_BUFFER, sizeof(Vertex_XYZUV_RGBA)*g_BatchNumLetters*6, pVertToDraws, GL_DYNAMIC_DRAW );
                //    glBufferSubData( GL_ARRAY_BUFFER, sizeof(Vertex_XYZUV_RGBA)*g_pRTQGlobals->m_BatchNumLetters*6,
                //                     sizeof(Vertex_XYZUV_RGBA)*textstrlen*6, (void*)pVertToDraws );

                //    checkGlError( "glBufferSubData" );

                //    g_pRTQGlobals->m_BatchNumLetters += textstrlen;
                //}
            }
            else
            {
                //return 0;

                //if( g_pRTQGlobals->m_VBOsInitialized ) //m_pVertexBuffer != 0 )
                //{
                //    MyBindBuffer( GL_ARRAY_BUFFER, g_pRTQGlobals->m_pVertexBufferIDImmediate->m_CurrentBufferID );
                //    glBufferData( GL_ARRAY_BUFFER, sizeof(Vertex_XYZUV_RGBA)*textstrlen*6, pVertToDraws, GL_DYNAMIC_DRAW );
                //    //glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(Vertex_XYZUV_RGBA)*textstrlen*6, (void*)pVertToDraws );
                //}

                // TODO: MYENGINE
                //Shader_Base* pShader = 0; //(Shader_Base*)g_pGame->m_pShader_TextureVertexColor->GlobalPass();
                ShaderGroup* pShaderGroup = g_pRTQGlobals->m_pMaterial->GetShader();
                if( pShaderGroup )
                {
                    Shader_Base* pShader = (Shader_Base*)pShaderGroup->GlobalPass();
                    if( pShader )
                    {
                        int size = sizeof(Vertex_XYZUV_RGBA)*textstrlen*6;
                        memcpy( &g_pRTQGlobals->m_pVertexBufferIDImmediate->m_pData[0], pVertToDraws, size );
                        //(Vertex_XYZUV_RGBA*)g_pRTQGlobals->m_pVertexBufferIDImmediate->m_pData,6

                        g_pRTQGlobals->m_pVertexBufferIDImmediate->m_Dirty = true;
                        g_pRTQGlobals->m_pVertexBufferIDImmediate->Rebuild( 0, sizeof(Vertex_XYZUV_RGBA)*textstrlen*6 );

                        // TODO: MYENGINE
                        //g_pRTQGlobals->m_pMaterial->SetShader( g_pGame->m_pShader_TextureVertexColor );
                        g_pRTQGlobals->m_pMaterial->SetTextureColor( pFont->GetTexture() );

                        // Enable blending if necessary. TODO: sort draws and only set this once.
                        if( g_pRTQGlobals->m_pMaterial->IsTransparent( pShader ) )
                        {
                            g_pRenderer->SetBlendEnabled( true );
                            g_pRenderer->SetBlendFunc( MyRE::BlendFactor_SrcAlpha, MyRE::BlendFactor_OneMinusSrcAlpha );
                        }

                        if( pShader->ActivateAndProgramShader(
                            g_pRTQGlobals->m_pVertexBufferIDImmediate, 0, MyRE::IndexType_U16,
                            g_pRTQGlobals->m_pMatProj, g_pRTQGlobals->m_pMatView, //&g_pGame->m_OrthoMatrixGameSize,
                            &position, g_pRTQGlobals->m_pMaterial ) )
                        //if( pShader->ActivateAndProgramShader( &g_pGame->m_OrthoMatrixGameSize,
                        //    &position, pVertToDraws, pFont->GetTexture()->m_TextureID ) )
                        {
    #if USE_D3D
                            g_pD3DContext->Draw( textstrlen*6, 0 );
    #else
                            g_pRenderer->DrawArrays( MyRE::PrimitiveType_Triangles, 0, textstrlen*6, false );
                            //g_pRenderer->DrawElements( MyRE::PrimitiveType_Triangles, textstrlen*6, MyRE::IndexType_U16, 0 );   //The starting point of the IBO
    #endif
                            pShader->DeactivateShader( g_pRTQGlobals->m_pVertexBufferIDImmediate, true );
                        }

                        // Always disable blending.
                        g_pRenderer->SetBlendEnabled( false );
                    }
                }
            }

            // if we did draw too long a string, free the memory we allocated... inefficient, but better than it was.
            // TODO: fix this eventually... for now LOGInfo should spit out warning if this is happening.
            if( pAllocatedVerts )
                delete[] pAllocatedVerts;
        }
    }

    return numlines;
}

void RenderTextQuickBatchStart(FontDefinition* pFont)
{
    MyAssert( g_pRTQGlobals );
    MyAssert( pFont );
    MyAssert( pFont->GetTexture() );
    MyAssert( pFont->GetTexture()->GetTextureID() );

    if( g_pRTQGlobals->m_BatchMode == true && g_pRTQGlobals->m_BatchNumLetters )
    {
        if( g_pRTQGlobals->m_pBatchTexture == pFont->GetTexture() )
        {
            LOGInfo( LOGTag, "RenderTextQuickBatchStart called without endbatch and with same texture\n" );
        }
        else
        {
            LOGInfo( LOGTag, "RenderTextQuickBatchStart called without an endbatch\n" );
            RenderTextQuickBatchEnd();
        }
    }

    g_pRTQGlobals->m_BatchMode = true;
    g_pRTQGlobals->m_pBatchTexture = pFont->GetTexture();
    g_pRTQGlobals->m_BatchNumLetters = 0;
}

void RenderTextQuickBatchEnd()
{
    MyAssert( g_pRTQGlobals );
    MyAssert( g_pRTQGlobals->m_pMaterial );

    if( g_pRTQGlobals->m_BatchMode == false )
        return;

    g_pRTQGlobals->m_BatchMode = false;

    if( g_pRTQGlobals->m_BatchNumLetters == 0 )
        return;

    MyMatrix position;
    position.SetIdentity();

    // rebuild buffer if dirty.
    g_pRTQGlobals->m_pVertexBuffer->m_Dirty = true;
    g_pRTQGlobals->m_pVertexBuffer->Rebuild( 0, sizeof(Vertex_XYZUV_RGBA)*g_pRTQGlobals->m_BatchNumLetters*6 );
    MyAssert( g_pRTQGlobals->m_pVertexBuffer->m_Dirty == false );

    // TODO: MYENGINE
    //Shader_Base* pShader = 0;//(Shader_Base*)g_pGame->m_pShader_TextureVertexColor->GlobalPass();
    ShaderGroup* pShaderGroup = g_pRTQGlobals->m_pMaterial->GetShader();
    if( pShaderGroup )
    {
        Shader_Base* pShader = (Shader_Base*)pShaderGroup->GlobalPass();
        if( pShader )
        {
            // Enable blending if necessary. TODO: sort draws and only set this once.
            if( g_pRTQGlobals->m_pMaterial->IsTransparent( pShader ) )
            {
                g_pRenderer->SetBlendEnabled( true );
                g_pRenderer->SetBlendFunc( MyRE::BlendFactor_SrcAlpha, MyRE::BlendFactor_OneMinusSrcAlpha );
            }

            // TODO: MYENGINE
            //g_pRTQGlobals->m_pMaterial->SetShader( g_pGame->m_pShader_TextureVertexColor );
            g_pRTQGlobals->m_pMaterial->SetTextureColor( g_pRTQGlobals->m_pBatchTexture );

            if( pShader->ActivateAndProgramShader(
                    g_pRTQGlobals->m_pVertexBuffer, 0, MyRE::IndexType_U16,
                    g_pRTQGlobals->m_pMatProj, g_pRTQGlobals->m_pMatView, //&g_pGame->m_OrthoMatrixGameSize,
                    &position, g_pRTQGlobals->m_pMaterial ) )
            {
    #if USE_D3D
                //g_pD3DContext->Draw( textstrlen*6, 0 );
                g_pRenderer->DrawArrays( GL_TRIANGLES, 0, g_pRTQGlobals->m_BatchNumLetters*6, false );
    #else
                g_pRenderer->DrawArrays( MyRE::PrimitiveType_Triangles, 0, g_pRTQGlobals->m_BatchNumLetters*6, false );
                //g_pRenderer->DrawElements( MyRE::PrimitiveType_Triangles, textstrlen*6, MyRE::IndexType_U16, 0, false );   //The starting point of the IBO
    #endif
                pShader->DeactivateShader( g_pRTQGlobals->m_pVertexBuffer, true );
            }

            // Always disable blending.
            g_pRenderer->SetBlendEnabled( false );
        }
    }

    g_pRTQGlobals->m_BatchNumLetters = 0;
    g_pRTQGlobals->m_pBatchTexture = 0;
}
