//
// Copyright (c) 2012-2014 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "GameCommonHeader.h"

#include "ScreenManager.h"
#include "Screen_Base.h"

ScreenManager* g_pScreenManager = 0;

ScreenManager::ScreenManager()
: m_ScreensActive(MAX_SCREENS)
, m_ScreensClosing(MAX_SCREENS)
{
}

ScreenManager::~ScreenManager()
{
    for( unsigned int i=0; i<m_ScreensActive.Count(); i++ )
    {
        if( m_ScreensActive[i]->m_ScreenIsBeingCached == false )
            delete( m_ScreensActive[i] );
    }
    for( unsigned int i=0; i<m_ScreensClosing.Count(); i++ )
    {
        if( m_ScreensActive[i]->m_ScreenIsBeingCached == false )
            delete( m_ScreensClosing[i] );
    }
}

Screen_Base* ScreenManager::GetBottomScreen()
{
    if( m_ScreensActive.Count() == 0 )
        return 0;

    return m_ScreensActive[ 0 ];
}

Screen_Base* ScreenManager::GetTopScreen()
{
    if( m_ScreensActive.Count() == 0 )
        return 0;

    return m_ScreensActive[ m_ScreensActive.Count()-1 ];
}

Screen_Base* ScreenManager::GetScreenAboveMe(Screen_Base* pScreen)
{
    // check stack for screen, ignore top-most screen.
    for( unsigned int i=0; i<m_ScreensActive.Count()-1; i++ )
    {
        if( m_ScreensActive[i] == pScreen )
            return m_ScreensActive[i+1];
    }

    return 0;
}

Screen_Base* ScreenManager::GetScreenBelowMe(Screen_Base* pScreen)
{
    // check stack for screen, ignore bottom-most screen.
    for( unsigned int i=1; i<m_ScreensActive.Count(); i++ )
    {
        if( m_ScreensActive[i] == pScreen )
            return m_ScreensActive[i-1];
    }

    return 0;
}

Screen_Base* ScreenManager::FindScreenWithID(int id)
{
    for( unsigned int i=0; i<m_ScreensActive.Count(); i++ )
    {
        if( m_ScreensActive[i]->GetScreenID() == id )
            return m_ScreensActive[i];
    }

    for( unsigned int i=0; i<m_ScreensClosing.Count(); i++ )
    {
        if( m_ScreensClosing[i]->GetScreenID() == id )
            return m_ScreensActive[i];
    }

    return 0;
}

int ScreenManager::GetTopScreenID()
{
    Screen_Base* pScreen = GetTopScreen();

    if( pScreen == 0 )
        return -1;

    return pScreen->GetScreenID();
}

bool ScreenManager::IsScreenWithIDOpen(int id)
{
    for( unsigned int i=0; i<m_ScreensActive.Count(); i++ )
    {
        if( m_ScreensActive[i]->GetScreenID() == id )
            return true;
    }

    return false;
}

void ScreenManager::InsertScreen(int index, Screen_Base* screen)
{
    MyAssert( m_ScreensActive.Count() < MAX_SCREENS );
    m_ScreensActive.InsertAtIndex( index, screen );
}

bool ScreenManager::IsScreenBeingManaged(Screen_Base* screen)
{
    for( unsigned int i=0; i<m_ScreensActive.Count(); i++ )
    {
        if( m_ScreensActive[i] == screen )
            return true;
    }
    for( unsigned int i=0; i<m_ScreensClosing.Count(); i++ )
    {
        if( m_ScreensClosing[i] == screen )
            return true;
    }

    return false;
}

void ScreenManager::InsertUnderTop(Screen_Base* screen)
{
    m_ScreensActive.InsertAtIndex( m_ScreensActive.Count()-1, screen );
}

void ScreenManager::CloseScreen(Screen_Base* screen)
{
    screen->OnClose();

    bool removed = m_ScreensActive.Remove_MaintainOrder( screen );
    MyAssert( removed );

    screen->CloseScreen();

    m_ScreensClosing.Add( screen );
}

void ScreenManager::RemoveScreen(Screen_Base* screen)
{
    bool removed = m_ScreensActive.Remove_MaintainOrder( screen );
    MyAssert( removed );
}

void ScreenManager::CloseAllScreens(bool closelowest)
{
    unsigned int bottomscreentoclose = 1;
    if( closelowest )
        bottomscreentoclose = 0;

    // close from top to bottom:
    for( unsigned int i=m_ScreensActive.Count()-1; i<m_ScreensActive.Count() && i>=bottomscreentoclose; i-- )
    {
        Screen_Base* pScreen = m_ScreensActive.RemoveIndex_MaintainOrder( i );
        m_ScreensClosing.Add( pScreen );

        pScreen->CloseScreen();
    }
}

void ScreenManager::CloseTopScreen()
{
    Screen_Base* pScreen = m_ScreensActive.RemoveIndex_MaintainOrder( m_ScreensActive.Count()-1 );
    MyAssert( pScreen );

    pScreen->CloseScreen();

    m_ScreensClosing.Add( pScreen );
}

void ScreenManager::PushScreen(Screen_Base* screen)
{
    MyAssert( m_ScreensActive.Count() < MAX_SCREENS );
    m_ScreensActive.Add( screen );
}

Screen_Base* ScreenManager::PopScreen()
{
    MyAssert( false );
    return 0;
    //return m_ScreensActive.RemoveIndex_MaintainOrder( m_ScreensActive.Count()-1 );
}

bool ScreenManager::IsSettled()
{
    int i;
    int lowestscreentodraw = 0;

    for( i=m_ScreensActive.Count()-1; i>=0; i-- )
    {
        if( m_ScreensActive[i]->IsOpaque() )
        {
            lowestscreentodraw = i;
            break;
        }
    }

    for( i=0; i<(int)m_ScreensActive.Count(); i++ )
    {
        if( m_ScreensActive[i]->m_AlwaysDraw || i >= lowestscreentodraw )
        {
            if( m_ScreensActive[i]->IsSettled() == false )
                return false;
        }
    }

    for( unsigned int i=0; i<m_ScreensClosing.Count(); i++ )
    {
        if( m_ScreensClosing[i]->IsSettled() == false )
            return false;
    }

    return true;
}

void ScreenManager::Tick(double timepassed)
{
    for( unsigned int i=0; i<m_ScreensActive.Count(); i++ )
    {
        m_ScreensActive[i]->Tick( timepassed );

        if( m_ScreensActive[i]->GetState() == SS_DoneClosing )
        {
            MyAssert( false ); // should already be in closing list when closing.
            Screen_Base* pScreen = m_ScreensActive.RemoveIndex_MaintainOrder( i );
            i--;

            if( pScreen->m_ScreenIsBeingCached == false )
                SAFE_DELETE( pScreen );
        }
    }

    for( unsigned int i=0; i<m_ScreensClosing.Count(); i++ )
    {
        m_ScreensClosing[i]->Tick( timepassed );

        if( m_ScreensClosing[i]->GetState() == SS_DoneClosing )
        {
            Screen_Base* pScreen = m_ScreensClosing.RemoveIndex_MaintainOrder( i );
            i--;

            if( pScreen->m_ScreenIsBeingCached == false )
                SAFE_DELETE( pScreen );
        }
    }
}

void ScreenManager::Draw()
{
    int i;
    int lowestscreentodraw = 0;

    for( i=m_ScreensActive.Count()-1; i>=0; i-- )
    {
        if( m_ScreensActive[i]->IsOpaque() )
        {
            lowestscreentodraw = i;
            break;
        }
    }

    for( i=0; i<(int)m_ScreensActive.Count(); i++ )
    {
        if( m_ScreensActive[i]->m_AlwaysDraw || i >= lowestscreentodraw )
            m_ScreensActive[i]->Draw();
    }

    for( unsigned int i=0; i<m_ScreensClosing.Count(); i++ )
    {
        m_ScreensClosing[i]->Draw();
    }
}

void ScreenManager::OnResized()
{
    for( unsigned int i=0; i<m_ScreensActive.Count(); i++ )
    {
        m_ScreensActive[i]->OnResized();
    }

    for( unsigned int i=0; i<m_ScreensClosing.Count(); i++ )
    {
        m_ScreensClosing[i]->OnResized();
    }
}

bool ScreenManager::OnTouch(int action, int id, float x, float y, float pressure, float size)
{
    //MyAssert( m_ScreensActive.Count() >= 1 );
    if( m_ScreensActive.Count() == 0 )
        return false;

    int currentindex = m_ScreensActive.Count()-1;
    bool usedinput = false;
    while( 1 )
    {
        Screen_Base* pScreen = m_ScreensActive[currentindex];
        usedinput = pScreen->OnTouch( action, id, x, y, pressure, size );

        if( pScreen->CanInputPassThrough() == false || usedinput || currentindex == 0 )
            break;

        currentindex--;
    }

    return usedinput;
}

bool ScreenManager::OnButtons(GameCoreButtonActions action, GameCoreButtonIDs id)
{
    if( m_ScreensActive.Count() > 0 )
    {
        Screen_Base* pScreen = m_ScreensActive[m_ScreensActive.Count()-1];
        return pScreen->OnButtons( action, id );
    }

    return false;
}

bool ScreenManager::OnKeyDown(int keycode, int unicodechar)
{
    if( m_ScreensActive.Count() > 0 )
    {
        Screen_Base* pScreen = m_ScreensActive[m_ScreensActive.Count()-1];
        return pScreen->OnKeyDown( keycode, unicodechar );
    }

    return false;
}
