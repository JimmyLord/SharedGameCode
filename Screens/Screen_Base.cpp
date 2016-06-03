//
// Copyright (c) 2012-2016 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "GameCommonHeader.h"
#include "Screen_Base.h"
#include "../../../SharedGameCode/Menus/MenuButton.h"
#include "../../../SharedGameCode/Menus/MenuScrollingText.h"
#include "../../../SharedGameCode/Menus/MenuInputBox.h"
#include "../../../SharedGameCode/Menus/MenuCheckBox.h"

Screen_Base::Screen_Base()
{
    m_ScreenName[0] = 0;
    m_ScreenOverlayIndex = -1;

    m_pParentScreen = 0;
    m_ScreenHasBeenInitialized = false;
    m_ScreenIsBeingCached = false;

    m_ScreenToShow = ScreenOverlay_DontChange;
    m_ScreenToShowParentPage = 0;
    m_ScreenToShowParam1 = 0;
    m_ScreenToShowParam2 = 0;

    m_ScreenID = 0;
    m_DestroyCurrentScreen = false;
    m_AlwaysDraw = false;
    m_Transparent = false;
    m_AllowInputToPassThrough = false;

    m_TimeAlive = 0;
    m_State = SS_Opening;
    m_TimeInState = 0;
    m_TimeToOpen = 0.2f;
    m_TimeToClose = 0;

    m_MenuItemsNeeded = 0;

    for( int i=0; i<MAX_MENUITEMS; i++ )
        m_pMenuItems[i] = 0;

    m_pMenuItemHeld = 0;

    m_MenuItemPressedIgnoreGesturesUntilFingerUp = false;

    m_LayoutJSON_Tall = 0;
    m_LayoutJSON_Wide = 0;
    m_LayoutJSON_Square = 0;
}

Screen_Base::~Screen_Base()
{
#if MYFW_USING_WX
    if( g_pPanelObjectList )
    {
        g_pPanelObjectList->RemoveObject( this );

        for( int i=0; i<MAX_MENUITEMS; i++ )
            g_pPanelObjectList->RemoveObject( m_pMenuItems[i] );
    }
#endif //MYFW_USING_WX

    for( int i=0; i<MAX_MENUITEMS; i++ )
        SAFE_DELETE( m_pMenuItems[i] );

    free( m_LayoutJSON_Tall );
    free( m_LayoutJSON_Wide );
    free( m_LayoutJSON_Square );
}

#if MYFW_USING_LUA
void Screen_Base::LuaRegister(lua_State* luastate)
{
    luabridge::getGlobalNamespace( luastate )
        .beginClass<Screen_Base>( "Screen_Base" )
            //.addData( "ComponentTransform", &GameObject::m_pComponentTransform )
            //.addFunction( "StartTutorial", &GameType_InfiniteDung::StartTutorial )
        .endClass();
}
#endif

void Screen_Base::OnClose()
{
}

#if _DEBUG
//char* Screen_Base::ExportMenuLayout()
//{
//    cJSON* menuitemarray = cJSON_CreateArray();
//
//    for( int i=0; i<MAX_MENUITEMS; i++ )
//    {
//        MenuItem* pMenuItem = m_pMenuItems[i];
//
//        if( pMenuItem )
//        {
//            cJSON* menuitem = cJSON_CreateObject();
//            cJSON_AddItemToArray( menuitemarray, menuitem );
//
//            cJSON_AddStringToObject( menuitem, "Name", pMenuItem->m_Name );
//
//            cJSON_AddNumberToObject( menuitem, "X", pMenuItem->m_Position.x );
//            cJSON_AddNumberToObject( menuitem, "Y", pMenuItem->m_Position.y );
//
//            //cJSON_AddNumberToObject( menuitem, "Scale", pMenuItem->m_Scale.x );
//
//            //cJSON_AddNumberToObject( menuitem, "SX", pMenuItem->m_Size.x );
//            //cJSON_AddNumberToObject( menuitem, "SY", pMenuItem->m_Size.y );
//
//            //cJSON_AddNumberToObject( menuitem, "OffX", pMenuItem->m_PositionOffset.x );
//            //cJSON_AddNumberToObject( menuitem, "OffY", pMenuItem->m_PositionOffset.y );
//
//            switch( pMenuItem->m_MenuItemType )
//            {
//            case MIT_Sprite:
//                {
//                    MenuSprite* pMenuSprite = GetMenuSprite( i );
//
//                    cJSON_AddNumberToObject( menuitem, "W", pMenuItem->m_Transform.m11 );
//                    cJSON_AddNumberToObject( menuitem, "H", pMenuItem->m_Transform.m22 );
//
//                    cJSON_AddNumberToObject( menuitem, "BGShadowX", pMenuSprite->m_DropShadowOffset.x );
//                    cJSON_AddNumberToObject( menuitem, "BGShadowY", pMenuSprite->m_DropShadowOffset.y );
//                }
//                break;
//
//            case MIT_Text: // MenuText_SaveLoad
//                {
//                    MenuText* pMenuText = GetMenuText( i );
//
//                    cJSON_AddNumberToObject( menuitem, "FontHeight", pMenuText->m_FontHeight );
//
//                    cJSON_AddNumberToObject( menuitem, "TextShadowX", pMenuText->m_DropShadowOffsetX );
//                    cJSON_AddNumberToObject( menuitem, "TextShadowY", pMenuText->m_DropShadowOffsetY );
//
//                    cJSON_AddNumberToObject( menuitem, "Justify", pMenuText->m_Justification );
//                }
//                break;
//
//            case MIT_Button:
//                {
//                    MenuButton* pMenuButton = GetMenuButton( i );
//
//                    cJSON_AddNumberToObject( menuitem, "W", pMenuItem->m_Transform.m11 );
//                    cJSON_AddNumberToObject( menuitem, "H", pMenuItem->m_Transform.m22 );
//
//                    cJSON_AddNumberToObject( menuitem, "IW", pMenuButton->m_InputWidth );
//                    cJSON_AddNumberToObject( menuitem, "IH", pMenuButton->m_InputHeight );
//
//                    cJSON_AddNumberToObject( menuitem, "FontHeight", pMenuButton->m_FontHeight );
//
//                    cJSON_AddNumberToObject( menuitem, "BGShadowX", pMenuButton->m_DropShadowOffsetBG.x );
//                    cJSON_AddNumberToObject( menuitem, "BGShadowY", pMenuButton->m_DropShadowOffsetBG.y );
//
//                    cJSON_AddNumberToObject( menuitem, "TextShadowX", pMenuButton->m_DropShadowOffsetText.x );
//                    cJSON_AddNumberToObject( menuitem, "TextShadowY", pMenuButton->m_DropShadowOffsetText.y );
//                }
//                break;
//
//            case MIT_Base:
//            case MIT_InputBox:
//            case MIT_ScrollingText:
//            case MIT_ScrollBox:
//            case MIT_CheckBox:
//            case MIT_NumMenuItemTypes:
//                break;
//            }
//        }
//    }
//
//    char* savestring = cJSON_PrintUnformatted( menuitemarray );
//    cJSON_Delete( menuitemarray );
//
//    char* RidiculousReallocatedStringSoICanUseMyNewDeleteOverridesWithoutChangingThecJSONCodeToUseThemForAllItsSmallAllocations;
//    int len = (int)strlen( savestring );
//    RidiculousReallocatedStringSoICanUseMyNewDeleteOverridesWithoutChangingThecJSONCodeToUseThemForAllItsSmallAllocations = MyNew char[len+1];
//    strcpy_s( RidiculousReallocatedStringSoICanUseMyNewDeleteOverridesWithoutChangingThecJSONCodeToUseThemForAllItsSmallAllocations, len+1, savestring );
//    cJSONExt_free( savestring );
//
//    return RidiculousReallocatedStringSoICanUseMyNewDeleteOverridesWithoutChangingThecJSONCodeToUseThemForAllItsSmallAllocations;
//}
#endif

//void Screen_Base::ImportMenuLayout(const char* layout)
//{
//    if( layout == 0 )
//        return;
//
//    cJSON* menuitemarray = cJSON_Parse( layout );
//    MyAssert( menuitemarray );
//
//    if( menuitemarray )
//    {
//        int numitems = cJSON_GetArraySize( menuitemarray );
//        MyAssert( numitems <= MAX_MENUITEMS );
//
//        for( int i=0; i<numitems; i++ )
//        {
//            cJSON* menuitem = cJSON_GetArrayItem( menuitemarray, i );
//
//            cJSON* objname = cJSON_GetObjectItem( menuitem, "Name" );
//
//            int itemindex = -1;
//            for( itemindex = 0; itemindex < MAX_MENUITEMS; itemindex++ )
//            {
//                if( strcmp( objname->valuestring, m_pMenuItems[itemindex]->m_Name ) == 0 )
//                    break;
//            }
//
//            if( itemindex < MAX_MENUITEMS )
//            {
//                float x = 0;
//                float y = 0;
//                float w = 0;
//                float h = 0;
//                float iw = -1;
//                float ih = -1;
//
//                MenuItem* pMenuItem = m_pMenuItems[itemindex];
//
//                cJSONExt_GetFloat( menuitem, "X", &x );
//                cJSONExt_GetFloat( menuitem, "Y", &y );
//
//                cJSONExt_GetFloat( menuitem, "Scale", &pMenuItem->m_Scale.x );
//                cJSONExt_GetFloat( menuitem, "Scale", &pMenuItem->m_Scale.y );
//                cJSONExt_GetFloat( menuitem, "Scale", &pMenuItem->m_Scale.z );
//
//                cJSONExt_GetFloat( menuitem, "SX", &pMenuItem->m_Size.x );
//                cJSONExt_GetFloat( menuitem, "SY", &pMenuItem->m_Size.y );
//
//                cJSONExt_GetFloat( menuitem, "W", &w );
//                cJSONExt_GetFloat( menuitem, "H", &h );
//
//                cJSONExt_GetFloat( menuitem, "IW", &iw );
//                cJSONExt_GetFloat( menuitem, "IH", &ih );
//
//                cJSONExt_GetFloat( menuitem, "OffX", &pMenuItem->m_PositionOffset.x );
//                cJSONExt_GetFloat( menuitem, "OffY", &pMenuItem->m_PositionOffset.y );
//
//                pMenuItem->SetPositionAndSize( x, y, w, h, iw, ih );
//
//                switch( pMenuItem->m_MenuItemType )
//                {
//                case MIT_Sprite:
//                    {
//                        MenuSprite* pMenuSprite = GetMenuSprite( itemindex );
//
//                        cJSONExt_GetFloat( menuitem, "BGShadowX", &pMenuSprite->m_DropShadowOffset.x );
//                        cJSONExt_GetFloat( menuitem, "BGShadowY", &pMenuSprite->m_DropShadowOffset.y );
//                    }
//                    break;
//
//                case MIT_Text: // MenuText_SaveLoad
//                    {
//                        MenuText* pMenuText = GetMenuText( itemindex );
//
//                        cJSONExt_GetFloat( menuitem, "FontHeight", &pMenuText->m_FontHeight );
//                        cJSONExt_GetFloat( menuitem, "TextShadowX", &pMenuText->m_DropShadowOffsetX );
//                        cJSONExt_GetFloat( menuitem, "TextShadowY", &pMenuText->m_DropShadowOffsetY );
//
//                        cJSONExt_GetUnsignedChar( menuitem, "Justify", &pMenuText->m_Justification );
//                    }
//                    break;
//
//                case MIT_Button:
//                    {
//                        MenuButton* pMenuButton = GetMenuButton( itemindex );
//
//                        cJSONExt_GetFloat( menuitem, "FontHeight", &pMenuButton->m_FontHeight );
//                        cJSONExt_GetFloat( menuitem, "BGShadowX", &pMenuButton->m_DropShadowOffsetBG.x );
//                        cJSONExt_GetFloat( menuitem, "BGShadowY", &pMenuButton->m_DropShadowOffsetBG.y );
//                        cJSONExt_GetFloat( menuitem, "TextShadowX", &pMenuButton->m_DropShadowOffsetText.x );
//                        cJSONExt_GetFloat( menuitem, "TextShadowY", &pMenuButton->m_DropShadowOffsetText.y );
//                    }
//                    break;
//
//                case MIT_Base:
//                case MIT_InputBox:
//                case MIT_ScrollingText:
//                case MIT_ScrollBox:
//                case MIT_CheckBox:
//                case MIT_NumMenuItemTypes:
//                    break;
//                }
//            }
//        }
//
//        cJSON_Delete( menuitemarray );
//    }
//}

//void FillProps(void* pObject)
//{
//    Screen_Base* pScreen = (Screen_Base*)pObject;
//
//    g_pPanelWatch->AddFloat( "TimeAlive", &pScreen->m_TimeAlive, 0, pScreen->m_TimeToOpen );
//};

const char* Screen_Base::GetMenuItemLabel(MenuItem* pMenuItem)
{
    static char label[100];

    MenuItemTypes type = pMenuItem->m_MenuItemType;

    switch( type )
    {
    case MIT_Base:          sprintf_s( label, 100, "MItem" );                                               break;
    case MIT_Sprite:        sprintf_s( label, 100, "MSprite" );                                             break;
    case MIT_Text:          sprintf_s( label, 100, "MText(%s)", ((MenuText*)pMenuItem)->m_String );         break;
    case MIT_Button:        sprintf_s( label, 100, "MButton(%s)", ((MenuButton*)pMenuItem)->m_Strings[0] ); break;
    case MIT_ScrollBox:     sprintf_s( label, 100, "MScrollBox" );                                          break;
    case MIT_ScrollingText: sprintf_s( label, 100, "MScrollingText" );                                      break;
    case MIT_InputBox:      sprintf_s( label, 100, "MInputBox" );                                           break;
    case MIT_CheckBox:      sprintf_s( label, 100, "MCheckBox" );                                           break;
    case MIT_NumMenuItemTypes: break;
    }

    return label;
}

void Screen_Base::CreateMenuItems(int numitems, MenuItemDefinition* menuitems)
{
    for( int i=0; i<numitems; i++ )
    {
        switch( menuitems[i].type )
        {
        case MIT_Sprite:
            {
                MenuSprite* pMenuSprite = CreateMenuSprite( i );

                pMenuSprite->SetName( menuitems[i].name );

                MySprite* pSprite = g_pGame->m_pResources->m_pSprites[menuitems[i].spriteindex];
                pMenuSprite->SetMaterial( MenuSprite::Material_Sprite, menuitems[i].pMaterials[MenuSprite::Material_Sprite] );
                pMenuSprite->SetMaterial( MenuSprite::Material_Shadow, menuitems[i].pMaterials[MenuSprite::Material_Shadow] );
                //pMenuSprite->SetSprites( pSprite, pSprite );

                //pMenuSprite->m_BGColor = menuitems[i].BGColor;
            }
            break;

        case MIT_Text:
            {
                MenuText* pText = CreateMenuText( i, menuitems[i].maxletters );

                pText->SetName( menuitems[i].name );

                pText->SetFont( menuitems[i].font );
                if( menuitems[i].string1 )
                    pText->SetString( menuitems[i].string1 );
            }
            break;

        case MIT_Button:
            {
                MenuButton* pButton = CreateMenuButton( i, menuitems[i].maxletters );

                pButton->SetName( menuitems[i].name );

                MySprite* pSprite = g_pGame->m_pResources->m_pSprites[menuitems[i].spriteindex];
                pButton->SetMaterial( MenuButton::Material_BG,          menuitems[i].pMaterials[MenuButton::Material_BG] );
                pButton->SetMaterial( MenuButton::Material_BGDisabled,  menuitems[i].pMaterials[MenuButton::Material_BGDisabled] );
                pButton->SetMaterial( MenuButton::Material_BGPressed,   menuitems[i].pMaterials[MenuButton::Material_BGPressed] );
                pButton->SetMaterial( MenuButton::Material_BGOverlay,   menuitems[i].pMaterials[MenuButton::Material_BGOverlay] );
                pButton->SetMaterial( MenuButton::Material_Shadow,      menuitems[i].pMaterials[MenuButton::Material_Shadow] );
                //pButton->SetSprites( pSprite, pSprite, pSprite, 0, pSprite );

                //pButton->m_BGColor = menuitems[i].BGColor;

                pButton->SetFont( menuitems[i].font );
                if( menuitems[i].string1 || menuitems[i].string2 || menuitems[i].string3 )
                    pButton->SetString( menuitems[i].string1, menuitems[i].string2, menuitems[i].string3 );

                pButton->m_ButtonAction[0] = (char)menuitems[i].buttonaction;
            }
            break;

        case MIT_Base:
        case MIT_InputBox:
        case MIT_ScrollingText:
        case MIT_ScrollBox:
        case MIT_CheckBox:
        case MIT_NumMenuItemTypes:
            break;
        }
    }
}

void Screen_Base::Init()
{
    m_ScreenHasBeenInitialized = true;

#if MYFW_USING_WX
    // Add this screen to the root of the tree.
    wxTreeItemId rootid = g_pPanelObjectList->GetTreeRoot();
    wxTreeItemId screenid = g_pPanelObjectList->AddObject( this, Screen_Base::StaticFillPropertiesWindow, 0, rootid, m_ScreenName );

    // Add all of it's menu items to the screen object.
    for( int i=0; i<MAX_MENUITEMS; i++ )
    {
        if( m_pMenuItems[i] )
        {
            const char* label = GetMenuItemLabel( m_pMenuItems[i] );
            wxTreeItemId id;
            switch( m_pMenuItems[i]->m_MenuItemType )
            {
            case MIT_Sprite:        id = g_pPanelObjectList->AddObject( m_pMenuItems[i], MenuSprite::StaticFillPropertiesWindow,        0, screenid, label ); break;
            case MIT_Text:          id = g_pPanelObjectList->AddObject( m_pMenuItems[i], MenuText::StaticFillPropertiesWindow,          0, screenid, label ); break;
            case MIT_Button:        id = g_pPanelObjectList->AddObject( m_pMenuItems[i], MenuButton::StaticFillPropertiesWindow,        0, screenid, label ); break;
            case MIT_ScrollBox:     id = g_pPanelObjectList->AddObject( m_pMenuItems[i], MenuScrollBox::StaticFillPropertiesWindow,     0, screenid, label ); break;
            case MIT_ScrollingText: id = g_pPanelObjectList->AddObject( m_pMenuItems[i], MenuScrollingText::StaticFillPropertiesWindow, 0, screenid, label ); break;
            case MIT_InputBox:      id = g_pPanelObjectList->AddObject( m_pMenuItems[i], MenuInputBox::StaticFillPropertiesWindow,      0, screenid, label ); break;
            case MIT_CheckBox:      id = g_pPanelObjectList->AddObject( m_pMenuItems[i], MenuCheckBox::StaticFillPropertiesWindow,      0, screenid, label ); break;

            case MIT_Base:
            case MIT_NumMenuItemTypes:
            default:                id = g_pPanelObjectList->AddObject( m_pMenuItems[i], MenuItem::StaticFillPropertiesWindow,          0, screenid, label ); break;
            }

            // if this is a scrollbox containing more mene items, then add those to that tree limb.
            if( m_pMenuItems[i]->m_MenuItemType == MIT_ScrollBox )
            {
                MenuScrollBox* pScrollBox = (MenuScrollBox*)m_pMenuItems[i];

                for( int j=0; j<pScrollBox->m_NumMenuItems; j++ )
                {
                    MenuItem* pItem = pScrollBox->GetMenuItem(j);
                    const char* label = GetMenuItemLabel( pItem );
                    switch( m_pMenuItems[i]->m_MenuItemType )
                    {
                    case MIT_Sprite:        g_pPanelObjectList->AddObject( pItem, MenuSprite::StaticFillPropertiesWindow,        0, id, label ); break;
                    case MIT_Text:          g_pPanelObjectList->AddObject( pItem, MenuText::StaticFillPropertiesWindow,          0, id, label ); break;
                    case MIT_Button:        g_pPanelObjectList->AddObject( pItem, MenuButton::StaticFillPropertiesWindow,        0, id, label ); break;
                    case MIT_ScrollBox:     g_pPanelObjectList->AddObject( pItem, MenuScrollBox::StaticFillPropertiesWindow,     0, id, label ); break;
                    case MIT_ScrollingText: g_pPanelObjectList->AddObject( pItem, MenuScrollingText::StaticFillPropertiesWindow, 0, id, label ); break;
                    case MIT_InputBox:      g_pPanelObjectList->AddObject( pItem, MenuInputBox::StaticFillPropertiesWindow,      0, id, label ); break;
                    case MIT_CheckBox:      g_pPanelObjectList->AddObject( pItem, MenuCheckBox::StaticFillPropertiesWindow,      0, id, label ); break;

                    case MIT_Base:
                    case MIT_NumMenuItemTypes:
                    default:                g_pPanelObjectList->AddObject( pItem, MenuItem::StaticFillPropertiesWindow,          0, id, label ); break;
                    }
                }
            }
        }
    }
#endif //MYFW_USING_WX
}

void Screen_Base::OnDisplay()
{
    UpdateMenuItems( false );
}

void Screen_Base::OnResized(int x, int y, int w, int h)
{
    UpdateMenuItems( false );
}

void Screen_Base::UpdateMenuItems(bool initialsetup)
{
    //float scrw = g_pGame->m_DeviceWidth;
    //float scrh = g_pGame->m_DeviceHeight;

    //if( m_ScreenOverlayIndex != -1 )
    //{
    //    if( scrw > scrh )
    //        ImportMenuLayout( g_pScreenOverlayManager->m_LayoutJSONStrings[m_ScreenOverlayIndex][LayoutType_Wide] );
    //    if( scrw < scrh )
    //        ImportMenuLayout( g_pScreenOverlayManager->m_LayoutJSONStrings[m_ScreenOverlayIndex][LayoutType_Tall] );
    //    if( scrw == scrh )
    //        ImportMenuLayout( g_pScreenOverlayManager->m_LayoutJSONStrings[m_ScreenOverlayIndex][LayoutType_Square] );
    //}
}

float Screen_Base::QueryPercentageStateComplete()
{
    if( m_State == SS_Opening )
    {
        return (float)(m_TimeAlive / m_TimeToOpen);
    }

    if( m_State == SS_Closing )
    {
        return 1;
    }

    return 1;
}

void Screen_Base::CloseScreen()
{
    m_State = SS_Closing;
    m_TimeInState = 0;

    for( int i=0; i<m_MenuItemsNeeded; i++ )
    {
        if( m_pMenuItems[i] )
        {
            m_pMenuItems[i]->StartClosing();
        }
    }
}

bool Screen_Base::IsSettled()
{
    return false;
}

void Screen_Base::Tick(double TimePassed)
{
    m_TimeAlive += TimePassed;
    m_TimeInState += TimePassed;

    if( m_State == SS_Closing )
    {
        if( m_TimeInState > m_TimeToClose )
        {
            m_State = SS_DoneClosing;
            return;
        }
    }
    else
    {
        if( m_TimeAlive > m_TimeToOpen )
            m_State = SS_Running;
    }

    for( int i=0; i<m_MenuItemsNeeded; i++ )
    {
        if( m_pMenuItems[i] )
        {
            m_pMenuItems[i]->Tick( TimePassed );
        }
    }

    SwitchScreenOverlay();
}

void Screen_Base::Draw()
{
}

bool Screen_Base::OnTouch(int action, int id, float x, float y, float pressure, float size)
{
    if( m_State != SS_Running )
        return true;

    return false;
}

void Screen_Base::BasicMenuHandleHeldFunc(int fingerindex, float x, float y)
{
    if( fingerindex == -1 )
        return;

    for( int i=m_MenuItemsNeeded-1; i>=0; i-- )
    {
        if( GetMenuItem(i) )
        {
            GetMenuItem(i)->ReleaseOnNoCollision( fingerindex, x, y );
        }
    }
}

InputFinger* Screen_Base::GetFingerInfo(int id)
{
    for( int i=0; i<10; i++ )
    {
        if( g_Fingers[i].id == id )
            return &g_Fingers[i];
    }

    return 0;
}

bool Screen_Base::BasicMenuTouchFunc(int action, int id, float x, float y, float pressure, float size)
{
    // flip y to make it start at bottom left.
    y = g_pGame->m_GameHeight - y;

    switch( action )
    {
    case GCBA_Down: // new finger down
        {
            g_Fingers[id].set( x, y, id );

            for( int i=m_MenuItemsNeeded-1; i>=0; i-- )
            {
                if( GetMenuItem(i) )
                {
                    if( GetMenuItem(i)->HoldOnCollision( id, x, y, true ) )
                        return true;
                }
            }
        }
        break;

    case GCBA_Held: // any finger might have moved
        {
            int fingerindex = -1;

            for( int i=0; i<10; i++ )
            {
                if( g_Fingers[i].id == id )
                    fingerindex = i;
            }

            if( fingerindex != -1 )
            {
                g_Fingers[fingerindex].set( x, y, id );

                BasicMenuHandleHeldFunc( fingerindex, x, y );
            }
        }
        break;

    case GCBA_Up: // any finger up
        {
            g_Fingers[id].reset();

            for( int i=m_MenuItemsNeeded-1; i>=0; i-- )
            {
                if( GetMenuItem(i) )
                {
                    const char* actionstring = GetMenuItem(i)->TriggerOnCollision( id, x, y, true );
                    if( actionstring )
                    {
                        int action = actionstring[0];

                        if( action != -1 && OnMenuAction( action ) )
                            return true;
                    }
                }
            }
        }
        break;
    }

    return false;
}

#if MYFW_USING_WX
void Screen_Base::FillPropertiesWindow()
{
    g_pPanelWatch->ClearAllVariables();
    g_pPanelWatch->AddDouble( "m_TimeAlive", &m_TimeAlive, 0, (float)m_TimeToOpen );
    g_pPanelWatch->AddDouble( "m_TimeInState", &m_TimeInState, 0, 100000 );
    g_pPanelWatch->AddDouble( "m_TimeToOpen", &m_TimeToOpen, 0, 100000 );
    g_pPanelWatch->AddDouble( "m_TimeToClose", &m_TimeToClose, 0, 100000 );
}
#endif //MYFW_USING_WX

int Screen_Base::GetMenuButtonHeldCount()
{
    int count = 0;

    for( int i=0; i<m_MenuItemsNeeded; i++ )
    {
        if( GetMenuItem(i) && GetMenuItemType(i) == MIT_Button )
        {
            if( GetMenuButton(i)->GetState() == MBS_HeldDown )
                count++;
        }
    }

    return count;
}

bool Screen_Base::OnButtons(GameCoreButtonActions action, GameCoreButtonIDs id)
{
    if( m_State != SS_Running )
        return true;

    return false;
}

bool Screen_Base::OnKeys(GameCoreButtonActions action, int keycode, int unicodechar)
{
#if MYFW_WINDOWS && MYFW_USING_WX && _DEBUG
    //if( keycode == 'R' && g_pGameCore->m_KeysHeld[MYKEYCODE_LCTRL] )
    //{
    //    float scrw = g_pGame->m_DeviceWidth;
    //    float scrh = g_pGame->m_DeviceHeight;

    //    if( scrw > scrh )
    //        ImportMenuLayout( m_LayoutJSON_Wide );
    //    if( scrw < scrh )
    //        ImportMenuLayout( m_LayoutJSON_Tall );
    //    if( scrw == scrh )
    //        ImportMenuLayout( m_LayoutJSON_Square );
    //}
    //if( keycode == 'S' && g_pGameCore->m_KeysHeld[MYKEYCODE_LCTRL] )
    //{
    //    float scrw = g_pGame->m_DeviceWidth;
    //    float scrh = g_pGame->m_DeviceHeight;

    //    if( m_ScreenOverlayIndex != -1 )
    //    {
    //        if( scrw > scrh )
    //        {
    //            SAFE_DELETE_ARRAY( g_pScreenOverlayManager->m_LayoutJSONStrings[m_ScreenOverlayIndex][LayoutType_Wide] );
    //            g_pScreenOverlayManager->m_LayoutJSONStrings[m_ScreenOverlayIndex][LayoutType_Wide] = ExportMenuLayout();
    //        }
    //        if( scrw < scrh )
    //        {
    //            SAFE_DELETE_ARRAY( g_pScreenOverlayManager->m_LayoutJSONStrings[m_ScreenOverlayIndex][LayoutType_Tall] );
    //            g_pScreenOverlayManager->m_LayoutJSONStrings[m_ScreenOverlayIndex][LayoutType_Tall] = ExportMenuLayout();
    //        }
    //        if( scrw == scrh )
    //        {
    //            SAFE_DELETE_ARRAY( g_pScreenOverlayManager->m_LayoutJSONStrings[m_ScreenOverlayIndex][LayoutType_Square] );
    //            g_pScreenOverlayManager->m_LayoutJSONStrings[m_ScreenOverlayIndex][LayoutType_Square] = ExportMenuLayout();
    //        }
    //    }

    //    g_pScreenOverlayManager->SaveLayouts();
    //}
#endif
    return false;
}

void Screen_Base::SetScreenOverlayToShow(ScreenOverlays screen, Screen_Base* pParentScreen, void* pPtr1, void* pPtr2)
{
    m_ScreenToShow = screen;
    m_ScreenToShowParentPage = pParentScreen;
    m_ScreenToShowParam1 = pPtr1;
    m_ScreenToShowParam2 = pPtr2;
}

void Screen_Base::ReplaceCurrentScreenOverlayWith(ScreenOverlays screen, Screen_Base* pParent, void* pPtr, void* pPtr2)
{
    m_DestroyCurrentScreen = true;
    SetScreenOverlayToShow( screen, pParent, pPtr, pPtr2 );
}

void Screen_Base::SwitchScreenOverlay()
{
    if( m_ScreenToShow != ScreenOverlay_DontChange )
    {
        Screen_Base* newscreen = 0;

#pragma warning(push)
#pragma warning(disable:4061) // enumerator in switch statement is not explicitly handled by a case label

        switch( m_ScreenToShow )
        {
        case ScreenOverlay_DontChange:
        case ScreenOverlay_Destroy:
            g_pScreenManager->CloseScreen( this );
            break;

        case ScreenOverlay_RemoveDontDestroy:
            g_pScreenManager->RemoveScreen( this );
            break;

        default:
            if( m_DestroyCurrentScreen )
                g_pScreenManager->CloseScreen( this );
            newscreen = g_pScreenOverlayManager->CreateScreen( m_ScreenToShow, m_ScreenToShowParentPage, m_ScreenToShowParam1, m_ScreenToShowParam2 );
            break;
        }

#pragma warning(pop)

        if( newscreen )
        {
            g_pScreenManager->PushScreen( newscreen );
            if( newscreen->m_ScreenHasBeenInitialized == false )
            {
                newscreen->Init();
                newscreen->m_ScreenHasBeenInitialized = true;
            }

            newscreen->OnDisplay();
        }
    }

    m_ScreenToShow = ScreenOverlay_DontChange;
    m_ScreenToShowParentPage = 0;
    m_ScreenToShowParam1 = 0;
    m_ScreenToShowParam2 = 0;
}

void Screen_Base::DrawAllMenuItems(MyMatrix* matviewproj)
{
    for( int i=0; i<m_MenuItemsNeeded; i++ )
    {
        if( m_pMenuItems[i] )
        {
            m_pMenuItems[i]->Draw( matviewproj );
        }
    }
}

void Screen_Base::DrawMenuItem(int index)
{
    m_pMenuItems[index]->Draw( 0 );
}

MenuItem* Screen_Base::GetMenuItem(int index)
{
    return m_pMenuItems[index];
}

MenuItemTypes Screen_Base::GetMenuItemType(int index)
{
    MyAssert( m_pMenuItems[index] != 0 );
    if( m_pMenuItems[index] == 0 )
        return MIT_NumMenuItemTypes;

    return m_pMenuItems[index]->m_MenuItemType;
}

#define GETMENUTYPE(index, type, typeclass) \
    MyAssert( m_pMenuItems[index]->m_MenuItemType == type ); \
    if( m_pMenuItems[index]->m_MenuItemType != type ) return 0; \
    return (typeclass*)m_pMenuItems[index];

MenuSprite* Screen_Base::GetMenuSprite(int index)               { GETMENUTYPE( index, MIT_Sprite, MenuSprite ); }
MenuText* Screen_Base::GetMenuText(int index)                   { GETMENUTYPE( index, MIT_Text, MenuText ); }
MenuButton* Screen_Base::GetMenuButton(int index)               { GETMENUTYPE( index, MIT_Button, MenuButton ); }
MenuScrollBox* Screen_Base::GetMenuScrollBox(int index)         { GETMENUTYPE( index, MIT_ScrollBox, MenuScrollBox ); }
MenuScrollingText* Screen_Base::GetMenuScrollingText(int index) { GETMENUTYPE( index, MIT_ScrollingText, MenuScrollingText ); }
MenuInputBox* Screen_Base::GetMenuInputBox(int index)           { GETMENUTYPE( index, MIT_InputBox, MenuInputBox ); }
MenuCheckBox* Screen_Base::GetMenuCheckBox(int index)           { GETMENUTYPE( index, MIT_CheckBox, MenuCheckBox ); }

#define CREATEMENUTYPE(index, typeclass) \
    MyAssert( m_pMenuItems[index] == 0 ); \
    m_pMenuItems[index] = MyNew typeclass; \
    return (typeclass*)m_pMenuItems[index];

#define CREATEMENUTYPEINT(index, typeclass, value) \
    MyAssert( m_pMenuItems[index] == 0 ); \
    m_pMenuItems[index] = MyNew typeclass(value); \
    return (typeclass*)m_pMenuItems[index];

MenuSprite* Screen_Base::CreateMenuSprite(int index)                    { CREATEMENUTYPE( index, MenuSprite ); }
MenuText* Screen_Base::CreateMenuText(int index, int maxletters)        { CREATEMENUTYPEINT( index, MenuText, maxletters ); }
MenuButton* Screen_Base::CreateMenuButton(int index, int maxletters)    { CREATEMENUTYPEINT( index, MenuButton, maxletters ); }
MenuScrollBox* Screen_Base::CreateMenuScrollBox(int index)              { CREATEMENUTYPE( index, MenuScrollBox ); }
MenuScrollingText* Screen_Base::CreateMenuScrollingText(int index)      { CREATEMENUTYPE( index, MenuScrollingText ); }
MenuInputBox* Screen_Base::CreateMenuInputBox(int index)                { CREATEMENUTYPE( index, MenuInputBox ); }
MenuCheckBox* Screen_Base::CreateMenuCheckBox(int index)                { CREATEMENUTYPE( index, MenuCheckBox ); }
