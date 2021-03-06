//
// Copyright (c) 2012-2016 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "SharedCommonHeader.h"
#include "../Core/RenderTextQuick.h"
#include "../Core/MyMeshText.h"
#include "MenuText.h"

MenuText::MenuText(GameCore* pGameCore)
: MenuItem( pGameCore )
, m_TextColor(255,255,255,255)
, m_TextShadowColor(0,0,0,200)
{
    m_MenuItemType = MIT_Text;

    m_String[0] = 0;

    m_pMaterial = m_pGameCore->GetManagers()->GetMaterialManager()->CreateMaterial();

    int maxletters = MAX_MenuText_STRING;
    m_pMeshText = MyNew MyMeshText( maxletters, 0, m_pGameCore );
    m_MeshAllocatedLocally = true;
    m_DrawAsPartOfBatch = false;

    m_pFont = 0;
    m_FontHeight = 30.0f; // must default to 30 for save/load
    m_LineHeight = 24.0f; // must default to 24 for save/load

    // set some reasonable defaults.
    m_Position.x = 100;
    m_Position.y = 400;

    strcpy_s( m_String, MAX_MenuText_STRING, "Text Item" );
    if( m_pGameCore->GetManagers()->GetFontManager() )
        SetFont( m_pGameCore->GetManagers()->GetFontManager()->GetFirstFont() );

    m_TextSize.Set( 0, 0 ); // must default to 0,0 for save/load

    m_Justification = Justify_Center; // must default to Justify_Center for save/load
    m_DropShadowOffsetX = 3; // must default to 3 for save/load
    m_DropShadowOffsetY = -3; // must default to -3 for save/load
}

MenuText::MenuText(GameCore* pGameCore, int maxletters)
: MenuItem( pGameCore )
, m_TextColor(255,255,255,255)
, m_TextShadowColor(0,0,0,200)
{
    m_MenuItemType = MIT_Text;

    m_String[0] = 0;

    m_pMaterial = m_pGameCore->GetManagers()->GetMaterialManager()->CreateMaterial();

    if( maxletters == -1 )
        m_pMeshText = MyNew MyMeshText( MAX_MenuText_STRING*2, 0, m_pGameCore );
    else if( maxletters > 0 )
        m_pMeshText = MyNew MyMeshText( maxletters, 0, m_pGameCore );
    else
        m_pMeshText = MyNew MyMeshText( MAX_MenuText_STRING, 0, m_pGameCore );

    m_MeshAllocatedLocally = true;
    m_DrawAsPartOfBatch = false;

    m_pFont = 0;
    m_FontHeight = 30;
    m_LineHeight = 30*0.8f;

    m_TextSize.Set( 0, 0 );

    m_Justification = Justify_CenterX|Justify_CenterY;
    m_DropShadowOffsetX = 3;
    m_DropShadowOffsetY = -3;
}

MenuText::MenuText(GameCore* pGameCore, MyMeshText* pMeshText)
: MenuItem( pGameCore )
, m_TextColor(255,255,255,255)
, m_TextShadowColor(0,0,0,200)
{
    m_MenuItemType = MIT_Text;

    m_String[0] = 0;

    m_pMaterial = m_pGameCore->GetManagers()->GetMaterialManager()->CreateMaterial();

    m_pMeshText = pMeshText;
    m_MeshAllocatedLocally = false;
    m_DrawAsPartOfBatch = true;

    m_pFont = 0;
    m_FontHeight = 30;
    m_LineHeight = 30*0.8f;

    m_TextSize.Set( 0, 0 );

    m_Justification = Justify_CenterX|Justify_CenterY;
    m_DropShadowOffsetX = 3;
    m_DropShadowOffsetY = -3;
}

MenuText::~MenuText()
{
    if( m_MeshAllocatedLocally )
        SAFE_RELEASE( m_pMeshText );

    SAFE_RELEASE( m_pFont );

    SAFE_RELEASE( m_pMaterial );
}

MenuText& MenuText::operator=(const MenuText& other)
{
    MyAssert( &other != this );

    MenuItem::operator=( other );

    //this->m_Justification = other.m_Justification;

    return *this;
}

MenuText* CastAs_MenuText(MenuItem* pMenuItem)
{
    return (MenuText*)pMenuItem;
}

#if MYFW_USING_LUA
void MenuText::LuaRegister(lua_State* luastate)
{
    luabridge::getGlobalNamespace( luastate ).addFunction( "CastAs_MenuText", CastAs_MenuText ); // MenuText* CastAs_MenuText(MenuItem* pMenuItem)

    luabridge::getGlobalNamespace( luastate )
        .beginClass<MenuText>( "MenuText" )
            //.addData( "localmatrix", &MenuText::m_LocalTransform )
            .addFunction( "SetPositionAndSize", &MenuText::SetPositionAndSize ) // void MenuText::SetPositionAndSize(float x, float y, float w, float h, float inputw, float inputh)
            .addFunction( "SetString", &MenuText::SetString ) // void MenuText::SetString(const char* str)
        .endClass();
}
#endif //MYFW_USING_LUA

void MenuText::Draw(MyMatrix* pMatProj, MyMatrix* pMatView)
{
    if( m_Visible == false )
        return;

    if( m_pFont == 0 )
        return;

    m_pMeshText->m_pFont = m_pFont; //ugh...

    float shadowoffx = m_DropShadowOffsetX;
    float shadowoffy = m_DropShadowOffsetY;

    float posx = m_Position.x;
    float posy = m_Position.y;

    ColorByte textcolor = m_TextColor;
    ColorByte textshadowcolor = m_TextShadowColor;

    if( m_String[0] != 0 )
    {
        if( shadowoffx != 0 && shadowoffy != 0 )
        {
            m_pMeshText->CreateString( m_DrawAsPartOfBatch, m_FontHeight, posx+shadowoffx, posy+shadowoffy, 0, 0, m_Justification, textshadowcolor, m_TextSize, m_String );
            m_pMeshText->CreateString( true, m_FontHeight, posx, posy, 0, 0, m_Justification, textcolor, m_TextSize, m_String );
        }
        else
        {
            m_pMeshText->CreateString( m_DrawAsPartOfBatch, m_FontHeight, posx, posy, 0, 0, m_Justification, textcolor, m_TextSize, m_String );
        }
        //m_pMaterial->SetShader( g_pGame->m_pShader_TextureVertexColor );
        //m_pMaterial->SetTextureColor( m_pFont->m_pTextureDef );
        //m_pMeshText->SetMaterial( m_pMaterial, 0 );

        if( m_DrawAsPartOfBatch == false )
        {
            // create a material for the font on the stack and set it. TODO: do better...
            MaterialDefinition pTempMaterial( m_pGameCore->GetManagers()->GetMaterialManager() );
            pTempMaterial.SetShader( m_pGameCore->GetManagers()->GetShaderGroupManager()->FindShaderGroupByName( "Shader_TextureVertexColor" ) );
            pTempMaterial.SetBlendType( MyRE::MaterialBlendType_On );
            pTempMaterial.SetTextureColor( m_pFont->GetTexture() );
            m_pMeshText->SetMaterial( &pTempMaterial, 0 );
            m_pMeshText->Draw( pMatProj, pMatView, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
            m_pMeshText->SetMaterial( 0, 0 );
#if _DEBUG
            pTempMaterial.RemoveFinalRefIfCreatedOnStackToAvoidAssertInDestructor();
#endif
        }
    }
}

MyRect MenuText::GetBoundingRect()
{
    MyRect rect;

    float textwidth = m_TextSize.x;//1000;

    rect.x = (int)(m_Position.x);
    rect.w = (int)textwidth;
    rect.y = (int)(m_Position.y);
    rect.h = (int)m_FontHeight;

    if( m_Justification & Justify_CenterX )
        rect.x -= (int)(textwidth/2);
    if( m_Justification & Justify_Right )
        rect.x -= (int)textwidth;

    if( m_Justification & Justify_CenterY )
        rect.y -= (int)(m_FontHeight/2);
    if( m_Justification & Justify_Top )
        rect.y -= (int)m_FontHeight;

    return rect;
}

// Exposed to Lua, change elsewhere if function signature changes.
void MenuText::SetPositionAndSize(float x, float y, float w, float h, float inputw, float inputh)
{
    m_Position.x = x;
    m_Position.y = y;

    m_TextSize.Set( w, h );
}

// Exposed to Lua, change elsewhere if function signature changes.
void MenuText::SetString(const char* str)
{
    SetStringFormatted( str );
}

void MenuText::SetStringFormatted(const char* str, ...)
{
    MyAssert( str != 0 );

    char buffer[MAX_MenuText_STRING];
    va_list arg;
    va_start( arg, str );
    vsnprintf_s( buffer, sizeof(buffer), _TRUNCATE, str, arg );
    va_end(arg);

    buffer[MAX_MenuText_STRING-1] = 0; // vsnprintf_s might do this, but docs are unclear

    strcpy_s( m_String, MAX_MenuText_STRING, buffer );
}

void MenuText::SetFont(FontDefinition* pFont)
{
    if( pFont )
        pFont->AddRef();
    SAFE_RELEASE( m_pFont );
    m_pFont = pFont;
}

#if MYFW_USING_WX
void MenuText::FillPropertiesWindow()
{
    // Search for MenuText_SaveLoad
    MenuItem::FillPropertiesWindow();

    g_pPanelWatch->Add2Floats( "Height", "Font", "Line", &m_FontHeight, &m_LineHeight, 0, 200 );
    g_pPanelWatch->Add2Floats( "Shadow Offset", "x", "y", &m_DropShadowOffsetX, &m_DropShadowOffsetY, -10, 10 );
    g_pPanelWatch->AddUnsignedChar( "Justify", &m_Justification, -5, 5 );

    if( m_pFont && m_pFont->m_pFile )
        g_pPanelWatch->AddPointerWithDescription( "Font", &m_pFont, m_pFont->m_pFile->GetFullPath(), this, MenuText::StaticOnDropFont );
    else
        g_pPanelWatch->AddPointerWithDescription( "Font", &m_pFont, "no font", this, MenuText::StaticOnDropFont );

    g_pPanelWatch->AddString( "String", &m_String[0], MAX_MenuText_STRING );
}

void MenuText::OnDropFont(int controlid, int x, int y)
{
    DragAndDropItem* pDropItem = g_DragAndDropStruct.GetItem( 0 );

    if( pDropItem->m_Type == DragAndDropType_FileObjectPointer )
    {
        MyFileObject* pFile = (MyFileObject*)pDropItem->m_Value;
        MyAssert( pFile );

        const char* pPath = pFile->GetFullPath();
        size_t len = strlen( pPath );
        const char* filenameext = &pPath[len-4];

        if( strcmp( filenameext, ".fnt" ) == 0 )
        {
            FontDefinition* pFontDef = m_pGameCore->GetManagers()->GetFontManager()->FindFont( pFile );
            if( pFontDef != m_pFont )
            {
                SAFE_RELEASE( m_pFont );
                if( pFontDef == 0 )
                    pFontDef = m_pGameCore->GetManagers()->GetFontManager()->CreateFont( pFile );
                else
                    pFontDef->AddRef();
                m_pFont = pFontDef;
            }
        }

        // update the panel so new Shader name shows up.
        g_pPanelWatch->GetVariableProperties( g_DragAndDropStruct.GetControlID() )->m_Description = pFile->GetFullPath();
    }
}
#endif //MYFW_USING_WX
