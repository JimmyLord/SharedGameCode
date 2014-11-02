//
// Copyright (c) 2012-2014 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not
// claim that you wrote the original software. If you use this software
// in a product, an acknowledgment in the product documentation would be
// appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
// misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "GameCommonHeader.h"

#include "RenderTextQuick.h"

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
    m_BatchMode = false;
    m_BatchTexture = 0;
    m_BatchNumLetters = 0;
    m_VBONumVerts = 0;
    m_VBOsInitialized = false;
    m_pVertexBufferIDImmediate = 0;
    m_pVertexBuffer = 0;
    //BufferDefinition* m_pIndexBufferID = 0; // TODO: cleanup.

    m_WordWrap = false;
    m_WordWrapWidthLimit = 0;
    m_WordWrapLineIncSize = 0;
    m_WordWrapCountLinesOnly = false;
};

RenderTextQuickGlobals::~RenderTextQuickGlobals()
{
    if( m_pVertexBufferIDImmediate )
        m_pVertexBufferIDImmediate->Release();
    if( m_pVertexBuffer )
        m_pVertexBuffer->Release();
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

    return pFont->m_pFont->GetSize( tempbuffer, fontheight );
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
    assert( g_pRTQGlobals );

    const char* stringtodraw = text;
    if( g_pLanguageTable != 0 && text[0] == '.' )
        stringtodraw = g_pLanguageTable->LookUp( text );

    int numlines = 0;

    if( pFont && pFont->m_pFont && pFont->m_pTextureDef && pFont->m_pTextureDef->m_TextureID )
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

                    assert( singlelinebufferpos < singlelinebuffer + 300 );
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
                //    assert( g_pRTQGlobals->m_pVertexBufferIDImmediate->m_Dirty == false );
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
                assert( g_pRTQGlobals->m_pVertexBufferIDImmediate == 0 );
                assert( g_pRTQGlobals->m_pVertexBuffer == 0 );

                g_pRTQGlobals->m_VBOsInitialized = true;

                g_pRTQGlobals->m_VBONumVerts = 1200*6;

                // create a large buffer...
                g_pRTQGlobals->m_pVertexBufferIDImmediate = g_pBufferManager->CreateBuffer( 0, sizeof(Vertex_XYZUV_RGBA)*100*6, GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW, true, 2, VertexFormat_XYZUV_RGBA, "RenderTextQuick", "VertsImmediate" );

                g_pRTQGlobals->m_pVertexBuffer = g_pBufferManager->CreateBuffer( 0, sizeof(Vertex_XYZUV_RGBA)*g_pRTQGlobals->m_VBONumVerts, GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW, false, 2, VertexFormat_None, "RenderTextQuick", "Verts" );
            }

            textstrlen = pFont->m_pFont->GenerateVerts( stringtodraw, false, pVertToDraws, fontheight, GL_TRIANGLES, justificationflags, color );

            MyMatrix position;
            position.SetIdentity();
            position.Rotate( rotz, 0, 0, 1 );
            position.SetPosition( x, y - (numlines-1)*g_pRTQGlobals->m_WordWrapLineIncSize, z );

            //if( g_pRTQGlobals->m_pVertexBufferIDImmediate->m_Dirty )
            //{
            //    LOGInfo( LOGTag, "m_pVertexBufferIDImmediate->m_Dirty\n" );
            //    g_pRTQGlobals->m_pVertexBufferIDImmediate->Rebuild( 0, g_pRTQGlobals->m_pVertexBufferIDImmediate->m_DataSize );
            //}
            //assert( g_pRTQGlobals->m_pVertexBufferIDImmediate->m_Dirty == false );
            //checkGlError( "MyBindBuffer" );

            if( g_pRTQGlobals->m_BatchMode )
            {
                for( int i=0; i<textstrlen*6; i++ )
                {
                    Vector3 out = position.TransformVector3( *(Vector3*)&pVertToDraws[i].x );
                    pVertToDraws[i].x = out.x;
                    pVertToDraws[i].y = out.y;
                    pVertToDraws[i].z = out.z;
                }
                
                if( g_pRTQGlobals->m_BatchTexture != pFont->m_pTextureDef->m_TextureID ||
                    (g_pRTQGlobals->m_BatchNumLetters + textstrlen)*6 > g_pRTQGlobals->m_VBONumVerts )
                {
                    LOGInfo( LOGTag, "RenderTextQuick Batch Forced to render, look into it\n" );
                    RenderTextQuickEndBatch();
                    RenderTextQuickStartBatch( pFont );
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

                Shader_Base* pShader = (Shader_Base*)g_pGame->m_pShader_TextureVertexColor->GlobalPass();
                if( pShader )
                {   
                    int size = sizeof(Vertex_XYZUV_RGBA)*textstrlen*6;
                    memcpy( &g_pRTQGlobals->m_pVertexBufferIDImmediate->m_pData[0], pVertToDraws, size );
                    //(Vertex_XYZUV_RGBA*)g_pRTQGlobals->m_pVertexBufferIDImmediate->m_pData,6

                    g_pRTQGlobals->m_pVertexBufferIDImmediate->m_Dirty = true;
                    g_pRTQGlobals->m_pVertexBufferIDImmediate->Rebuild( 0, sizeof(Vertex_XYZUV_RGBA)*textstrlen*6 );

                    if( pShader->ActivateAndProgramShader(
                        VertexFormat_XYZUV_RGBA, g_pRTQGlobals->m_pVertexBufferIDImmediate, 0, GL_UNSIGNED_SHORT,
                        &g_pGame->m_OrthoMatrixGameSize, &position, pFont->m_pTextureDef->m_TextureID ) )
                    //if( pShader->ActivateAndProgramShader( &g_pGame->m_OrthoMatrixGameSize,
                    //    &position, pVertToDraws, pFont->m_pTextureDef->m_TextureID ) )
                    {
#if USE_D3D
                        g_pD3DContext->Draw( textstrlen*6, 0 );
#else
                        MyDrawArrays( GL_TRIANGLES, 0, textstrlen*6 );
                        //MyDrawElements( GL_TRIANGLES, textstrlen*6, GL_UNSIGNED_SHORT, 0 );   //The starting point of the IBO
#endif
                        pShader->DeactivateShader( g_pRTQGlobals->m_pVertexBufferIDImmediate );
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

void RenderTextQuickStartBatch(FontDefinition* pFont)
{
    assert( g_pRTQGlobals );
    assert( pFont );
    assert( pFont->m_pTextureDef );
    assert( pFont->m_pTextureDef->m_TextureID );

    if( g_pRTQGlobals->m_BatchMode == true && g_pRTQGlobals->m_BatchNumLetters )
    {
        if( g_pRTQGlobals->m_BatchTexture == pFont->m_pTextureDef->m_TextureID )
        {
            LOGInfo( LOGTag, "RenderTextQuickStartBatch called without endbatch and with same texture\n" );
        }
        else
        {
            LOGInfo( LOGTag, "RenderTextQuickStartBatch called without an endbatch\n" );
            RenderTextQuickEndBatch();
        }
    }

    g_pRTQGlobals->m_BatchMode = true;
    g_pRTQGlobals->m_BatchTexture = pFont->m_pTextureDef->m_TextureID;
    g_pRTQGlobals->m_BatchNumLetters = 0;
}

void RenderTextQuickEndBatch()
{
    assert( g_pRTQGlobals );

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
    assert( g_pRTQGlobals->m_pVertexBuffer->m_Dirty == false );

    Shader_Base* pShader = (Shader_Base*)g_pGame->m_pShader_TextureVertexColor->GlobalPass();
    if( pShader )
    {
        if( pShader->ActivateAndProgramShader(
                VertexFormat_XYZUV_RGBA, g_pRTQGlobals->m_pVertexBuffer, 0, GL_UNSIGNED_SHORT,
                &g_pGame->m_OrthoMatrixGameSize, &position, g_pRTQGlobals->m_BatchTexture ) )
        {
#if USE_D3D
            //g_pD3DContext->Draw( textstrlen*6, 0 );
            MyDrawArrays( GL_TRIANGLES, 0, g_pRTQGlobals->m_BatchNumLetters*6 );
#else
            MyDrawArrays( GL_TRIANGLES, 0, g_pRTQGlobals->m_BatchNumLetters*6 );
            //MyDrawElements( GL_TRIANGLES, textstrlen*6, GL_UNSIGNED_SHORT, 0 );   //The starting point of the IBO
#endif
            pShader->DeactivateShader( g_pRTQGlobals->m_pVertexBuffer );
        }
    }

    g_pRTQGlobals->m_BatchNumLetters = 0;
    g_pRTQGlobals->m_BatchTexture = 0;
}

