//
// Copyright (c) 2012-2014 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "GameCommonHeader.h"
#include "Effect_ParticleBurst.h"

#if Effect_ParticleBurst_Enabled

Effect_ParticleBurst::Effect_ParticleBurst(ParticleRenderer* pRenderer)
{
    m_TimeAlive = 0;

    m_Particles.AllocateObjects( 1000 );
    for( unsigned int i=0; i<m_Particles.Length(); i++ )
    {
        ParticleBurst* pObj = MyNew ParticleBurst;
        m_Particles.AddInactiveObject( pObj );
    }

    m_TimeTilNextSpawn = 0;

    m_BurstDuration = 100;

    m_BurstTimeLeft = m_BurstDuration;
    
    m_SpawnTime = 0.1f;
    m_SpawnTimeVariation = 0.1f;
    m_Center = Vector2( 320.0f, 800.0f );
    m_CenterVariation = Vector2( 1, 1 );
    m_InitialSpeedBoost = 1;
    m_Size = 42;
    m_SizeVariation = 21;
    m_ScaleSpeed = -1.0f;
    m_Color1 = ColorByte(255,255,255,255);
    m_Color2 = ColorByte(255,0,0,255);
    m_UseColorsAsOptions = false;
    m_ColorTransitionDelay = 0;
    m_ColorTransitionSpeed = -1.0f;
    m_Speed = 0;
    m_SpeedVariation = 75.23f;
    m_TimeToLive = 0.5f;
    m_TimeToLiveVariation = 0.0f;

    m_AlphaModifier = 1.0f;

    m_pRenderer = pRenderer;
}

Effect_ParticleBurst::~Effect_ParticleBurst()
{
    m_Particles.DeleteAllObjectsInPool();
}

void Effect_ParticleBurst::Init()
{
    //LOGInfo( LOGTag, "Effect_ParticleBurst::Init()\n" );
}

void Effect_ParticleBurst::CreateBurst(int number, Vector2 pos)
{
    while( number )
    {
        number--;

        ParticleBurst* pParticle = m_Particles.MakeObjectActive();
        if( pParticle )
        {
            pParticle->pos.x = pos.x;
            pParticle->pos.y = pos.y;
            if( m_CenterVariation.x != 0 )
                pParticle->pos.x += (rand()%(int)m_CenterVariation.x - m_CenterVariation.x/2);
            if( m_CenterVariation.y != 0 )
                pParticle->pos.y += (rand()%(int)m_CenterVariation.y - m_CenterVariation.y/2);
            pParticle->size = m_Size + (rand()%10000)/10000.0f * m_SizeVariation;

            if( m_UseColorsAsOptions )
            {
                int color = rand()%2;
                if( color == 0 )
                    pParticle->color = m_Color1;
                else //if( color == 1 )
                    pParticle->color = m_Color2;
            }
            else
            {
                pParticle->color = ColorByte( rand()%255, rand()%255, rand()%255, 255 );
            }

            pParticle->speed.x = m_Speed + (rand()%10000-5000)/10000.0f * m_SpeedVariation;
            pParticle->speed.y = m_Speed + (rand()%10000-5000)/10000.0f * m_SpeedVariation;
            pParticle->timealive = 0;
            pParticle->timetolive = m_TimeToLive;
            if( m_TimeToLiveVariation != 0 )
                pParticle->timetolive += (rand()%10000-5000)/10000.0f * m_TimeToLiveVariation;

            //if( m_InitialSpeedBoost )
            {
                pParticle->pos.x += pParticle->speed.x * m_InitialSpeedBoost;
                pParticle->pos.y += pParticle->speed.y * m_InitialSpeedBoost;
            }
        }
    }
}

void Effect_ParticleBurst::Tick(double TimePassed)
{
    m_TimeAlive += TimePassed;

    for( unsigned int i=0; i<m_Particles.m_ActiveObjects.Count(); i++ )
    {
        ParticleBurst* pParticle = m_Particles.m_ActiveObjects[i];
        pParticle->pos.x += pParticle->speed.x * (float)TimePassed;
        pParticle->pos.y += pParticle->speed.y * (float)TimePassed;
        
        pParticle->timealive += (float)TimePassed;

        float perc = pParticle->timealive / pParticle->timetolive;
        MyClamp( perc, 0.0f, 1.0f );
        //float antiperc = 1 - perc;

        ColorByte color;
        float tempperc = (perc - m_ColorTransitionDelay) / (1 - m_ColorTransitionDelay);
        MyClamp( tempperc, 0.0f, 1.0f );
        color.r = (unsigned char)(pParticle->color.r + pParticle->color.r * m_ColorTransitionSpeed * tempperc);
        color.g = (unsigned char)(pParticle->color.g + pParticle->color.g * m_ColorTransitionSpeed * tempperc);
        color.b = (unsigned char)(pParticle->color.b + pParticle->color.b * m_ColorTransitionSpeed * tempperc);
        color.a = (unsigned char)(pParticle->color.a + pParticle->color.a * m_ColorTransitionSpeed * tempperc);

        color.a = (unsigned char)(color.a * m_AlphaModifier);

        //LOGInfo( LOGTag, "%f %d\n", tempperc, color.a );

        float size = pParticle->size + pParticle->size * m_ScaleSpeed * perc;
//#if MYFW_IOS
//        size *= 2;
//#endif
        //float size = pParticle->size * antiperc;

        if( pParticle->timealive > pParticle->timetolive )
        {
            m_Particles.MakeObjectInactiveByIndex( i );
            i--;
        }

        //Vector2 pos( pParticle->pos.x + size/2, pParticle->pos.y + size/2 );
        Vector2 pos( pParticle->pos.x, pParticle->pos.y ); //+ size/4 );

        if( size > 0 )            
            m_pRenderer->AddPoint( pos, 0, color, size );
    }
}

void Effect_ParticleBurst::Draw()
{
    m_pRenderer->Draw( 0 );
}

bool Effect_ParticleBurst::OnTouch(int action, int id, float x, float y, float pressure, float size)
{
    return false;
}

bool Effect_ParticleBurst::OnButtons(GameCoreButtonActions action, GameCoreButtonIDs id)
{
    return false;
}

#endif // Effect_ParticleBurst_Enabled
