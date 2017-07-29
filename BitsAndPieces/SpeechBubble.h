//
// Copyright (c) 2014-2017 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#ifndef __SpeechBubble_H__
#define __SpeechBubble_H__

class MySprite9;
class MySprite;

class SpeechBubble
{
protected:
    MySprite9* m_pBubble;
    MySprite* m_pPoint;

    const char* m_String;
    const char* m_LastString; // for fading out.

    MyMeshText* m_pTextMesh;

    FontDefinition* m_pFont;

public:
    Vector2 m_CurrentPos;
    ColorByte m_CurrentColor;
    float m_CurrentWidth;
    float m_CurrentHeight;

    Vector2 m_DestPos;
    ColorByte m_DestColor;
    float m_DestWidth;
    float m_DestHeight;

public:
    SpeechBubble(FontDefinition* pFont);
    ~SpeechBubble();

    void Tick(double timepassed);

    void SetSize(float width, float height);
    void SetPosition(Vector3 pos);
    void SetString(const char* string);
    void Draw(MyMatrix* matviewproj);
};

#endif //__SpeechBubble_H__
