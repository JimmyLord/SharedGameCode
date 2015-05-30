//
// Copyright (c) 2012-2014 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#ifndef __MenuSprite_H__
#define __MenuSprite_H__

#include "MenuItem.h"

class MenuSprite : public MenuItem
{
public:
    unsigned char m_Justification;

public:
    float m_BGWidth;
    float m_BGHeight;

    Vector4 m_BGSpriteUVs;
    ColorByte m_BGColor;

    bool m_HasShadow;
    float m_DropShadowOffsetBG_X;
    float m_DropShadowOffsetBG_Y;
    Vector4 m_ShadowSpriteUVs;

protected:
    MySprite* m_pBGSprite;
    MySprite* m_pShadowSprite;

public:
    MenuSprite();
    ~MenuSprite();

    //virtual void StartClosing();
    //virtual void Tick(double TimePassed);
    virtual void Draw();

    //virtual float TestCollision(int fingerid, float x, float y, bool fingerwentdown = true);
    //virtual bool HoldOnCollision(int fingerid, float x, float y, bool releaseifnottouching, bool fingerwentdown = true);
    //virtual bool ReleaseOnNoCollision(int fingerid, float x, float y);
    //virtual int TriggerOnCollision(int fingerid, float x, float y, bool careifheld, bool releaseifnocollision = true); // returns the action triggered, -1 is no action.
    virtual MyRect GetBoundingRect();

    virtual void SetPositionAndSize(float x, float y, float w, float h, float inputw = -1, float inputh = -1);
    void SetSprites(MySprite* bgsprite, MySprite* shadowsprite);
    void SetSpritesCopy(MySprite* bgsprite, MySprite* shadowsprite);

    void SetBGShadow(float offsetx, float offsety) { m_DropShadowOffsetBG_X = offsetx; m_DropShadowOffsetBG_Y = offsety; }

    MySprite* GetBGSprite() { return m_pBGSprite; }

#if MYFW_USING_WX
    static void StaticFillPropertiesWindow(void* pObjectPtr, unsigned int count) { ((MenuSprite*)pObjectPtr)->FillPropertiesWindow(); }
    void FillPropertiesWindow();
#endif //MYFW_USING_WX
};

#endif //__MenuSprite_H__
