//
// Copyright (c) 2012-2014 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "GameCommonHeader.h"
#include "Core/ResourceManager.h"
#include "MenuItem.h"

MenuItem::MenuItem()
{
    m_MenuItemType = MIT_Base;

    m_Name[0] = 0;

    m_Visible = true;
    m_Enabled = true;

    m_Closing = false;

    m_Scale.Set( 1, 1, 1 );
    m_Transform.SetIdentity();
    m_Size.Set( 0, 0 );

    m_PositionOffset.Set( 0, 0 );

    m_UseTweenIn = false;
    m_UseTweenOut = false;

    for( int i=0; i<4; i++ )
        m_MenuItemNavigation[i] = 0;
}

MenuItem::~MenuItem()
{
}

void MenuItem::StartClosing()
{
    m_Closing = true;
}

void MenuItem::Tick(double timepassed)
{
    if( m_Closing == false )
    {
        if( m_UseTweenIn )
            m_TweenIn.Tick( timepassed );
    }
    else if( m_Closing && m_UseTweenOut )
        m_TweenOut.Tick( timepassed );
}

void MenuItem::Draw()
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

void MenuItem::SetName(const char* name)
{
    strcpy_s( m_Name, MAX_MENUITEM_NAME_LENGTH, name );
}

void MenuItem::SetPositionOffset(float offx, float offy)
{
    m_PositionOffset.x = offx;
    m_PositionOffset.y = offy;
}

void MenuItem::SetPositionAndSize(float x, float y, float w, float h, float inputw, float inputh)
{
    m_Transform.m41 = x;
    m_Transform.m42 = y;
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

int MenuItem::TriggerOnCollision(int fingerid, float x, float y, bool careifheld, bool releaseifnocollision) // returns the action triggered, -1 is no action.
{
    return -1;
}

#if MYFW_USING_WX
void MenuItem::FillPropertiesWindow()
{
    g_pPanelWatch->ClearAllVariables();
    g_pPanelWatch->AddFloat( "m_PosX", &m_Transform.m41, -1000, 1000 );
    g_pPanelWatch->AddFloat( "m_PosY", &m_Transform.m42, -1000, 1000 );
    g_pPanelWatch->AddFloat( "m_Scale", &m_Scale.x, 0, 5 );
    g_pPanelWatch->AddFloat( "m_SizeX", &m_Size.x, 0, 10 );
    g_pPanelWatch->AddFloat( "m_SizeY", &m_Size.y, 0, 10 );
    g_pPanelWatch->AddFloat( "m_PositionOffsetX", &m_PositionOffset.x, -1000, 1000 );
    g_pPanelWatch->AddFloat( "m_PositionOffsetY", &m_PositionOffset.y, -1000, 1000 );
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
