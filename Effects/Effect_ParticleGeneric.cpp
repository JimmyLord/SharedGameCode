//
// Copyright (c) 2012-2017 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "SharedCommonHeader.h"
#include "Effect_ParticleGeneric.h"

Effect_ParticleGeneric::Effect_ParticleGeneric(ParticleRenderer* pRenderer)
{
    m_TimeAlive = 0;

    m_Particles.AllocateObjects( 1000 );
    for( unsigned int i=0; i<m_Particles.Length(); i++ )
    {
        ParticleGeneric* pObj = MyNew ParticleGeneric;
        m_Particles.AddInactiveObject( pObj );
    }

    m_TimeTilNextSpawn = 0;
    m_SpawnTime = 0.01f;
    m_SpawnTimeVariation = 0.01f;

    m_Center = Vector2( 320.0f, -100.0f );
    m_Width = 1000.0f;
    m_Height = 1;
    m_Size = 42;
    m_SizeVariation = 21;

    m_Color1 = ColorByte(255,255,255,255);
    m_Color2 = ColorByte(255,0,0,255);
    m_UseColorsAsOptions = false;
    m_ColorTransitionDelay = 0;
    m_ColorTransitionSpeed = -1.0f;

    m_Speed.Set( -40, 40.9f );
    m_SpeedVariation.Set( 0, 25.23f );
    m_MaxSpeed.Set( 30, 0 );
    m_MaxSpeedVariation.Set( 60, 0 );
    m_Sway.Set( 0, 0 );
    m_SwayVariation.Set( 0, 0 );

    m_TimeToLive = 0;
    m_TimeToLiveVariation = 0;
    m_FadeInTime = 0.2f;
    m_FadeOutTime = 0.2f;

    m_AlphaModifier = 1.0f;

    m_UseConstraints = true;
    m_Constraints.Set( 1000, -1000, 1000, -200 );

    m_pRenderer = pRenderer;
}

Effect_ParticleGeneric::~Effect_ParticleGeneric()
{
    m_Particles.DeleteAllObjectsInPool();
}

void Effect_ParticleGeneric::Init()
{
    //LOGInfo( LOGTag, "Effect_ParticleGeneric::Init()\n" );
}

void Effect_ParticleGeneric::FastForward(double TimeToPass)
{
    while( TimeToPass > 0 )
    {
        m_pRenderer->Reset();
        Tick( 1/30.0f );
        TimeToPass -= 1/30.0f;
    }
}

void Effect_ParticleGeneric::AddParticles(int numtoadd)
{
    for( int i=0; i<numtoadd; i++ )
    {
        ParticleGeneric* pParticle = m_Particles.MakeObjectActive();
        if( pParticle )
        {
            pParticle->particlerenderer = m_pRenderer;

            pParticle->pos.Set( m_Center.x + (rand()%(int)m_Width - m_Width/2), m_Center.y + (rand()%(int)m_Height - m_Height/2) );

            if( m_UseColorsAsOptions )
            {
                int color = rand()%2;
                if( color == 0 )
                    pParticle->color = m_Color1;
                if( color == 1 )
                    pParticle->color = m_Color2;
            }
            else
            {
                pParticle->color = ColorByte( rand()%255, rand()%255, rand()%255, 255 );
            }

            pParticle->size = m_Size + ((rand()%10000)/10000.0f - 0.5f) * m_SizeVariation;
            pParticle->speed.x = m_Speed.x + ((rand()%10000)/10000.0f - 0.5f) * m_SpeedVariation.x;
            pParticle->speed.y = m_Speed.y + ((rand()%10000)/10000.0f - 0.5f) * m_SpeedVariation.y;
            pParticle->maxspeed.x = m_MaxSpeed.x + ((rand()%10000)/10000.0f - 0.5f) * m_MaxSpeedVariation.x;
            pParticle->maxspeed.y = m_MaxSpeed.y + ((rand()%10000)/10000.0f - 0.5f) * m_MaxSpeedVariation.y;

            pParticle->timealive = 0;
            pParticle->timetolive = m_TimeToLive;
            if( m_TimeToLiveVariation != 0 )
                pParticle->timetolive += (rand()%10000-5000)/10000.0f * m_TimeToLiveVariation;

            pParticle->fadeintime = m_FadeInTime;
            pParticle->fadeouttime = m_FadeOutTime;
        }
    }
}

void Effect_ParticleGeneric::Tick(double TimePassed)
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
    if( m_SpawnTime != 0 && m_TimeTilNextSpawn < 0 )
    {
        m_TimeTilNextSpawn = m_SpawnTime + (rand()%10000)/10000.0f * m_SpawnTimeVariation;

        AddParticles( 1 );
    }

    for( unsigned int i=0; i<m_Particles.m_ActiveObjects.Count(); i++ )
    {
        ParticleGeneric* pParticle = m_Particles.m_ActiveObjects[i];
        pParticle->pos.x += pParticle->speed.x * (float)TimePassed;
        pParticle->pos.y += pParticle->speed.y * (float)TimePassed;

        //float speedshift = ((rand()%10000) - 5000)/10000.0f * 1000 * (float)TimePassed;
        //pParticle->speed.x += speedshift;
        //pParticle->speed.x -= 100;
        if( pParticle->speed.x > pParticle->maxspeed.x )
            pParticle->speed.x = pParticle->maxspeed.x;
        if( pParticle->speed.x < -pParticle->maxspeed.x )
            pParticle->speed.x = -pParticle->maxspeed.x;
        if( pParticle->speed.y > pParticle->maxspeed.y )
            pParticle->speed.y = pParticle->maxspeed.y;
        if( pParticle->speed.y < -pParticle->maxspeed.y )
            pParticle->speed.y = -pParticle->maxspeed.y;

        if( m_UseConstraints )
        {
            if( pParticle->pos.y > m_Constraints.x || pParticle->pos.y < m_Constraints.z ||  // top bottom
                pParticle->pos.x > m_Constraints.y || pParticle->pos.x < m_Constraints.w ) // right left
            {
                m_Particles.MakeObjectInactiveByIndex( i );
                i--;
            }
        }

        pParticle->timealive += (float)TimePassed;

        if( pParticle->timetolive > 0 && pParticle->timealive > pParticle->timetolive )
        {
            m_Particles.MakeObjectInactiveByIndex( i );
            i--;
        }

        float perc = pParticle->timealive / pParticle->timetolive;
        MyClamp( perc, 0.0f, 1.0f );

        ColorByte color;
        float tempperc = (perc - m_ColorTransitionDelay) / (1 - m_ColorTransitionDelay);
        MyClamp( tempperc, 0.0f, 1.0f );
        color.r = (unsigned char)(pParticle->color.r + pParticle->color.r * m_ColorTransitionSpeed * tempperc);
        color.g = (unsigned char)(pParticle->color.g + pParticle->color.g * m_ColorTransitionSpeed * tempperc);
        color.b = (unsigned char)(pParticle->color.b + pParticle->color.b * m_ColorTransitionSpeed * tempperc);
        color.a = (unsigned char)(pParticle->color.a + pParticle->color.a * m_ColorTransitionSpeed * tempperc);

        color.a = (unsigned char)(color.a * m_AlphaModifier);

        if( pParticle->timealive < pParticle->fadeintime )
        {
            color *= pParticle->timealive / pParticle->fadeintime;
        }
        if( pParticle->timealive > pParticle->timetolive - pParticle->fadeouttime )
        {
            color *= pParticle->timealive / (pParticle->timetolive - pParticle->fadeouttime);
        }

        float size = pParticle->size;

        if( pParticle->size > 0 )            
            pParticle->particlerenderer->AddPoint( pParticle->pos, 0, color, size );
    }
}

void Effect_ParticleGeneric::Draw(MyMatrix* matviewproj)
{
    m_pRenderer->Draw( 0, 0, matviewproj, 0, 0, 0, 0, 0, 0, 0, 0 );
}

bool Effect_ParticleGeneric::OnTouch(int action, int id, float x, float y, float pressure, float size)
{
    return false;
}

bool Effect_ParticleGeneric::OnButtons(GameCoreButtonActions action, GameCoreButtonIDs id)
{
    return false;
}
