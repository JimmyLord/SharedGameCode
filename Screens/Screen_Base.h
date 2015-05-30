//
// Copyright (c) 2012-2014 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#ifndef __Screen_Base_H__
#define __Screen_Base_H__

class Screen_Base;

class InputFinger;
class MenuItem;
class ScreenManager;

class MenuSprite;
class MenuText;
class MenuButton;
class MenuScrollBox;
class MenuScrollingText;
class MenuInputBox;
class MenuCheckBox;

#define MAX_MENUITEMS           128
#define MAX_SCREENNAME_LENGTH   32

#include "Core/ScreenOverlays.h"

enum ScreenStates
{
    SS_Opening,
    SS_Running,
    SS_Closing,
    SS_DoneClosing,
};

class Screen_Base
{
    friend class ScreenManager;

protected:
    char m_ScreenName[MAX_SCREENNAME_LENGTH];
    int m_ScreenOverlayIndex;

    Screen_Base* m_pParentScreen;
    bool m_ScreenHasBeenInitialized;
    bool m_ScreenIsBeingCached;

    ScreenOverlays m_ScreenToShow;
    Screen_Base* m_ScreenToShowParentPage;
    void* m_ScreenToShowParam1;
    void* m_ScreenToShowParam2;

    int m_ScreenID;
    bool m_DestroyCurrentScreen; // only used when swapping one screen for another.
    bool m_AlwaysDraw;
    bool m_Transparent;
    bool m_AllowInputToPassThrough;

    double m_TimeAlive;
    ScreenStates m_State;
    double m_TimeInState;
    double m_TimeToOpen;
    double m_TimeToClose;

    int m_MenuItemsNeeded;
private:
    MenuItem* m_pMenuItems[MAX_MENUITEMS];
protected:
    MenuItem* m_pMenuItemHeld;

    bool m_MenuItemPressedIgnoreGesturesUntilFingerUp;

protected:
    virtual void SwitchScreenOverlay();
    virtual void CloseScreen();

protected:
    char* m_LayoutJSON_Tall;
    char* m_LayoutJSON_Wide;
    char* m_LayoutJSON_Square;
#if _DEBUG
    char* ExportMenuLayout();
#endif
    void ImportMenuLayout(const char* layout);

public:
    Screen_Base();
    virtual ~Screen_Base();

    void SetScreenIsBeingCached() { m_ScreenIsBeingCached = true; }
    virtual void OnClose();

    float QueryPercentageStateComplete();
    void SetParentScreen(Screen_Base* pScreen) { m_pParentScreen = pScreen; }
    int GetScreenID() { return m_ScreenID; }
    ScreenStates GetState() { return m_State; }

    bool IsOpaque() { return !m_Transparent; }
    bool CanInputPassThrough() { return m_AllowInputToPassThrough; }

    const char* GetMenuItemLabel(MenuItem* pMenuItem);

    void CreateMenuItems(int numitems, MenuItemDefinition* menuitems);

    virtual void Init(); // one time init when first created.
    virtual void OnDisplay(); // reset to an initial state each time it's displayed.
    virtual void OnResized(); // window was resized, change the layout if aspect ratio requires.
    virtual void UpdateMenuItems(bool initialsetup);
    virtual bool IsSettled();
    virtual void Tick(double TimePassed);
    virtual void Draw();

    virtual bool OnTouch(int action, int id, float x, float y, float pressure, float size);
    virtual bool OnButtons(GameCoreButtonActions action, GameCoreButtonIDs id);
    virtual bool OnKeyDown(int keycode, int unicodechar);

    InputFinger* GetFingerInfo(int id);

    void BasicMenuHandleHeldFunc(int fingerindex, float x, float y);
    bool BasicMenuTouchFunc(int action, int id, float x, float y, float pressure, float size);
    virtual bool OnMenuAction(int action) { return false; }

#if MYFW_USING_WX
    static void StaticFillPropertiesWindow(void* pObjectPtr, unsigned int count) { ((Screen_Base*)pObjectPtr)->FillPropertiesWindow(); }
    void FillPropertiesWindow();
#endif //MYFW_USING_WX

    int GetMenuButtonHeldCount();

    void SetScreenOverlayToShow(ScreenOverlays screen, Screen_Base* pParent = 0, void* pPtr = 0, void* pPtr2 = 0);
    void ReplaceCurrentScreenOverlayWith(ScreenOverlays screen, Screen_Base* pParent = 0, void* pPtr = 0, void* pPtr2 = 0);

    void DrawAllMenuItems();
    void DrawMenuItem(int index);
    MenuItem* GetMenuItem(int index);

    MenuItemTypes GetMenuItemType(int index);

    MenuSprite* GetMenuSprite(int index);
    MenuText* GetMenuText(int index);
    MenuButton* GetMenuButton(int index);
    MenuScrollBox* GetMenuScrollBox(int index);
    MenuScrollingText* GetMenuScrollingText(int index);
    MenuInputBox* GetMenuInputBox(int index);
    MenuCheckBox* GetMenuCheckBox(int index);

    MenuSprite* CreateMenuSprite(int index);
    MenuText* CreateMenuText(int index, int maxletters = 0);
    MenuButton* CreateMenuButton(int index, int maxletters = 0);
    MenuScrollBox* CreateMenuScrollBox(int index);
    MenuScrollingText* CreateMenuScrollingText(int index);
    MenuInputBox* CreateMenuInputBox(int index);
    MenuCheckBox* CreateMenuCheckBox(int index);
};

#endif //__Screen_Base_H__
