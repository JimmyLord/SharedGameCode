//
// Copyright (c) 2012-2014 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "SharedCommonHeader.h"
#include "Effect_ParticleRisingGlow.h"

#if Effect_ParticleRisingGlow_Enabled

Effect_ParticleRisingGlow::Effect_ParticleRisingGlow(ParticleRenderer* pRenderer)
{
    m_TimeAlive = 0;

    m_Particles.AllocateObjects( 1000 );
    for( unsigned int i=0; i<m_Particles.Length(); i++ )
    {
        ParticleRisingGlow* pObj = MyNew ParticleRisingGlow;
        m_Particles.AddInactiveObject( pObj );
    }

    m_TimeTilNextSpawn = 0;
    m_SpawnTime = 0.01f;
    m_SpawnTimeVariation = 0.01f;

    m_Center = Vector2( 320.0f, -100.0f );
    m_Width = 1000.0f;
    m_Size = 42;
    m_SizeVariation = 21;
    m_Color1 = ColorByte(255,255,255,255);
    m_Color2 = ColorByte(255,0,0,255);
    m_Speed = 40.9f;
    m_SpeedVariation = 25.23f;
    m_Sway = 30;
    m_SwayVariation = 60;

    m_pRenderer = pRenderer;
}

Effect_ParticleRisingGlow::~Effect_ParticleRisingGlow()
{
    m_Particles.DeleteAllObjectsInPool();
}

void Effect_ParticleRisingGlow::Init()
{
    //LOGInfo( LOGTag, "Effect_ParticleRisingGlow::Init()\n" );
}

void Effect_ParticleRisingGlow::FastForward(double TimeToPass)
{
    while( TimeToPass > 0 )
    {
        m_pRenderer->Reset();
        Tick( 1/30.0f );
        TimeToPass -= 1/30.0f;
    }
}

void Effect_ParticleRisingGlow::Tick(double TimePassed)
{
    m_TimeAlive += TimePassed;

    //if( m_BurstTimeLeft > 0 )
    //{
    //    m_TimeTilNextSpawn -= (float)TimePassed;
    //    while( m_TimeTilNextSpawn < 0 )
    //    {
    //        if( m_TimeTilNextSpawn < 0 )
    //        {

    m_TimeTilNextSpawn -= (float)TimePassed;
    if( m_TimeTilNextSpawn < 0 )
    {
        for( int i=0; i<2; i++ )
        {
            m_TimeTilNextSpawn = m_SpawnTime + (rand()%10000)/10000.0f * m_SpawnTimeVariation;

            ParticleRisingGlow* pParticle = m_Particles.MakeObjectActive();
            if( pParticle )
            {
                pParticle->pos.Set( m_Center.x + (rand()%(int)m_Width - m_Width/2), m_Center.y );
                pParticle->color = ColorByte( rand()%255, rand()%255, rand()%255, 255 );
                pParticle->size = m_Size + (rand()%10000)/10000.0f * m_SizeVariation;
                pParticle->speed.Set( -40, m_Speed + (rand()%10000)/10000.0f * m_SpeedVariation );
                pParticle->sway = m_Sway + (rand()%10000)/10000.0f * m_SwayVariation;
            }
        }
    }

    for( unsigned int i=0; i<m_Particles.m_ActiveObjects.Count(); i++ )
    {
        ParticleRisingGlow* pParticle = m_Particles.m_ActiveObjects[i];
        pParticle->pos.x += pParticle->speed.x * (float)TimePassed;
        pParticle->pos.y += pParticle->speed.y * (float)TimePassed;

        float speedshift = ((rand()%10000) - 5000)/10000.0f * 1000 * (float)TimePassed;
        pParticle->speed.x += speedshift;
        pParticle->speed.x -= 100;
        if( pParticle->speed.x > pParticle->sway )
            pParticle->speed.x = pParticle->sway;
        if( pParticle->speed.x < -pParticle->sway )
            pParticle->speed.x = -pParticle->sway;

        if( pParticle->pos.y > 1000 )
        {
            m_Particles.MakeObjectInactiveByIndex( i );
            i--;
        }
        else if( pParticle->pos.x < -200 )
        {
            m_Particles.MakeObjectInactiveByIndex( i );
            i--;
        }

        float size = pParticle->size;
//#if MYFW_IOS
//        size *= 2;
//#endif

        if( pParticle->size > 0 )            
            m_pRenderer->AddPoint( pParticle->pos, 0, pParticle->color, size );
    }
}

void Effect_ParticleRisingGlow::Draw()
{
    m_pRenderer->Draw( 0,0,0,0,0,0,0,0,0,0,0 );
}

bool Effect_ParticleRisingGlow::OnTouch(int action, int id, float x, float y, float pressure, float size)
{
    return false;
}

bool Effect_ParticleRisingGlow::OnButtons(GameCoreButtonActions action, GameCoreButtonIDs id)
{
    return false;
}

#endif // Effect_ParticleRisingGlow_Enabled
