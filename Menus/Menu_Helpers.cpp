//
// Copyright (c) 2012-2015 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "SharedCommonHeader.h"

#define GETMENUTYPE(itemarray, index, type, typeclass) \
    MyAssert( itemarray[index]->m_MenuItemType == type ); \
    if( itemarray[index]->m_MenuItemType != type ) return 0; \
    return (typeclass*)itemarray[index];

MenuSprite* GetMenuSprite(MenuItem** itemarray, int index)               { GETMENUTYPE( itemarray, index, MIT_Sprite, MenuSprite ); }
MenuText* GetMenuText(MenuItem** itemarray, int index)                   { GETMENUTYPE( itemarray, index, MIT_Text, MenuText ); }
MenuButton* GetMenuButton(MenuItem** itemarray, int index)               { GETMENUTYPE( itemarray, index, MIT_Button, MenuButton ); }
MenuScrollBox* GetMenuScrollBox(MenuItem** itemarray, int index)         { GETMENUTYPE( itemarray, index, MIT_ScrollBox, MenuScrollBox ); }
MenuScrollingText* GetMenuScrollingText(MenuItem** itemarray, int index) { GETMENUTYPE( itemarray, index, MIT_ScrollingText, MenuScrollingText ); }
MenuInputBox* GetMenuInputBox(MenuItem** itemarray, int index)           { GETMENUTYPE( itemarray, index, MIT_InputBox, MenuInputBox ); }
MenuCheckBox* GetMenuCheckBox(MenuItem** itemarray, int index)           { GETMENUTYPE( itemarray, index, MIT_CheckBox, MenuCheckBox ); }

#define CREATEMENUTYPE(pGameCore, itemarray, index, typeclass) \
    MyAssert( itemarray[index] == 0 ); \
    itemarray[index] = MyNew typeclass(pGameCore); \
    return (typeclass*)itemarray[index];

#define CREATEMENUTYPEINT(pGameCore, itemarray, index, typeclass, value) \
    MyAssert( itemarray[index] == 0 ); \
    itemarray[index] = MyNew typeclass(pGameCore, value); \
    return (typeclass*)itemarray[index];

MenuSprite* CreateMenuSprite(GameCore* pGameCore, MenuItem** itemarray, int index)                    { CREATEMENUTYPE( pGameCore, itemarray, index, MenuSprite ); }
MenuText* CreateMenuText(GameCore* pGameCore, MenuItem** itemarray, int index, int maxletters)        { CREATEMENUTYPEINT( pGameCore, itemarray, index, MenuText, maxletters ); }
MenuButton* CreateMenuButton(GameCore* pGameCore, MenuItem** itemarray, int index, int maxletters)    { CREATEMENUTYPEINT( pGameCore, itemarray, index, MenuButton, maxletters ); }
//MenuScrollBox* CreateMenuScrollBox(GameCore* pGameCore, MenuItem** itemarray, int index)              { CREATEMENUTYPE( pGameCore, itemarray, index, MenuScrollBox ); }
MenuScrollingText* CreateMenuScrollingText(GameCore* pGameCore, MenuItem** itemarray, int index)      { CREATEMENUTYPE( pGameCore, itemarray, index, MenuScrollingText ); }
MenuInputBox* CreateMenuInputBox(GameCore* pGameCore, MenuItem** itemarray, int index)                { CREATEMENUTYPE( pGameCore, itemarray, index, MenuInputBox ); }
MenuCheckBox* CreateMenuCheckBox(GameCore* pGameCore, MenuItem** itemarray, int index)                { CREATEMENUTYPE( pGameCore, itemarray, index, MenuCheckBox ); }

Vector2 Menu_ImportExport::GetRelativePositionToAnchorPoint(MenuItemAnchorPoint anchorpointtype, Vector2 pos, Vector4 ExtentsBLTR)
{
    float bottom = ExtentsBLTR.x;
    float left = ExtentsBLTR.y;
    float top = ExtentsBLTR.z;
    float right = ExtentsBLTR.w;

    if( anchorpointtype == Anchor_TopLeft )
        pos -= Vector2( left, top );
    else if( anchorpointtype == Anchor_TopRight )
        pos -= Vector2( right, top );
    else if( anchorpointtype == Anchor_BottomLeft )
        pos -= Vector2( left, bottom );
    else if( anchorpointtype == Anchor_BottomRight )
        pos -= Vector2( right, bottom );

    return pos;
}

Vector2 Menu_ImportExport::GetAbsolutePositionFromAnchorPoint(MenuItemAnchorPoint anchorpointtype, Vector2 pos, Vector4 ExtentsBLTR)
{
    float bottom = ExtentsBLTR.x;
    float left = ExtentsBLTR.y;
    float top = ExtentsBLTR.z;
    float right = ExtentsBLTR.w;

    if( anchorpointtype == Anchor_TopLeft )
        pos += Vector2( left, top );
    else if( anchorpointtype == Anchor_TopRight )
        pos += Vector2( right, top );
    else if( anchorpointtype == Anchor_BottomLeft )
        pos += Vector2( left, bottom );
    else if( anchorpointtype == Anchor_BottomRight )
        pos += Vector2( right, bottom );

    return pos;
}

cJSON* Menu_ImportExport::ExportMenuLayout(MenuItem** itemarray, unsigned int numitems, Vector4 ExtentsBLTR)
{
    cJSON* menuitemarray = cJSON_CreateArray();

    for( unsigned int i=0; i<numitems; i++ )
    {
        MenuItem* pMenuItem = itemarray[i];

        if( pMenuItem )
        {
            cJSON* menuitem = cJSON_CreateObject();
            cJSON_AddItemToArray( menuitemarray, menuitem );

            cJSON_AddNumberToObject( menuitem, "MIT", pMenuItem->m_MenuItemType );

            cJSON_AddStringToObject( menuitem, "Name", pMenuItem->m_Name );

            cJSONExt_AddNumberToObjectIfDiffers( menuitem, "Anchor", (int)pMenuItem->m_AnchorPoint, 0 );

            if( pMenuItem->m_MenuItemType == MIT_Button || pMenuItem->m_MenuItemType == MIT_InputBox || pMenuItem->m_MenuItemType == MIT_ScrollingText )
                cJSONExt_AddNumberToObjectIfDiffers( menuitem, "Navigable", pMenuItem->m_Navigable, true );
            else
                cJSONExt_AddNumberToObjectIfDiffers( menuitem, "Navigable", pMenuItem->m_Navigable, false );
           
            Vector2 relativepos = pMenuItem->m_Position;
            if( pMenuItem->m_AnchorPoint != Anchor_None )
                relativepos = GetRelativePositionToAnchorPoint( pMenuItem->m_AnchorPoint, pMenuItem->m_Position, ExtentsBLTR );

            cJSON_AddNumberToObject( menuitem, "X", relativepos.x );
            cJSON_AddNumberToObject( menuitem, "Y", relativepos.y );

            switch( pMenuItem->m_MenuItemType )
            {
            case MIT_Sprite:
                {
                    MenuSprite* pMenuSprite = GetMenuSprite( itemarray, i );

                    cJSON_AddNumberToObject( menuitem, "W", pMenuSprite->m_BGSize.x );
                    cJSON_AddNumberToObject( menuitem, "H", pMenuSprite->m_BGSize.y );

                    cJSONExt_AddFloatArrayToObject( menuitem, "Shadow", &pMenuSprite->m_DropShadowOffset.x, 2 );

                    for( unsigned int i=0; i<MenuSprite::Materials_NumTypes; i++ )
                    {
                        if( pMenuSprite->m_pMaterials[i] )
                            cJSON_AddStringToObject( menuitem, MenuSprite::m_MaterialNames[i], pMenuSprite->m_pMaterials[i]->GetMaterialDescription() );
                    }
                }
                break;

            case MIT_Text: // MenuText_SaveLoad
                {
                    MenuText* pMenuText = GetMenuText( itemarray, i );

                    //cJSON_AddNumberToObject( menuitem, "LettersNeeded", pMenuText->m_String );

                    cJSONExt_AddNumberToObjectIfDiffers( menuitem, "W", pMenuText->m_TextSize.x, 0.0f );
                    cJSONExt_AddNumberToObjectIfDiffers( menuitem, "H", pMenuText->m_TextSize.y, 0.0f );

                    cJSONExt_AddNumberToObjectIfDiffers( menuitem, "FontHeight", pMenuText->m_FontHeight, 30.0f );
                    cJSONExt_AddNumberToObjectIfDiffers( menuitem, "LineHeight", pMenuText->m_LineHeight, 24.0f );

                    cJSONExt_AddNumberToObjectIfDiffers( menuitem, "TextShadowX", pMenuText->m_DropShadowOffsetX, 3.0f );
                    cJSONExt_AddNumberToObjectIfDiffers( menuitem, "TextShadowY", pMenuText->m_DropShadowOffsetY, -3.0f );

                    cJSONExt_AddNumberToObjectIfDiffers( menuitem, "Justify", (unsigned char)pMenuText->m_Justification, (unsigned char)Justify_Center );

                    if( pMenuText->GetFont() )
                        cJSON_AddStringToObject( menuitem, "Font", pMenuText->GetFont()->GetFile()->GetFullPath() );

                    if( pMenuText->m_String[0] != 0 )
                        cJSON_AddStringToObject( menuitem, "String", pMenuText->m_String );
                }
                break;

            case MIT_Button:
            case MIT_InputBox:
            case MIT_ScrollingText:
                {
                    MenuButton* pMenuButton;
                    if( pMenuItem->m_MenuItemType == MIT_InputBox )
                        pMenuButton = (MenuButton*)GetMenuInputBox( itemarray, i );
                    else if( pMenuItem->m_MenuItemType == MIT_ScrollingText )
                        pMenuButton = GetMenuScrollingText( itemarray, i );
                    else
                        pMenuButton = GetMenuButton( itemarray, i );

                    cJSONExt_AddFloatArrayToObject( menuitem, "TextSize", &pMenuButton->m_TextSize.x, 2 );

                    cJSON_AddNumberToObject( menuitem, "W", pMenuButton->m_BGSize.x );
                    cJSON_AddNumberToObject( menuitem, "H", pMenuButton->m_BGSize.y );

                    cJSONExt_AddNumberToObjectIfDiffers( menuitem, "IW", pMenuButton->m_InputWidth, 0.0f );
                    cJSONExt_AddNumberToObjectIfDiffers( menuitem, "IH", pMenuButton->m_InputHeight, 0.0f );

                    cJSONExt_AddNumberToObjectIfDiffers( menuitem, "FontHeight", pMenuButton->m_FontHeight, 30.0f );
                    cJSONExt_AddNumberToObjectIfDiffers( menuitem, "LineHeight", pMenuButton->m_LineHeight, 24.0f );

                    cJSONExt_AddNumberToObjectIfDiffers( menuitem, "Justify", (unsigned char)pMenuButton->m_Justification, (unsigned char)Justify_Center );

                    cJSONExt_AddFloatArrayToObject( menuitem, "BGShadow", &pMenuButton->m_DropShadowOffsetBG.x, 2 );
                    cJSONExt_AddFloatArrayToObject( menuitem, "TextShadow", &pMenuButton->m_DropShadowOffsetText.x, 2 );

                    for( unsigned int i=0; i<MenuButton::Materials_NumTypes; i++ )
                    {
                        if( pMenuButton->m_pMaterials[i] )
                            cJSON_AddStringToObject( menuitem, MenuButton::m_MaterialNames[i], pMenuButton->m_pMaterials[i]->GetMaterialDescription() );
                    }

                    if( pMenuButton->m_pFont )
                        cJSON_AddStringToObject( menuitem, "Font", pMenuButton->m_pFont->GetFile()->GetFullPath() );

                    if( pMenuButton->m_Strings[0][0] != 0 )
                        cJSON_AddStringToObject( menuitem, "String1", pMenuButton->m_Strings[0] );
                    if( pMenuButton->m_Strings[1][0] != 0 )
                        cJSON_AddStringToObject( menuitem, "String2", pMenuButton->m_Strings[1] );
                    if( pMenuButton->m_Strings[2][0] != 0 )
                        cJSON_AddStringToObject( menuitem, "String3", pMenuButton->m_Strings[2] );

                    if( pMenuButton->m_ButtonAction[0] != 0 )
                        cJSON_AddStringToObject( menuitem, "Action", pMenuButton->m_ButtonAction );

                    if( pMenuItem->m_MenuItemType == MIT_InputBox )
                    {
                        cJSON_AddNumberToObject( menuitem, "MaxLength", ((MenuInputBox*)pMenuButton)->m_MaxLength );
                    }
                }

                if( pMenuItem->m_MenuItemType == MIT_ScrollingText )
                {
                    MenuScrollingText* pMenuScrollingText = GetMenuScrollingText( itemarray, i );

                    //if( pMenuScrollingText->m_StringToShow )
                    //    cJSON_AddStringToObject( menuitem, "StringToShow", pMenuScrollingText->m_StringToShow );

                    cJSON_AddNumberToObject( menuitem, "TopFade0", pMenuScrollingText->m_TopFade0 );
                    cJSON_AddNumberToObject( menuitem, "TopFade1", pMenuScrollingText->m_TopFade1 );

                    cJSON_AddNumberToObject( menuitem, "BottomFade1", pMenuScrollingText->m_BottomFade1 );
                    cJSON_AddNumberToObject( menuitem, "BottomFade0", pMenuScrollingText->m_BottomFade0 );
                }
                break;

            case MIT_ScrollBox:
                break;

            case MIT_Base:
            case MIT_CheckBox:
            case MIT_NumMenuItemTypes:
                MyAssert( false ); // need to add support for more, if I ever use them again.
                break;
            }
        }
    }

    return menuitemarray;
}

unsigned int Menu_ImportExport::ImportMenuLayout(GameCore* pGameCore, cJSON* layout, MenuItem** itemarray, unsigned int maxitems, Vector4 ExtentsBLTR)
{
    if( layout == 0 )
        return 0;

    unsigned int numitems = 0;

    cJSON* jMenuItemArray = layout;
    if( jMenuItemArray )
    {
        if( jMenuItemArray )
        {
            numitems = cJSON_GetArraySize( jMenuItemArray );
            MyAssert( numitems <= maxitems );

            for( unsigned int i=0; i<numitems; i++ )
            {
                if( i < maxitems )
                {
                    cJSON* jMenuItem = cJSON_GetArrayItem( jMenuItemArray, i );

                    cJSON* jMIT = cJSON_GetObjectItem( jMenuItem, "MIT" );
            
                    int lettersneeded = -1;
                    cJSONExt_GetInt( jMenuItem, "LettersNeeded", &lettersneeded );

                    MenuItem* pMenuItem = 0;
                    if( jMIT->valueint == MIT_Sprite )        pMenuItem = CreateMenuSprite( pGameCore, itemarray, i );
                    if( jMIT->valueint == MIT_Text )          pMenuItem = CreateMenuText( pGameCore, itemarray, i, lettersneeded );
                    if( jMIT->valueint == MIT_Button )        pMenuItem = CreateMenuButton( pGameCore, itemarray, i, lettersneeded );
                    if( jMIT->valueint == MIT_InputBox )      pMenuItem = CreateMenuInputBox( pGameCore, itemarray, i );
                    if( jMIT->valueint == MIT_ScrollingText ) pMenuItem = CreateMenuScrollingText( pGameCore, itemarray, i );

                    cJSONExt_GetString( jMenuItem, "Name", pMenuItem->m_Name, MenuItem::MAX_MENUITEM_NAME_LENGTH );

                    cJSONExt_GetUnsignedInt( jMenuItem, "Anchor", (unsigned int*)&pMenuItem->m_AnchorPoint );

                    cJSONExt_GetBool( jMenuItem, "Navigable", &pMenuItem->m_Navigable );            

                    float x = 0;
                    float y = 0;
                    float w = 0;
                    float h = 0;
                    float iw = -1;
                    float ih = -1;

                    cJSONExt_GetFloat( jMenuItem, "X", &x );
                    cJSONExt_GetFloat( jMenuItem, "Y", &y );

                    //cJSONExt_GetFloat( jMenuItem, "Scale", &pMenuItem->m_Scale.x );
                    //cJSONExt_GetFloat( jMenuItem, "Scale", &pMenuItem->m_Scale.y );
                    //cJSONExt_GetFloat( jMenuItem, "Scale", &pMenuItem->m_Scale.z );

                    //cJSONExt_GetFloat( jMenuItem, "SX", &pMenuItem->m_Size.x );
                    //cJSONExt_GetFloat( jMenuItem, "SY", &pMenuItem->m_Size.y );

                    cJSONExt_GetFloat( jMenuItem, "W", &w );
                    cJSONExt_GetFloat( jMenuItem, "H", &h );

                    cJSONExt_GetFloat( jMenuItem, "IW", &iw );
                    cJSONExt_GetFloat( jMenuItem, "IH", &ih );

                    //cJSONExt_GetFloat( jMenuItem, "OffX", &pMenuItem->m_PositionOffset.x );
                    //cJSONExt_GetFloat( jMenuItem, "OffY", &pMenuItem->m_PositionOffset.y );

                    Vector2 absolutepos = Vector2(x,y);
                    if( pMenuItem->m_AnchorPoint != Anchor_None )
                        absolutepos = GetAbsolutePositionFromAnchorPoint( pMenuItem->m_AnchorPoint, Vector2(x,y), ExtentsBLTR );

                    pMenuItem->SetPositionAndSize( absolutepos.x, absolutepos.y, w, h, iw, ih );

                    switch( pMenuItem->m_MenuItemType )
                    {
                    case MIT_Sprite:
                        {
                            MenuSprite* pMenuSprite = (MenuSprite*)pMenuItem;

                            cJSONExt_GetFloatArray( jMenuItem, "Shadow", &pMenuSprite->m_DropShadowOffset.x, 2 );

                            for( unsigned int i=0; i<MenuSprite::Materials_NumTypes; i++ )
                            {
                                cJSON* jMaterial = cJSON_GetObjectItem( jMenuItem, MenuSprite::m_MaterialNames[i] );
                                if( jMaterial )
                                {
                                    MaterialDefinition* pMaterial = pGameCore->GetManagers()->GetMaterialManager()->LoadMaterial( jMaterial->valuestring );
                                    if( pMaterial )
                                    {
                                        pMenuSprite->SetMaterial( i, pMaterial );
                                        pMaterial->Release();
                                    }
                                }
                            }
                        }
                        break;

                    case MIT_Text: // MenuText_SaveLoad
                        {
                            MenuText* pMenuText = (MenuText*)pMenuItem;

                            cJSONExt_GetFloat( jMenuItem, "FontHeight", &pMenuText->m_FontHeight );
                            cJSONExt_GetFloat( jMenuItem, "LineHeight", &pMenuText->m_LineHeight );
                            cJSONExt_GetFloat( jMenuItem, "TextShadowX", &pMenuText->m_DropShadowOffsetX );
                            cJSONExt_GetFloat( jMenuItem, "TextShadowY", &pMenuText->m_DropShadowOffsetY );

                            cJSONExt_GetUnsignedChar( jMenuItem, "Justify", &pMenuText->m_Justification );

                            cJSON* jFont = cJSON_GetObjectItem( jMenuItem, "Font" );
                            if( jFont )
                            {
                                FontDefinition* pFont = pGameCore->GetManagers()->GetFontManager()->CreateFont( jFont->valuestring );
                                pMenuText->SetFont( pFont );
                                pFont->Release();
                            }

                            cJSONExt_GetString( jMenuItem, "String", pMenuText->m_String, MenuText::MAX_MenuText_STRING );
                        }
                        break;

                    case MIT_Button:
                    case MIT_InputBox:
                    case MIT_ScrollingText:
                        {
                            MenuButton* pMenuButton = (MenuButton*)pMenuItem;

                            cJSONExt_GetFloatArray( jMenuItem, "TextSize", &pMenuButton->m_TextSize.x, 2 );

                            cJSONExt_GetFloat( jMenuItem, "FontHeight", &pMenuButton->m_FontHeight );
                            cJSONExt_GetFloat( jMenuItem, "LineHeight", &pMenuButton->m_LineHeight );
                            cJSONExt_GetFloatArray( jMenuItem, "BGShadow", &pMenuButton->m_DropShadowOffsetBG.x, 2 );
                            cJSONExt_GetFloatArray( jMenuItem, "TextShadow", &pMenuButton->m_DropShadowOffsetText.x, 2 );

                            cJSONExt_GetUnsignedChar( jMenuItem, "Justify", &pMenuButton->m_Justification );

                            // Legacy
                            cJSONExt_GetFloat( jMenuItem, "BGShadowX", &pMenuButton->m_DropShadowOffsetBG.x );
                            cJSONExt_GetFloat( jMenuItem, "BGShadowY", &pMenuButton->m_DropShadowOffsetBG.y );
                            cJSONExt_GetFloat( jMenuItem, "TextShadowX", &pMenuButton->m_DropShadowOffsetText.x );
                            cJSONExt_GetFloat( jMenuItem, "TextShadowY", &pMenuButton->m_DropShadowOffsetText.y );

                            for( unsigned int i=0; i<MenuButton::Materials_NumTypes; i++ )
                            {
                                cJSON* jMaterial = cJSON_GetObjectItem( jMenuItem, MenuButton::m_MaterialNames[i] );
                                if( jMaterial )
                                {
                                    MaterialDefinition* pMaterial = pGameCore->GetManagers()->GetMaterialManager()->LoadMaterial( jMaterial->valuestring );
                                    if( pMaterial )
                                    {
                                        pMenuButton->SetMaterial( i, pMaterial );
                                        pMaterial->Release();
                                    }
                                }
                            }

                            cJSON* jFont = cJSON_GetObjectItem( jMenuItem, "Font" );
                            if( jFont )
                            {
                                FontDefinition* pFont = pGameCore->GetManagers()->GetFontManager()->CreateFont( jFont->valuestring );
                                pMenuButton->SetFont( pFont );
                                pFont->Release();
                            }

                            cJSONExt_GetString( jMenuItem, "String1", pMenuButton->m_Strings[0], MenuButton::MAX_STRING_LENGTH );
                            cJSONExt_GetString( jMenuItem, "String2", pMenuButton->m_Strings[1], MenuButton::MAX_STRING_LENGTH );
                            cJSONExt_GetString( jMenuItem, "String3", pMenuButton->m_Strings[2], MenuButton::MAX_STRING_LENGTH );

                            cJSONExt_GetString( jMenuItem, "Action", pMenuButton->m_ButtonAction, MenuButton::MAX_BUTTON_ACTION_LENGTH );

                            if( pMenuItem->m_MenuItemType == MIT_InputBox )
                            {
                                cJSONExt_GetInt( jMenuItem, "MaxLength", &((MenuInputBox*)pMenuButton)->m_MaxLength );
                            }
                        }

                        if( pMenuItem->m_MenuItemType == MIT_ScrollingText )
                        {
                            MenuScrollingText* pMenuScrollingText = (MenuScrollingText*)pMenuItem;

                            // TODO: need to allocate memory if this is stored...
                            //cJSONExt_GetString( jMenuItem, "StringToShow", pMenuScrollingText->m_StringToShow, size of buffer );

                            cJSONExt_GetFloat( jMenuItem, "TopFade0", &pMenuScrollingText->m_TopFade0 );
                            cJSONExt_GetFloat( jMenuItem, "TopFade1", &pMenuScrollingText->m_TopFade1 );

                            cJSONExt_GetFloat( jMenuItem, "BottomFade1", &pMenuScrollingText->m_BottomFade1 );
                            cJSONExt_GetFloat( jMenuItem, "BottomFade0", &pMenuScrollingText->m_BottomFade0 );
                        }
                        break;

                    case MIT_Base:
                    case MIT_ScrollBox:
                    case MIT_CheckBox:
                    case MIT_NumMenuItemTypes:
                        break;
                    }
                }
            }

            //cJSON_Delete( jMenuItemArray );
        }
    }

    return numitems;
}
