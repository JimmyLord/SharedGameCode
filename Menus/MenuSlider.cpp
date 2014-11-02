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
#include "Core/ResourceManager.h"
#include "MenuSlider.h"

ColorByte MenuSliderColors[MSCT_NumColors] = 
{
    ColorByte(255,255,255,255), // MSCT_BarColor
    ColorByte(200,200,200,255), // MSCT_HandleColor

    ColorByte(0,0,0,0),         // MSCT_BGColor
};

MenuSlider::MenuSlider()
{
    m_Disabled = false;

    m_ValuePerc = 0;

    for( int i=0; i<MSCT_NumColors; i++ )
        m_Colors[i] = MenuSliderColors[i];

    m_PosX = 0;
    m_PosY = 0;
    m_VisualRange = 0;
    m_Justification = 0;
    m_InputWidth = 0;
    m_InputHeight = 0;

    m_BarThickness = 0;
    m_HandleWidth = 0;
    m_HandleHeight = 0;

    m_DropShadowOffset = 0;

    m_BGWidth = 0;
    m_BGHeight = 0;
    m_DropShadowOffsetBG_X = 0;
    m_DropShadowOffsetBG_Y = 0;
}

MenuSlider::~MenuSlider()
{
}

void MenuSlider::Draw()
{
    float centerx = m_PosX;
    float top = m_PosY;
    // TODO: take more justifications and vertical/horizontal into account for top and left.
    //       should ideally be done once instead of every frame.
    //if( m_Justification & Justify_Left )
    //{
    //    centerx += m_BarThickness/2;
    //}

    float emptypos = (m_PosY - m_VisualRange);

    g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->Create( "MenuSlider", m_BarThickness, m_VisualRange, 0, 1, 0, 1, Justify_CenterX|Justify_Top );
    g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->SetPosition( centerx, top, 0 );
    g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->SetTint( m_Colors[MSCT_BarColor] );
    g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->Draw( &g_pGame->m_OrthoMatrix );

    g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->Create( "MenuSlider", m_HandleWidth, m_HandleHeight, 0, 1, 0, 1, Justify_CenterX|Justify_Top );
    g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->SetPosition( centerx, emptypos + m_ValuePerc*m_VisualRange, 0 );
    g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->SetTint( m_Colors[MSCT_HandleColor] );
    g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->Draw( &g_pGame->m_OrthoMatrix );
}

int MenuSlider::CheckForCollisionPosition(float x, float y, bool held)
{
    if( m_Disabled )
        return 0;

    float transx = x / g_pGame->m_GameWidth;
    float transy = y / g_pGame->m_GameHeight;

    float centerx = m_PosX;
    float centery = m_PosY;
    // TODO: take more justifications and vertical/horizontal into account for top and left.
    //       should ideally be done once instead of every frame.
    //if( m_Justification & Justify_CenterX )
    //{
    //    left -= m_BarThickness/2;
    //}
    if( m_Justification & Justify_Top )
    {
        centery -= m_VisualRange/2;
    }

    //float emptypos = (m_PosY - m_VisualRange);

    // if touch is in box, then pop slider to that location.
    if( held ||
        ( fabs( centerx - transx ) < (m_InputWidth/2) &&
        fabs( centery - transy ) < (m_InputHeight/2) )
      )
    {
        //float top = m_PosY;

        m_ValuePerc = 1 - (m_PosY - transy) / m_VisualRange;
        MyClamp( m_ValuePerc, 0.0f, 1.0f );

        return 1;
    }

    return 0;
}
