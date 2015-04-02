//
// Copyright (c) 2012-2014 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "GameCommonHeader.h"
#include "MenuSprite.h"

MenuSprite::MenuSprite()
: m_BGColor(50,50,50,255)
{
    m_MenuItemType = MIT_Sprite;

    m_Justification = Justify_CenterX|Justify_CenterY;
    m_BGWidth = 50;
    m_BGHeight = 50;
    m_DropShadowOffsetBG_X = 0;
    m_DropShadowOffsetBG_Y = 0;

    m_pBGSprite = 0;
    m_BGSpriteUVs = Vector4( 0, 1, 0, 1 );

    m_HasShadow = true;
    m_DropShadowOffsetBG_X = 0;
    m_DropShadowOffsetBG_Y = 0;
    m_pShadowSprite = 0;
    m_ShadowSpriteUVs = Vector4( 0, 1, 0, 1 );
}

MenuSprite::~MenuSprite()
{
    SAFE_RELEASE( m_pBGSprite );
    SAFE_RELEASE( m_pShadowSprite );
}

void MenuSprite::Draw()
{
    if( m_Visible == false )
        return;

    //float scrw = g_pGame->m_GameWidth;
    //float scrh = g_pGame->m_GameHeight;

    float bgshadowoffx = m_DropShadowOffsetBG_X;
    float bgshadowoffy = m_DropShadowOffsetBG_Y;

    //float posx = (m_PositionOffset.x + m_Transform.m41) / scrw;
    //float posy = (m_PositionOffset.y + m_Transform.m42) / scrh;
    //float bgwidth = m_BGWidth / scrw * m_Scale.x;
    //float bgheight = m_BGHeight / scrh * m_Scale.y;

    MySprite* pSprite = m_pBGSprite;
    //Vector4 uvs = m_BGSpriteUVs;
    ColorByte bgcolor = m_BGColor;

    //if( pSprite )
    //    pSprite->SetTint( ColorByte(255,255,255,255) );

    //if( pSprite == 0 )
    //{
    //    pSprite = g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable];
    //    //pSprite->SetTint( bgcolor );
    //}

    MySprite* pShadowSprite = m_pShadowSprite;
    //Vector4 shuvs = m_ShadowSpriteUVs;
    //if( pShadowSprite == 0 )
    //{
    //    pShadowSprite = g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable];
    //    shuvs = Vector4( 0, 1, 0, 1 );
    //}

    if( pSprite )
    {
        if( pShadowSprite && m_HasShadow )
        {
            //pShadowSprite->Create( bgwidth, bgheight, shuvs.x, shuvs.y, shuvs.z, shuvs.w, m_Justification );

            ColorByte shadowcolor = ColorByte(0,0,0,bgcolor.a/4);
            pShadowSprite->SetTint( shadowcolor );
            //pShadowSprite->SetPosition( posx+bgshadowoffx, posy-bgshadowoffy, 0.1f );
            pShadowSprite->SetTransform( m_Transform );
            pShadowSprite->m_Position.m41 += bgshadowoffx;
            pShadowSprite->m_Position.m42 += bgshadowoffy;

            pShadowSprite->Draw( &g_pGame->m_OrthoMatrixGameSize );
        }

        //pSprite->Create( bgwidth, bgheight, uvs.x, uvs.y, uvs.z, uvs.w, m_Justification );

        pSprite->SetTint( bgcolor );
        //pSprite->SetPosition( posx, posy, 0 );
        pSprite->SetTransform( m_Transform );

        pSprite->Draw( &g_pGame->m_OrthoMatrixGameSize );
    }

}

MyRect MenuSprite::GetBoundingRect()
{
    MyRect rect;

    rect.x = (int)(m_Transform.m41 + m_PositionOffset.x);
    rect.w = (int)m_BGWidth;
    rect.y = (int)(m_Transform.m42 + m_PositionOffset.y);
    rect.h = (int)m_BGHeight;

    if( m_Justification & Justify_CenterX )
        rect.x -= (int)(m_BGWidth/2);
    if( m_Justification & Justify_Right )
        rect.x -= (int)m_BGWidth;

    if( m_Justification & Justify_CenterY )
        rect.y -= (int)(m_BGHeight/2);
    if( m_Justification & Justify_Top )
        rect.y -= (int)m_BGHeight;

    return rect;
}

void MenuSprite::SetPositionAndSize(float x, float y, float w, float h, float inputw, float inputh)
{
    m_Transform.SetIdentity();
    m_Transform.Scale( w, h, 1 );
    m_Transform.SetTranslation( x, y, 0 );

    m_BGWidth = w;
    m_BGHeight = h;
}

void MenuSprite::SetSprites(MySprite* bgsprite, MySprite* shadowsprite)
{
    assert( m_pBGSprite == 0 );
    assert( m_pShadowSprite == 0 );

    if( bgsprite )
    {
        m_pBGSprite = bgsprite;
        m_pBGSprite->AddRef();
    }

    if( shadowsprite )
    {
        m_pShadowSprite = shadowsprite;
        m_pShadowSprite->AddRef();
    }
}

void MenuSprite::SetSpritesCopy(MySprite* bgsprite, MySprite* shadowsprite)
{
    assert( m_pBGSprite == 0 );
    assert( m_pShadowSprite == 0 );

    if( bgsprite )
    {
        m_pBGSprite = MyNew MySprite( bgsprite, "SetSpritesCopy" );
    }

    if( shadowsprite )
    {
        m_pShadowSprite = MyNew MySprite( shadowsprite, "SetSpritesCopy" );
    }
}

#if MYFW_USING_WX
void MenuSprite::FillPropertiesWindow()
{
    MenuItem::FillPropertiesWindow();
    g_pPanelWatch->AddBool( "HasShadow", &m_HasShadow, 0, 1 );
    g_pPanelWatch->AddFloat( "ShadowOffsetX", &m_DropShadowOffsetBG_X, -5, 5 );
    g_pPanelWatch->AddFloat( "ShadowOffsetY", &m_DropShadowOffsetBG_Y, -5, 5 );
    g_pPanelWatch->AddFloat( "BG Shadow Offset X", &m_DropShadowOffsetBG_X, -10, 10 );
    g_pPanelWatch->AddFloat( "BG Shadow Offset Y", &m_DropShadowOffsetBG_Y, -10, 10 );
}
#endif //MYFW_USING_WX
