//
// Copyright (c) 2012-2015 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#ifndef __MenuScrollingText_H__
#define __MenuScrollingText_H__

#include "MenuScrollBox.h"

class MenuScrollingText : public MenuScrollBox
{
protected:
    MaterialDefinition* m_pMaterial;
    MyMeshText* m_peTextMesh;

public:
    const char* m_StringToShow;
    float m_TopFade0;
    float m_TopFade1;
    float m_BottomFade1;
    float m_BottomFade0;

public:
    MenuScrollingText(GameCore* pGameCore);
    virtual ~MenuScrollingText();

    void SetTextMesh(MyMeshText* pMesh);

    virtual void Draw(MyMatrix* pMatProj, MyMatrix* pMatView);

#if MYFW_USING_WX
    // Object panel callbacks.
    static void StaticFillPropertiesWindow(void* pObjectPtr, wxTreeItemId id, unsigned int count) { ((MenuScrollingText*)pObjectPtr)->FillPropertiesWindow(); }
    void FillPropertiesWindow();
#endif //MYFW_USING_WX
};

#endif //__MenuScrollingText_H__
