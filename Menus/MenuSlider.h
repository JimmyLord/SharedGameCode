//
// Copyright (c) 2012-2014 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#ifndef __MenuSlider_H__
#define __MenuSlider_H__

#include "MenuItem.h"

enum MenuSliderColorTypes
{
    MSCT_BarColor,
    MSCT_HandleColor,
    MSCT_BGColor,

    MSCT_NumColors,
};

enum MenuSliderStyle
{
    MSS_Horizontal,
    MSS_Vertical,
};

extern ColorByte MenuSliderColors[MSCT_NumColors];

class MenuSlider : public MenuItem
{
public:
    bool m_Disabled;

    float m_ValuePerc;

    ColorByte m_Colors[MSCT_NumColors];

    float m_PosX;
    float m_PosY;
    float m_VisualRange;
    unsigned char m_Justification;
    float m_InputWidth;
    float m_InputHeight;

    float m_BarThickness; // all terms set as if bar is vertical.
    float m_HandleWidth;
    float m_HandleHeight;

    float m_DropShadowOffset;

    float m_BGWidth;
    float m_BGHeight;
    float m_DropShadowOffsetBG_X;
    float m_DropShadowOffsetBG_Y;

    MySprite* m_pSprite;

public:
    MenuSlider();
    ~MenuSlider();

    void Draw(MyMatrix* pMatProj, MyMatrix* pMatView);
    virtual int CheckForCollisionPositionSlider(float x, float y, float gamewidth, float gameheight, bool held);
};

#endif //__MenuSlider_H__