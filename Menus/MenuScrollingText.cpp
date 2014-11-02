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
#include "../Screens/Screen_Base.h"
#include "MenuItem.h"
#include "MenuScrollingText.h"

MenuScrollingText::MenuScrollingText()
{
    m_MenuItemType = MIT_ScrollingText;

    m_peTextMesh = 0;

    m_StringToShow = 0;
    m_TopFade0 = 0;
    m_TopFade1 = 0;
    m_BottomFade1 = 0;
    m_BottomFade0 = 0;
}

MenuScrollingText::~MenuScrollingText()
{
    // SAFE_DELETE( m_peTextMesh ); // allocated externally
}

void MenuScrollingText::SetTextMesh(MyMeshText* pMesh)
{
    m_peTextMesh = pMesh;
}

void MenuScrollingText::Draw()
{
    assert( m_peTextMesh );

    MenuScrollBox::Draw();

    float x = m_Transform.m41 + m_PositionOffset.x + m_ScrollAmount.x;
    float y = m_Transform.m42 + m_PositionOffset.y + m_ScrollAmount.y;

    m_peTextMesh->ClearText();

    if( m_StringToShow == 0 )
        return;

    const char* m_LangString = m_StringToShow;
    if( m_StringToShow[0] == '.' )
        m_LangString = g_pLanguageTable->LookUp( m_StringToShow );

    char line[100];
    line[0] = 0;
    int len = (int)strlen( m_LangString );
    int cchar = 0;
    int linecount = 0;

    for( int i=0; i<=len; i++ ) // don't use i if (i == len)
    {
        if( m_LangString[i] == '\n' || i == len )
        {
            //ColorByte focuscolor = ColorByte(255,255,255,255);
            //ColorByte offfocuscolor = ColorByte(255,255,255,0);

            ColorByte color = m_TextColor; //focuscolor;
            ColorByte shadowcolor = m_TextShadowColor;

            float ny = y - linecount*m_FontHeight;
            float perc = 1;

            if( ny > m_TopFade1 )
                perc = 1 - (ny - m_TopFade1) / (m_TopFade0 - m_TopFade1); // 250-200 / 250-200;
            else if( ny < m_BottomFade1 )
                perc = 1 - (m_BottomFade1 - ny) / (m_BottomFade1 - m_BottomFade0); // 200-150 / 200-150;

            MyClamp( perc, 0.0f, 1.0f );

            //color = focuscolor * perc + offfocuscolor * (1-perc);
            //shadowcolor = focuscolor * perc + offfocuscolor * (1-perc);
            color.a = (unsigned char)(color.a * perc);
            shadowcolor.a = (unsigned char)(shadowcolor.a * perc);

            if( color.a > 0 && strlen( line ) > 0 )
            {
                assert( m_peTextMesh->m_pFont == m_pFont );
                m_peTextMesh->m_pFont = m_pFont; // ugh...

                if( m_peTextMesh )
                {
                    m_peTextMesh->CreateString( true, m_FontHeight, x+m_DropShadowOffsetText_X, ny+m_DropShadowOffsetText_Y, 0, 0, Justify_Left|Justify_CenterY, shadowcolor, m_Size, line );
                    m_peTextMesh->CreateString( true, m_FontHeight, x, ny, 0, 0, Justify_Left|Justify_CenterY, color, m_Size, line );
                }
                else
                {
                    RenderTextQuickWithColorAndShadow( m_pFont, m_FontHeight, x, ny, Justify_Left|Justify_CenterY, color, shadowcolor, m_DropShadowOffsetText_X, m_DropShadowOffsetText_Y, line );
                }
            }
            line[0] = 0;
            linecount++;
            cchar = 0;
        }
        else if( cchar < 100-1 )
        {
            line[cchar] = m_LangString[i];
            line[cchar+1] = 0;
            cchar++;
        }
    }

    m_peTextMesh->SetShaderAndTexture( g_pGame->m_pShader_TextureVertexColor, m_pFont->m_pTextureDef );
    m_peTextMesh->Draw( &g_pGame->m_OrthoMatrixGameSize, 0, 0, 0, 0, 0, 0 );
}
