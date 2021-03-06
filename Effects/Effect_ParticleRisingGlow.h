//
// Copyright (c) 2012-2014 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#ifndef __Effect_ParticleRisingGlow_H__
#define __Effect_ParticleRisingGlow_H__

struct ParticleRisingGlow
{
    Vector2 pos;
    float size;
    ColorByte color;
    Vector2 speed;
    float sway;
};

class Effect_ParticleRisingGlow
{
public:
    double m_TimeAlive;

    ParticleRenderer* m_pRenderer;

    MyActivePool<ParticleRisingGlow*> m_Particles;

    Vector2 m_Center;
    float m_Width;

    float m_TimeTilNextSpawn;
    float m_SpawnTime;
    float m_SpawnTimeVariation;

    float m_Size;
    float m_SizeVariation;
    ColorByte m_Color1;
    ColorByte m_Color2;
    float m_Speed;
    float m_SpeedVariation;
    float m_Sway;
    float m_SwayVariation;

public:
    Effect_ParticleRisingGlow(ParticleRenderer* pRenderer);
    virtual ~Effect_ParticleRisingGlow();

    virtual void Init();
    virtual void Tick(double TimePassed);
    virtual void Draw();

    virtual bool OnTouch(int action, int id, float x, float y, float pressure, float size);
    virtual bool OnButtons(GameCoreButtonActions action, GameCoreButtonIDs id);

    void FastForward(double TimeToPass);
};

#endif //__Effect_ParticleRisingGlow_H__
