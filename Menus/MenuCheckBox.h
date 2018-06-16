//
// Copyright (c) 2012-2014 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#ifndef __MenuCheckBox_H__
#define __MenuCheckBox_H__

#include "MenuItem.h"

#define MAX_MENUCHECKBOX_STRING 64

enum MenuCheckBoxColorTypes
{
    MCBCT_CheckedBG,
    MCBCT_UncheckedBG,

    MCBCT_Text,

    MCBCT_NumColors,
};

extern ColorByte DefaultMenuCheckBoxColors[MCBCT_NumColors];

enum MenuCheckBoxState
{
    MCBS_Disabled,
    MCBS_Idle,
    MCBS_HeldDown,
    MCBS_Triggered,
};

class MenuCheckBox : public MenuItem
{
    static const int MAX_BUTTON_ACTION_LENGTH = 32;

protected:
    MenuCheckBoxState m_State;

    int m_FingerHolding;

public:
    bool m_Checked;
    char m_Action[MAX_BUTTON_ACTION_LENGTH];

    float m_PosX;
    float m_PosY;
    unsigned char m_Justification;
    float m_InputWidth;
    float m_InputHeight;

    FontDefinition* m_pFont;
    float m_FontHeight;
    char m_String[MAX_MENUCHECKBOX_STRING];
    float m_TextOffsetX;
    float m_TextOffsetY;
    float m_TextJustification;
    float m_TextDropShadowOffsetX;
    float m_TextDropShadowOffsetY;
    ColorByte m_TextColor;

    float m_BGWidth;
    float m_BGHeight;
    float m_BGDropShadowOffsetX;
    float m_BGDropShadowOffsetY;

    MySprite* m_pCheckedBGSprite;
    Vector4 m_CheckedBGSpriteUVs;
    ColorByte m_CheckedBGColor;

    MySprite* m_pUncheckedBGSprite;
    Vector4 m_UncheckedBGSpriteUVs;
    ColorByte m_UncheckedBGColor;

    int m_SoundPressed;

protected:
    virtual bool CheckForCollision(float x, float y);
    void PlaySound();

public:
    MenuCheckBox();
    ~MenuCheckBox();

    virtual void Draw(MyMatrix* pMatProj, MyMatrix* pMatView);

    virtual float TestCollision(int fingerid, float x, float y, bool fingerwentdown = true);
    virtual bool HoldOnCollision(int fingerid, float x, float y, bool releaseifnottouching, bool fingerwentdown = true);
    virtual bool ReleaseOnNoCollision(int fingerid, float x, float y);
    virtual const char* TriggerOnCollision(int fingerid, float x, float y, bool careifheld, bool releaseifnocollision = true); // returns the action triggered, -1 is no action.

    //void SetDisabled(bool settodisabled);
    //bool IsDisabled();
};

#endif //__MenuCheckBox_H__