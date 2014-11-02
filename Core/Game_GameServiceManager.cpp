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

#include "GameCommonHeader.h"
#include "Game_GameServiceManager.h"
#include "Core/ProfileManager.h"

Game_GameServiceManager::Game_GameServiceManager()
{
}

Game_GameServiceManager::~Game_GameServiceManager()
{
}

void Game_GameServiceManager::AddScoreToList(char* username, char* password, int gameid, int boardid, int score, const char* boardlabel, bool overwrite, GameServiceProtocol protocol)
{
    GameServiceManager::AddScoreToList( username, password, gameid, boardid, score, boardlabel, true, protocol );

    if( protocol == GameServiceProtocol_MyServer || protocol == GameServiceProtocol_All )
        g_pGame->m_pProfileManager->m_CurrentProfile->m_HighScoresSubmitted[GameServiceProtocol_MyServer][boardid] = false;

    if( protocol == GameServiceProtocol_GameCenter || protocol == GameServiceProtocol_All )
        g_pGame->m_pProfileManager->m_CurrentProfile->m_HighScoresSubmitted[GameServiceProtocol_GameCenter][boardid] = false;

//    if( protocol == GameServiceProtocol_ScoreLoop || protocol == GameServiceProtocol_All )
//        g_pGame->m_pProfileManager->m_CurrentProfile->m_HighScoresSubmitted[GameServiceProtocol_ScoreLoop][boardid] = false;
}

void Game_GameServiceManager::AddAchToList(char* username, char* password, int gameid, int achid, float perc, const char* achidstr, GameServiceProtocol protocol)
{
    GameServiceManager::AddAchToList( username, password, gameid, achid, perc, achidstr, protocol );

    if( protocol == GameServiceProtocol_MyServer || protocol == GameServiceProtocol_All )
        g_pGame->m_pProfileManager->m_CurrentProfile->m_AchievementsSubmitted[GameServiceProtocol_MyServer][achid] = false;

    if( protocol == GameServiceProtocol_GameCenter || protocol == GameServiceProtocol_All )
        g_pGame->m_pProfileManager->m_CurrentProfile->m_AchievementsSubmitted[GameServiceProtocol_GameCenter][achid] = false;

    if( protocol == GameServiceProtocol_ScoreLoop || protocol == GameServiceProtocol_All )
        g_pGame->m_pProfileManager->m_CurrentProfile->m_AchievementsSubmitted[GameServiceProtocol_ScoreLoop][achid] = false;
}

void Game_GameServiceManager::MarkLeaderboardSubmitComplete(bool success, GameServiceProtocol protocol, int boardid, int score)
{
    GameServiceManager::MarkLeaderboardSubmitComplete( success, protocol, boardid, score );

    g_pGame->m_pProfileManager->m_CurrentProfile->m_HighScoresSubmitted[protocol][boardid] = true;
    
    g_pGame->KickOffASave();
}

void Game_GameServiceManager::MarkAchSubmitComplete(bool success, GameServiceProtocol protocol, int achid)
{
    GameServiceManager::MarkAchSubmitComplete( success, protocol, achid );
    
    g_pGame->m_pProfileManager->m_CurrentProfile->m_AchievementsSubmitted[protocol][achid] = true;

    g_pGame->KickOffASave();
}
