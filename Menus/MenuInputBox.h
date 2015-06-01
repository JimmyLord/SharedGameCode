//
// Copyright (c) 2012-2014 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#ifndef __MenuInputBox_H__
#define __MenuInputBox_H__

#include "MenuButton.h"

class MenuInputBox : public MenuButton
{
public:
    static const int MaxStringLength = 128; // with shadow

    int m_MaxLength;
    char m_InputBuffer[MaxStringLength+1];

public:
    MenuInputBox();
    virtual ~MenuInputBox();

    virtual void Draw(MyMatrix* matviewproj);

    virtual int TriggerOnCollision(int fingerid, float x, float y, bool careifheld, bool releaseifnocollision = true); // returns the action triggered, -1 is no action.

    virtual bool OnKeyDown(int keycode, int unicodechar);

    virtual void SetString(const char* str1, const char* str2 = 0, const char* str3 = 0);
    virtual void SetInputString(const char* str, ...);
};

#endif //__MenuInputBox_H__