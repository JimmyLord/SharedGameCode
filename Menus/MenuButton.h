//
// Copyright (c) 2012-2014 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#ifndef __MenuButton_H__
#define __MenuButton_H__

#include "MenuItem.h"

class MyMeshText;

#define MAX_MENUBUTTON_STRING 64

//MenuButtonColorSelectable = ColorByte(255,81,205,255);

enum MenuButtonColorTypes
{
    MBCT_SelectableBG,
    MBCT_SelectableText,

    MBCT_DisabledBG,
    MBCT_DisabledText,

    MBCT_NumColors,
};

enum MenuButtonState
{
    MBS_Disabled,
    MBS_Idle,
    MBS_HeldDown,
    MBS_Triggered,
};

extern ColorByte MenuButtonColors[MBCT_NumColors];
extern ColorByte MenuButtonColorsPlain[MBCT_NumColors];
extern MySprite* g_DefaultEnabledBGSprite;
extern Vector4 g_DefaultEnabledBGSpriteUVs;
extern MySprite* g_DefaultDisabledBGSprite;
extern Vector4 g_DefaultDisabledBGSpriteUVs;

enum MenuButtonTextStyle
{
    MBTS_SingleLine,
    MBTS_DoubleLine,
    MBTS_TripleLine,
};

class MenuButton : public MenuItem
{
    friend class Screen_Base; // for import/export of menu page layout

protected:
    MenuButtonState m_State;

    double m_TimeHeld;
    int m_FingerHolding;

    unsigned char m_Justification; // not really supported.

    float m_DropShadowOffsetText_X;
    float m_DropShadowOffsetText_Y;

    float m_DropShadowOffsetBG_X;
    float m_DropShadowOffsetBG_Y;

public:
    MenuButtonTextStyle m_Style;
    char m_Strings[3][MAX_MENUBUTTON_STRING];
    char m_ToolTipString[MAX_MENUBUTTON_STRING];

    MaterialDefinition* m_pMaterial;
    MyMeshText* m_pMeshText;

    // behaviour
    int m_ButtonAction;
    bool m_AllowPressWhenDraggedOver;

    // visuals
    FontDefinition* m_pFont;
    float m_FontHeight;
    float m_LineHeight;

    float m_InputWidth;
    float m_InputHeight;

    TextShadowStyles m_TextShadowStyle;

public:
    MyMesh* m_pBGMesh;
    MyMatrix m_MeshTransform;
    Camera3D* m_pBGMeshCamera;
    MyLight* m_pBGMeshLight;

    bool m_BGSpriteOn;
    Vector2 m_TextOffset;
    ColorByte m_TextColor;
    ColorByte m_TextShadowColor;
    Vector4 m_BGSpriteUVs;
    ColorByte m_BGColor;
    ColorByte m_BGShadowColor;

    ColorByte m_DisabledTextColor;
    Vector4 m_DisabledBGSpriteUVs;
    ColorByte m_DisabledBGColor;

    bool m_UsePressedState;
    ColorByte m_PressedTextColor;
    ColorByte m_PressedBGColor;
    Vector4 m_PressedBGSpriteUVs;

    bool m_HasOverlay;
    Vector2 m_OverlaySize;
    Vector2 m_OverlayOffset;
    ColorByte m_OverlayBGColor;
    Vector4 m_OverlayBGSpriteUVs;

    Vector4 m_ShadowSpriteUVs;

    bool m_hack_PasswordHideString2;

    int m_SoundPressed;

protected:
    MySprite* m_pBGSprite;
    MySprite* m_pDisabledBGSprite;
    MySprite* m_pPressedBGSprite;
    MySprite* m_pOverlayBGSprite;
    MySprite* m_pShadowSprite;

public:
    virtual bool CheckForCollision(float x, float y);
    void PlaySound();

public:
    MenuButton(int maxletters);
    ~MenuButton();

    virtual void StartClosing();
    virtual void Tick(double TimePassed);
    virtual void Draw(MyMatrix* matviewproj);

    virtual float TestCollision(int fingerid, float x, float y, bool fingerwentdown = true);
    virtual bool HoldOnCollision(int fingerid, float x, float y, bool releaseifnottouching, bool fingerwentdown = true);
    virtual bool ReleaseOnNoCollision(int fingerid, float x, float y);
    virtual int TriggerOnCollision(int fingerid, float x, float y, bool careifheld, bool releaseifnocollision = true); // returns the action triggered, -1 is no action.
    virtual bool ClearHeldState();

    virtual MyRect GetBoundingRect();

    //void SetDisabled(bool settodisabled);
    //bool IsDisabled();

    virtual void SetPositionAndSize(float x, float y, float w, float h, float inputw = -1, float inputh = -1);
    virtual void SetString(const char* str1, const char* str2 = 0, const char* str3 = 0);
    virtual void SetStringNumber(int stringnumber, const char* str1, ...);
    virtual void SetToolTipString(const char* str);
    void SetPressedState(const ColorByte& textcolor, const ColorByte& bgcolor, MySprite* sprite, const Vector4& uvs);
    void SetOverlay(const Vector2& size, const Vector2& offset, const ColorByte& bgcolor, MySprite* sprite, const Vector4& uvs);

    MenuButtonState GetState() { return m_State; }

    void SetSprites(MySprite* bg, MySprite* disabled, MySprite* pressed, MySprite* overlay, MySprite* shadow);
    void SetSpritesCopy(MySprite* bg, MySprite* disabled, MySprite* pressed, MySprite* overlay, MySprite* shadow);
    void SetSpritesBGShadow(MySprite* sprite);

    void SetTextShadow(float offsetx, float offsety) { m_DropShadowOffsetText_X = offsetx; m_DropShadowOffsetText_Y = offsety; }
    void SetBGShadow(float offsetx, float offsety) { m_DropShadowOffsetBG_X = offsetx; m_DropShadowOffsetBG_Y = offsety; }
    MySprite* GetBGSprite() { return m_pBGSprite; }

#if MYFW_USING_WX
    static void StaticFillPropertiesWindow(void* pObjectPtr, unsigned int count) { ((MenuButton*)pObjectPtr)->FillPropertiesWindow(); }
    void FillPropertiesWindow();

    static void StaticOnDropFont(void* pObjectPtr, int controlid, wxCoord x, wxCoord y) { ((MenuButton*)pObjectPtr)->OnDropFont(controlid, x, y); }
    void OnDropFont(int controlid, wxCoord x, wxCoord y);
#endif //MYFW_USING_WX
};

#endif //__MenuButton_H__
