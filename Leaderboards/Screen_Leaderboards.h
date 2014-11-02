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

#ifndef __Screen_Leaderboards_H__
#define __Screen_Leaderboards_H__

#include "../../../SharedGameCode/Screens/Screen_Base.h"
#include "Core/LeaderboardsConfig.h"

class Screen_Leaderboards;
extern Screen_Leaderboards* g_pScreen_Leaderboards;

enum LeaderboardActions
{
    LeaderboardActions_Login,
    LeaderboardActions_SwitchBoard_Prev,
    LeaderboardActions_SwitchBoard_Next,
    LeaderboardActions_JumpToTopOrPlayer,
    LeaderboardActions_GameCenter,
    LeaderboardActions_Back,
    LeaderboardActions_NumButtons,
};

//enum LeaderboardFilterTime
//{
//    HSFT_Round,
//    HSFT_Today,
//    HSFT_AllTime,
//    HSFT_NumOptions,
//};

enum LeaderboardFilterPlayers
{
    LeaderboardFilterPlayers_Top,
    LeaderboardFilterPlayers_User,
    LeaderboardFilterPlayers_NumOptions,
};

class Screen_Leaderboards : public Screen_Base
{
public:
    float m_ScrollAmount;
    float m_Momentum;
    bool m_WeScrolledTheList;

    int m_InitialFinger;

    LeaderboardBoards m_Board;
    //LeaderboardFilterTime m_FilterTime;
    LeaderboardFilterPlayers m_FilterPlayers;

    int m_ScoreIndexAtTop;
    int m_ScoreRangeOnScreen;

    bool m_WaitingForInfo;

public:
    Screen_Leaderboards();
    virtual ~Screen_Leaderboards();

    virtual void Init();
    virtual bool IsSettled();
    virtual void Tick(double TimePassed);
    virtual void Draw();

    virtual bool OnTouch(int action, int id, float x, float y, float pressure, float size);
    virtual bool OnButtons(GameCoreButtonActions action, GameCoreButtonIDs id);

    bool OnMenuAction(int action);
};

#endif //__Screen_Leaderboards_H__
