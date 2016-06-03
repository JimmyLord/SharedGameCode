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

//#include "GameCommonHeader.h"
#include "MenuButton.h"
#include "../Camera/SharedCamera3D.h"

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

const char* MenuButton::m_MaterialNames[Materials_NumTypes] =
{
    "MatBG", "MatBGDisabled", "MatBGPressed", "MatBGOverlay", "MatShadow",
};

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

    m_Navigable = true; // for MenuButton/MenuInputBox must default to true for save/load, false for other types.

    m_TimeHeld = 0;
    m_FingerHolding = -1;

    for( int i=0; i<3; i++ )
        m_Strings[i][0] = 0;
    m_ToolTipString[0] = 0;

    //m_pMaterial = 0; //g_pMaterialManager->CreateMaterial();

    if( maxletters == -1 )
        m_pMeshText = MyNew MyMeshText( 3*MAX_STRING_LENGTH, 0 );
    else if( maxletters > 0 )
        m_pMeshText = MyNew MyMeshText( maxletters, 0 );
    else
        m_pMeshText = 0;

    m_ButtonAction[0] = 0;
    m_AllowPressWhenDraggedOver = false;

    // setup a decently sized button
    m_BGSize.Set( 300, 200 );
    m_TextSize.Set( 0, 0 ); // no size on text, so it won't wrap

    m_Position.Set( 300, 300 );

    m_pFont = 0;
    m_FontHeight = 30.0f; // must default to 30 for save/load
    m_LineHeight = 24.0f; // must default to 24 for save/load

    m_Justification = Justify_Center; // must default to Justify_Center for save/load
    m_InputWidth = 0; // must default to 0 for save/load
    m_InputHeight = 0; // must default to 0 for save/load

    m_DropShadowOffsetText.Set( 0, 0 );
    m_DropShadowOffsetBG.Set( 0, 0 );

    m_pBGMesh = 0;
    m_pBGMeshCamera = 0;
    m_pBGMeshLight = 0;

    m_BGSpriteOn = true;
    m_BGSpriteUVs.Set( 0, 1, 0, 1 );
    m_DisabledBGSpriteUVs.Set( 0, 1, 0, 1 );

    m_ShadowSpriteUVs.Set( 0, 1, 0, 1 );

    //m_Scale.Set( 1, 1, 1 );

    m_hack_PasswordHideString2 = false;
    
    m_SoundPressed = 0; //GameAudioCue_None;
#if SOUNDHACK_USE_CLICK_AS_DEFAULT
    m_SoundPressed = GameAudioCue_Click;
#endif

    //m_Style = MBTS_SingleLine;

    //m_pBGSprite = 0;
    //m_pDisabledBGSprite = 0;
    //m_pPressedBGSprite = 0;
    //m_pOverlayBGSprite = 0;
    //m_pShadowSprite = 0;

    m_pSprite = 0;

    for( unsigned int i=0; i<Materials_NumTypes; i++ )
        m_pMaterials[i] = 0;
}

MenuButton::~MenuButton()
{
    //SAFE_RELEASE( m_pMaterial );
    SAFE_RELEASE( m_pMeshText );

    SAFE_RELEASE( m_pFont );

    //SAFE_RELEASE( m_pBGSprite );
    //SAFE_RELEASE( m_pDisabledBGSprite );
    //SAFE_RELEASE( m_pPressedBGSprite );
    //SAFE_RELEASE( m_pOverlayBGSprite );
    //SAFE_RELEASE( m_pShadowSprite );

    SAFE_RELEASE( m_pSprite );

    for( unsigned int i=0; i<Materials_NumTypes; i++ )
        SAFE_RELEASE( m_pMaterials[i] );
}

MenuButton& MenuButton::operator=(const MenuButton& other)
{
    MyAssert( &other != this );

    MenuItem::operator=( other );

    this->m_Justification = other.m_Justification;
    this->m_DropShadowOffsetText = other.m_DropShadowOffsetText;

    this->m_DropShadowOffsetBG = other.m_DropShadowOffsetBG;

    this->m_BGSize = other.m_BGSize;
    this->m_TextSize = other.m_TextSize;

    this->SetFont( other.m_pFont );
    this->SetString( other.m_Strings[0], other.m_Strings[1], other.m_Strings[2] );
    this->SetToolTipString( other.m_ToolTipString );

    this->SetButtonAction( other.m_ButtonAction );

    this->m_AllowPressWhenDraggedOver = other.m_AllowPressWhenDraggedOver;
    
    this->m_FontHeight = other.m_FontHeight;
    this->m_LineHeight = other.m_LineHeight;
    
    this->m_InputWidth = other.m_InputWidth;
    this->m_InputHeight = other.m_InputHeight;
    
    this->m_TextShadowStyle = other.m_TextShadowStyle;

    //this->m_pBGMesh = other.aaaaaa;
    //this->m_BGMeshTransform = other.aaaaaa;
    //this->m_pBGMeshCamera = other.aaaaaa;
    //this->m_pBGMeshLight = other.aaaaaa;
    //this->m_BGSpriteOn = other.aaaaaa;
    //this->m_TextOffset = other.aaaaaa;
    //this->m_TextColor = other.aaaaaa;
    //this->m_TextShadowColor = other.aaaaaa;
    //this->m_BGSpriteUVs = other.aaaaaa;
    //this->m_BGColor = other.aaaaaa;
    //this->m_BGShadowColor = other.aaaaaa;

    //this->m_DisabledTextColor = other.aaaaaa;
    //this->m_DisabledBGSpriteUVs = other.aaaaaa;
    //this->m_DisabledBGColor = other.aaaaaa;
    //this->m_UsePressedState = other.aaaaaa;
    //this->m_PressedTextColor = other.aaaaaa;
    //this->m_PressedBGColor = other.aaaaaa;
    //this->m_PressedBGSpriteUVs = other.aaaaaa;
    //this->m_HasOverlay = other.aaaaaa;
    //this->m_OverlaySize = other.aaaaaa;
    //this->m_OverlayOffset = other.aaaaaa;
    //this->m_OverlayBGColor = other.aaaaaa;
    //this->m_OverlayBGSpriteUVs = other.aaaaaa;
    //this->m_ShadowSpriteUVs = other.aaaaaa;
    //this->m_hack_PasswordHideString2 = other.aaaaaa;
    
    this->m_SoundPressed = other.m_SoundPressed;

    //this->m_pSprite = other.aaaaaa;

    for( int i=0; i<Materials_NumTypes; i++ )
        this->SetMaterial( i, other.m_pMaterials[i] );

    return *this;
}

#if MYFW_USING_LUA
void MenuButton::LuaRegister(lua_State* luastate)
{
    luabridge::getGlobalNamespace( luastate )
        .beginClass<MenuButton>( "MenuButton" )
            //.addData( "localmatrix", &MenuButton::m_LocalTransform )
            
            .addFunction( "SetSize", &MenuButton::SetSize )
            .addFunction( "SetPositionAndSize", &MenuButton::SetPositionAndSize )
            .addFunction( "SetString", &MenuButton::SetString )
            .addFunction( "SetStringNumber", &MenuButton::SetStringNumber )
            
            .addFunction( "GetSize", &MenuButton::GetSize )
            .addFunction( "GetBGSize", &MenuButton::GetBGSize )
        .endClass();
}
#endif //MYFW_USING_LUA

void MenuButton::StartClosing()
{
    MenuItem::StartClosing();
}

void MenuButton::Tick(double timepassed)
{
    m_TimeHeld += timepassed;

    MenuItem::Tick( timepassed );
}

void MenuButton::Draw(MyMatrix* matviewproj)
{
    if( m_Visible == false )
        return;

    // create a sprite if one doesn't exist.
    if( m_pSprite == 0 )
    {
        m_pSprite = MyNew MySprite( false );
        m_pSprite->Create( "MenuButton Sprite", 1, 1, 0, 1, 0, 1, Justify_Center, false );
    }

    if( m_pMeshText )
    {
        m_pMeshText->m_SubmeshList[0]->m_NumVertsToDraw = 0;
        m_pMeshText->m_SubmeshList[0]->m_NumIndicesToDraw = 0;
    }

    //float scrw = g_pGame->m_GameWidth;
    //float scrh = g_pGame->m_GameHeight;

    TextShadowStyles textshadowstyle = m_TextShadowStyle;
    if( m_DropShadowOffsetText.x == 0 && m_DropShadowOffsetText.y == 0 )
        textshadowstyle = TextShadowStyle_None;

    float lineheight = m_LineHeight;
    float shadowoffx = m_DropShadowOffsetText.x;
    float shadowoffy = m_DropShadowOffsetText.y;
    float buttonshadowoffx = m_DropShadowOffsetBG.x;
    float buttonshadowoffy = m_DropShadowOffsetBG.y;

    if( lineheight == -1 )
        lineheight = m_FontHeight*0.8f;

    //float posx = m_Transform.m41;
    //float posy = m_Transform.m42;

    ColorByte textcolor = m_TextColor;
    ColorByte textshadowcolor = m_TextShadowColor;
    //MySprite* pSprite = m_pBGSprite;
    MaterialDefinition* pMaterial = m_pMaterials[Material_BG];
    MyMesh* pMesh = m_pBGMesh;
    Vector4 uvs = m_BGSpriteUVs;
    ColorByte bgcolor = m_BGColor;
    if( m_Enabled == false )//m_State == MBS_Disabled )
    {
        textcolor = m_DisabledTextColor;
        bgcolor = m_DisabledBGColor;
        pMaterial = m_pMaterials[Material_BGDisabled];
        //pSprite = m_pDisabledBGSprite;
        uvs = m_DisabledBGSpriteUVs;
    }
    if( m_UsePressedState && m_State == MBS_HeldDown )
    {
        textcolor = m_PressedTextColor;
        bgcolor = m_PressedBGColor;
        if( m_pMaterials[Material_BGPressed] )
            pMaterial = m_pMaterials[Material_BGPressed];
        //if( m_pPressedBGSprite )
            //pSprite = m_pPressedBGSprite;
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

        //MySprite* pShadowSprite = m_pShadowSprite;
        MaterialDefinition* pMaterialShadow = m_pMaterials[Material_Shadow];
        //Vector4 shuvs = m_ShadowSpriteUVs;
        //if( pShadowSprite == 0 )
        //{
        //    pShadowSprite = g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable];
        //    shuvs = Vector4( 0, 1, 0, 1 );
        //}

        if( m_pSprite )
        {
            MyMatrix transform;
            transform.SetIdentity();

            transform.m41 = m_Position.x;
            transform.m42 = m_Position.y;
            transform.m11 = m_BGSize.x;
            transform.m22 = m_BGSize.y;

            if( pMaterialShadow && (buttonshadowoffx != 0 || buttonshadowoffy != 0) )
            {
                m_pSprite->SetMaterial( pMaterialShadow );
                //m_pSprite->SetTint( m_BGShadowColor );

                MyMatrix shadowmat = transform;
                shadowmat.Translate( buttonshadowoffx, buttonshadowoffy, 0 );

                //m_pSprite->SetTransform( shadowmat );
                m_pSprite->Draw( &shadowmat, matviewproj ); //&g_pGame->m_OrthoMatrixGameSize );
            }

            if( pMaterial != 0 )
            {
                m_pSprite->SetMaterial( pMaterial );
                //m_pSprite->SetTint( bgcolor );

                //m_pSprite->SetTransform( transform );
                //transform.m41 = 300;
                //transform.m42 = 300;
                m_pSprite->Draw( &transform, matviewproj ); //&g_pGame->m_OrthoMatrixGameSize );
            }
        }
    }

    // TODO: MYENGINE
    //if( m_HasOverlay )
    //{
    //    bgcolor = m_OverlayBGColor;
    //    pSprite = m_pOverlayBGSprite;
    //    uvs = m_OverlayBGSpriteUVs;

    //    if( pSprite )
    //    {
    //        pSprite->SetTint( bgcolor );

    //        MyMatrix transform;
    //        transform.SetIdentity();
    //        transform.Scale( m_OverlaySize.x, m_OverlaySize.y, 1 );
    //        transform.m41 = m_Transform.m41 + m_PositionOffset.x + m_OverlayOffset.x;
    //        transform.m42 = m_Transform.m42 + m_PositionOffset.y + m_OverlayOffset.y;

    //        pSprite->SetTransform( transform );
    //        pSprite->Draw( matviewproj ); //&g_pGame->m_OrthoMatrixGameSize );
    //    }
    //}

    if( pMesh && m_pBGMeshCamera )
    {
        // FIXME
        //float gamew = g_pGame->m_GameWidth;
        //float gameh = g_pGame->m_GameHeight;

        //int devw = (int)g_pGame->m_GameFullWidth;
        //int devh = (int)g_pGame->m_GameFullHeight;

        MyMatrix matfinalmesh = m_BGMeshTransform;
        //matfinalmesh.m41 = (((-g_pGame->m_OrthoLeft + m_Position.x) / devw) - 0.5f) * m_pBGMeshCamera->m_FrustumRightEdgeZ0*2;
        //matfinalmesh.m42 = (((-g_pGame->m_OrthoBottom + m_Position.y) / devh) - 0.5f) * m_pBGMeshCamera->m_FrustumTopEdgeZ0*2;
        matfinalmesh.m43 = 0;
        
        int numlights = 0;
        if( m_pBGMeshLight )
        {
            Vector3 lightpos = m_pBGMeshLight->m_Position;
            //lightpos.x = ((lightpos.x / gamew) - 0.5f) * m_pBGMeshCamera->m_FrustumRightEdgeZ0*2;
            //lightpos.y = ((lightpos.y / gameh) - 0.5f) * m_pBGMeshCamera->m_FrustumTopEdgeZ0*2;

            m_pBGMeshLight->m_Position = lightpos;

            numlights = 1;
        }

        pMesh->Draw( &matfinalmesh, &m_pBGMeshCamera->m_matViewProj, &m_pBGMeshCamera->m_Eye, 0,
                     m_pBGMeshLight, numlights, 0, 0, 0, 0 );
    }

    float y;

    if( m_pFont )
    {
        if( m_pMeshText )
            m_pMeshText->m_pFont = m_pFont; // ugh...

        float posx = m_Position.x + m_TextOffset.x;
        float posy = m_Position.y + m_TextOffset.y;

        if( m_State == MBS_HeldDown && m_ToolTipString[0] != 0 )
        {
            MyAssert( false );

            // ATM hardcoded for option overlay in A Game of Words.
            y = posy + 100;
            shadowoffx = -2;
            shadowoffy = 2;
            RenderTextQuickWithColor( m_pFont, m_FontHeight, posx+shadowoffx, y+shadowoffy, m_Justification, textshadowcolor, m_ToolTipString );
            shadowoffx = -2;
            shadowoffy = -2;
            RenderTextQuickWithColor( m_pFont, m_FontHeight, posx+shadowoffx, y+shadowoffy, m_Justification, textshadowcolor, m_ToolTipString );
            shadowoffx = 2;
            shadowoffy = 2;
            RenderTextQuickWithColor( m_pFont, m_FontHeight, posx+shadowoffx, y+shadowoffy, m_Justification, textshadowcolor, m_ToolTipString );
            shadowoffx = 2;
            shadowoffy = -2;
            RenderTextQuickWithColorAndShadow( m_pFont, m_FontHeight, posx, y, m_Justification, textcolor, textshadowcolor, shadowoffx, shadowoffy, m_ToolTipString );
        }

        //if( m_Strings[0][0] != 0 )
        if( m_pMeshText )
        {
            if( m_Strings[2][0] != 0 ) //m_Style == MBTS_TripleLine )
            {
                y = posy + lineheight*1;
                if( m_Strings[0][0] != 0 )
                {
                    m_pMeshText->CreateStringColorAndShadowStyle( true, m_FontHeight, posx, y, m_Justification, textcolor, textshadowstyle, textshadowcolor, shadowoffx, shadowoffy, m_TextSize, m_Strings[0] );
                }

                y = posy - lineheight*0;
                if( m_Strings[1][0] != 0 )
                {
                    m_pMeshText->CreateStringColorAndShadowStyle( true, m_FontHeight, posx, y, m_Justification, textcolor, textshadowstyle, textshadowcolor, shadowoffx, shadowoffy, m_TextSize, m_Strings[1] );
                }

                y = posy - lineheight*1;
                if( m_Strings[2][0] != 0 )
                {
                    m_pMeshText->CreateStringColorAndShadowStyle( true, m_FontHeight, posx, y, m_Justification, textcolor, textshadowstyle, textshadowcolor, shadowoffx, shadowoffy, m_TextSize, m_Strings[2] );
                }
            }
            else if( m_Strings[1][0] != 0 ) //m_Style == MBTS_DoubleLine )
            {
                y = posy + lineheight/2;
                if( m_Strings[0][0] != 0 )
                {
                    m_pMeshText->CreateStringColorAndShadowStyle( true, m_FontHeight, posx, y, m_Justification, textcolor, textshadowstyle, textshadowcolor, shadowoffx, shadowoffy, m_TextSize, m_Strings[0] );
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

                        m_pMeshText->CreateStringColorAndShadowStyle( true, m_FontHeight, posx, y, m_Justification, textcolor, textshadowstyle, textshadowcolor, shadowoffx, shadowoffy, m_TextSize, stars );
                    }
                    else
                    {
                        m_pMeshText->CreateStringColorAndShadowStyle( true, m_FontHeight, posx, y, m_Justification, textcolor, textshadowstyle, textshadowcolor, shadowoffx, shadowoffy, m_TextSize, m_Strings[1] );
                    }
                }
            }
            else //if( m_Style == MBTS_SingleLine )
            {
                y = posy;
                if( m_Strings[0][0] != 0 )
                {
                    m_pMeshText->CreateStringColorAndShadowStyle( false, m_FontHeight, posx, y, m_Justification, textcolor, textshadowstyle, textshadowcolor, shadowoffx, shadowoffy, m_TextSize, m_Strings[0] );
                }
            }
        }
    }

    // draw the text mesh.
    if( m_pFont && m_pMeshText )
    {
        m_pMeshText->m_MeshReady = true;

        // create a material for the font on the stack and set it. TODO: do better...
        MaterialDefinition pTempMaterial;
        //m_pMaterial->SetShader( g_pGame->m_pShader_TextureVertexColor );
        pTempMaterial.SetShader( g_pShaderGroupManager->FindShaderGroupByName( "Shader_TextureVertexColor" ) );
        pTempMaterial.SetBlendType( MaterialBlendType_On );
        pTempMaterial.SetTextureColor( m_pFont->m_pTextureDef );
        m_pMeshText->SetMaterial( &pTempMaterial, 0 );
        m_pMeshText->Draw( 0, matviewproj, 0, 0, 0, 0, 0, 0, 0, 0 );
        m_pMeshText->SetMaterial( 0, 0 );
#if _DEBUG
        pTempMaterial.RemoveFinalRefIfCreatedOnStackToAvoidAssertInDestructor();
#endif
    }

    checkGlError( "end of MenuButton::Draw()" );
}

bool MenuButton::CheckForCollision(float x, float y)
{
    if( m_State == MBS_Disabled )
        return false;

    if( m_Enabled == false )
        return false;

    //float scrw = g_pGame->m_GameWidth;
    //float scrh = g_pGame->m_GameHeight;

    float posx = m_Position.x;
    float posy = m_Position.y;

    // for the first 5th of a second, make the button huge, so quick taps will register even if users finger moves.
    float sizemultiplier = 1.0f;
    if( m_State == MBS_HeldDown && m_TimeHeld > 0 && m_TimeHeld < 0.2001f ) // FINGER_STABILITY_TIME
        sizemultiplier = 3.0f;

    float inputwidth = m_InputWidth <= 0 ? m_BGSize.x : m_InputWidth;
    float inputheight = m_InputHeight <= 0 ? m_BGSize.y : m_InputHeight;

    if( fabs( posx - x ) < (inputwidth / 2.0f)*sizemultiplier &&
        fabs( posy - y ) < (inputheight / 2.0f)*sizemultiplier )
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
        float posx = m_Position.x;
        float posy = m_Position.y;

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

const char* MenuButton::TriggerOnCollision(int fingerid, float x, float y, bool careifheld, bool releaseifnocollision)
{
    if( careifheld && m_FingerHolding != fingerid )
        return 0;

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

    return 0;
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

    float inputwidth = m_InputWidth == -1 ? m_BGSize.x : m_InputWidth;
    float inputheight = m_InputHeight == -1 ? m_BGSize.y : m_InputHeight;

    rect.x = (int)(m_Position.x);
    rect.w = (int)inputwidth;
    rect.y = (int)(m_Position.y);
    rect.h = (int)inputheight;

    if( m_Justification & Justify_CenterX )
        rect.x -= (int)(inputwidth/2);
    if( m_Justification & Justify_Right )
        rect.x -= (int)inputwidth;

    if( m_Justification & Justify_CenterY )
        rect.y -= (int)(inputheight/2);
    if( m_Justification & Justify_Top )
        rect.y -= (int)inputheight;

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
// TODO: MYENGINE
    //if( m_SoundPressed != GameAudioCue_None )
    //{
    //    g_pGame->m_pGameAudio->Play( m_SoundPressed );
    //}
}

void MenuButton::SetSize(float w, float h, float inputw, float inputh)
{
    m_BGSize.Set( w, h );

    m_InputWidth = inputw;
    m_InputHeight = inputh;
}

void MenuButton::SetPositionAndSize(float x, float y, float w, float h, float inputw, float inputh)
{
    m_Position.Set( x, y );
    m_BGSize.Set( w, h );

    m_InputWidth = inputw;
    m_InputHeight = inputh;
}

void MenuButton::SetString(const char* str1, const char* str2, const char* str3)
{
    int totallen = 0;

    MyAssert( str1 != 0 );
    if( str1 != 0 )
    {
        if( str1[0] == '.' )
            totallen += strlen( g_pLanguageTable->LookUp( str1 ) );
        else
            totallen += strlen( str1 );
        sprintf_s( m_Strings[0], MAX_STRING_LENGTH, "%s", str1 );
        //m_Style = MBTS_SingleLine;
    }

    if( str2 != 0 )
    {
        if( str2[0] == '.' )
            totallen += strlen( g_pLanguageTable->LookUp( str2 ) );
        else
            totallen += strlen( str2 );
        sprintf_s( m_Strings[1], MAX_STRING_LENGTH, "%s", str2 );
        //m_Style = MBTS_DoubleLine;
    }
    else
    {
        m_Strings[1][0] = 0;
    }

    if( str3 != 0 )
    {
        if( str3[0] == '.' )
            totallen += strlen( g_pLanguageTable->LookUp( str3 ) );
        else
            totallen += strlen( str3 );
        sprintf_s( m_Strings[2], MAX_STRING_LENGTH, "%s", str3 );
        //m_Style = MBTS_TripleLine;
    }
    else
    {
        m_Strings[2][0] = 0;
    }

    if( m_pMeshText == 0 )
    {
        int multiplier = 1;
        multiplier = g_TextShadowStyleLetterCount[m_TextShadowStyle];

        m_pMeshText = MyNew MyMeshText( totallen * multiplier, 0 );
    }
}

void MenuButton::SetStringNumber(int stringnumber, const char* str1)
{
    SetStringNumberFormatted( stringnumber, str1 );
}

void MenuButton::SetStringNumberFormatted(int stringnumber, const char* str1, ...)
{
    MyAssert( stringnumber >= 0 && stringnumber < 3 );

    va_list arg;
    va_start(arg, str1);
    vsnprintf_s( m_Strings[stringnumber], MAX_STRING_LENGTH, _TRUNCATE, str1, arg );
    va_end(arg);

    m_Strings[stringnumber][MAX_STRING_LENGTH-1] = 0; // vsnprintf_s might do this, but docs are unclear

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
    sprintf_s( m_ToolTipString, MAX_STRING_LENGTH, "%s", str );
}

void MenuButton::SetButtonAction(const char* action)
{
    sprintf_s( m_ButtonAction, MAX_BUTTON_ACTION_LENGTH, "%s", action );
}

void MenuButton::SetButtonActionAsChar(char action)
{
    m_ButtonAction[0] = action;
    m_ButtonAction[1] = 0;
}

//void MenuButton::SetPressedState(const ColorByte& textcolor, const ColorByte& bgcolor, MySprite* sprite, const Vector4& uvs)
//{
//    MyAssert( m_pPressedBGSprite == 0 );
//
//    m_UsePressedState = true;
//    m_PressedTextColor = textcolor;
//    m_PressedBGColor = bgcolor;
//    if( sprite )
//        m_pPressedBGSprite = MyNew MySprite( sprite, "SetPressedState" );
//    m_PressedBGSpriteUVs = uvs;
//}
//
//void MenuButton::SetOverlay(const Vector2& size, const Vector2& offset, const ColorByte& bgcolor, MySprite* sprite, const Vector4& uvs)
//{
//    //MyAssert( m_pOverlayBGSprite == 0 );
//
//    m_HasOverlay = true;
//    m_OverlaySize = size;
//    m_OverlayOffset = offset;
//    m_OverlayBGColor = bgcolor;
//    if( m_pOverlayBGSprite )
//    {
//        m_pOverlayBGSprite->GetMaterial()->SetTextureColor( sprite->GetMaterial()->GetTextureColor() );
//    }
//    else
//    {
//        m_pOverlayBGSprite = MyNew MySprite( sprite, "MenuButton::SetOverlay" );
//    }
//    m_OverlayBGSpriteUVs = uvs;
//}
//
//void MenuButton::SetSprites(MySprite* bg, MySprite* disabled, MySprite* pressed, MySprite* overlay, MySprite* shadow)
//{
//    MyAssert( m_pBGSprite == 0 );
//    MyAssert( m_pDisabledBGSprite == 0 );
//    MyAssert( m_pPressedBGSprite == 0 );
//    MyAssert( m_pOverlayBGSprite == 0 );
//    MyAssert( m_pShadowSprite == 0 );
//
//    if( bg )
//    {
//        m_pBGSprite = bg;
//        m_pBGSprite->AddRef();
//    }
//
//    if( disabled )
//    {
//        m_pDisabledBGSprite = disabled;
//        m_pDisabledBGSprite->AddRef();
//    }
//
//    if( pressed )
//    {
//        m_pPressedBGSprite = pressed;
//        m_pPressedBGSprite->AddRef();
//    }
//
//    if( overlay )
//    {
//        m_pOverlayBGSprite = overlay;
//        m_pOverlayBGSprite->AddRef();
//    }
//
//    if( shadow )
//    {
//        m_pShadowSprite = shadow;
//        m_pShadowSprite->AddRef();
//    }
//}
//
//void MenuButton::SetSpritesCopy(MySprite* bg, MySprite* disabled, MySprite* pressed, MySprite* overlay, MySprite* shadow)
//{
//    MyAssert( m_pBGSprite == 0 );
//    MyAssert( m_pDisabledBGSprite == 0 );
//    MyAssert( m_pPressedBGSprite == 0 );
//    MyAssert( m_pOverlayBGSprite == 0 );
//    MyAssert( m_pShadowSprite == 0 );
//
//    if( bg )
//    {
//        m_pBGSprite = MyNew MySprite( bg, "MenuButton::SetSpritesCopy" );
//    }
//
//    if( disabled )
//    {
//        m_pDisabledBGSprite = MyNew MySprite( disabled, "MenuButton::SetSpritesCopy" );
//    }
//
//    if( pressed )
//    {
//        m_pPressedBGSprite = MyNew MySprite( pressed, "MenuButton::SetSpritesCopy" );
//    }
//
//    if( overlay )
//    {
//        m_pOverlayBGSprite = MyNew MySprite( overlay, "MenuButton::SetSpritesCopy" );
//    }
//
//    if( shadow )
//    {
//        m_pShadowSprite = MyNew MySprite( shadow, "MenuButton::SetSpritesCopy" );
//    }
//}
//
//void MenuButton::SetSpritesBGShadow(MySprite* sprite)
//{
//    SetSprites( sprite, 0, 0, 0, sprite );
//}

void MenuButton::SetMaterial(unsigned int materialindex, MaterialDefinition* pMaterial)
{
    if( pMaterial )
        pMaterial->AddRef();
    SAFE_RELEASE( m_pMaterials[materialindex] );
    m_pMaterials[materialindex] = pMaterial;
}

void MenuButton::SetFont(FontDefinition* pFont)
{
    if( pFont )
        pFont->AddRef();
    SAFE_RELEASE( m_pFont );
    m_pFont = pFont;
}

#if MYFW_USING_WX
void MenuButton::FillPropertiesWindow()
{
    MenuItem::FillPropertiesWindow();

    g_pPanelWatch->Add2Floats( "Height", "Font", "Line", &m_FontHeight, &m_LineHeight, 0, 200 );
    g_pPanelWatch->AddVector2( "Size", &m_BGSize, 0, 1000 );
    g_pPanelWatch->Add2Floats( "InputSize", "w", "h", &m_InputWidth, &m_InputHeight, 0, 1000 );
    g_pPanelWatch->AddVector2( "BGShadowOffset", &m_DropShadowOffsetBG, -10, 10 );
    g_pPanelWatch->AddVector2( "TextShadowOffset", &m_DropShadowOffsetText, -10, 10 );

    if( m_pFont && m_pFont->m_pFile )
        g_pPanelWatch->AddPointerWithDescription( "Font", &m_pFont, m_pFont->m_pFile->m_FullPath, this, MenuButton::StaticOnDropFont );
    else
        g_pPanelWatch->AddPointerWithDescription( "Font", &m_pFont, "no font", this, MenuButton::StaticOnDropFont );

    for( unsigned int i=0; i<Materials_NumTypes; i++ )
    {
        if( m_pMaterials[i] )
            m_CONTROLID_Materials[i] = g_pPanelWatch->AddPointerWithDescription( m_MaterialNames[i], &m_pMaterials[i], m_pMaterials[i]->GetName(), this, MenuButton::StaticOnDropMaterial, MenuButton::StaticOnValueChanged );
        else
            m_CONTROLID_Materials[i] = g_pPanelWatch->AddPointerWithDescription( m_MaterialNames[i], &m_pMaterials[i], "no material", this, MenuButton::StaticOnDropMaterial, MenuButton::StaticOnValueChanged );
    }

    g_pPanelWatch->AddString( "String1", &m_Strings[0][0], MAX_STRING_LENGTH );
    g_pPanelWatch->AddString( "String2", &m_Strings[1][0], MAX_STRING_LENGTH );
    g_pPanelWatch->AddString( "String3", &m_Strings[2][0], MAX_STRING_LENGTH );

    g_pPanelWatch->AddString( "Action", &m_ButtonAction[0], MAX_BUTTON_ACTION_LENGTH );
}

void MenuButton::OnDropFont(int controlid, wxCoord x, wxCoord y)
{
    if( g_DragAndDropStruct.m_Type == DragAndDropType_FileObjectPointer )
    {
        MyFileObject* pFile = (MyFileObject*)g_DragAndDropStruct.m_Value;
        MyAssert( pFile );

        size_t len = strlen( pFile->m_FullPath );
        const char* filenameext = &pFile->m_FullPath[len-4];

        if( strcmp( filenameext, ".fnt" ) == 0 )
        {
            FontDefinition* pFontDef = g_pFontManager->FindFont( pFile );
            if( pFontDef != m_pFont )
            {
                SAFE_RELEASE( m_pFont );
                if( pFontDef == 0 )
                    pFontDef = g_pFontManager->CreateFont( pFile );
                else
                    pFontDef->AddRef();
                m_pFont = pFontDef;
            }
        }

        // update the panel so new Shader name shows up.
        g_pPanelWatch->m_pVariables[g_DragAndDropStruct.m_ID].m_Description = pFile->m_FullPath;
    }
}

void MenuButton::OnDropMaterial(int controlid, wxCoord x, wxCoord y)
{
    if( g_DragAndDropStruct.m_Type == DragAndDropType_MaterialDefinitionPointer )
    {
        MaterialDefinition* pMaterial = (MaterialDefinition*)g_DragAndDropStruct.m_Value;
        MyAssert( pMaterial );
        
        unsigned int i;
        for( i=0; i<Materials_NumTypes; i++ )
        {
            if( m_CONTROLID_Materials[i] == controlid )
                break;
        }
        SetMaterial( i, pMaterial );

        // update the panel so new Material name shows up.
        g_pPanelWatch->m_pVariables[g_DragAndDropStruct.m_ID].m_Description = pMaterial->GetName();
    }
}

void MenuButton::OnValueChanged(int controlid, bool finishedchanging)
{
    if( controlid == -1 )
        return;

    for( int i=0; i<Materials_NumTypes; i++ )
    {
        if( controlid == m_CONTROLID_Materials[i] )
        {
            wxString text = g_pPanelWatch->m_pVariables[controlid].m_Handle_TextCtrl->GetValue();
            if( text == "" )
            {
                // if blank, then remove the material.
                MyAssert( g_pPanelWatch->m_pVariables[controlid].m_Handle_TextCtrl != 0 );
                g_pPanelWatch->ChangeDescriptionForPointerWithDescription( controlid, "no material" );
                SetMaterial( i, 0 );
            }
            else
            {
                // if not blank, then reset the text box back to the material name...
                // TODO: handle typing in a name
                if( m_pMaterials[i] )
                {
                    g_pPanelWatch->ChangeDescriptionForPointerWithDescription( controlid, m_pMaterials[i]->GetName() );
                }
            }
        }
    }
}
#endif //MYFW_USING_WX
