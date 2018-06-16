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
#include "../Core/RenderTextQuick.h"
#include "MenuInputBox.h"

MenuInputBox::MenuInputBox()
: MenuButton(MaxStringLength)
{
    m_MenuItemType = MIT_InputBox;

    m_InputBuffer[0] = 0;

    m_MaxLength = MaxStringLength;
}

MenuInputBox::~MenuInputBox()
{
}

void MenuInputBox::SetString(const char* str1, const char* str2, const char* str3)
{
    MenuButton::SetString( str1, str2, str3 );

    sprintf_s( m_InputBuffer, m_MaxLength+1, "%s", str2 );
}

void MenuInputBox::SetInputString(const char* str, ...)
{
    va_list arg;
    va_start( arg, str );
    vsnprintf_s( m_InputBuffer, m_MaxLength+1, _TRUNCATE, str, arg );
    va_end( arg );
}

void MenuInputBox::Draw(MyMatrix* pMatProj, MyMatrix* pMatView)
{
    MenuButton::SetString( m_InputBuffer );
    //SetString( m_Strings[0], m_InputBuffer );

    MenuButton::Draw( pMatProj, pMatView );
}

const char* MenuInputBox::TriggerOnCollision(int fingerid, float x, float y, bool careifheld, bool releaseifnocollision)
{
    const char* action = MenuButton::TriggerOnCollision( fingerid, x, y, careifheld, releaseifnocollision );

    if( action != 0 )
    {
        g_pGameCore->RequestKeyboardOpen();
        return m_ButtonAction;
    }

    return action;
}

bool MenuInputBox::OnKeys(GameCoreButtonActions action, int keycode, int unicodechar)
{
    if( action != GCBA_Down )
        return false;

    int nextpos = (int)strlen( m_InputBuffer );

    LOGInfo( LOGTag, "OnKeyDown - keycode %d - unicodechar %d - nextpos %d\n", keycode, unicodechar, nextpos );

    if( keycode == MYKEYCODE_BACKSPACE )
    {
        if( nextpos > 0 )
        {
            m_InputBuffer[nextpos-1] = 0;
        }
        return true;
    }

    if( keycode == MYKEYCODE_ENTER )
    {
        g_pGameCore->RequestKeyboardClose();
        return true;
    }

    //MyAssert( nextpos < m_MaxLength );
    if( nextpos >= m_MaxLength )
        return true;

    //if( (key >= 'a' && key <= 'z') ||
    //    (key >= 'A' && key <= 'Z') ||
    //    (key >= '0' && key <= '9') ||
    //    (key == ' ') )
    if( unicodechar >= 32 && unicodechar <= 125 &&
        unicodechar != '%' )
    {
        m_InputBuffer[nextpos] = (char)unicodechar; // conversion is ok since unicodechar is between 32 and 125
        nextpos++;
        m_InputBuffer[nextpos] = 0;

        return true;
    }

    LOGInfo( LOGTag, "Ignoring key - keycode %d - unicodechar %d\n", keycode, unicodechar );
    return false;
}
