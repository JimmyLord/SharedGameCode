//
// Copyright (c) 2012-2016 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#ifndef __ScreenManager_H__
#define __ScreenManager_H__

class Screen_Base;
class ScreenManager;

extern ScreenManager* g_pScreenManager;

class ScreenManager
{
protected:
    const static unsigned int MAX_SCREENS = 10;

    MyList<Screen_Base*> m_ScreensActive;
    MyList<Screen_Base*> m_ScreensClosing;

    // rect matching ortho matrix uses to draw menu page, used by menubutton mesh drawing.
    MyRect m_DeviceRect;

public:
    ScreenManager();
    virtual ~ScreenManager();

    virtual Screen_Base* GetBottomScreen();
    virtual Screen_Base* GetTopScreen();
    virtual Screen_Base* GetScreenAboveMe(Screen_Base* pScreen);
    virtual Screen_Base* GetScreenBelowMe(Screen_Base* pScreen);
    virtual Screen_Base* FindScreenWithID(int id);
    virtual int GetTopScreenID();
    virtual bool IsScreenWithIDOpen(int id);
    virtual void InsertScreen(int index, Screen_Base* screen);
    virtual void PushScreen(Screen_Base* screen);
    virtual void CloseScreen(Screen_Base* screen);
    virtual void RemoveScreen(Screen_Base* screen);
    virtual void CloseAllScreens(bool closelowest = true);
    virtual void CloseTopScreen();
    virtual bool IsScreenBeingManaged(Screen_Base* screen);

    MyRect GetDeviceRect() { return m_DeviceRect; }

protected:
    virtual void InsertUnderTop(Screen_Base* screen);
    virtual Screen_Base* PopScreen();

public:
    virtual bool IsSettled(); // are visible screens settled.
    virtual void Tick(float timepassed);
    virtual void Draw();

    virtual void OnResized(int x, int y, int w, int h);
    virtual bool OnTouch(int action, int id, float x, float y, float pressure, float size);
    virtual bool OnButtons(GameCoreButtonActions action, GameCoreButtonIDs id);
    virtual bool OnKeys(GameCoreButtonActions action, int keycode, int unicodechar);
};

#endif //__ScreenManager_H__
