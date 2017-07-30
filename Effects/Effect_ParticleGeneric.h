//
// Copyright (c) 2012-2017 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#ifndef __Effect_ParticleGeneric_H__
#define __Effect_ParticleGeneric_H__

struct ParticleGeneric
{
    ParticleRenderer* particlerenderer;
    Vector2 pos;
    float size;
    ColorByte color;
    Vector2 speed;
    Vector2 sway;
    Vector2 maxspeed;
    float timealive;
    float timetolive;
    float fadeintime;
    float fadeouttime;
};

class Effect_ParticleGeneric
{
public:
    double m_TimeAlive;

    ParticleRenderer* m_pRenderer;

    MyActivePool<ParticleGeneric*> m_Particles;

    Vector2 m_Center;
    float m_Width;
    float m_Height;

    float m_TimeTilNextSpawn;
    float m_SpawnTime;
    float m_SpawnTimeVariation;

    float m_Size;
    float m_SizeVariation;

    ColorByte m_Color1;
    ColorByte m_Color2;
    bool m_UseColorsAsOptions;
    float m_ColorTransitionDelay;
    float m_ColorTransitionSpeed;

    Vector2 m_Speed;
    Vector2 m_SpeedVariation;
    Vector2 m_MaxSpeed;
    Vector2 m_MaxSpeedVariation;
    Vector2 m_Sway;
    Vector2 m_SwayVariation;

    float m_TimeToLive;
    float m_TimeToLiveVariation;
    float m_FadeInTime;
    float m_FadeOutTime;

    float m_AlphaModifier;

    bool m_UseConstraints;
    Vector4 m_Constraints; // top/right/bottom/left

public:
    Effect_ParticleGeneric(ParticleRenderer* pRenderer);
    virtual ~Effect_ParticleGeneric();

    virtual void Init();
    void FastForward(double TimeToPass);
    void AddParticles(int numtoadd);
    virtual void Tick(double TimePassed);
    virtual void Draw(MyMatrix* matviewproj);

    virtual bool OnTouch(int action, int id, float x, float y, float pressure, float size);
    virtual bool OnButtons(GameCoreButtonActions action, GameCoreButtonIDs id);
};

#endif //__Effect_ParticleGeneric_H__
