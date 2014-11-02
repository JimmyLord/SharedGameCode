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

#ifndef __Game_GameServiceManager_H__
#define __Game_GameServiceManager_H__

#include "../../../Framework/MyFramework/SourceCommon/Networking/GameServiceManager.h"

class Game_GameServiceManager : public GameServiceManager
{
protected:

public:
    Game_GameServiceManager();
    virtual ~Game_GameServiceManager();

    virtual void AddScoreToList(char* username, char* password, int gameid, int boardid, int score, const char* boardlabel, bool overwrite, GameServiceProtocol protocol);
    virtual void AddAchToList(char* username, char* password, int gameid, int achid, float perc, const char* achidstr, GameServiceProtocol protocol);
    
    virtual void MarkLeaderboardSubmitComplete(bool success, GameServiceProtocol protocol, int boardid, int score);
    virtual void MarkAchSubmitComplete(bool success, GameServiceProtocol protocol, int achid);
};

#endif //__Game_GameServiceManager_H__
