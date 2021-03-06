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
//#include "../Screens/Screen_Base.h"
#include "../Core/RenderTextQuick.h"
#include "../Core/MyMeshText.h"
#include "../Screens/ScreenManager.h"
#include "../Menus/LanguageTable.h"
//#include "Core/InputFinger.h"
#include "MenuItem.h"
#include "MenuScrollBox.h"
#include "MenuScrollingText.h"
#include "MenuSprite.h"
#include "MenuText.h"
#include "MenuInputBox.h"

MenuScrollBox::MenuScrollBox(GameCore* pGameCore)
: MenuButton( pGameCore, 0 )
{
    m_MenuItemType = MIT_ScrollBox;

    m_pParentMenuPage = 0;

    m_pMenuItems = 0;
    m_NumMenuItems = 0;

    m_ScrollAmount.Set( 0, 0 );
    m_Momentum.Set( 0, 0 );

    m_MinScrollAmount.Set( 0, 0 );
    m_MaxScrollAmount.Set( 400, 400 );

    m_ScreenTop = 640;
    m_ScreenBottom = 0;

    m_InitialFinger = -1;
}

MenuScrollBox::~MenuScrollBox()
{
    for( int i=0; i<m_NumMenuItems; i++ )
    {
        SAFE_DELETE( m_pMenuItems[i] );
    }

    SAFE_DELETE_ARRAY( m_pMenuItems );
}

void MenuScrollBox::Tick(float deltaTime)
{
    MenuButton::Tick( deltaTime );

    if( m_InitialFinger == -1 && m_Momentum.LengthSquared() != 0 )
    {
        m_Momentum *= 0.99f;

        m_ScrollAmount.x -= m_Momentum.x * deltaTime * 30;
        m_ScrollAmount.y += m_Momentum.y * deltaTime * 30;
    }

    MyClamp( m_ScrollAmount.x, m_MinScrollAmount.x, m_MaxScrollAmount.x );
    MyClamp( m_ScrollAmount.y, m_MinScrollAmount.y, m_MaxScrollAmount.y );
}

void MenuScrollBox::Draw(MyMatrix* pMatProj, MyMatrix* pMatView)
{
    //MenuButton::Draw();
    if( m_Visible == false )
        return;

    // draw the menu items.
    int i=0;
    for( i=0; i<m_NumMenuItems; i++ )
    {
        if( m_pMenuItems[i] )
        {
            //float offx = m_Position.x - m_ScrollAmount.x;
            //float offy = m_Position.y + m_ScrollAmount.y;
            //m_pMenuItems[i]->SetPositionOffset( offx, offy );

            MyRect bbox = m_pMenuItems[i]->GetBoundingRect();

            //if( bbox.y + bbox.h < 0 || bbox.y > g_pGame->m_GameHeight ) // use for debugging...

            //m_ScreenBottom = g_pGame->m_OrthoBottom;
            //m_ScreenTop = g_pGame->m_OrthoTop;

            if( bbox.y + bbox.h < m_ScreenBottom || bbox.y > m_ScreenTop )
                continue;

            m_pMenuItems[i]->Draw( pMatProj, pMatView );
        }
    }
}

bool MenuScrollBox::CheckForCollision(float x, float y)
{
    return false;
    //return MenuButton::CheckForCollision( x, y );
}

bool MenuScrollBox::HoldOnCollision(int fingerid, float x, float y, bool releaseifnottouching, bool fingerwentdown)
{
    return false;
    //return MenuButton::HoldOnCollision( fingerid, x, y, releaseifnottouching, fingerwentdown );
}

bool MenuScrollBox::ReleaseOnNoCollision(int fingerid, float x, float y)
{
    return false;
    //return MenuButton::ReleaseOnNoCollision( fingerid, x, y );
}

const char* MenuScrollBox::TriggerOnCollision(int fingerid, float x, float y, bool careifheld, bool releaseifnocollision) // returns the action triggered, -1 is no action.
{
    return 0;
    //return MenuButton::TriggerOnCollision( fingerid, x, y, careifheld, releaseifnocollision );
}

int MenuScrollBox::OnTouch(int action, int id, float x, float y, float pressure, float size)
{
#if 0 // disabled to make NaCl build work
    //if( m_FingerHolding != fingerid )
    //    return false;

    //if( m_State != MBS_Idle )
    //    return false;

    // flip y to make it start at bottom left.
    //y = g_pGame->m_GameHeight - y;

    bool newfingerdown = false;
    bool newfingerup = false;

    if( action == GCBA_Down ) // first finger down
    {
        g_Fingers[id].set( x, y, id );
        newfingerdown = true;

        m_InitialFinger = id;
        m_Momentum.Set( 0, 0 );

        //bool found = false;
        for( int i=m_NumMenuItems-1; i>=0; i-- )
        {
            if( m_pMenuItems[i] )
            {
                if( m_pMenuItems[i]->HoldOnCollision( id, x, y, true ) )
                {
                    //found = true;
                    //break;
                }
            }
        }

        return true;
    }

    if( action == GCBA_Up ) // last finger up... i.e. no fingers still down.
    {
        for( int i=0; i<10; i++ )
        {
            g_Fingers[i].reset();
        }
        newfingerup = true;

        m_InitialFinger = -1;
    }

    if( action == GCBA_Held ) // any finger might have moved
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

            int diffx = (int)(g_Fingers[fingerindex].currx) - (int)(g_Fingers[fingerindex].lastx);
            int diffy = (int)(g_Fingers[fingerindex].curry) - (int)(g_Fingers[fingerindex].lasty);

            if( id == m_InitialFinger )
            {
                if( diffx != 0 )
                {
                    m_ScrollAmount.x -= diffx;
                    m_Momentum.x = (float)diffx;
                }
                if( diffy != 0 )
                {
                    m_ScrollAmount.y += diffy;
                    m_Momentum.y = (float)diffy;
                }
            }

            if( abs(diffx) > 4 || abs(diffy) > 4 )
            {
                for( int i=m_NumMenuItems-1; i>=0; i-- )
                {
                    if( GetMenuItemType(i) == MIT_Button )
                        GetMenuButton( i )->ClearHeldState();
                }
            }
            else
            {
                for( int i=m_NumMenuItems-1; i>=0; i-- )
                {
                    if( m_pMenuItems[i] )
                    {
                        if( m_pMenuItems[i]->ReleaseOnNoCollision( fingerindex, x, y ) )
                        {
                            //// finger is further down than when it was on the button.
                            //if( y < m_pMenuItems[i]->m_Transform.m42 )
                            //{
                            //    if( GetMenuItemType(i) == MIT_Button )
                            //    {
                            //        if( OnMenuReleasedDown( GetMenuButton( i )->m_ButtonAction ) )
                            //            return true;
                            //    }
                            //}
                        }
                    }
                }
            }
        }

        return true;
    }

    //if( newfingerdown )
    //{
    //}

    if( newfingerup )
    {
        MyAssert( m_pParentMenuPage != 0 );
        if( m_pParentMenuPage )
        {
            for( int i=m_NumMenuItems-1; i>=0; i-- )
            {
                if( m_pMenuItems[i] )
                {
                    const char* action = m_pMenuItems[i]->TriggerOnCollision( id, x, y, true );

                    if( action != 0 ) //&& m_pParentMenuPage->OnMenuAction( action ) )
                        return true;
                }
            }
        }

        return true;
    }
#endif // 0 // disabled to make NaCl build work

    return false;
}

void MenuScrollBox::CreateMenuItemArray(int numitems)
{
    m_NumMenuItems = numitems;
    m_pMenuItems = MyNew MenuItemPtr[numitems];
    for( int i=0; i<numitems; i++ )
        m_pMenuItems[i] = 0;
}

MenuItem* MenuScrollBox::GetMenuItem(int index)
{
    return m_pMenuItems[index];
}

MenuItemTypes MenuScrollBox::GetMenuItemType(int index)
{
    MyAssert( m_pMenuItems[index] != 0 );
    if( m_pMenuItems[index] == 0 )
        return MIT_NumMenuItemTypes;

    return m_pMenuItems[index]->m_MenuItemType;
}

#define GETMENUTYPE_Scrollbox(index, type, typeclass) \
    MyAssert( m_pMenuItems[index]->m_MenuItemType == type ); \
    if( m_pMenuItems[index]->m_MenuItemType != type ) return 0; \
    return (typeclass*)m_pMenuItems[index];

MenuSprite* MenuScrollBox::GetMenuSprite(int index)
{
    GETMENUTYPE_Scrollbox( index, MIT_Sprite, MenuSprite );
}

MenuText* MenuScrollBox::GetMenuText(int index)
{
    GETMENUTYPE_Scrollbox( index, MIT_Text, MenuText );
}

MenuButton* MenuScrollBox::GetMenuButton(int index)
{
    GETMENUTYPE_Scrollbox( index, MIT_Button, MenuButton );
}

MenuScrollBox* MenuScrollBox::GetMenuScrollBox(int index)
{
    GETMENUTYPE_Scrollbox( index, MIT_ScrollBox, MenuScrollBox );
}

MenuScrollingText* MenuScrollBox::GetMenuScrollingText(int index)
{
    GETMENUTYPE_Scrollbox( index, MIT_ScrollingText, MenuScrollingText );
}

MenuInputBox* MenuScrollBox::GetMenuInputBox(int index)
{
    GETMENUTYPE_Scrollbox( index, MIT_InputBox, MenuInputBox );
}

MenuSprite* MenuScrollBox::CreateMenuSprite(GameCore* pGameCore, int index)
{
    MyAssert( m_pMenuItems[index] == 0 );
    m_pMenuItems[index] = MyNew MenuSprite( pGameCore );
    return (MenuSprite*)m_pMenuItems[index];
}

MenuText* MenuScrollBox::CreateMenuText(GameCore* pGameCore, int index, MyMeshText* pMeshText)
{
    MyAssert( m_pMenuItems[index] == 0 );
    m_pMenuItems[index] = MyNew MenuText( pGameCore, pMeshText );
    return (MenuText*)m_pMenuItems[index];
}

MenuButton* MenuScrollBox::CreateMenuButton(GameCore* pGameCore, int index)
{
    MyAssert( m_pMenuItems[index] == 0 );
    m_pMenuItems[index] = MyNew MenuButton( pGameCore, 0 );
    return (MenuButton*)m_pMenuItems[index];
}

MenuScrollBox* MenuScrollBox::CreateMenuScrollBox(GameCore* pGameCore, int index)
{
    MyAssert( m_pMenuItems[index] == 0 );
    m_pMenuItems[index] = MyNew MenuScrollBox( pGameCore );
    return (MenuScrollBox*)m_pMenuItems[index];
}

MenuScrollingText* MenuScrollBox::CreateMenuScrollingText(GameCore* pGameCore, int index)
{
    MyAssert( m_pMenuItems[index] == 0 );
    m_pMenuItems[index] = MyNew MenuScrollingText( pGameCore );
    return (MenuScrollingText*)m_pMenuItems[index];
}

MenuInputBox* MenuScrollBox::CreateMenuInputBox(GameCore* pGameCore, int index)
{
    MyAssert( m_pMenuItems[index] == 0 );
    m_pMenuItems[index] = MyNew MenuInputBox( pGameCore );
    return (MenuInputBox*)m_pMenuItems[index];
}
