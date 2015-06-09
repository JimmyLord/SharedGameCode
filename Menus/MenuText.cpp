//
// Copyright (c) 2012-2015 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include PCHFILE
#include "MenuText.h"

MenuText::MenuText()
: m_TextColor(255,255,255,255)
, m_TextShadowColor(0,0,0,200)
{
    m_MenuItemType = MIT_Text;

    m_String[0] = 0;

    m_pMaterial = g_pMaterialManager->CreateMaterial();

    int maxletters = MAX_MenuText_STRING;
    m_pMeshText = MyNew MyMeshText( maxletters, 0 );
    m_MeshAllocatedLocally = true;
    m_DrawAsPartOfBatch = false;

    m_pFont = 0;
    m_FontHeight = 30;
    m_LineHeight = 30*0.8f;

    // set some reasonable defaults.
    m_Transform.m41 = 100;
    m_Transform.m42 = 400;

    strcpy_s( m_String, MAX_MenuText_STRING, "Text Item" );
    if( g_pFontManager )
        m_pFont = g_pFontManager->GetFirstFont();

    m_Justification = Justify_CenterX|Justify_CenterY;
    m_DropShadowOffsetX = 3;
    m_DropShadowOffsetY = -3;

    m_Scale.Set( 1, 1, 1 );
}

MenuText::MenuText(int maxletters)
: m_TextColor(255,255,255,255)
, m_TextShadowColor(0,0,0,200)
{
    m_MenuItemType = MIT_Text;

    m_String[0] = 0;

    m_pMaterial = g_pMaterialManager->CreateMaterial();

    if( maxletters == -1 )
        m_pMeshText = MyNew MyMeshText( MAX_MenuText_STRING*2, 0 );
    else if( maxletters > 0 )
        m_pMeshText = MyNew MyMeshText( maxletters, 0 );
    else
        m_pMeshText = MyNew MyMeshText( MAX_MenuText_STRING, 0 );

    m_MeshAllocatedLocally = true;
    m_DrawAsPartOfBatch = false;

    m_pFont = 0;
    m_FontHeight = 30;
    m_LineHeight = 30*0.8f;

    m_Justification = Justify_CenterX|Justify_CenterY;
    m_DropShadowOffsetX = 3;
    m_DropShadowOffsetY = -3;

    m_Scale.Set( 1, 1, 1 );
}

MenuText::MenuText(MyMeshText* pMeshText)
: m_TextColor(255,255,255,255)
, m_TextShadowColor(0,0,0,200)
{
    m_MenuItemType = MIT_Text;

    m_String[0] = 0;

    m_pMaterial = g_pMaterialManager->CreateMaterial();

    m_pMeshText = pMeshText;
    m_MeshAllocatedLocally = false;
    m_DrawAsPartOfBatch = true;

    m_pFont = 0;
    m_FontHeight = 30;
    m_LineHeight = 30*0.8f;

    m_Justification = Justify_CenterX|Justify_CenterY;
    m_DropShadowOffsetX = 3;
    m_DropShadowOffsetY = -3;

    m_Scale.Set( 1, 1, 1 );
}

MenuText::~MenuText()
{
    if( m_MeshAllocatedLocally )
        SAFE_RELEASE( m_pMeshText );

    SAFE_RELEASE( m_pMaterial );
}

void MenuText::Draw(MyMatrix* matviewproj)
{
    if( m_Visible == false )
        return;

    if( m_pFont == 0 )
        return;

    m_pMeshText->m_pFont = m_pFont; //ugh...

    float shadowoffx = m_DropShadowOffsetX;
    float shadowoffy = m_DropShadowOffsetY;

    float posx = (m_PositionOffset.x + m_Transform.m41);
    float posy = (m_PositionOffset.y + m_Transform.m42);

    ColorByte textcolor = m_TextColor;
    ColorByte textshadowcolor = m_TextShadowColor;

    if( m_String[0] != 0 )
    {
        if( shadowoffx != 0 && shadowoffy != 0 )
        {
            m_pMeshText->CreateString( m_DrawAsPartOfBatch, m_FontHeight*m_Scale.y, posx+shadowoffx, posy+shadowoffy, 0, 0, m_Justification, textshadowcolor, m_Size, m_String );
            m_pMeshText->CreateString( true, m_FontHeight*m_Scale.y, posx, posy, 0, 0, m_Justification, textcolor, m_Size, m_String );
        }
        else
        {
            m_pMeshText->CreateString( m_DrawAsPartOfBatch, m_FontHeight*m_Scale.y, posx, posy, 0, 0, m_Justification, textcolor, m_Size, m_String );
        }
        //m_pMaterial->SetShader( g_pGame->m_pShader_TextureVertexColor );
        //m_pMaterial->SetTextureColor( m_pFont->m_pTextureDef );
        //m_pMeshText->SetMaterial( m_pMaterial, 0 );

        if( m_DrawAsPartOfBatch == false )
        {
            // create a material for the font on the stack and set it. TODO: do better...
            MaterialDefinition pTempMaterial;
            pTempMaterial.SetShader( g_pShaderGroupManager->FindShaderGroupByName( "Shader_TextureVertexColor" ) );
            pTempMaterial.SetBlendType( MaterialBlendType_On );
            pTempMaterial.SetTextureColor( m_pFont->m_pTextureDef );
            m_pMeshText->SetMaterial( &pTempMaterial, 0 );
            m_pMeshText->Draw( matviewproj, 0, 0, 0, 0, 0, 0, 0 );
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

    float textwidth = m_Size.x;//1000;

    rect.x = (int)(m_Transform.m41 + m_PositionOffset.x);
    rect.w = (int)textwidth;
    rect.y = (int)(m_Transform.m42 + m_PositionOffset.y);
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

void MenuText::SetPositionAndSize(float x, float y, float w, float h, float inputw, float inputh)
{
    m_Transform.m41 = x;
    m_Transform.m42 = y;

    m_Size.Set( w, h );
}

void MenuText::SetString(const char* str, ...)
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

#if MYFW_USING_WX
void MenuText::FillPropertiesWindow()
{
    // Search for MenuText_SaveLoad
    MenuItem::FillPropertiesWindow();

    g_pPanelWatch->Add2Floats( "Height", "Font", "Line", &m_FontHeight, &m_LineHeight, 0, 200 );
    g_pPanelWatch->Add2Floats( "Shadow Offset", "x", "y", &m_DropShadowOffsetX, &m_DropShadowOffsetY, -10, 10 );
    g_pPanelWatch->AddUnsignedChar( "Justify", &m_Justification, -5, 5 );    

    if( m_pFont && m_pFont->m_pFile )
        g_pPanelWatch->AddPointerWithDescription( "Font", &m_pFont, m_pFont->m_pFile->m_FullPath, this, MenuText::StaticOnDropFont );
    else
        g_pPanelWatch->AddPointerWithDescription( "Font", &m_pFont, "no font", this, MenuText::StaticOnDropFont );

    g_pPanelWatch->AddString( "String", &m_String[0], MAX_MenuText_STRING );
}

void MenuText::OnDropFont(int controlid, wxCoord x, wxCoord y)
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
            if( pFontDef == 0 )
                pFontDef = g_pFontManager->CreateFont( pFile );
            m_pFont = pFontDef;
        }

        // update the panel so new Shader name shows up.
        g_pPanelWatch->m_pVariables[g_DragAndDropStruct.m_ID].m_Description = pFile->m_FullPath;
    }
}
#endif //MYFW_USING_WX
