//
// Copyright (c) 2012-2014 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include PCHFILE
//#include "Core/ResourceManager.h"
//#include "../Screens/Screen_Base.h"
#include "MenuItem.h"
#include "MenuScrollBox.h"
#include "MenuScrollingText.h"
#include "MenuInputBox.h"

MenuScrollBox::MenuScrollBox()
: MenuButton(0)
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

void MenuScrollBox::Tick(double timepassed)
{
    MenuButton::Tick( timepassed );

    if( m_InitialFinger == -1 && m_Momentum.LengthSquared() != 0 )
    {
        m_Momentum *= 0.99f;
        
        m_ScrollAmount.x -= m_Momentum.x * (float)timepassed * 30;
        m_ScrollAmount.y += m_Momentum.y * (float)timepassed * 30;
    }

    MyClamp( m_ScrollAmount.x, m_MinScrollAmount.x, m_MaxScrollAmount.x );
    MyClamp( m_ScrollAmount.y, m_MinScrollAmount.y, m_MaxScrollAmount.y );
}

void MenuScrollBox::Draw(MyMatrix* matviewproj)
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
            float offx = m_Transform.m41 - m_ScrollAmount.x;
            float offy = m_Transform.m42 + m_ScrollAmount.y;
            m_pMenuItems[i]->SetPositionOffset( offx, offy );

            MyRect bbox = m_pMenuItems[i]->GetBoundingRect();

            //if( bbox.y + bbox.h < 0 || bbox.y > g_pGame->m_GameHeight ) // use for debugging...

            //m_ScreenBottom = g_pGame->m_OrthoBottom;
            //m_ScreenTop = g_pGame->m_OrthoTop;

            if( bbox.y + bbox.h < m_ScreenBottom || bbox.y > m_ScreenTop )
                continue;

            m_pMenuItems[i]->Draw( matviewproj );
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

int MenuScrollBox::TriggerOnCollision(int fingerid, float x, float y, bool careifheld, bool releaseifnocollision) // returns the action triggered, -1 is no action.
{
    return -1;
    //return MenuButton::TriggerOnCollision( fingerid, x, y, careifheld, releaseifnocollision );
}

int MenuScrollBox::OnTouch(int action, int id, float x, float y, float pressure, float size)
{
// TODO: MYENGINE
    ////if( m_FingerHolding != fingerid )
    ////    return false;

    ////if( m_State != MBS_Idle )
    ////    return false;

    //// flip y to make it start at bottom left.
    ////y = g_pGame->m_GameHeight - y;

    //bool newfingerdown = false;
    //bool newfingerup = false;

    //if( action == GCBA_Down ) // first finger down
    //{
    //    g_Fingers[id].set( x, y, id );
    //    newfingerdown = true;

    //    m_InitialFinger = id;
    //    m_Momentum.Set( 0, 0 );

    //    bool found = false;
    //    for( int i=m_NumMenuItems-1; i>=0; i-- )
    //    {
    //        if( m_pMenuItems[i] )
    //        {
    //            if( m_pMenuItems[i]->HoldOnCollision( id, x, y, true ) )
    //            {
    //                found = true;
    //                break;
    //            }
    //        }
    //    }
    //}

    //if( action == GCBA_Up ) // last finger up... i.e. no fingers still down.
    //{
    //    for( int i=0; i<10; i++ )
    //    {
    //        g_Fingers[i].reset();
    //    }
    //    newfingerup = true;

    //    m_InitialFinger = -1;
    //}

    //if( action == GCBA_Held ) // any finger might have moved
    //{
    //    int fingerindex = -1;

    //    for( int i=0; i<10; i++ )
    //    {
    //        if( g_Fingers[i].id == id )
    //            fingerindex = i;
    //    }

    //    if( fingerindex != -1 )
    //    {
    //        g_Fingers[fingerindex].set( x, y, id );

    //        int diffx = (int)(g_Fingers[fingerindex].currx) - (int)(g_Fingers[fingerindex].lastx);
    //        int diffy = (int)(g_Fingers[fingerindex].curry) - (int)(g_Fingers[fingerindex].lasty);

    //        if( id == m_InitialFinger )
    //        {
    //            if( diffx != 0 )
    //            {
    //                m_ScrollAmount.x -= diffx;
    //                m_Momentum.x = (float)diffx;
    //            }
    //            if( diffy != 0 )
    //            {
    //                m_ScrollAmount.y += diffy;
    //                m_Momentum.y = (float)diffy;
    //            }
    //        }

    //        if( abs(diffx) > 4 || abs(diffy) > 4 )
    //        {
    //            for( int i=m_NumMenuItems-1; i>=0; i-- )
    //            {
    //                if( GetMenuItemType(i) == MIT_Button )
    //                    GetMenuButton( i )->ClearHeldState();
    //            }
    //        }
    //        else
    //        {
    //            for( int i=m_NumMenuItems-1; i>=0; i-- )
    //            {
    //                if( m_pMenuItems[i] )
    //                {
    //                    if( m_pMenuItems[i]->ReleaseOnNoCollision( fingerindex, x, y ) )
    //                    {
    //                        //// finger is further down than when it was on the button.
    //                        //if( y < m_pMenuItems[i]->m_Transform.m42 )
    //                        //{
    //                        //    if( GetMenuItemType(i) == MIT_Button )
    //                        //    {
    //                        //        if( OnMenuReleasedDown( GetMenuButton( i )->m_ButtonAction ) )
    //                        //            return true;
    //                        //    }
    //                        //}
    //                    }
    //                }
    //            }
    //        }
    //    }
    //}

    ////if( newfingerdown )
    ////{   
    ////}

    //if( newfingerup )
    //{
    //    MyAssert( m_pParentMenuPage != 0 );
    //    if( m_pParentMenuPage )
    //    {
    //        for( int i=m_NumMenuItems-1; i>=0; i-- )
    //        {
    //            if( m_pMenuItems[i] )
    //            {
    //                int action = m_pMenuItems[i]->TriggerOnCollision( id, x, y, true );

    //                if( action != -1 && m_pParentMenuPage->OnMenuAction( action ) )
    //                    return true;
    //            }
    //        }
    //    }
    //}

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

#define GETMENUTYPE(index, type, typeclass) \
    MyAssert( m_pMenuItems[index]->m_MenuItemType == type ); \
    if( m_pMenuItems[index]->m_MenuItemType != type ) return 0; \
    return (typeclass*)m_pMenuItems[index];

MenuSprite* MenuScrollBox::GetMenuSprite(int index)
{
    GETMENUTYPE( index, MIT_Sprite, MenuSprite );
}

MenuText* MenuScrollBox::GetMenuText(int index)
{
    GETMENUTYPE( index, MIT_Text, MenuText );
}

MenuButton* MenuScrollBox::GetMenuButton(int index)
{
    GETMENUTYPE( index, MIT_Button, MenuButton );
}

MenuScrollBox* MenuScrollBox::GetMenuScrollBox(int index)
{
    GETMENUTYPE( index, MIT_ScrollBox, MenuScrollBox );
}

MenuScrollingText* MenuScrollBox::GetMenuScrollingText(int index)
{
    GETMENUTYPE( index, MIT_ScrollingText, MenuScrollingText );
}

MenuInputBox* MenuScrollBox::GetMenuInputBox(int index)
{
    GETMENUTYPE( index, MIT_InputBox, MenuInputBox );
}

MenuSprite* MenuScrollBox::CreateMenuSprite(int index)
{
    MyAssert( m_pMenuItems[index] == 0 );
    m_pMenuItems[index] = MyNew MenuSprite;
    return (MenuSprite*)m_pMenuItems[index];
}

MenuText* MenuScrollBox::CreateMenuText(int index, MyMeshText* pMeshText)
{
    MyAssert( m_pMenuItems[index] == 0 );
    m_pMenuItems[index] = MyNew MenuText( pMeshText );
    return (MenuText*)m_pMenuItems[index];
}

MenuButton* MenuScrollBox::CreateMenuButton(int index)
{
    MyAssert( m_pMenuItems[index] == 0 );
    m_pMenuItems[index] = MyNew MenuButton(0);
    return (MenuButton*)m_pMenuItems[index];
}

MenuScrollBox* MenuScrollBox::CreateMenuScrollBox(int index)
{
    MyAssert( m_pMenuItems[index] == 0 );
    m_pMenuItems[index] = MyNew MenuScrollBox;
    return (MenuScrollBox*)m_pMenuItems[index];
}

MenuScrollingText* MenuScrollBox::CreateMenuScrollingText(int index)
{
    MyAssert( m_pMenuItems[index] == 0 );
    m_pMenuItems[index] = MyNew MenuScrollingText;
    return (MenuScrollingText*)m_pMenuItems[index];
}

MenuInputBox* MenuScrollBox::CreateMenuInputBox(int index)
{
    MyAssert( m_pMenuItems[index] == 0 );
    m_pMenuItems[index] = MyNew MenuInputBox;
    return (MenuInputBox*)m_pMenuItems[index];
}
