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

#include "../../Framework/MyFramework/SourceCommon/CommonHeader.h"
#include "MenuSprite.h"

const char* MenuSprite::m_MaterialNames[Materials_NumTypes] =
{
    "MatSprite", "MatShadow",
};

MenuSprite::MenuSprite()
: m_BGColor(50,50,50,255)
{
    m_MenuItemType = MIT_Sprite;

    m_Justification = Justify_CenterX|Justify_CenterY;
    m_BGSize.Set( 50, 50 );
    m_DropShadowOffset.Set( 0, 0 );

    m_pSprite = 0;
    m_BGSpriteUVs = Vector4( 0, 1, 0, 1 );
    m_ShadowSpriteUVs = Vector4( 0, 1, 0, 1 );

    m_BGSize.x = 200;
    m_BGSize.y = 200;
    m_Position.x = 400;
    m_Position.y = 500;

    for( unsigned int i=0; i<Materials_NumTypes; i++ )
        m_pMaterials[i] = 0;
}

MenuSprite::~MenuSprite()
{
    SAFE_RELEASE( m_pSprite );

    for( unsigned int i=0; i<Materials_NumTypes; i++ )
        SAFE_RELEASE( m_pMaterials[i] );
}

MenuSprite& MenuSprite::operator=(const MenuSprite& other)
{
    MyAssert( &other != this );

    MenuItem::operator=( other );

    //this->m_Justification = other.m_Justification;

    return *this;
}

#if MYFW_USING_LUA
void MenuSprite::LuaRegister(lua_State* luastate)
{
    luabridge::getGlobalNamespace( luastate )
        .beginClass<MenuSprite>( "MenuSprite" )
            //.addData( "localmatrix", &MenuSprite::m_LocalTransform )

            .addFunction( "SetPositionAndSize", &MenuSprite::SetPositionAndSize )
        .endClass();
}
#endif //MYFW_USING_LUA

void MenuSprite::Draw(MyMatrix* matviewproj)
{
    if( m_Visible == false )
        return;

    if( m_pSprite == 0 )
    {
        m_pSprite = MyNew MySprite( false );
        m_pSprite->Create( "MenuSprite Sprite", 1, 1, 0, 1, 0, 1, Justify_Center, false );
    }

    //float scrw = g_pGame->m_GameWidth;
    //float scrh = g_pGame->m_GameHeight;

    float bgshadowoffx = m_DropShadowOffset.x;
    float bgshadowoffy = m_DropShadowOffset.y;

    //float posx = (m_PositionOffset.x + m_Transform.m41) / scrw;
    //float posy = (m_PositionOffset.y + m_Transform.m42) / scrh;
    //float bgwidth = m_BGWidth / scrw * m_Scale.x;
    //float bgheight = m_BGHeight / scrh * m_Scale.y;

    //MySprite* pSprite = m_pSprite;
    MaterialDefinition* pMaterial = m_pMaterials[Material_Sprite];

    //Vector4 uvs = m_BGSpriteUVs;
    ColorByte bgcolor = m_BGColor;

    //if( pSprite )
    //    pSprite->SetTint( ColorByte(255,255,255,255) );

    //if( pSprite == 0 )
    //{
    //    pSprite = g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable];
    //    //pSprite->SetTint( bgcolor );
    //}

    //MySprite* pShadowSprite = m_pShadowSprite;
    MaterialDefinition* pShadowMaterial = m_pMaterials[Material_Shadow];
    //Vector4 shuvs = m_ShadowSpriteUVs;
    //if( pShadowSprite == 0 )
    //{
    //    pShadowSprite = g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable];
    //    shuvs = Vector4( 0, 1, 0, 1 );
    //}

    //if( pSprite )
    {
        if( pShadowMaterial && m_DropShadowOffset.x != 0 && m_DropShadowOffset.y != 0 )
        {
            //pShadowSprite->Create( bgwidth, bgheight, shuvs.x, shuvs.y, shuvs.z, shuvs.w, m_Justification );

            //ColorByte shadowcolor = ColorByte(0,0,0,bgcolor.a/4);
            //pShadowSprite->SetTint( shadowcolor );
            ////pShadowSprite->SetPosition( posx+bgshadowoffx, posy-bgshadowoffy, 0.1f );
            m_pSprite->SetMaterial( pShadowMaterial );
            MyMatrix world;
            world.SetIdentity();
            world.m11 = m_BGSize.x;
            world.m22 = m_BGSize.y;
            world.m41 = m_Position.x + bgshadowoffx;
            world.m42 = m_Position.y + bgshadowoffy;

            m_pSprite->Draw( &world, matviewproj );//&g_pGame->m_OrthoMatrixGameSize );
        }

        //pSprite->Create( bgwidth, bgheight, uvs.x, uvs.y, uvs.z, uvs.w, m_Justification );

        if( pMaterial )
        {
            m_pSprite->SetMaterial( pMaterial );
            //m_pSprite->SetTint( bgcolor );
            //pSprite->SetPosition( posx, posy, 0 );
            MyMatrix world;
            world.SetIdentity();
            world.m11 = m_BGSize.x;
            world.m22 = m_BGSize.y;
            world.m41 = m_Position.x;
            world.m42 = m_Position.y;

            m_pSprite->Draw( &world, matviewproj );//&g_pGame->m_OrthoMatrixGameSize );
        }
    }

}

MyRect MenuSprite::GetBoundingRect()
{
    MyRect rect;

    rect.x = (int)(m_Position.x);
    rect.w = (int)m_BGSize.x;
    rect.y = (int)(m_Position.y);
    rect.h = (int)m_BGSize.y;

    if( m_Justification & Justify_CenterX )
        rect.x -= (int)(m_BGSize.x/2);
    if( m_Justification & Justify_Right )
        rect.x -= (int)m_BGSize.x;

    if( m_Justification & Justify_CenterY )
        rect.y -= (int)(m_BGSize.y/2);
    if( m_Justification & Justify_Top )
        rect.y -= (int)m_BGSize.y;

    return rect;
}

void MenuSprite::SetPositionAndSize(float x, float y, float w, float h, float inputw, float inputh)
{
    m_Position.Set( x, y );
    m_BGSize.Set( w, h );
}

void MenuSprite::SetSprites(MySprite* bgsprite, MySprite* shadowsprite)
{
    MyAssert( false );
    //MyAssert( m_pBGSprite == 0 );
    //MyAssert( m_pShadowSprite == 0 );

    //if( bgsprite )
    //{
    //    m_pBGSprite = bgsprite;
    //    m_pBGSprite->AddRef();
    //}

    //if( shadowsprite )
    //{
    //    m_pShadowSprite = shadowsprite;
    //    m_pShadowSprite->AddRef();
    //}
}

void MenuSprite::SetSpritesCopy(MySprite* bgsprite, MySprite* shadowsprite)
{
    MyAssert( false );
    //MyAssert( m_pBGSprite == 0 );
    //MyAssert( m_pShadowSprite == 0 );

    //if( bgsprite )
    //{
    //    m_pBGSprite = MyNew MySprite( bgsprite, "SetSpritesCopy" );
    //}

    //if( shadowsprite )
    //{
    //    m_pShadowSprite = MyNew MySprite( shadowsprite, "SetSpritesCopy" );
    //}
}

void MenuSprite::SetMaterial(unsigned int materialindex, MaterialDefinition* pMaterial)
{
    if( pMaterial )
        pMaterial->AddRef();
    SAFE_RELEASE( m_pMaterials[materialindex] );
    m_pMaterials[materialindex] = pMaterial;
}

#if MYFW_USING_WX
void MenuSprite::FillPropertiesWindow()
{
    MenuItem::FillPropertiesWindow();

    g_pPanelWatch->Add2Floats( "Size", "w", "h", &m_BGSize.x, &m_BGSize.y, 0, 1000 );
    g_pPanelWatch->AddVector2( "Shadow Offset", &m_DropShadowOffset, -10, 10 );

    for( unsigned int i=0; i<Materials_NumTypes; i++ )
    {
        if( m_pMaterials[i] )
            m_CONTROLID_Materials[i] = g_pPanelWatch->AddPointerWithDescription( m_MaterialNames[i], &m_pMaterials[i], m_pMaterials[i]->GetName(), this, MenuSprite::StaticOnDropMaterial );
        else
            m_CONTROLID_Materials[i] = g_pPanelWatch->AddPointerWithDescription( m_MaterialNames[i], &m_pMaterials[i], "no material", this, MenuSprite::StaticOnDropMaterial );
    }
}

void MenuSprite::OnDropMaterial(int controlid, wxCoord x, wxCoord y)
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
#endif //MYFW_USING_WX
