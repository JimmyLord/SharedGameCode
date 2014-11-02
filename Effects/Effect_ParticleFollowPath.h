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

#ifndef __Effect_ParticleFollowPath_H__
#define __Effect_ParticleFollowPath_H__

#if Effect_ParticleFollowPath_Enabled

#define Effect_ParticleFollowPath_MaxPathNodes  10

struct ParticleFollowPath
{
    Vector3 pos;
    float size;
    ColorByte color;
    Vector3 speed;
    float timealive;
    float timetolive;
};

class Effect_ParticleFollowPath
{
public:
    double m_TimeAlive;

    ParticleRenderer* m_pRenderer;

    MyActivePool<ParticleFollowPath*> m_Particles;

    float m_TimeTilNextSpawn;
    float m_BurstTimeLeft;

    float m_BurstDuration;

    float m_SpawnTime;
    float m_SpawnTimeVariation;
    float m_InitialSpeedBoost;
    Vector2 m_Center;
    Vector2 m_CenterVariation;
    float m_Size;
    float m_SizeVariation;
    float m_ScaleSpeed;
    ColorByte m_Color1;
    ColorByte m_Color2;
    bool m_UseColorsAsOptions;
    float m_ColorTransitionDelay;
    float m_ColorTransitionSpeed;
    float m_Speed;
    float m_SpeedVariation;
    float m_TimeToLive;
    float m_TimeToLiveVariation;

    int m_NumPathNodes;
    Vector3 m_PathNodes[Effect_ParticleFollowPath_MaxPathNodes];
    Vector3 m_PathOffset;

    float m_SpawnTimeLeft;

    float m_AlphaModifier;

public:
    Effect_ParticleFollowPath(ParticleRenderer* pRenderer);
    virtual ~Effect_ParticleFollowPath();

    virtual void Init();
    virtual void Tick(double TimePassed);
    virtual void Draw();

    virtual bool OnTouch(int action, int id, float x, float y, float pressure, float size);
    virtual bool OnButtons(GameCoreButtonActions action, GameCoreButtonIDs id);

    void Start(int number, float spawnduration);
    void AddBurst(int number);
};

#endif // Effect_ParticleFollowPath_Enabled

#endif //__Effect_ParticleFollowPath_H__
