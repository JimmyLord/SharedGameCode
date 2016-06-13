//
// Copyright (c) 2012-2014 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "SharedCommonHeader.h"
#include "ScoreChunk.h"

void ScoreChunk::Reset()
{
    m_pFont = 0;
    m_String[0] = 0;
    m_Value = 0;
    m_FontHeight = 0;
    m_X = -10000;
    m_Y = -10000;
    m_Size = 0;

    m_Color = ColorByte(255, 255, 255, 255);
    m_ShadowColor = ColorByte(0, 0, 0, 128);

    m_ShadowOffsetX = 0;
    m_ShadowOffsetY = 0;

    m_Angle = 0;

    m_InitialDelay = 0;
    m_MoveSpeed = 10;
    m_SitTime = 0;
    m_TimeAlive = 0;
    m_TimeToLive = 1;
}

void ScoreChunk::Setup( MyMeshText* pTextMesh, float x, float y, float fontheight, ColorByte color, ScoreChunkAnimType animtype, float duration, unsigned char justification, const char* string, int value )
{
    SetupWithDest( pTextMesh, x, y, x, y, fontheight, color, animtype, duration, 0, 10, justification, string, value );
}

void ScoreChunk::SetupWithDest( MyMeshText* pTextMesh, float x, float y, float dx, float dy, float fontheight, ColorByte color, ScoreChunkAnimType animtype, float duration, float sittime, float movespeed, unsigned char justification, const char* string, int value )
{
    Reset();

    m_pTextMesh = pTextMesh;

    m_FontHeight = fontheight;

    m_StartX = m_X = x;
    m_StartY = m_Y = y;

    m_DestX = dx;
    m_DestY = dy;

    strcpy_s( m_String, MaxString, string );

    m_Value = value;
    m_Color = color;

    m_AnimType = animtype;
    m_MoveSpeed = movespeed;
    m_SitTime = sittime;
    m_TimeToLive = duration;

    m_Justification = justification;
}

void ScoreChunk::Tick(double TimePassed)
{
    float timepassed = (float)TimePassed;
    m_TimeAlive += timepassed;

    if( m_TimeAlive < m_InitialDelay )
        return;

    float timealive = m_TimeAlive - m_InitialDelay;

    if( m_AnimType == SCAT_MoveUpAndVanish )
    {
        m_Y += m_MoveSpeed * timepassed;
        m_Color.a = (unsigned char)(255 * (m_TimeToLive-timealive)/m_TimeToLive);
        m_ShadowColor.a = (unsigned char)(255 * (m_TimeToLive-timealive)/m_TimeToLive * 0.75f);
    }
    else if( m_AnimType == SCAT_SitThenMoveToDest ) // will only work if duration is greater than sittime.
    {
        float sittime = m_SitTime;
        if( timealive < sittime ) // sit for the first second
        {
        }
        else
        {
            float timetomove = m_TimeToLive - sittime;
            float perctowarddest = (timealive - sittime) / timetomove;

            m_X = m_StartX + (m_DestX - m_StartX)*perctowarddest;
            m_Y = m_StartY + (m_DestY - m_StartY)*perctowarddest;
            m_Color.a = (unsigned char)(255 * (1.0f - perctowarddest));
            m_ShadowColor.a = (unsigned char)(255 * (1.0f - perctowarddest));
        }
    }
    else if( m_AnimType == SCAT_SitThenVanish )
    {
        //m_Angle += (float)TimePassed * 100.0f;
        //return;
    }

    if( timealive > m_TimeToLive )
    {
        g_pGame->m_ObjectPool_ScoreChunks.MakeObjectInactive( this );
    }
}

void ScoreChunk::Draw()
{
    if( m_TimeAlive < m_InitialDelay )
        return;

    float posx = m_X;
    float posy = m_Y;

    //int scrw = (int)g_pGame->m_GameWidth;
    //int scrh = (int)g_pGame->m_GameHeight;
    float fontheight = m_FontHeight;

    char temptext[MaxString];
    sprintf_s( temptext, MaxString, m_String, m_Value );

    if( m_ShadowOffsetX != 0 || m_ShadowOffsetY != 0 )
    {
        if( m_ShadowOffsetX == -1 )
        {
            m_pTextMesh->CreateStringColorZAndRot( true, fontheight, posx - m_ShadowOffsetY, posy - m_ShadowOffsetY, 0, m_Angle, m_Justification, m_ShadowColor, Vector2(0,0), temptext );
            m_pTextMesh->CreateStringColorZAndRot( true, fontheight, posx + m_ShadowOffsetY, posy - m_ShadowOffsetY, 0, m_Angle, m_Justification, m_ShadowColor, Vector2(0,0), temptext );
            m_pTextMesh->CreateStringColorZAndRot( true, fontheight, posx - m_ShadowOffsetY, posy + m_ShadowOffsetY, 0, m_Angle, m_Justification, m_ShadowColor, Vector2(0,0), temptext );
            m_pTextMesh->CreateStringColorZAndRot( true, fontheight, posx + m_ShadowOffsetY, posy + m_ShadowOffsetY, 0, m_Angle, m_Justification, m_ShadowColor, Vector2(0,0), temptext );
        }
        else
        {
            m_pTextMesh->CreateStringColorZAndRot( true, fontheight, posx + m_ShadowOffsetX, posy + m_ShadowOffsetY, 0, m_Angle, m_Justification, m_ShadowColor, Vector2(0,0), temptext );
        }
    }

    m_pTextMesh->CreateStringColorZAndRot( true, fontheight, posx, posy, 0, m_Angle, m_Justification, m_Color, Vector2(0,0), temptext );
}

bool ScoreChunk::IsActive()
{
    if( m_TimeAlive > m_TimeToLive + m_InitialDelay )
        return false;

    return true;
}
