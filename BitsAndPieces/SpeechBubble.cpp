//
// Copyright (c) 2014-2015 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "GameCommonHeader.h"
#include "SpeechBubble.h"
#include "../../Framework/MyFramework/SourceCommon/Sprites/MySprite9.h"

SpeechBubble::SpeechBubble()
{
    m_pBubble = MyNew MySprite9();
    m_pPoint = MyNew MySprite( false );
    m_pTextMesh = MyNew MyMeshText( 100, g_pGame->m_pSystemFont ); // 50 chars with shadow
    m_String = 0;

    //m_CurrentPos;
    m_CurrentColor.Set( 150,150,150,0 );
    m_CurrentWidth = 50;
    m_CurrentHeight = 20;

    //m_DestPos;
    m_DestColor.Set( 150,150,150,0 );
    m_DestWidth = 50;
    m_DestHeight = 20;

    // set materials.
    MaterialDefinition* pMaterial = 0;

    pMaterial = g_pMaterialManager->FindMaterialByFilename( "Data/Materials/Nevis60.mymaterial" );
    m_pTextMesh->SetMaterial( pMaterial, 0 );

    pMaterial = g_pMaterialManager->FindMaterialByFilename( "Data/Materials/SpeechBubble.mymaterial" );
    m_pBubble->SetMaterial( pMaterial );

    pMaterial = g_pMaterialManager->FindMaterialByFilename( "Data/Materials/SpeechBubblePoint.mymaterial" );
    m_pPoint->SetMaterial( pMaterial );
}

SpeechBubble::~SpeechBubble()
{
    SAFE_RELEASE( m_pTextMesh );

    SAFE_DELETE( m_pBubble );
    SAFE_RELEASE( m_pPoint );
}

void SpeechBubble::Tick(double timepassed)
{
    float amounttochange = 0.1f;

    if( m_DestPos != m_CurrentPos ) m_CurrentPos += (m_DestPos - m_CurrentPos) * amounttochange;

    if( m_DestWidth != m_CurrentWidth ) m_CurrentWidth += (m_DestWidth - m_CurrentWidth) * amounttochange;
    if( m_DestHeight != m_CurrentHeight ) m_CurrentHeight += (m_DestHeight - m_CurrentHeight) * amounttochange;

    if( m_DestColor.a != m_CurrentColor.a )
    {
        float change = ((float)(m_DestColor.a - m_CurrentColor.a)) * 0.2f;
        if( change > -1 && change < 0  ) change = -1;
        if( change > 0 && change < 1 ) change = 1;
        //char changechar = (char)change;
        float alpha = m_CurrentColor.a + change;
        MyClamp( alpha, 0.0f, 255.0f );
        //m_CurrentColor.a += changechar;
        m_CurrentColor.a = (unsigned char)alpha;
    }

    m_pBubble->Create( 0, 10, 10.0f+m_CurrentWidth, 10.0f+m_CurrentWidth+10,
                       0, 10, 10.0f+m_CurrentHeight, 10.0f+m_CurrentHeight+10,
                       0, 0.15625f, 1-0.15625f, 1,
                       0, 0.15625f, 1-0.15625f, 1 );
    m_pPoint->Create( "SpeechBubble", 64, 64, 0, 1, 0, 1, Justify_Top );

    //m_pBubble->SetTint( m_CurrentColor );
    //m_pPoint->SetTint( m_CurrentColor );
}

void SpeechBubble::SetSize(float width, float height)
{
    //if( width == m_CurrentWidth && height == m_CurrentHeight )
    //    return;

    m_DestWidth = width;
    m_DestHeight = height;

    if( m_CurrentColor.a == 0 )
    {
        m_CurrentWidth = width;
        m_CurrentHeight = height;
    }

    //m_pBubble->SetShaderAndTexture( g_pGame->m_pShader_Font, g_pGame->m_pResources->m_pTextures[TL_SpeechBubble]->m_TextureID );
    //m_pPoint->SetShaderAndTexture( g_pGame->m_pShader_Font, g_pGame->m_pResources->m_pTextures[TL_SpeechBubblePoint]->m_TextureID );
}

void SpeechBubble::SetPosition(Vector3 pos)
{
    m_DestPos.Set( pos.x, pos.y );
    m_CurrentPos = m_DestPos;
}

void SpeechBubble::SetString(const char* string)
{
    if( m_String != string )
        m_LastString = m_String;

    m_String = string;

    if( m_String == 0 )
    {
        m_DestColor.a = 0;
        return;
    }

    Vector2 size = g_pGame->m_pSystemFont->m_pBMFont->GetSize( m_String, 20 );

    SetSize( size.x + 4, size.y + 20 - 14 );

    m_DestColor.a = 255;
}

void SpeechBubble::Draw(MyMatrix* matviewproj)
{
    const char* stringtoshow = m_String;

    if( stringtoshow == 0 )
        stringtoshow = m_LastString;

    if( stringtoshow && m_CurrentColor.a != 0 )
    {
        m_pBubble->SetPosition( m_CurrentPos.x, m_CurrentPos.y, 0 );
        m_pBubble->SetTint( m_CurrentColor );
        m_pBubble->Draw( matviewproj );

        m_pPoint->SetPosition( m_CurrentPos.x + m_CurrentWidth/2, m_CurrentPos.y + 10-2, 0 );
        m_pPoint->SetTint( m_CurrentColor );
        m_pPoint->Draw( matviewproj );

        Vector2 size = g_pGame->m_pSystemFont->m_pBMFont->GetSize( stringtoshow, 20 );

        m_pTextMesh->CreateStringColorAndShadow( false, 20, m_CurrentPos.x + 12, m_CurrentPos.y + 10 + size.y + 5, Justify_Left|Justify_Top,
            ColorByte(255,255,255,m_CurrentColor.a), ColorByte(0,0,0,m_CurrentColor.a), 2, -2, size, stringtoshow );
        m_pTextMesh->Draw( &g_pGame->m_OrthoMatrixGameSize, 0, 0, 0, 0, 0, 0, 0 );
    }
}
