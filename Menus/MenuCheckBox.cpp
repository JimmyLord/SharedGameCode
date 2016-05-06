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
//#include "../GameSliders.h"
//#include "../Core/ResourceManager.h"
//#include "../GravityBalls/GravityBalls_Resources.h"
#include "MenuCheckBox.h"

ColorByte DefaultMenuCheckBoxColors[MCBCT_NumColors] = 
{
    ColorByte(255,255,255,255), // MCBCT_CheckedBG
    ColorByte(128,128,128,128), // MCBCT_UncheckedBG

    ColorByte(255,255,255,255), // MCBCT_Text
};

MenuCheckBox::MenuCheckBox()
{
    m_MenuItemType = MIT_CheckBox;

    m_State = MCBS_Idle;
    m_FingerHolding = -1;

    m_Checked = false;
    m_Action[0] = 0;

    m_PosX = 0;
    m_PosY = 0;
    m_Justification = 0;
    m_InputWidth = 0;
    m_InputHeight = 0;

    m_pFont = 0;
    m_FontHeight = 30;
    m_String[0] = 0;
    m_TextOffsetX = 0;
    m_TextOffsetY = 0;
    m_TextJustification = 0;
    m_TextDropShadowOffsetX = 0;
    m_TextDropShadowOffsetY = 0;
    m_TextColor = ColorByte(255,255,255,255);

    m_BGWidth = 0;
    m_BGHeight = 0;
    m_BGDropShadowOffsetX = 0;
    m_BGDropShadowOffsetY = 0;

    m_pCheckedBGSprite = 0;
    m_CheckedBGSpriteUVs = Vector4( 0, 1, 0, 1 );
    m_CheckedBGColor = ColorByte(255,255,255,255);

    m_pUncheckedBGSprite = 0;
    m_UncheckedBGSpriteUVs = Vector4( 0, 1, 0, 1 );
    m_UncheckedBGColor = ColorByte(255,255,255,255);

    m_SoundPressed = 0;//GameAudioCue_None;
}

MenuCheckBox::~MenuCheckBox()
{
}

void MenuCheckBox::Draw(MyMatrix* matviewproj)
{
    if( m_Visible == false )
        return;
    
    if( m_State == MCBS_Disabled )
        return;

    //float scrw = g_pGame->m_GameWidth;
    //float scrh = g_pGame->m_GameHeight;

    float shadowoffx = m_TextDropShadowOffsetX;
    float shadowoffy = m_TextDropShadowOffsetY;
    float CheckBoxshadowoffx = m_BGDropShadowOffsetX;
    float CheckBoxshadowoffy = m_BGDropShadowOffsetY;

    float bgwidth = m_BGWidth;
    float bgheight = m_BGHeight;

    MySprite* pSprite = m_pCheckedBGSprite;
    Vector4 uvs = m_CheckedBGSpriteUVs;
    ColorByte bgcolor = m_CheckedBGColor;
    if( m_Checked == false )
    {
        pSprite = m_pUncheckedBGSprite;
        uvs = m_UncheckedBGSpriteUVs;
        bgcolor = m_UncheckedBGColor;
    }

    if( pSprite == 0 )
    {
        return;
        //pSprite = g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable];
    }

    pSprite->SetTint( bgcolor );

    MySprite* pShadowSprite = pSprite; //m_pShadowSprite;
    Vector4 shuvs = uvs;
    if( pShadowSprite == 0 )
    {
        //pShadowSprite = g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable];
        shuvs = Vector4( 0, 1, 0, 1 );
    }

    // draw the checkbox with drop shadow.
    if( pSprite )
    {
        if( pShadowSprite )
        {
            pShadowSprite->Create( "MenuCheckBox", bgwidth, bgheight, shuvs.x, shuvs.y, shuvs.z, shuvs.w, Justify_CenterX|Justify_CenterY );

            pShadowSprite->SetTint( ColorByte(0,0,0,64) );
            MyMatrix world;
            world.SetIdentity();
            world.SetTranslation( m_PosX+CheckBoxshadowoffx, m_PosY+CheckBoxshadowoffy, 0.1f );
            pShadowSprite->Draw( &world, matviewproj ); //&g_pGame->m_OrthoMatrixGameSize );
        }

        pSprite->Create( "MenuCheckBox", bgwidth, bgheight, uvs.x, uvs.y, uvs.z, uvs.w, Justify_CenterX|Justify_CenterY );

        pSprite->SetTint( bgcolor );
        if( m_State == MCBS_HeldDown )
        {
            pSprite->SetTint( ColorByte(200,200,200,255) );
        }
        MyMatrix world;
        world.SetIdentity();
        world.SetTranslation( m_PosX, m_PosY, 0 );
        pSprite->Draw( &world, matviewproj ); //&g_pGame->m_OrthoMatrixGameSize );
    }

    float y;

    // draw text
    if( m_pFont != 0 && m_String[0] != 0 )
    {
        ColorByte textcolor = m_TextColor;

        ColorByte textshadowcolor(0,0,0,200);

        y = m_PosY;
        y += m_TextOffsetY;
        RenderTextQuickWithColor( m_pFont, m_FontHeight, m_PosX+shadowoffx, y-shadowoffy, m_Justification, textshadowcolor, m_String );
        RenderTextQuickWithColor( m_pFont, m_FontHeight, m_PosX, y, m_Justification, textcolor, m_String );
    }
}

bool MenuCheckBox::CheckForCollision(float x, float y)
{
    if( m_State == MCBS_Disabled )
        return false;

    if( m_Enabled == false )
        return false;

    float transx = x;
    float transy = y;

    if( fabs( m_PosX - transx ) < m_InputWidth &&
        fabs( m_PosY - transy ) < m_InputHeight )
    {
        return true;
    }

    return false;
}

float MenuCheckBox::TestCollision(int fingerid, float x, float y, bool fingerwentdown)
{
    //if( m_FingerHolding != -1 )
    //    return true;

    if( CheckForCollision( x, y ) )
    {
        float posx = m_Position.x;
        float posy = m_Position.y;

        float dist = (posx - x)*(posx - x) + (posy - y)*(posy - y);
        return dist;
    }

    return -1;
}

bool MenuCheckBox::HoldOnCollision(int fingerid, float x, float y, bool releaseifnottouching, bool fingerwentdown)
{
    if( m_FingerHolding != -1 )
        return true;

    if( CheckForCollision( x, y ) )
    {
        PlaySound();
        //LOGInfo( LOGTag, "CheckBox Held\n" );

        m_FingerHolding = fingerid;
        m_State = MCBS_HeldDown;
        return true;
    }
    else if( releaseifnottouching )
    {
        //if( m_FingerHolding != -1 )
        //    LOGInfo( LOGTag, "CheckBox releaseifnottouching\n" );

        m_FingerHolding = -1;
        m_State = MCBS_Idle;
    }

    return false;
}

bool MenuCheckBox::ReleaseOnNoCollision(int fingerid, float x, float y)
{
    if( m_FingerHolding != fingerid )
        return false;

    if( CheckForCollision( x, y ) )
    {
        return false;
    }
    else
    {
        if( m_State == MCBS_HeldDown )
        {
            PlaySound();
            //LOGInfo( LOGTag, "CheckBox ReleaseOnNoCollision\n" );

            m_FingerHolding = -1;
            m_State = MCBS_Idle;
            return true;
        }
    }

    return false;
}

const char* MenuCheckBox::TriggerOnCollision(int fingerid, float x, float y, bool careifheld, bool releaseifnocollision)
{
    if( careifheld && m_FingerHolding != fingerid )
        return 0;

    if( CheckForCollision( x, y ) )
    {
        PlaySound();
        //LOGInfo( LOGTag, "CheckBox TriggerOnCollision\n" );
        m_Checked = !m_Checked;

        return m_Action;
    }

    return 0;
}

//void MenuCheckBox::SetDisabled(bool settodisabled)
//{
//    if( settodisabled == true )
//    {
//        m_State = MCBS_Disabled;
//    }
//
//    // only set CheckBox back to idle if we actually are disabled.
//    if( settodisabled == false && m_State == MCBS_Disabled )
//    {
//        m_State = MCBS_Idle;
//    }
//}
//
//bool MenuCheckBox::IsDisabled()
//{
//    if( m_State == MCBS_Disabled )
//        return true;
//
//    return false;
//}

void MenuCheckBox::PlaySound()
{
// TODO: MYENGINE
//    if( m_SoundPressed != GameAudioCue_None )
//    {
//        g_pGame->m_pGameAudio->Play( m_SoundPressed );
//    }
//    else
//    {
//#if ORTHOHACK_ZERO_TO_ONE
//        g_pGame->m_pGameAudio->Play( GameAudioCue_Click );
//#endif
//    }
}
