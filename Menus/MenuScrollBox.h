//
// Copyright (c) 2012-2014 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#ifndef __MenuScrollBox_H__
#define __MenuScrollBox_H__

#include "MenuButton.h"

typedef MenuItem* MenuItemPtr;

class MenuSprite;
class MenuText;
class MenuButton;
class MenuScrollBox;
class MenuScrollingText;
class MenuInputBox;
class Screen_Base;

class MenuScrollBox : public MenuButton
{
public:
    Screen_Base* m_pParentMenuPage;
private:
    MenuItemPtr* m_pMenuItems;
public:
    int m_NumMenuItems;

    Vector2 m_ScrollAmount;
    Vector2 m_Momentum;

    Vector2 m_MinScrollAmount;
    Vector2 m_MaxScrollAmount;

    int m_InitialFinger;

    int m_ScreenTop;
    int m_ScreenBottom;

public:
    MenuScrollBox();
    virtual ~MenuScrollBox();

    virtual void Tick(float deltaTime);
    virtual void Draw(MyMatrix* pMatProj, MyMatrix* pMatView);
    virtual bool CheckForCollision(float x, float y);

    //virtual float TestCollision(int fingerid, float x, float y, bool fingerwentdown = true);
    virtual bool HoldOnCollision(int fingerid, float x, float y, bool releaseifnottouching, bool fingerwentdown = true);
    virtual bool ReleaseOnNoCollision(int fingerid, float x, float y);
    virtual const char* TriggerOnCollision(int fingerid, float x, float y, bool careifheld, bool releaseifnocollision = true); // returns the action triggered, -1 is no action.

    int OnTouch(int action, int id, float x, float y, float pressure, float size);

    void CreateMenuItemArray(int numitems);

    MenuItem* GetMenuItem(int index);

    MenuItemTypes GetMenuItemType(int index);

    MenuSprite* GetMenuSprite(int index);
    MenuText* GetMenuText(int index);
    MenuButton* GetMenuButton(int index);
    MenuScrollBox* GetMenuScrollBox(int index);
    MenuScrollingText* GetMenuScrollingText(int index);
    MenuInputBox* GetMenuInputBox(int index);

    MenuSprite* CreateMenuSprite(int index);
    MenuText* CreateMenuText(int index, MyMeshText* pMeshText);
    MenuButton* CreateMenuButton(int index);
    MenuScrollBox* CreateMenuScrollBox(int index);
    MenuScrollingText* CreateMenuScrollingText(int index);
    MenuInputBox* CreateMenuInputBox(int index);
};

#endif //__MenuScrollBox_H__
