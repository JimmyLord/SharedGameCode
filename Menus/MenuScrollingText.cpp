//
// Copyright (c) 2012-2016 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

//#ifndef PCHFILE
//#define PCHFILE "EngineCommonHeader.h"
//#endif

#include PCHFILE
//#include "Core/ResourceManager.h"
//#include "../Screens/Screen_Base.h"
#include "MenuItem.h"
#include "MenuScrollingText.h"

MenuScrollingText::MenuScrollingText()
{
    m_MenuItemType = MIT_ScrollingText;

    m_pMaterial = g_pMaterialManager->CreateMaterial();
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

    SAFE_RELEASE( m_pMaterial );
}

void MenuScrollingText::SetTextMesh(MyMeshText* pMesh)
{
    m_peTextMesh = pMesh;
}

void MenuScrollingText::Draw(MyMatrix* matviewproj)
{
    if( m_peTextMesh == 0 )
        return;
    MyAssert( m_peTextMesh );

    MenuScrollBox::Draw( matviewproj );

    float x = m_Position.x + m_ScrollAmount.x;
    float y = m_Position.y + m_ScrollAmount.y;

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
                MyAssert( m_peTextMesh->m_pFont == m_pFont );
                m_peTextMesh->m_pFont = m_pFont; // ugh...

                if( m_peTextMesh )
                {
                    if( m_DropShadowOffsetText.x != 0 || m_DropShadowOffsetText.y != 0 )
                        m_peTextMesh->CreateString( true, m_FontHeight, x+m_DropShadowOffsetText.x, ny+m_DropShadowOffsetText.y, 0, 0, Justify_Left|Justify_CenterY, shadowcolor, m_TextSize, line );
                    m_peTextMesh->CreateString( true, m_FontHeight, x, ny, 0, 0, Justify_Left|Justify_CenterY, color, m_TextSize, line );
                }
                else
                {
                    RenderTextQuickWithColorAndShadow( m_pFont, m_FontHeight, x, ny, Justify_Left|Justify_CenterY, color, shadowcolor, m_DropShadowOffsetText.x, m_DropShadowOffsetText.y, line );
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

    m_peTextMesh->m_MeshReady = true;
    // TODO: MYENGINE
    //m_pMaterial->SetShader( g_pGame->m_pShader_TextureVertexColor );
    m_pMaterial->SetShader( g_pShaderGroupManager->FindShaderGroupByName( "Shader_TextureVertexColor" ) );
    m_pMaterial->SetTextureColor( m_pFont->m_pTextureDef );
    m_peTextMesh->SetMaterial( m_pMaterial, 0 );
    //m_peTextMesh->Draw( &g_pGame->m_OrthoMatrixGameSize, 0, 0, 0, 0, 0, 0, 0 );
    m_peTextMesh->Draw( 0, matviewproj, 0, 0, 0, 0, 0, 0, 0, 0 );
}

#if MYFW_USING_WX
void MenuScrollingText::FillPropertiesWindow()
{
    MenuButton::FillPropertiesWindow();

    g_pPanelWatch->AddFloat( "Fade-Top End", &m_TopFade0, 0, 2000 );
    g_pPanelWatch->AddFloat( "Fade-Top Start", &m_TopFade1, 0, 2000 );
    g_pPanelWatch->AddFloat( "Fade-Bottom Start", &m_BottomFade1, 0, 2000 );
    g_pPanelWatch->AddFloat( "Fade-Bottom End", &m_BottomFade0, 0, 2000 );
}
#endif //MYFW_USING_WX
