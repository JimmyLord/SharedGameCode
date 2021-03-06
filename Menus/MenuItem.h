//
// Copyright (c) 2012-2017 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#ifndef __MenuItem_H__
#define __MenuItem_H__

class ComponentMenuPage;
class GameCore;
class MenuItem;
class Screen_Base;

enum MenuItemTypes
{
    MIT_Base,
    MIT_Sprite,
    MIT_Text,
    MIT_Button,
    MIT_ScrollBox,
    MIT_ScrollingText,
    MIT_InputBox,
    MIT_CheckBox,
    MIT_NumMenuItemTypes,
};

enum MenuItemAnchorPoint
{
    Anchor_None,
    Anchor_TopLeft,
    Anchor_TopRight,
    Anchor_BottomLeft,
    Anchor_BottomRight,
    Anchor_NumTypes,
};

struct MenuItemDefinition
{
    const char* name;
    MenuItemTypes type;
    MySprite* pSprite;
    MaterialDefinition* pMaterials[5]; //MenuButton::Materials_NumTypes]; // buttons have the most material types.
    //float x;
    //float y;
    //int justify;
    //float scale;
    //float w;
    //float h;
    //float offsetx;
    //float offsety;
    FontDefinition* font;
    int maxletters;
    const char* string1;
    const char* string2;
    const char* string3;
    int buttonaction;
};

enum MenuNavDir
{
    MenuNavDir_Up,
    MenuNavDir_Right,
    MenuNavDir_Down,
    MenuNavDir_Left,
};

class MenuItem
{
public:
    static const int MAX_MENUITEM_NAME_LENGTH = 32;

    GameCore* m_pGameCore;

    MenuItemTypes m_MenuItemType;

    char m_Name[MAX_MENUITEM_NAME_LENGTH];

    bool m_Visible;
    bool m_Enabled;

    bool m_Closing;

    MenuItemAnchorPoint m_AnchorPoint;
    Vector2 m_Position;

    bool m_UseTweenIn;      // TODO: save/load
    MyTweener m_TweenIn;    // TODO: save/load
    bool m_UseTweenOut;     // TODO: save/load
    MyTweener m_TweenOut;   // TODO: save/load

    bool m_Navigable;
    int m_MenuItemNavigation[4]; // TODO: save/load item names, make references to items somehow, handle delete/reorder.

    Screen_Base* m_pParentScreen;

#if MYFW_USING_MYENGINE
    ComponentMenuPage* m_pMenuPage;
#endif

public:
    MenuItem(GameCore* pGameCore);
    virtual ~MenuItem();

    MenuItem& operator=(const MenuItem& other);

#if MYFW_USING_LUA
    static void LuaRegister(lua_State* luastate);
#endif //MYFW_USING_LUA

    virtual void StartClosing();
    virtual void Tick(float deltaTime);
    virtual void Draw(MyMatrix* pMatProj, MyMatrix* pMatView);
    virtual bool CheckForCollision(float x, float y);
    virtual int CheckForCollisionPosition(float x, float y, bool held);

    void SetName(const char* name);
    
    virtual void SetPosition(float x, float y);
    virtual void SetSize(float w, float h, float inputw = -1, float inputh = -1);
    virtual void SetPositionAndSize(float x, float y, float w, float h, float inputw = -1, float inputh = -1);
    virtual void SetAnchorPoint(MenuItemAnchorPoint anchortype);

    virtual Vector2 GetSize() { return Vector2( 0, 0 ); }

    virtual void SetVisible(bool visible) { m_Visible = visible; }
    virtual void SetEnabled(bool enabled) { m_Enabled = enabled; }
    virtual void SetVisibleAndEnabled(bool value) { m_Enabled = value; m_Visible = value; }

    virtual float TestCollision(int fingerid, float x, float y, bool fingerwentdown = true); // returns -1 if not touching, distance-squared if it is.
    virtual bool HoldOnCollision(int fingerid, float x, float y, bool releaseifnottouching, bool fingerwentdown = true);
    virtual bool ReleaseOnNoCollision(int fingerid, float x, float y);
    virtual const char* TriggerOnCollision(int fingerid, float x, float y, bool careifheld, bool releaseifnocollision = true); // returns the action triggered, -1 is no action.

    virtual MyRect GetBoundingRect() { return MyRect(0,0,0,0); }

    virtual void SetNavigable(bool navigable) { m_Navigable = navigable; }

    void SetMenuItemNavigation(int up, int right, int down, int left);
    int GetMenuItemNavigation(MenuNavDir dir);
};

#endif //__MenuItem_H__