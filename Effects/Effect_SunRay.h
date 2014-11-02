//
// Copyright (c) 2012-2014 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not
// claim that you wrote the original software. If you use this software
// in a product, an acknowledgment in the product documentation would be
// appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
// misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#ifndef __Effect_SunRay_H__
#define __Effect_SunRay_H__

#if Effect_SunRay_Enabled

class Effect_SunRay
{
protected:
    double m_TimeAlive;

    MySprite_XYZVertexColor* m_pSpriteRayColor1;
    MySprite_XYZVertexColor* m_pSpriteRayColor2;

    Vector2 m_Center;
    int m_NumSlices;
    float m_Length;
    ColorByte m_Color1;
    ColorByte m_Color2;
    float m_Speed; // degrees per second

    float m_AngleOffset;

public:
    Effect_SunRay();
    virtual ~Effect_SunRay();

    virtual void Init();
    virtual void Tick(double TimePassed);
    virtual void Draw();

    virtual bool OnTouch(int action, int id, float x, float y, float pressure, float size);
    virtual bool OnButtons(GameCoreButtonActions action, GameCoreButtonIDs id);

    virtual void Init(Vector2 center, int numslices, float length, ColorByte color1, ColorByte color2, float speed);
};

#endif // EFFECT_SunRay_Enabled

#endif //__Effect_SunRay_H__
