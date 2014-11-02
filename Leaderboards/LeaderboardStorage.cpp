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

#include "LeaderboardStorage.h"
#include "Core/LeaderboardsConfig.h"
#include "Core/ProfileManager.h"

LeaderboardStorage::LeaderboardStorage()
{
    m_pWebRequestObject = 0;
    m_TimeSinceLastRequestSent = 0;
    m_MaxTimeToWaitToWebResponse = 30;

    for( int i=0; i<NumLeaderboardStorageBlocksCached; i++ )
    {
        m_Blocks[i].m_InUse = false;
    }

    for( int i=0; i<Leaderboard_NumBoards; i++ )
    {
        m_UserIndices[i] = -1;
    }

    m_RequestedOffset = -1;
    m_RequestedBoardID = -1;
}

LeaderboardStorage::~LeaderboardStorage()
{
    SAFE_DELETE(m_pWebRequestObject);
}

void LeaderboardStorage::Init()
{
    m_pWebRequestObject = MyNew WebRequestObject;
    //m_pWebRequestObject->Init( "www.flatheadgames.com", 80 );
    m_pWebRequestObject->Init( "208.83.209.13", 80 );    
}

bool LeaderboardStorage::HasANetworkErrorOccured()
{
    return m_pWebRequestObject->DidSomethingGoWrong();
}

void LeaderboardStorage::Tick(double TimePassed)
{
    if( m_pWebRequestObject )
        m_pWebRequestObject->Tick();
    
    m_TimeSinceLastRequestSent += (float)TimePassed;
    if( m_TimeSinceLastRequestSent > m_MaxTimeToWaitToWebResponse )
    {
        m_pWebRequestObject->Reset();
    }
    
    if( m_pWebRequestObject->DidSomethingGoWrong() )
        return;

    if( m_RequestedBoardID >= 0 && m_pWebRequestObject->IsBusy() == false && m_pWebRequestObject->GetResult() == 0 )
        GetScoreBlock();

    // invalidate all blocks older than 3 minutes.
    if( true )
    {
        for( int i=0; i<NumLeaderboardStorageBlocksCached; i++ )
        {
            if( m_Blocks[i].m_InUse )
            {
                double age = MyTime_GetSystemTime() - m_Blocks[i].m_TimeRetrieved;
                if( age > 3*60 )
                    m_Blocks[i].m_InUse = false;
            }
        }
    }

    // if we received anything from the web, parse it and store it.
    if( m_pWebRequestObject->IsBusy() == false )
    {
        char* result = m_pWebRequestObject->GetResult();
        if( result )
        {
            cJSON* root = cJSON_Parse( result );
            if( root )
            {
                cJSON* obj;

                obj = cJSON_GetObjectItem( root, "NumEntries" );
                if( obj )
                {
                    g_pGame->m_pLeaderboardStorage->m_TotalPlayers[m_RequestedBoardID] = obj->valueint;
                }

                // read in "UserOffset", only returned if we are looking for a user.
                obj = cJSON_GetObjectItem( root, "UserOffset" );
                if( obj )
                {
                    m_UserIndices[m_RequestedBoardID] = obj->valueint - 1;
                }

                int blockoffset = m_RequestedOffset;
                obj = cJSON_GetObjectItem( root, "BlockOffset" );
                if( obj )
                {
                    blockoffset = obj->valueint;
                }

                int boardid = m_RequestedBoardID;

                // find a block of names and scores to store this info into.
                int blocktouse = FindDuplicateBlockOrReturnNew( blockoffset, boardid );
                m_Blocks[blocktouse].m_InUse = true;
                m_Blocks[blocktouse].m_Offset = blockoffset;
                m_Blocks[blocktouse].m_BoardID = boardid;
                m_Blocks[blocktouse].m_TimeRetrieved = MyTime_GetSystemTime();
                m_Blocks[blocktouse].m_NumEntriesInBlock = 0;

                cJSON* NameArray = cJSON_GetObjectItem( root, "UserNames" );
                if( NameArray )
                {
                    m_Blocks[blocktouse].m_NumEntriesInBlock = cJSON_GetArraySize( NameArray );

                    for( int i=0; i<m_Blocks[blocktouse].m_NumEntriesInBlock; i++ )
                    {
                        LeaderboardBlockEntry* pPlayer = &m_Blocks[blocktouse].m_Entries[i];
                        cJSON* temp;
                        temp = cJSON_GetArrayItem( NameArray, i ); if( temp ) strcpy_s( pPlayer->m_Username, 15+1, temp->valuestring );

                        // if we find our own name, then store it's index.
                        if( _stricmp( temp->valuestring, g_pGame->m_pProfileManager->m_CurrentProfile->m_Username ) == 0 )
                            m_UserIndices[m_RequestedBoardID] = blockoffset + i;
                    }

                    // if we got names, we should get Scores.
                    cJSON* ScoreArray = cJSON_GetObjectItem( root, "Scores" );
                    if( ScoreArray )
                    {
                        int numitems = cJSON_GetArraySize( ScoreArray );

                        for( int i=0; i<numitems; i++ )
                        {
                            LeaderboardBlockEntry* pPlayer = &m_Blocks[blocktouse].m_Entries[i];
                            cJSON* temp;
                            temp = cJSON_GetArrayItem( ScoreArray, i );
                            if( temp )
                                pPlayer->m_Score = temp->valueint;
                        }
                    }
                }

                cJSON_Delete( root );
            }
            
            m_RequestedOffset = -1;
            m_RequestedBoardID = -1;

            m_pWebRequestObject->ClearResult();
        }
    }
}

int LeaderboardStorage::FindBlockContainingID(int offset, int boardid)
{
    int baseindex = (offset/LeaderboardStorageBlockSize) * LeaderboardStorageBlockSize;

    // check for old block containing this index.
    for( int i=0; i<NumLeaderboardStorageBlocksCached; i++ )
    {
        if( m_Blocks[i].m_InUse == true && m_Blocks[i].m_BoardID == boardid )
        {
            if( m_Blocks[i].m_Offset == baseindex )
            {
                if( offset < m_Blocks[i].m_Offset + m_Blocks[i].m_NumEntriesInBlock )
                    return i;
                else
                    return i; // even though the block doesn't have the record we want, return it if we have it.
                    //return -1; // we have the block loaded, but it doesn't contain the entry.
            }
        }
    }

    // didn't find block, request it... assuming no other blocks are requested.
    if( m_pWebRequestObject->IsBusy() == false && m_pWebRequestObject->GetResult() == 0 )
    {
        m_RequestedOffset = offset;
        m_RequestedBoardID = boardid;
    }

    return -1;
}

void LeaderboardStorage::GetScoreBlock()
{
    if( m_pWebRequestObject->IsBusy() == false && m_pWebRequestObject->GetResult() == 0 )
    {
        int offset = m_RequestedOffset;
        int boardid = m_RequestedBoardID;

        // request a page of scores.
        char page[255];
        if( offset == -1 )
        {
            sprintf_s( page, 255, "/mygameservice/leaderboard_get.php?username=%s&gameid=%d&boardid=%d",
                       g_pGame->m_pProfileManager->m_CurrentProfile->m_Username,
                       LeaderboardGameID,
                       boardid );
        }
        else
        {
            sprintf_s( page, 255, "/mygameservice/leaderboard_get.php?offset=%d&gameid=%d&boardid=%d",
                       offset,
                       LeaderboardGameID,
                       boardid );
        }

        m_pWebRequestObject->RequestWebPage( page );
        m_TimeSinceLastRequestSent = 0;
    }
}

int LeaderboardStorage::FindDuplicateBlockOrReturnNew(int offset, int boardid)
{
    // check for old block containing this index.
    for( int i=0; i<NumLeaderboardStorageBlocksCached; i++ )
    {
        if( m_Blocks[i].m_InUse == true )
        {
            if( m_Blocks[i].m_Offset == offset && 
                m_Blocks[i].m_BoardID == boardid )
                return i;
        }
    }

    // find an empty block.
    for( int i=0; i<NumLeaderboardStorageBlocksCached; i++ )
    {
        if( m_Blocks[i].m_InUse == false )
        {
            return i;
        }
    }

    // find the oldest block in our list:
    double oldesttime = MyTime_GetSystemTime();
    int oldestblock = 0;
    for( int i=0; i<NumLeaderboardStorageBlocksCached; i++ )
    {
        if( m_Blocks[i].m_TimeRetrieved < oldesttime )
        {
            oldesttime = m_Blocks[i].m_TimeRetrieved;
            oldestblock = i;
        }
    }

    return oldestblock;

    //// shouldn't hit this, but if it does, just use the first block as a failsafe.
    //return 0;
}