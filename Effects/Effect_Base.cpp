//
// Copyright (c) 2012-2014 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "GameCommonHeader.h"
#include "Effect_Base.h"

Effect_Base::Effect_Base()
{
    m_TimeAlive = 0;
}

Effect_Base::~Effect_Base()
{
}

void Effect_Base::Init()
{
    //LOGInfo( LOGTag, "Effect_Base::Init()\n" );
}

void Effect_Base::Tick(double TimePassed)
{
    m_TimeAlive += TimePassed;
}

void Effect_Base::Draw()
{
}

bool Effect_Base::OnTouch(int action, int id, float x, float y, float pressure, float size)
{
    return false;
}

bool Effect_Base::OnButtons(GameCoreButtonActions action, GameCoreButtonIDs id)
{
    return false;
}
