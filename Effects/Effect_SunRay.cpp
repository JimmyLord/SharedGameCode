//
// Copyright (c) 2012-2015 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "GameCommonHeader.h"
#include "Effect_SunRay.h"

#if Effect_SunRay_Enabled

Effect_SunRay::Effect_SunRay()
{
    m_TimeAlive = 0;

    m_pSpriteRayColor1 = MyNew MySprite_XYZVertexColor( false );
    m_pSpriteRayColor2 = MyNew MySprite_XYZVertexColor( false );

    m_Center = Vector2( 320.0f, 480.0f );
    m_NumSlices = 30;
    m_Length = 1000;
    m_Color1 = ColorByte(255,255,255,255);
    m_Color2 = ColorByte(255,0,0,255);
    m_Speed = 20;
    
    m_AngleOffset = 0;

    m_pSpriteRayColor1->Create( "Effect_SunRay", 1000, 20, 0, 1, 0, 1, Justify_Bottom | Justify_Left );
    //m_pSpriteRayColor1->SetShaderAndTexture( g_pGame->m_pShader_TextureVertexColor, g_pGame->m_pResources->m_pSpriteTextures[SL_WhiteSquare] );
    m_pSpriteRayColor1->SetPosition( 320.0f, 480.0f, 0.0f );

    m_pSpriteRayColor2->Create( "Effect_SunRay", 1000, 20, 0, 1, 0, 1, Justify_Bottom | Justify_Left );
    //m_pSpriteRayColor2->SetShaderAndTexture( g_pGame->m_pShader_TextureVertexColor, g_pGame->m_pResources->m_pSpriteTextures[SL_WhiteSquare] );
    m_pSpriteRayColor2->SetPosition( 320.0f, 480.0f, 0.0f );
}

Effect_SunRay::~Effect_SunRay()
{
    SAFE_RELEASE( m_pSpriteRayColor1 );
    SAFE_RELEASE( m_pSpriteRayColor2 );
}

void Effect_SunRay::Init()
{
    //LOGInfo( LOGTag, "Effect_SunRay::Init()\n" );
}

void Effect_SunRay::Init(Vector2 center, int numslices, float length, ColorByte color1, ColorByte color2, float speed)
{
    m_Center = center;
    m_NumSlices = numslices;
    m_Length = length;
    m_Color1 = color1;
    m_Color2 = color2;
    m_Speed = speed;

    float raythickness = sin((PI*2)/numslices) * m_Length / 2;
    raythickness *= 1.03f;

    Vertex_XYZUV_RGBA* pVerts;

    pVerts = (Vertex_XYZUV_RGBA*)m_pSpriteRayColor1->GetVerts( true );
    pVerts[0].x = 0.0f;      pVerts[0].y = 0.0f;
    pVerts[1].x = m_Length;  pVerts[1].y = raythickness;
    pVerts[2].x = 0;         pVerts[2].y = 0;
    pVerts[3].x = m_Length;  pVerts[3].y = -raythickness;

    pVerts = (Vertex_XYZUV_RGBA*)m_pSpriteRayColor2->GetVerts( true );
    pVerts[0].x = 0.0f;      pVerts[0].y = 0.0f;
    pVerts[1].x = m_Length;  pVerts[1].y = raythickness;
    pVerts[2].x = 0;         pVerts[2].y = 0;
    pVerts[3].x = m_Length;  pVerts[3].y = -raythickness;
}

void Effect_SunRay::Tick(double TimePassed)
{
    m_TimeAlive += TimePassed;

    m_AngleOffset += (float)(m_Speed * TimePassed);
}

void Effect_SunRay::Draw()
{
    if( m_Length == 0 )
        return;

    m_pSpriteRayColor1->SetPosition( m_Center.x, m_Center.y, 0.0f );
    m_pSpriteRayColor2->SetPosition( m_Center.x, m_Center.y, 0.0f );

    ColorByte color, coloralpha;

    color = m_Color1;
    coloralpha = color;
    coloralpha.a = 0;
    m_pSpriteRayColor1->SetVertexColors( color, coloralpha, color, coloralpha );

    color = m_Color2;
    coloralpha = color;
    coloralpha.a = 0;
    m_pSpriteRayColor2->SetVertexColors( color, coloralpha, color, coloralpha );

    float degreesperslice = 360.0f/m_NumSlices;
    float angleoffset = m_AngleOffset;
    for( int i=0; i<m_NumSlices; i+=2 )
    {
        m_pSpriteRayColor1->SetZRotation( angleoffset + i * degreesperslice );
        m_pSpriteRayColor1->Draw( &g_pGame->m_OrthoMatrixGameSize );
    }

    for( int i=1; i<m_NumSlices; i+=2 )
    {
        m_pSpriteRayColor2->SetZRotation( angleoffset + i * degreesperslice );
        m_pSpriteRayColor2->Draw( &g_pGame->m_OrthoMatrixGameSize );
    }
}

bool Effect_SunRay::OnTouch(int action, int id, float x, float y, float pressure, float size)
{
    return false;
}

bool Effect_SunRay::OnButtons(GameCoreButtonActions action, GameCoreButtonIDs id)
{
    return false;
}

#endif // EFFECT_SunRay_Enabled