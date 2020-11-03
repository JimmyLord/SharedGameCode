//
// Copyright (c) 2012-2017 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#ifndef __ScoreChunk_H__
#define __ScoreChunk_H__

enum ScoreChunkAnimType
{
    SCAT_None,
    SCAT_MoveUpAndVanish,
    SCAT_SitThenMoveToDest,
    SCAT_SitThenVanish,
};

class ScoreChunk
{
    static const int MaxString = 64;
public:
    MyActivePool<ScoreChunk*>* m_pScoreChunkPool;

    FontDefinition* m_pFont;
    MyMeshText* m_pTextMesh;
    char m_String[MaxString];
    int m_Value;
    float m_FontHeight;
    float m_X;
    float m_Y;
    float m_StartX;
    float m_StartY;
    float m_DestX;
    float m_DestY;
    ColorByte m_Color;
    float m_Size;
    unsigned char m_Justification;

    ColorByte m_ShadowColor;
    int m_ShadowOffsetX; // -1 for 4 shadow "outline"
    int m_ShadowOffsetY; // if x == -1, y is offset

    float m_Angle;

    ScoreChunkAnimType m_AnimType;
    float m_InitialDelay;
    float m_MoveSpeed;
    float m_SitTime;
    float m_TimeAlive;
    float m_TimeToLive;

    ScoreChunk(MyActivePool<ScoreChunk*>* pPool);

    void Reset();
    void Setup( MyMeshText* pTextMesh, float x, float y, float fontheight, ColorByte color, ScoreChunkAnimType animtype, float duration, unsigned char justification, const char* string, int value );
    void SetupWithDest( MyMeshText* pTextMesh, float x, float y, float dx, float dy, float fontheight, ColorByte color, ScoreChunkAnimType animtype, float duration, float sittime, float movespeed, unsigned char justification, const char* string, int value );

    void Tick(float TimePassed);
    void Draw();

    bool IsActive();
};

#endif //__ScoreChunk_H__
