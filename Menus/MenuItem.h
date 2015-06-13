//
// Copyright (c) 2012-2014 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#ifndef __MenuItem_H__
#define __MenuItem_H__

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

struct MenuItemDefinition
{
    const char* name;
    MenuItemTypes type;
    int spriteindex;
    ColorByte BGColor;
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

#if MYFW_USING_WX
typedef void (*MenuItemDeletedCallbackFunc)(void* pObjectPtr, MenuItem* pMenuItem);
struct MenuItemDeletedCallbackStruct
{
    void* pObj;
    MenuItemDeletedCallbackFunc pFunc;
};
#endif

class MenuItem
#if MYFW_USING_WX
: public wxEvtHandler
#endif
{
public:
    static const int MAX_MENUITEM_NAME_LENGTH = 32;

    MenuItemTypes m_MenuItemType;

    char m_Name[MAX_MENUITEM_NAME_LENGTH];

    bool m_Visible;
    bool m_Enabled;

    bool m_Closing;

    Vector3 m_Scale;
    MyMatrix m_Transform;
    Vector2 m_Size;

    Vector2 m_PositionOffset;

    bool m_UseTweenIn;
    MyTweener m_TweenIn;
    bool m_UseTweenOut;
    MyTweener m_TweenOut;

    int m_MenuItemNavigation[4];

public:
    MenuItem();
    virtual ~MenuItem();

    virtual void StartClosing();
    virtual void Tick(double timepassed);
    virtual void Draw(MyMatrix* matviewproj);
    virtual bool CheckForCollision(float x, float y);
    virtual int CheckForCollisionPosition(float x, float y, bool held);

    void SetName(const char* name);
    void SetPositionOffset(float offx, float offy);
    
    virtual void SetPositionAndSize(float x, float y, float w, float h, float inputw = -1, float inputh = -1);

    virtual void SetVisible(bool visible) { m_Visible = visible; }
    virtual void SetEnabled(bool enabled) { m_Enabled = enabled; }
    virtual void SetVisibleAndEnabled(bool value) { m_Enabled = value; m_Visible = value; }

    virtual float TestCollision(int fingerid, float x, float y, bool fingerwentdown = true); // returns -1 if not touching, distance-squared if it is.
    virtual bool HoldOnCollision(int fingerid, float x, float y, bool releaseifnottouching, bool fingerwentdown = true);
    virtual bool ReleaseOnNoCollision(int fingerid, float x, float y);
    virtual const char* TriggerOnCollision(int fingerid, float x, float y, bool careifheld, bool releaseifnocollision = true); // returns the action triggered, -1 is no action.

    virtual MyRect GetBoundingRect() { return MyRect(0,0,0,0); }

#if MYFW_USING_WX
    static void StaticFillPropertiesWindow(void* pObjectPtr, unsigned int count) { ((MenuItem*)pObjectPtr)->FillPropertiesWindow(); }
    void FillPropertiesWindow();

    // Object panel callbacks.
    static void StaticOnRightClick(void* pObjectPtr, wxTreeItemId id) { ((MenuItem*)pObjectPtr)->OnRightClick(); }
    virtual void OnRightClick();
    void OnPopupClick(wxEvent &evt);

    MenuItemDeletedCallbackStruct m_MenuItemDeletedCallbackStruct;
    void RegisterMenuItemDeletedCallback(void* pObj, MenuItemDeletedCallbackFunc pFunc);
#endif //MYFW_USING_WX

    void SetMenuItemNavigation(int up, int right, int down, int left);
    int GetMenuItemNavigation(MenuNavDir dir);
};

#endif //__MenuItem_H__