//
// Copyright (c) 2012-2016 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "SharedCommonHeader.h"
//#include "Core/ResourceManager.h"
#include "MenuItem.h"

const char* MenuItemAnchorPointStrings[Anchor_NumTypes] =
{
    "None",
    "Top Left",
    "Top Right",
    "Bottom Left",
    "Bottom Right",
};

MenuItem::MenuItem(GameCore* pGameCore)
{
    m_pGameCore = pGameCore;

    m_MenuItemType = MIT_Base;

    m_Name[0] = 0;

    m_Visible = true;
    m_Enabled = true;

    m_Closing = false;

    m_AnchorPoint = Anchor_None; // must default to Anchor_None(0) for save/load
    m_Position.Set( 0, 0 );
    //m_Scale.Set( 1, 1, 1 );
    //m_Transform.SetIdentity();
    //m_Size.Set( 0, 0 );

    //m_PositionOffset.Set( 0, 0 );

    m_UseTweenIn = false;
    m_UseTweenOut = false;

    m_Navigable = false; // must default to false for save/load, MenuButton default is true.
    for( int i=0; i<4; i++ )
        m_MenuItemNavigation[i] = 0;

    m_pParentScreen = 0;

#if MYFW_USING_MYENGINE
    m_pMenuPage = 0;
#endif

#if MYFW_USING_WX
    m_MenuItemDeletedCallbackStruct.pFunc = 0;
    m_MenuItemDeletedCallbackStruct.pObj = 0;
#endif
}

MenuItem::~MenuItem()
{
}

MenuItem& MenuItem::operator=(const MenuItem& other)
{
    MyAssert( &other != this );

    this->m_Visible = other.m_Visible;
    this->m_Enabled = other.m_Enabled;

    this->m_Closing = other.m_Closing;

    this->m_AnchorPoint = other.m_AnchorPoint;
    this->m_Position = other.m_Position;

    //bool m_UseTweenIn;      // TODO: save/load
    //MyTweener m_TweenIn;    // TODO: save/load
    //bool m_UseTweenOut;     // TODO: save/load
    //MyTweener m_TweenOut;   // TODO: save/load

    this->m_Navigable = other.m_Navigable;
    for( int i=0; i<4; i++ )
        this->m_MenuItemNavigation[i] = other.m_MenuItemNavigation[i];

    return *this;
}

#if MYFW_USING_LUA
void MenuItem::LuaRegister(lua_State* luastate)
{
    luabridge::getGlobalNamespace( luastate )
        .beginClass<MenuItem>( "MenuItem" )
            //.addData( "localmatrix", &ComponentMenuPage::m_LocalTransform )

            .addFunction( "SetName", &MenuItem::SetName ) // void MenuItem::SetName(const char* name)

            .addFunction( "SetPosition", &MenuItem::SetPosition ) // void MenuItem::SetPosition(float x, float y)
            .addFunction( "SetSize", &MenuItem::SetSize ) // void MenuItem::SetSize(float w, float h, float inputw, float inputh)
            .addFunction( "SetPositionAndSize", &MenuItem::SetPositionAndSize ) // void MenuItem::SetPositionAndSize(float x, float y, float w, float h, float inputw, float inputh)
            .addFunction( "SetAnchorPoint", &MenuItem::SetAnchorPoint ) // void MenuItem::SetAnchorPoint(MenuItemAnchorPoint anchortype)

            .addFunction( "GetSize", &MenuItem::GetSize ) // Vector2 MenuItem::GetSize()

            .addFunction( "SetVisible", &MenuItem::SetVisible ) // void MenuItem::SetVisible(bool visible)
            .addFunction( "SetEnabled", &MenuItem::SetEnabled ) // void MenuItem::SetEnabled(bool enabled)
            .addFunction( "SetVisibleAndEnabled", &MenuItem::SetVisibleAndEnabled ) // void MenuItem::SetVisibleAndEnabled(bool value)

            .addFunction( "GetBoundingRect", &MenuItem::GetBoundingRect ) // MyRect MenuItem::GetBoundingRect()
        .endClass();
}
#endif //MYFW_USING_LUA

void MenuItem::StartClosing()
{
    m_Closing = true;
}

void MenuItem::Tick(float deltaTime)
{
    if( m_Closing == false )
    {
        if( m_UseTweenIn )
            m_TweenIn.Tick( deltaTime );
    }
    else if( m_Closing && m_UseTweenOut )
        m_TweenOut.Tick( deltaTime );
}

void MenuItem::Draw(MyMatrix* pMatProj, MyMatrix* pMatView)
{
}

bool MenuItem::CheckForCollision(float x, float y)
{
    return 0;
}

int MenuItem::CheckForCollisionPosition(float x, float y, bool held)
{
    return 0;
}

// Exposed to Lua, change elsewhere if function signature changes.
void MenuItem::SetName(const char* name)
{
    MyAssert( name );

    unsigned int charstocopy = (unsigned int)strlen( name )+1;
    if( charstocopy > (unsigned int)MAX_MENUITEM_NAME_LENGTH )
        charstocopy = MAX_MENUITEM_NAME_LENGTH-1;

    strncpy_s( m_Name, MAX_MENUITEM_NAME_LENGTH, name, charstocopy );
    m_Name[MAX_MENUITEM_NAME_LENGTH-1] = 0;

#if MYFW_USING_WX
    if( g_pPanelObjectList )
    {
        g_pPanelObjectList->RenameObject( this, m_Name );
    }
#endif //MYFW_USING_WX
}

// Exposed to Lua, change elsewhere if function signature changes.
void MenuItem::SetPosition(float x, float y)
{
    m_Position.Set( x, y );
}

// Exposed to Lua, change elsewhere if function signature changes.
void MenuItem::SetSize(float w, float h, float inputw, float inputh)
{
    SetPositionAndSize( m_Position.x, m_Position.y, w, h, inputw, inputh );
}

// Exposed to Lua, change elsewhere if function signature changes.
void MenuItem::SetPositionAndSize(float x, float y, float w, float h, float inputw, float inputh)
{
    m_Position.Set( x, y );
}

// Exposed to Lua, change elsewhere if function signature changes.
void MenuItem::SetAnchorPoint(MenuItemAnchorPoint anchortype)
{
    m_AnchorPoint = anchortype;
}

float MenuItem::TestCollision(int fingerid, float x, float y, bool fingerwentdown)
{
    return -1;
}

bool MenuItem::HoldOnCollision(int fingerid, float x, float y, bool releaseifnottouching, bool fingerwentdown)
{
    return false;
}

bool MenuItem::ReleaseOnNoCollision(int fingerid, float x, float y)
{
    return false;
}

const char* MenuItem::TriggerOnCollision(int fingerid, float x, float y, bool careifheld, bool releaseifnocollision) // returns the action triggered, -1 is no action.
{
    return 0;
}

#if MYFW_USING_WX
void MenuItem::FillPropertiesWindow()
{
    g_pPanelWatch->ClearAllVariables();

    g_pPanelWatch->AddVector2( "Position", &m_Position, -1000, 1000 );
    g_pPanelWatch->AddEnum( "Anchor", (int*)&m_AnchorPoint, Anchor_NumTypes, MenuItemAnchorPointStrings, this, StaticOnAnchorTypeChanged );

    g_pPanelWatch->AddBool( "Navigable", &m_Navigable, 0, 1 );
}

void MenuItem::OnRightClick()
{
 	wxMenu menu;
    menu.SetClientData( this );

    menu.Append( 1000, "Delete Menu Item" );
 	menu.Connect( wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&MenuItemEventHandler::OnPopupClick );

    // blocking call.
    g_pPanelWatch->PopupMenu( &menu ); // there's no reason this is using g_pPanelWatch other than convenience.
}

void MenuItemEventHandler::OnPopupClick(wxEvent &evt)
{
    MenuItem* pMenuItem = (MenuItem*)static_cast<wxMenu*>(evt.GetEventObject())->GetClientData();

    int id = evt.GetId();
    if( id == 1000 )
    {
        if( pMenuItem->m_MenuItemDeletedCallbackStruct.pFunc )
            pMenuItem->m_MenuItemDeletedCallbackStruct.pFunc( pMenuItem->m_MenuItemDeletedCallbackStruct.pObj, pMenuItem );
    }
}

void MenuItem::RegisterMenuItemDeletedCallback(void* pObj, MenuItemDeletedCallbackFunc pFunc)
{
    m_MenuItemDeletedCallbackStruct.pFunc = pFunc;
    m_MenuItemDeletedCallbackStruct.pObj = pObj;
}

void MenuItem::OnLabelEdit(wxString newlabel)
{
    size_t len = newlabel.length();
    if( len > 0 )
    {
        SetName( newlabel );
    }
}

void MenuItem::OnAnchorTypeChanged(int controlid, bool finishedchanging, double oldvalue)
{
}

void MenuItem::OnDrag()
{
    g_DragAndDropStruct.Add( DragAndDropType_MenuItem, this );
}
#endif //MYFW_USING_WX

void MenuItem::SetMenuItemNavigation(int up, int right, int down, int left)
{
    m_MenuItemNavigation[MenuNavDir_Up] = up;
    m_MenuItemNavigation[MenuNavDir_Right] = right;
    m_MenuItemNavigation[MenuNavDir_Down] = down;
    m_MenuItemNavigation[MenuNavDir_Left] = left;
}

int MenuItem::GetMenuItemNavigation(MenuNavDir dir)
{
    return m_MenuItemNavigation[dir];
}
