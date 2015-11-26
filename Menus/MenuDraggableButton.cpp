//
// Copyright (c) 2012-2015 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#ifndef PCHFILE
#define PCHFILE "EngineCommonHeader.h"
#endif

#include PCHFILE
#include "MenuDraggableButton.h"

bool MenuDraggableButton::ReleaseOnNoCollision(int fingerid, float x, float y)
{
    //MenuButton::ReleaseOnNoCollision( fingerid, x, y );

    if( m_FingerHolding != fingerid )
        return false;

    m_Position.Set( x, y );

    return false;
}
