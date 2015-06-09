//
// Copyright (c) 2012-2015 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#ifndef __MenuText_H__
#define __MenuText_H__

#include "MenuItem.h"

class MyMeshText;

class MenuText : public MenuItem
{
public:
    static const int MAX_MenuText_STRING = 128;

public:
    char m_String[MAX_MenuText_STRING];

    MaterialDefinition* m_pMaterial;
    MyMeshText* m_pMeshText;
    bool m_MeshAllocatedLocally;
    bool m_DrawAsPartOfBatch;

    FontDefinition* m_pFont;
    float m_FontHeight;
    float m_LineHeight;

    unsigned char m_Justification;
    float m_DropShadowOffsetX;
    float m_DropShadowOffsetY;

    ColorByte m_TextColor;
    ColorByte m_TextShadowColor;

protected:

public:
    MenuText();
    MenuText(int maxletters);
    MenuText(MyMeshText* pMeshText);
    ~MenuText();

    //virtual void StartClosing();
    //virtual void Tick(double TimePassed);
    virtual void Draw(MyMatrix* matviewproj);

    //virtual float TestCollision(int fingerid, float x, float y, bool fingerwentdown = true);
    //virtual bool HoldOnCollision(int fingerid, float x, float y, bool releaseifnottouching, bool fingerwentdown = true);
    //virtual bool ReleaseOnNoCollision(int fingerid, float x, float y);
    //virtual const char* TriggerOnCollision(int fingerid, float x, float y, bool careifheld, bool releaseifnocollision = true); // returns the action triggered, -1 is no action.
    virtual MyRect GetBoundingRect();

    virtual void SetPositionAndSize(float x, float y, float w, float h, float inputw = -1, float inputh = -1);
    void SetString(const char* str, ...);

#if MYFW_USING_WX
    static void StaticFillPropertiesWindow(void* pObjectPtr, unsigned int count) { ((MenuText*)pObjectPtr)->FillPropertiesWindow(); }
    void FillPropertiesWindow();

    static void StaticOnDropFont(void* pObjectPtr, int controlid, wxCoord x, wxCoord y) { ((MenuText*)pObjectPtr)->OnDropFont(controlid, x, y); }
    void OnDropFont(int controlid, wxCoord x, wxCoord y);
#endif //MYFW_USING_WX
};

#endif //__MenuText_H__
