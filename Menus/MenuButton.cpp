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
#include "MenuButton.h"

ColorByte MenuButtonColors[MBCT_NumColors] = 
{
    ColorByte(255,255,255,255), //100,100,150,255), //255,81,205,255),  // MBCT_SelectableBG
    ColorByte(255,255,255,255),    // MBCT_SelectableText

    ColorByte(255,255,255,255), //150,100,100,255), //0,150,150,255),   // MBCT_DisabledBG
    ColorByte(255,255,255,200), // MBCT_DisabledText
};

ColorByte MenuButtonColorsPlain[MBCT_NumColors] = 
{
    ColorByte(100,100,100,255), //100,100,150,255), //255,81,205,255),  // MBCT_SelectableBG
    ColorByte(255,255,255,255),    // MBCT_SelectableText

    ColorByte(50,50,50,255), //150,100,100,255), //0,150,150,255),   // MBCT_DisabledBG
    ColorByte(100,100,100,200), // MBCT_DisabledText
};

MySprite* g_DefaultEnabledBGSprite = 0;
Vector4 g_DefaultEnabledBGSpriteUVs;
MySprite* g_DefaultDisabledBGSprite = 0;
Vector4 g_DefaultDisabledBGSpriteUVs;

MenuButton::MenuButton(int maxletters)
: m_TextShadowStyle(TextShadowStyle_Single)
, m_TextOffset(0,0)
, m_TextColor(255,255,255,255)
, m_TextShadowColor(0,0,0,200)
, m_BGColor(50,50,50,255)
, m_BGShadowColor(0,0,0,64)
, m_DisabledTextColor(255,255,255,128)
, m_DisabledBGColor(50,50,50,128)
, m_UsePressedState(true)
, m_PressedTextColor(255,255,255,255)
, m_PressedBGColor(200,200,200,255)
, m_PressedBGSpriteUVs(0,1,0,1)
, m_HasOverlay(false)
, m_OverlaySize(0,0)
, m_OverlayOffset(0,0)
, m_OverlayBGColor(255,255,255,255)
, m_OverlayBGSpriteUVs(0,1,0,1)
{
    m_MenuItemType = MIT_Button;

    m_State = MBS_Idle;

    m_TimeHeld = 0;
    m_FingerHolding = -1;

    for( int i=0; i<3; i++ )
        m_Strings[i][0] = 0;
    m_ToolTipString[0] = 0;

    if( maxletters == -1 )
        m_pMeshText = MyNew MyMeshText( 3*MAX_MENUBUTTON_STRING, 0 );
    else if( maxletters > 0 )
        m_pMeshText = MyNew MyMeshText( maxletters, 0 );
    else
        m_pMeshText = 0;

    m_ButtonAction = 0;
    m_AllowPressWhenDraggedOver = false;

    m_pFont = 0;
    m_FontHeight = 30;
    m_LineHeight = 30*0.8f;

    m_Justification = Justify_CenterX|Justify_CenterY;
    m_InputWidth = 50;
    m_InputHeight = 50;

    m_DropShadowOffsetText_X = 0;
    m_DropShadowOffsetText_Y = 0;
    m_DropShadowOffsetBG_X = 0;
    m_DropShadowOffsetBG_Y = 0;

    m_pBGMesh = 0;
    m_pBGMeshCamera = 0;
    m_pBGMeshLight = 0;

    m_BGSpriteOn = true;
    m_BGSpriteUVs = Vector4( 0, 1, 0, 1 );
    m_DisabledBGSpriteUVs = Vector4( 0, 1, 0, 1 );

    m_ShadowSpriteUVs = Vector4( 0, 1, 0, 1 );

    m_Scale.Set( 1, 1, 1 );

    m_hack_PasswordHideString2 = false;
    
    m_SoundPressed = GameAudioCue_None;
#if SOUNDHACK_USE_CLICK_AS_DEFAULT
    m_SoundPressed = GameAudioCue_Click;
#endif

    m_Style = MBTS_SingleLine;

    m_pBGSprite = 0;
    m_pDisabledBGSprite = 0;
    m_pPressedBGSprite = 0;
    m_pOverlayBGSprite = 0;
    m_pShadowSprite = 0;
}

MenuButton::~MenuButton()
{
    SAFE_DELETE( m_pMeshText );

    SAFE_RELEASE( m_pBGSprite );
    SAFE_RELEASE( m_pDisabledBGSprite );
    SAFE_RELEASE( m_pPressedBGSprite );
    SAFE_RELEASE( m_pOverlayBGSprite );
    SAFE_RELEASE( m_pShadowSprite );
}

void MenuButton::StartClosing()
{
    MenuItem::StartClosing();
}

void MenuButton::Tick(double timepassed)
{
    m_TimeHeld += timepassed;

    MenuItem::Tick( timepassed );
}

void MenuButton::Draw()
{
    if( m_Visible == false )
        return;

    if( m_pMeshText )
    {
        m_pMeshText->m_NumVertsToDraw = 0;
        m_pMeshText->m_NumIndicesToDraw = 0;
    }

    //float scrw = g_pGame->m_GameWidth;
    //float scrh = g_pGame->m_GameHeight;

    TextShadowStyles textshadowstyle = m_TextShadowStyle;
    if( m_DropShadowOffsetText_X == 0 && m_DropShadowOffsetText_Y == 0 )
        textshadowstyle = TextShadowStyle_None;

    float lineheight = m_LineHeight;
    float shadowoffx = m_DropShadowOffsetText_X;
    float shadowoffy = m_DropShadowOffsetText_Y;
    float buttonshadowoffx = m_DropShadowOffsetBG_X;
    float buttonshadowoffy = m_DropShadowOffsetBG_Y;

    //float posx = m_Transform.m41;
    //float posy = m_Transform.m42;

    ColorByte textcolor = m_TextColor;
    ColorByte textshadowcolor = m_TextShadowColor;
    MySprite* pSprite = m_pBGSprite;
    MyMesh* pMesh = m_pBGMesh;
    Vector4 uvs = m_BGSpriteUVs;
    ColorByte bgcolor = m_BGColor;
    if( m_Enabled == false )//m_State == MBS_Disabled )
    {
        textcolor = m_DisabledTextColor;
        bgcolor = m_DisabledBGColor;
        pSprite = m_pDisabledBGSprite;
        uvs = m_DisabledBGSpriteUVs;
    }
    if( m_UsePressedState && m_State == MBS_HeldDown )
    {
        textcolor = m_PressedTextColor;
        bgcolor = m_PressedBGColor;
        if( m_pPressedBGSprite )
            pSprite = m_pPressedBGSprite;
        uvs = m_PressedBGSpriteUVs;
    }

    if( m_BGSpriteOn )
    {
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
            MyMatrix transform = m_Transform;

            transform.m41 += m_PositionOffset.x;
            transform.m42 += m_PositionOffset.y;

            if( pShadowSprite && (buttonshadowoffx != 0 || buttonshadowoffy != 0) )
            {
                pShadowSprite->SetTint( m_BGShadowColor );

                MyMatrix shadowmat = transform;
                shadowmat.Translate( buttonshadowoffx, buttonshadowoffy, 0 );

                pShadowSprite->SetTransform( shadowmat );
                pShadowSprite->Draw( &g_pGame->m_OrthoMatrixGameSize );
            }

            pSprite->SetTint( bgcolor );

            pSprite->SetTransform( transform );
            pSprite->Draw( &g_pGame->m_OrthoMatrixGameSize );
        }
    }

    if( m_HasOverlay )
    {
        bgcolor = m_OverlayBGColor;
        pSprite = m_pOverlayBGSprite;
        uvs = m_OverlayBGSpriteUVs;

        if( pSprite )
        {
            pSprite->SetTint( bgcolor );

            MyMatrix transform;
            transform.SetIdentity();
            transform.Scale( m_OverlaySize.x, m_OverlaySize.y, 1 );
            transform.m41 = m_Transform.m41 + m_PositionOffset.x + m_OverlayOffset.x;
            transform.m42 = m_Transform.m42 + m_PositionOffset.y + m_OverlayOffset.y;

            pSprite->SetTransform( transform );
            pSprite->Draw( &g_pGame->m_OrthoMatrixGameSize );
        }
    }

    if( pMesh && m_pBGMeshCamera )
    {
        float gamew = g_pGame->m_GameWidth;
        float gameh = g_pGame->m_GameHeight;

        float devw = g_pGame->m_GameFullWidth;
        float devh = g_pGame->m_GameFullHeight;

        MyMatrix matfinalmesh = m_MeshTransform;
        //matfinalmesh.Multiply( &m_Transform );
        matfinalmesh.SetTranslation( m_Transform.m41, m_Transform.m42, m_Transform.m43 );
        pMesh->m_Position = matfinalmesh;
        pMesh->m_Position.m41 = (((-g_pGame->m_OrthoLeft + matfinalmesh.m41) / devw) - 0.5f) * m_pBGMeshCamera->m_FrustumRightEdgeZ0*2;
        pMesh->m_Position.m42 = (((-g_pGame->m_OrthoBottom + matfinalmesh.m42) / devh) - 0.5f) * m_pBGMeshCamera->m_FrustumTopEdgeZ0*2;
        pMesh->m_Position.m43 = matfinalmesh.m43;

        int numlights = 0;
        if( m_pBGMeshLight )
        {
            Vector3 lightpos = m_pBGMeshLight->m_Position;
            lightpos.x = ((lightpos.x / gamew) - 0.5f) * m_pBGMeshCamera->m_FrustumRightEdgeZ0*2;
            lightpos.y = ((lightpos.y / gameh) - 0.5f) * m_pBGMeshCamera->m_FrustumTopEdgeZ0*2;
            //lightpos.z = lightpos.z;

            m_pBGMeshLight->m_Position = lightpos;

            numlights = 1;
        }

        pMesh->Draw( &m_pBGMeshCamera->m_matViewProj, &m_pBGMeshCamera->m_Eye, m_pBGMeshLight, numlights, 0, 0, 0, 0 );
    }

    float y;

    if( m_pFont )
    {
        if( m_pMeshText )
            m_pMeshText->m_pFont = m_pFont; // ugh...

        float posx = m_Transform.m41 + m_TextOffset.x + m_PositionOffset.x;
        float posy = m_Transform.m42 + m_TextOffset.y + m_PositionOffset.y;

        if( m_State == MBS_HeldDown && m_ToolTipString[0] != 0 )
        {
            assert( false );

            // ATM hardcoded for option overlay in A Game of Words.
            y = posy + 100;
            shadowoffx = -2;
            shadowoffy = 2;
            RenderTextQuickWithColor( m_pFont, m_FontHeight*m_Scale.y, posx+shadowoffx, y+shadowoffy, m_Justification, textshadowcolor, m_ToolTipString );
            shadowoffx = -2;
            shadowoffy = -2;
            RenderTextQuickWithColor( m_pFont, m_FontHeight*m_Scale.y, posx+shadowoffx, y+shadowoffy, m_Justification, textshadowcolor, m_ToolTipString );
            shadowoffx = 2;
            shadowoffy = 2;
            RenderTextQuickWithColor( m_pFont, m_FontHeight*m_Scale.y, posx+shadowoffx, y+shadowoffy, m_Justification, textshadowcolor, m_ToolTipString );
            shadowoffx = 2;
            shadowoffy = -2;
            RenderTextQuickWithColorAndShadow( m_pFont, m_FontHeight*m_Scale.y, posx, y, m_Justification, textcolor, textshadowcolor, shadowoffx, shadowoffy, m_ToolTipString );
        }

        //if( m_Strings[0][0] != 0 )
        if( m_pMeshText )
        {
            if( m_Style == MBTS_SingleLine )
            {
                y = posy;
                if( m_Strings[0][0] != 0 )
                {
                    m_pMeshText->CreateStringColorAndShadowStyle( true, m_FontHeight*m_Scale.y, posx, y, m_Justification, textcolor, textshadowstyle, textshadowcolor, shadowoffx, shadowoffy, m_Size, m_Strings[0] );
                }
            }
            else if( m_Style == MBTS_DoubleLine )
            {
                y = posy + lineheight/2;
                if( m_Strings[0][0] != 0 )
                {
                    m_pMeshText->CreateStringColorAndShadowStyle( true, m_FontHeight*m_Scale.y, posx, y, m_Justification, textcolor, textshadowstyle, textshadowcolor, shadowoffx, shadowoffy, m_Size, m_Strings[0] );
                }

                y = posy - lineheight/2;
                if( m_Strings[1][0] != 0 )
                {
                    if( m_hack_PasswordHideString2 )
                    {
                        int len = (int)strlen( m_Strings[1] );
                        char stars[32+1];
                        int i;
                        for( i=0; i<len; i++ )
                            stars[i] = '*';
                        stars[i] = 0;

                        m_pMeshText->CreateStringColorAndShadowStyle( true, m_FontHeight*m_Scale.y, posx, y, m_Justification, textcolor, textshadowstyle, textshadowcolor, shadowoffx, shadowoffy, m_Size, stars );
                    }
                    else
                    {
                        m_pMeshText->CreateStringColorAndShadowStyle( true, m_FontHeight*m_Scale.y, posx, y, m_Justification, textcolor, textshadowstyle, textshadowcolor, shadowoffx, shadowoffy, m_Size, m_Strings[1] );
                    }
                }
            }
            else if( m_Style == MBTS_TripleLine )
            {
                y = posy + lineheight*1;
                if( m_Strings[0][0] != 0 )
                {
                    m_pMeshText->CreateStringColorAndShadowStyle( true, m_FontHeight*m_Scale.y, posx, y, m_Justification, textcolor, textshadowstyle, textshadowcolor, shadowoffx, shadowoffy, m_Size, m_Strings[0] );
                }

                y = posy - lineheight*0;
                if( m_Strings[1][0] != 0 )
                {
                    m_pMeshText->CreateStringColorAndShadowStyle( true, m_FontHeight*m_Scale.y, posx, y, m_Justification, textcolor, textshadowstyle, textshadowcolor, shadowoffx, shadowoffy, m_Size, m_Strings[1] );
                }

                y = posy - lineheight*1;
                if( m_Strings[2][0] != 0 )
                {
                    m_pMeshText->CreateStringColorAndShadowStyle( true, m_FontHeight*m_Scale.y, posx, y, m_Justification, textcolor, textshadowstyle, textshadowcolor, shadowoffx, shadowoffy, m_Size, m_Strings[2] );
                }
            }
        }
    }

    if( m_pFont && m_pMeshText )
    {
        m_pMeshText->SetShaderAndTexture( g_pGame->m_pShader_TextureVertexColor, m_pFont->m_pTextureDef );
        m_pMeshText->Draw( &g_pGame->m_OrthoMatrixGameSize, 0, 0, 0, 0, 0, 0, 0 );
    }
}

bool MenuButton::CheckForCollision(float x, float y)
{
    if( m_State == MBS_Disabled )
        return false;

    if( m_Enabled == false )
        return false;

    //float scrw = g_pGame->m_GameWidth;
    //float scrh = g_pGame->m_GameHeight;

    float posx = m_Transform.m41 + m_PositionOffset.x;
    float posy = m_Transform.m42 + m_PositionOffset.y;

    // for the first 5th of a second, make the button huge, so quick taps will register even if users finger moves.
    float sizemultiplier = 1.0f;
    if( m_State == MBS_HeldDown && m_TimeHeld > 0 && m_TimeHeld < 0.2001f ) // FINGER_STABILITY_TIME
        sizemultiplier = 3.0f;

    if( fabs( posx - x ) < (m_InputWidth / 2.0f)*sizemultiplier &&
        fabs( posy - y ) < (m_InputHeight / 2.0f)*sizemultiplier )
    {
        return true;
    }

    return false;
}

float MenuButton::TestCollision(int fingerid, float x, float y, bool fingerwentdown)
{
    // if this is a check for a drag over and we don't allow it, ignore the message.
    if( fingerwentdown == false && m_AllowPressWhenDraggedOver == false )
        return -1;

    //if( m_FingerHolding != -1 )
    //    return true;

    if( CheckForCollision( x, y ) )
    {
        float posx = m_Transform.m41;
        float posy = m_Transform.m42;

        float dist = (posx - x)*(posx - x) + (posy - y)*(posy - y);
        return dist;
    }

    return -1;
}

bool MenuButton::HoldOnCollision(int fingerid, float x, float y, bool releaseifnottouching, bool fingerwentdown)
{
    // if this is a check for a drag over and we don't allow it, ignore the message.
    if( fingerwentdown == false && m_AllowPressWhenDraggedOver == false )
        return false;

    if( m_FingerHolding != -1 )
        return true;

    if( CheckForCollision( x, y ) )
    {
        //PlaySound();
        //LOGInfo( LOGTag, "Button Held\n" );

        m_TimeHeld = 0;
        m_FingerHolding = fingerid;
        m_State = MBS_HeldDown;
        return true;
    }
    else if( releaseifnottouching )
    {
        //if( m_FingerHolding != -1 )
        //    LOGInfo( LOGTag, "Button releaseifnottouching\n" );

        m_TimeHeld = 0;
        m_FingerHolding = -1;
        m_State = MBS_Idle;
    }

    return false;
}

bool MenuButton::ReleaseOnNoCollision(int fingerid, float x, float y)
{
    if( m_FingerHolding != fingerid )
        return false;

    if( CheckForCollision( x, y ) )
    {
        return false;
    }
    else
    {
        //PlaySound();
        //LOGInfo( LOGTag, "Button ReleaseOnNoCollision\n" );

        ClearHeldState();
        return true;
    }

    return false;
}

int MenuButton::TriggerOnCollision(int fingerid, float x, float y, bool careifheld, bool releaseifnocollision)
{
    if( careifheld && m_FingerHolding != fingerid )
        return -1;

    if( CheckForCollision( x, y ) )
    {
        PlaySound();
        ClearHeldState();
        //LOGInfo( LOGTag, "Button TriggerOnCollision\n" );

        return m_ButtonAction;
    }
    else if( releaseifnocollision )
    {
        ClearHeldState();
    }

    return -1;
}

bool MenuButton::ClearHeldState()
{
    m_TimeHeld = 0;
    m_FingerHolding = -1;
    m_State = MBS_Idle;

    return true;
}

MyRect MenuButton::GetBoundingRect()
{
    MyRect rect;

    rect.x = (int)(m_Transform.m41 + m_PositionOffset.x);
    rect.w = (int)m_InputWidth;
    rect.y = (int)(m_Transform.m42 + m_PositionOffset.y);
    rect.h = (int)m_InputHeight;

    if( m_Justification & Justify_CenterX )
        rect.x -= (int)(m_InputWidth/2);
    if( m_Justification & Justify_Right )
        rect.x -= (int)m_InputWidth;

    if( m_Justification & Justify_CenterY )
        rect.y -= (int)(m_InputHeight/2);
    if( m_Justification & Justify_Top )
        rect.y -= (int)m_InputHeight;

    return rect;
}

//void MenuButton::SetDisabled(bool settodisabled)
//{
//    if( settodisabled == true )
//    {
//        m_State = MBS_Disabled;
//    }
//
//    // only set button back to idle if we actually are disabled.
//    if( settodisabled == false && m_State == MBS_Disabled )
//    {
//        m_State = MBS_Idle;
//    }
//}

//bool MenuButton::IsDisabled()
//{
//    if( m_State == MBS_Disabled )
//        return true;
//
//    return false;
//}

void MenuButton::PlaySound()
{
    if( m_SoundPressed != GameAudioCue_None )
    {
        g_pGame->m_pGameAudio->Play( m_SoundPressed );
    }
}

void MenuButton::SetPositionAndSize(float x, float y, float w, float h, float inputw, float inputh)
{
    m_Transform.SetIdentity();
    m_Transform.Scale( w, h, 1 );
    m_Transform.SetTranslation( x, y, 0 );

    if( inputw < 0 )
    {
        m_InputWidth = w;
        m_InputHeight = h;
    }
    else
    {
        m_InputWidth = inputw;
        m_InputHeight = inputh;
    }
}

void MenuButton::SetString(const char* str1, const char* str2, const char* str3)
{
    int totallen = 0;

    assert( str1 != 0 );
    if( str1 != 0 )
    {
        if( str1[0] == '.' )
            totallen += strlen( g_pLanguageTable->LookUp( str1 ) );
        else
            totallen += strlen( str1 );
        sprintf_s( m_Strings[0], MAX_MENUBUTTON_STRING, "%s", str1 );
        m_Style = MBTS_SingleLine;
    }

    if( str2 != 0 )
    {
        if( str2[0] == '.' )
            totallen += strlen( g_pLanguageTable->LookUp( str2 ) );
        else
            totallen += strlen( str2 );
        sprintf_s( m_Strings[1], MAX_MENUBUTTON_STRING, "%s", str2 );
        m_Style = MBTS_DoubleLine;
    }

    if( str3 != 0 )
    {
        if( str3[0] == '.' )
            totallen += strlen( g_pLanguageTable->LookUp( str3 ) );
        else
            totallen += strlen( str3 );
        sprintf_s( m_Strings[2], MAX_MENUBUTTON_STRING, "%s", str3 );
        m_Style = MBTS_TripleLine;
    }

    if( m_pMeshText == 0 )
    {
        int multiplier = 1;
        multiplier = g_TextShadowStyleLetterCount[m_TextShadowStyle];

        m_pMeshText = MyNew MyMeshText( totallen * multiplier, 0 );
    }
}

void MenuButton::SetStringNumber(int stringnumber, const char* str1, ...)
{
    assert( stringnumber >= 0 && stringnumber < 3 );

    va_list arg;
    va_start(arg, str1);
    vsnprintf_s( m_Strings[stringnumber], MAX_MENUBUTTON_STRING, _TRUNCATE, str1, arg );
    va_end(arg);

    m_Strings[stringnumber][MAX_MENUBUTTON_STRING-1] = 0; // vsnprintf_s might do this, but docs are unclear

    if( m_pMeshText == 0 )
    {
        int totallen = (int)strlen( m_Strings[stringnumber] );
        int multiplier = 1;
        multiplier = g_TextShadowStyleLetterCount[m_TextShadowStyle];

        m_pMeshText = MyNew MyMeshText( totallen * multiplier, 0 );
    }
}

void MenuButton::SetToolTipString(const char* str)
{
    sprintf_s( m_ToolTipString, MAX_MENUBUTTON_STRING, "%s", str );
}

void MenuButton::SetPressedState(const ColorByte& textcolor, const ColorByte& bgcolor, MySprite* sprite, const Vector4& uvs)
{
    assert( m_pPressedBGSprite == 0 );

    m_UsePressedState = true;
    m_PressedTextColor = textcolor;
    m_PressedBGColor = bgcolor;
    if( sprite )
        m_pPressedBGSprite = MyNew MySprite( sprite, "SetPressedState" );
    m_PressedBGSpriteUVs = uvs;
}

void MenuButton::SetOverlay(const Vector2& size, const Vector2& offset, const ColorByte& bgcolor, MySprite* sprite, const Vector4& uvs)
{
    //assert( m_pOverlayBGSprite == 0 );

    m_HasOverlay = true;
    m_OverlaySize = size;
    m_OverlayOffset = offset;
    m_OverlayBGColor = bgcolor;
    if( m_pOverlayBGSprite )
    {
        m_pOverlayBGSprite->m_TextureID = sprite->m_TextureID;
        m_pOverlayBGSprite->m_pTexture = sprite->m_pTexture;
    }
    else
    {
        m_pOverlayBGSprite = MyNew MySprite( sprite, "MenuButton::SetOverlay" );
    }
    m_OverlayBGSpriteUVs = uvs;
}

void MenuButton::SetSprites(MySprite* bg, MySprite* disabled, MySprite* pressed, MySprite* overlay, MySprite* shadow)
{
    assert( m_pBGSprite == 0 );
    assert( m_pDisabledBGSprite == 0 );
    assert( m_pPressedBGSprite == 0 );
    assert( m_pOverlayBGSprite == 0 );
    assert( m_pShadowSprite == 0 );

    if( bg )
    {
        m_pBGSprite = bg;
        m_pBGSprite->AddRef();
    }

    if( disabled )
    {
        m_pDisabledBGSprite = disabled;
        m_pDisabledBGSprite->AddRef();
    }

    if( pressed )
    {
        m_pPressedBGSprite = pressed;
        m_pPressedBGSprite->AddRef();
    }

    if( overlay )
    {
        m_pOverlayBGSprite = overlay;
        m_pOverlayBGSprite->AddRef();
    }

    if( shadow )
    {
        m_pShadowSprite = shadow;
        m_pShadowSprite->AddRef();
    }
}

void MenuButton::SetSpritesCopy(MySprite* bg, MySprite* disabled, MySprite* pressed, MySprite* overlay, MySprite* shadow)
{
    assert( m_pBGSprite == 0 );
    assert( m_pDisabledBGSprite == 0 );
    assert( m_pPressedBGSprite == 0 );
    assert( m_pOverlayBGSprite == 0 );
    assert( m_pShadowSprite == 0 );

    if( bg )
    {
        m_pBGSprite = MyNew MySprite( bg, "MenuButton::SetSpritesCopy" );
    }

    if( disabled )
    {
        m_pDisabledBGSprite = MyNew MySprite( disabled, "MenuButton::SetSpritesCopy" );
    }

    if( pressed )
    {
        m_pPressedBGSprite = MyNew MySprite( pressed, "MenuButton::SetSpritesCopy" );
    }

    if( overlay )
    {
        m_pOverlayBGSprite = MyNew MySprite( overlay, "MenuButton::SetSpritesCopy" );
    }

    if( shadow )
    {
        m_pShadowSprite = MyNew MySprite( shadow, "MenuButton::SetSpritesCopy" );
    }
}

void MenuButton::SetSpritesBGShadow(MySprite* sprite)
{
    SetSprites( sprite, 0, 0, 0, sprite );
}

#if MYFW_USING_WX
void MenuButton::FillPropertiesWindow()
{
    MenuItem::FillPropertiesWindow();
    g_pPanelWatch->AddFloat( "FontHeight", &m_FontHeight, 0, 200 );
    g_pPanelWatch->AddFloat( "Width", &m_Transform.m11, 0, 1000 );
    g_pPanelWatch->AddFloat( "Height", &m_Transform.m22, 0, 1000 );
    g_pPanelWatch->AddFloat( "InputWidth", &m_InputWidth, 0, 1000 );
    g_pPanelWatch->AddFloat( "InputHeight", &m_InputHeight, 0, 1000 );
    g_pPanelWatch->AddFloat( "BGShadowOffsetX", &m_DropShadowOffsetBG_X, -10, 10 );
    g_pPanelWatch->AddFloat( "BGShadowOffsetY", &m_DropShadowOffsetBG_Y, -10, 10 );
    g_pPanelWatch->AddFloat( "TextShadowOffsetX", &m_DropShadowOffsetText_X, -10, 10 );
    g_pPanelWatch->AddFloat( "TextShadowOffsetY", &m_DropShadowOffsetText_Y, -10, 10 );

}
#endif //MYFW_USING_WX
