//
// Copyright (c) 2012-2016 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "GameCommonHeader.h"

#include "Screen_Leaderboards.h"
#include "LeaderboardStorage.h"
#include "../../../SharedGameCode/Menus/MenuButton.h"
#include "Core/ProfileManager.h"

Screen_Leaderboards* g_pScreen_Leaderboards = 0;

Screen_Leaderboards::Screen_Leaderboards()
{
    g_pScreen_Leaderboards = this;

    LOGInfo( LOGTag, "[Flow] - Showing Screen_Leaderboards\n" );

    m_MenuItemsNeeded = LeaderboardActions_NumButtons;

    m_Transparent = true;

    m_ScrollAmount = 0;
    m_Momentum = 0;
    m_WeScrolledTheList = false;

    m_InitialFinger = -1;

    m_Board = (LeaderboardBoards)0;

    m_Board = Leaderboard_Main;

    m_FilterPlayers = LeaderboardFilterPlayers_User;

    m_ScoreIndexAtTop = 0;
    m_ScoreRangeOnScreen = 0;

    m_WaitingForInfo = true;
}

Screen_Leaderboards::~Screen_Leaderboards()
{
    g_pScreen_Leaderboards = 0;
}

void Screen_Leaderboards::Init()
{
    Screen_Base::Init();

    float scrw = g_pGame->m_GameWidth;
    //float scrh = g_pGame->m_GameHeight;

    MenuButton* pButton;

    for( int i=0; i<m_MenuItemsNeeded; i++ )
    {
        CreateMenuButton( i );
        pButton = GetMenuButton( i );

        pButton->m_FontHeight = 40;
        pButton->m_LineHeight = 40 * 0.8f;

        pButton->SetFont( g_pGame->m_pResources->m_pFontText );
        pButton->SetTextShadow( 5, -5 );
        pButton->SetBGShadow( 6, -6 );

        pButton->m_TextColor = ColorByte(255,255,255,255);
        pButton->m_BGColor = MenuButtonColors[MBCT_SelectableBG];

        pButton->m_DisabledTextColor = ColorByte(0,25,25,255);
        pButton->m_DisabledBGColor = ColorByte(100,100,100,255);

        //pButton->m_SoundPressed = GameAudioCue_Explode;

        pButton->m_ButtonAction[0] = (char)i;
    }

    // set up the login button
    {
        pButton = GetMenuButton( LeaderboardActions_Login );

        //pButton->SetSprites( g_DefaultEnabledBGSprite, 0, 0, 0, 0 );
        MaterialDefinition* pMatGray = g_pMaterialManager->LoadMaterial( "Data/Materials/Gray.mymaterial" );
        MaterialDefinition* pMatDarkGray = g_pMaterialManager->LoadMaterial( "Data/Materials/DarkGray.mymaterial" );
        MaterialDefinition* pMatShadow = g_pMaterialManager->LoadMaterial( "Data/Materials/Shadow.mymaterial" );
        pButton->SetMaterial( MenuButton::Material_BG, pMatGray );
        pButton->SetMaterial( MenuButton::Material_BGDisabled, pMatGray );
        pButton->SetMaterial( MenuButton::Material_BGOverlay, pMatGray );
        pButton->SetMaterial( MenuButton::Material_BGPressed, pMatDarkGray );
        pButton->SetMaterial( MenuButton::Material_Shadow, pMatShadow );
        pMatGray->Release();
        pMatDarkGray->Release();
        pMatShadow->Release();

        sprintf_s( pButton->m_Strings[0], MenuButton::MAX_STRING_LENGTH, "Login" );
        pButton->m_FontHeight = 30;
        pButton->SetPositionAndSize( 640-90, 900, 120, 40 );
    }

    // set up the next and prev buttons
    {
        pButton = GetMenuButton( LeaderboardActions_SwitchBoard_Prev );
        pButton->m_TextColor = ColorByte( 0,0,0,32 );
        pButton->m_BGColor = ColorByte( 255, 255, 255, 0 );
        pButton->m_PressedBGColor = ColorByte( 255, 255, 255, 0 );
        pButton->m_FontHeight = 60;
        pButton->SetPositionAndSize( 22, 800-50, 64, 64 );
        pButton->m_InputWidth = 150;
        pButton->SetString( "<" );

        pButton = GetMenuButton( LeaderboardActions_SwitchBoard_Next );
        pButton->m_TextColor = ColorByte( 0,0,0,32 );
        pButton->m_BGColor = ColorByte( 255, 255, 255, 0 );
        pButton->m_PressedBGColor = ColorByte( 255, 255, 255, 0 );
        pButton->m_FontHeight = 60;
        pButton->SetPositionAndSize( scrw - 22, 800-50, 64, 64 );
        pButton->m_InputWidth = 150;
        pButton->SetString( ">" );
    }

    {
        MenuButton* pButton;

        pButton = GetMenuButton( LeaderboardActions_JumpToTopOrPlayer );
        pButton->m_TextColor = ColorByte( 0,30,51,255 );
        pButton->m_TextShadowColor = ColorByte( 0,0,0,128 );
        pButton->SetTextShadow( 2, -2 );
        pButton->m_BGColor = ColorByte( 199,217,245,196 );
        pButton->m_PressedBGColor = ColorByte( 199,217,245,128 );
        pButton->m_FontHeight = 40;
        pButton->SetPositionAndSize( 320, 680, 250, 45 );
    }

    // set up the back and clear buttons
    {
        MenuButton* pButton = GetMenuButton( LeaderboardActions_Back );
        //pButton->SetSprites( g_pGame->m_pResources->m_pSprites[SL_Button_Back], 0, 0, 0, 0 );
        MaterialDefinition* pMatGray = g_pMaterialManager->LoadMaterial( "Data/Materials/Gray.mymaterial" );
        MaterialDefinition* pMatDarkGray = g_pMaterialManager->LoadMaterial( "Data/Materials/DarkGray.mymaterial" );
        MaterialDefinition* pMatShadow = g_pMaterialManager->LoadMaterial( "Data/Materials/Shadow.mymaterial" );
        pButton->SetMaterial( MenuButton::Material_BG, pMatGray );
        pButton->SetMaterial( MenuButton::Material_BGDisabled, pMatGray );
        pButton->SetMaterial( MenuButton::Material_BGOverlay, pMatGray );
        pButton->SetMaterial( MenuButton::Material_BGPressed, pMatDarkGray );
        pButton->SetMaterial( MenuButton::Material_Shadow, pMatShadow );
        pMatGray->Release();
        pMatDarkGray->Release();
        pMatShadow->Release();

        pButton->m_BGColor = ColorByte( 255, 255, 255, 255 );
        pButton->SetPositionAndSize( scrw - 42, 42, 64, 64 );
        pButton->SetString( " " );
    }
    
    // set up the game center button
    {
        MenuButton* pButton = GetMenuButton( LeaderboardActions_GameCenter );
        //pButton->SetSprites( g_pGame->m_pResources->m_pSprites[SL_WhiteSquare], 0, 0, 0, g_pGame->m_pResources->m_pSprites[SL_WhiteSquare] );
        MaterialDefinition* pMatGray = g_pMaterialManager->LoadMaterial( "Data/Materials/Gray.mymaterial" );
        MaterialDefinition* pMatDarkGray = g_pMaterialManager->LoadMaterial( "Data/Materials/DarkGray.mymaterial" );
        MaterialDefinition* pMatShadow = g_pMaterialManager->LoadMaterial( "Data/Materials/Shadow.mymaterial" );
        pButton->SetMaterial( MenuButton::Material_BG, pMatGray );
        pButton->SetMaterial( MenuButton::Material_BGDisabled, pMatGray );
        pButton->SetMaterial( MenuButton::Material_BGOverlay, pMatGray );
        pButton->SetMaterial( MenuButton::Material_BGPressed, pMatDarkGray );
        pButton->SetMaterial( MenuButton::Material_Shadow, pMatShadow );
        pMatGray->Release();
        pMatDarkGray->Release();
        pMatShadow->Release();

        //pButton->m_pBGSprite = g_pGame->m_pResources->m_pSprites[SL_WhiteSquare]; //SL_GameCenter];
        //pButton->m_pShadowSprite = g_pGame->m_pResources->m_pSprites[SL_WhiteSquare]; //SL_GameCenter];

        pButton->m_BGColor = ColorByte( 255, 255, 255, 255 );
        pButton->SetPositionAndSize( 42, 42, 64, 64 );
        pButton->SetString( "" );
        
#if !MYFW_IOS
        pButton->SetVisibleAndEnabled( false );
#endif
    }
}

bool Screen_Leaderboards::IsSettled()
{
    if( fequal( m_Momentum, 0, 0.01f ) == false )
        return false;

    return true;
}

void Screen_Leaderboards::Tick(double TimePassed)
{
    Screen_Base::Tick( TimePassed );

    if( m_InitialFinger == -1 && m_Momentum != 0 )
    {
        m_Momentum *= 0.99f;
        
        m_ScrollAmount += m_Momentum * (float)TimePassed * 30;
    }
    
    {
        MenuButton* pButton = GetMenuButton( LeaderboardActions_JumpToTopOrPlayer );
        
        if( m_FilterPlayers == LeaderboardFilterPlayers_User )
            pButton->SetString( "Jump to top" );
        else
            pButton->SetString( "Find my score" );

        if( g_pGame->m_pProfileManager->m_CurrentProfile->m_UserIndex == -1 )
        {
            pButton->SetVisibleAndEnabled( false );
            m_FilterPlayers = LeaderboardFilterPlayers_Top;
        }
        else
            pButton->SetVisibleAndEnabled( true );
    }

    int numscores = 0;
    numscores = g_pGame->m_pLeaderboardStorage->m_TotalPlayers[m_Board];

    int numscoretoscrollthrough = numscores - 5;
    if( numscoretoscrollthrough < 0 )
        numscoretoscrollthrough = 0;

    //LOGInfo( LOGTag, "m_ScrollAmount: %f\n", m_ScrollAmount );

    int sep = 100;

    float bottomoflist = (float)numscoretoscrollthrough*sep;
    MyClamp( m_ScrollAmount, 0.0f, bottomoflist );

    m_ScoreIndexAtTop = 0;
    m_ScoreRangeOnScreen = 30;

    float topypixeltodraw = -400;
    m_ScoreIndexAtTop = (int)((topypixeltodraw + m_ScrollAmount) / sep);

    MyClamp( m_ScoreIndexAtTop, 0, 2000000000 ); // we only support 2 billion levels in the list.

    // check if we're still waiting for info.
    if( m_WaitingForInfo )
    {
        int indextoshow = -1;

        if( m_FilterPlayers == LeaderboardFilterPlayers_Top )
            indextoshow = 0;
        else if( m_FilterPlayers == LeaderboardFilterPlayers_User )
            indextoshow = g_pGame->m_pLeaderboardStorage->m_UserIndices[m_Board];

        int blockid = g_pGame->m_pLeaderboardStorage->FindBlockContainingID( indextoshow, m_Board );
        if( blockid == -1 )
        {
            m_WaitingForInfo = true;
        }
        else
        {
            m_WaitingForInfo = false;
    
            // we got what we needed to center on the user, let's jump there.
            if( m_FilterPlayers == LeaderboardFilterPlayers_User )
            {
                //m_ScoreIndexAtTop = g_pGame->m_pLeaderboardStorage->m_UserIndices[m_FilterMode][m_FilterTime];
                m_ScrollAmount = (float)sep*indextoshow; //m_ScoreIndexAtTop;
            }
            else
            {
                m_ScrollAmount = 0;
            }
        }
    }
}

void Screen_Leaderboards::Draw()
{
    Screen_Base::Draw();

    // half black
    g_pGame->m_pResources->m_pSprites[SL_WhiteSquare]->Create( 1.125f, 1.234f, 0, 1, 0, 1, Justify_CenterX|Justify_CenterY );
    g_pGame->m_pResources->m_pSprites[SL_WhiteSquare]->SetPosition( 0.5f, 0.5f, 0 );
    g_pGame->m_pResources->m_pSprites[SL_WhiteSquare]->SetTint( ColorByte(0,0,0,240) );
    g_pGame->m_pResources->m_pSprites[SL_WhiteSquare]->Draw( &g_pGame->m_OrthoMatrix );

    FontDefinition* pFont = g_pGame->m_pSystemFont;
    float scrw = g_pGame->m_GameWidth;
    float scrh = g_pGame->m_GameHeight;

    g_pGame->m_pResources->m_pSprites[SL_WhiteSquare]->SetTint( ColorByte(0,150,0,255) );

    float sep = 100;
    
    // Draw the scores.
    if( m_WaitingForInfo == true )
    {
        if( g_pGame->m_pLeaderboardStorage->HasANetworkErrorOccured() )
        {
            unsigned char justify = Justify_Left|Justify_CenterY;
            float fontheight = 40;
            float x = 0.05f * scrw;
            float y = 0.5f * scrh;
            float lineinc = 40;

            int lines = 0;
            RenderTextQuick( pFont, fontheight, x, y - lines*lineinc, justify, "The was an error communicating" ); lines++;
            RenderTextQuick( pFont, fontheight, x, y - lines*lineinc, justify, "with the game server." ); lines++;
            lines++;
            RenderTextQuick( pFont, fontheight, x, y - lines*lineinc, justify, "Are you connected to the internet?" ); lines++;
        }
        else
        {
            RenderTextQuick( pFont, 32, 0.5f*scrw, 0.5f*scrh, Justify_CenterX|Justify_CenterY, "Retrieving list..." );
        }
    }
    else
    {
        for( int i=m_ScoreIndexAtTop; i<m_ScoreIndexAtTop + m_ScoreRangeOnScreen; i++ )
        {
            //if( i >= g_pGame->m_pLeaderboardStorage->m_TotalPlayers[m_FilterMode][m_FilterTime] )
            //    break;

            int blockid = g_pGame->m_pLeaderboardStorage->FindBlockContainingID( i, m_Board );
            if( blockid == -1 )
            {
                m_WaitingForInfo = true;
                break;
            }

            LeaderboardStorageBlock* pBlock = &g_pGame->m_pLeaderboardStorage->m_Blocks[blockid];
            if( i >= pBlock->m_Offset + pBlock->m_NumEntriesInBlock )
            {
                if( i == 0 )
                {
                    RenderTextQuick( pFont, 32, 0.5f*scrw, 0.5f*scrh, Justify_CenterX|Justify_CenterY, "No Entries Found..." );
                }
                break;
            }

            sep = 100;
            float y = 600 - ((sep*i) - m_ScrollAmount);

            if( y < -100 || y > 1060.0f )
                continue;

            if( m_FilterPlayers == LeaderboardFilterPlayers_User && g_pGame->m_pLeaderboardStorage->m_UserIndices[m_Board] == i )
            {
                g_pGame->m_pResources->m_pSprites[SL_WhiteSquare]->SetTint( ColorByte(0,120,0,255) );
            }
            else if( blockid != -1 )
            {
                g_pGame->m_pResources->m_pSprites[SL_WhiteSquare]->SetTint( ColorByte(0,70,0,255) );
            }
            else
            {
                g_pGame->m_pResources->m_pSprites[SL_WhiteSquare]->SetTint( ColorByte(0,20,0,255) );
            }

            g_pGame->m_pResources->m_pSprites[SL_WhiteSquare]->Create( (float)scrw, sep*0.8f, 0, 1, 0, 1, Justify_CenterX|Justify_CenterY );
            g_pGame->m_pResources->m_pSprites[SL_WhiteSquare]->SetPosition( scrw*0.5f, y, 0 );
            g_pGame->m_pResources->m_pSprites[SL_WhiteSquare]->Draw( &g_pGame->m_OrthoMatrixGameSize );
        
            if( blockid != -1 )
            {
                int blockoffset = pBlock->m_Offset;
                LeaderboardBlockEntry* pInfo = &pBlock->m_Entries[i-blockoffset];

                int rank = i;

                RenderTextQuickWithColorAndShadow( pFont, 40, 20, y, Justify_Left|Justify_CenterY, ColorByte(255,255,255,255), ColorByte(0,0,0,128), 3, -3, "%d) %s", rank+1, pInfo->m_Username );
                RenderTextQuickWithColorAndShadow( pFont, 40, scrw-20, y, Justify_Right|Justify_CenterY, ColorByte(255,255,255,255), ColorByte(0,0,0,128), 3, -3, "%d points", pInfo->m_Score );

                //int valueint = pInfo->m_Score;
                //int billions = valueint / (1000000000);
                //int millions = (valueint % (1000000000)) / (1000000);
                //int thousands = (valueint % (1000000)) / 1000;
                //int ones = (valueint % 1000);
                //if( billions >= 1 )
                //    RenderTextQuick( pFont, fontheight*2, 0.98f, y, Justify_Right|Justify_CenterY, "%d,%03d,%03d,%03d", billions, millions, thousands, ones );
                //else if( millions >= 1 )
                //    RenderTextQuick( pFont, fontheight*2, 0.98f, y, Justify_Right|Justify_CenterY, "%d,%03d,%03d", millions, thousands, ones );
                //else if( thousands >= 1 )
                //    RenderTextQuick( pFont, fontheight*2, 0.98f, y, Justify_Right|Justify_CenterY, "%d,%03d", thousands, ones );
                //else
                //    RenderTextQuick( pFont, fontheight*2, 0.98f, y, Justify_Right|Justify_CenterY, "%d", ones );
            }
        }
    }

    {
        //float y = 800;
        //DrawTitle( 320, y, 80, 0, 0 );
        //
        //DrawMoreTitleText( 320, y+20, 80, "LEADERBOARDS" );

        char temp[100];
        strcpy_s( temp, 100, LeaderboardBoardFriendlyNames[m_Board] );
        for( unsigned int i=0; i<strlen(temp); i++ )
            temp[i] = (char)toupper(temp[i]);
        //DrawMoreTitleText( 320, y-50, 50, temp );
    }

    // draw the buttons.
    DrawAllMenuItems( &g_pGame->m_OrthoMatrix );

    pFont = g_pGame->m_pResources->m_pFontText;
    if( g_pGame->m_pProfileManager->m_CurrentProfile->m_UserIndex == -1 )
        RenderTextQuickWithColorAndShadow( pFont, 30, scrw, scrh, Justify_Right|Justify_Top, ColorByte(255,255,255,255), ColorByte(0,0,0,128), 3, -3, "Not logged in" );
    else
        RenderTextQuickWithColorAndShadow( pFont, 30, scrw, scrh, Justify_Right|Justify_Top, ColorByte(255,255,255,255), ColorByte(0,0,0,128), 3, -3, "Logged in as: %s", g_pGame->m_pProfileManager->m_CurrentProfile->m_Username );
}

bool Screen_Leaderboards::OnTouch(int action, int id, float x, float y, float pressure, float size)
{
    if( Screen_Base::OnTouch( action, id, x, y, pressure, size ) )
        return true;

    //LOGInfo( LOGTag, "OnTouch (%d %d)(%f,%f)(%f %f)\n", action, id, x, y, pressure, size);

    //if( action != 2 )
    //    LOGInfo( LOGTag, "OnTouch() id %d, actionmasked %d (%f, %f)\n", id, actionmasked, x, y );

    // flip y to make it start at bottom left.
    y = g_pGame->m_GameHeight - y;

    //static float totaldiffx = 0;

    bool newfingerdown = false;
    bool newfingerup = false;

    int fingerindex = -1;

    for( int i=0; i<10; i++ )
    {
        if( g_Fingers[i].id == id )
            fingerindex = i;
    }

    if( action == GCBA_Down ) // first finger down
    {
        m_WeScrolledTheList = false;

        g_Fingers[id].set( x, y, id );
        newfingerdown = true;
            
        m_InitialFinger = id;
        m_Momentum = 0;

        for( int i=m_MenuItemsNeeded-1; i>=0; i-- )
        {
            if( GetMenuItem(i) )
            {
                if( GetMenuItem(i)->HoldOnCollision( id, x, y, true ) )
                {
                    m_InitialFinger = -1;
                    return true;
                }
            }
        }
    }

    if( action == GCBA_Up ) // last finger up... i.e. no fingers still down.
    {
        for( int i=0; i<10; i++ )
        {
            g_Fingers[i].reset();
        }
        newfingerup = true;

        //LOGInfo( LOGTag, "Finger Up" );
        //totaldiffx = 0;

        m_InitialFinger = -1;
    }

    if( action == GCBA_Held ) // any finger might have moved
    {
        if( fingerindex != -1 )
        {
            g_Fingers[fingerindex].set( x, y, id );

            //int diffx = (int)(g_Fingers[fingerindex].currx) - (int)(g_Fingers[fingerindex].lastx);
            int diffy = (int)(g_Fingers[fingerindex].curry) - (int)(g_Fingers[fingerindex].lasty);
            //int totaldiffy = (int)(g_Fingers[fingerindex].curry) - (int)(g_Fingers[fingerindex].inity);

            //if( abs(totaldiffy) > 8 )
            //    m_WeScrolledTheList = true;

            if( diffy != 0 )
            {
                if( id == m_InitialFinger )
                {
                    m_ScrollAmount += diffy;
                    m_Momentum = (float)diffy;
                }
            }

            for( int i=m_MenuItemsNeeded-1; i>=0; i-- )
            {
                if( GetMenuItem(i) )
                {
                    GetMenuItem(i)->ReleaseOnNoCollision( fingerindex, x, y );
                }
            }
        }
    }

    if( newfingerdown )
    {   
    }

    if( newfingerup )
    {
        if( m_WeScrolledTheList == false )
        {
            for( int i=m_MenuItemsNeeded-1; i>=0; i-- )
            {
                if( GetMenuItem(i) )
                {
                    const char* actionstr = GetMenuItem(i)->TriggerOnCollision( id, x, y, true );
                    int action = actionstr[0];

                    if( action != -1 && OnMenuAction( action ) )
                    {
                        // bit of a hack here... button is intentionally left in MBS_Triggered state when pressed.
                        // resetting to idle since we don't have a trigger anim.
                        MenuButton* pButton = GetMenuButton( i );
                        pButton->ClearHeldState();
                        break;
                    }
                }
            }
        }
        //if( m_WeScrolledTheList == false )
        //{
        //    // we pressed one of the items in the list.
        //    if( m_WaitingForInfo == false )
        //    {
        //        for( int i=m_ScoreIndexAtTop; i<m_ScoreIndexAtTop + m_ScoreRangeOnScreen; i++ )
        //        {
        //            //if( i >= g_pGame->m_pLeaderboardStorage->m_TotalPlayers[m_FilterMode][m_FilterTime] )
        //            //    break;

        //            int blockid = g_pGame->m_pLeaderboardStorage->FindBlockContainingID( i, m_Board );

        //            if( blockid == -1 )
        //                break;

        //            float sep = 100;
        //            float targety = 960 * 0.5f - ((sep*i) - m_ScrollAmount);

        //            if( fabs( y - targety ) < (sep*0.8f) / 2 )
        //            {
        //                //// start a level:
        //                //g_pGame->m_GameTypeToSwitchToNextTick = GAMETYPE_PuzzleMaker;
        //                //g_pGame->m_GameOptions.Reset();
        //                //g_pGame->m_GameOptions.m_CreateMode = false;

        //                //int blockid = g_pGame->m_pLeaderboardStorage->FindBlockContainingID( i, m_Board );
        //                //int blockoffset = pBlock->m_Offset;

        //                //OnlinePuzzleInfo* pInfo = &pBlock->m_Entries[i-blockoffset];

        //                //g_pGame->m_GameOptions.m_PuzzleDef.m_UserIndex = pInfo->m_UserIndex;
        //                //strcpy_s( g_pGame->m_GameOptions.m_PuzzleDef.m_UserName, 15+1, pInfo->m_Username );
        //                //g_pGame->m_GameOptions.m_PuzzleDef.m_PuzzleIndex = pInfo->m_PuzzleIndex;
        //                //strcpy_s( g_pGame->m_GameOptions.m_PuzzleDef.m_PuzzleName, 32+1, pInfo->m_PuzzleName );
        //                //g_pGame->m_GameOptions.m_PuzzleDef.m_Version = pInfo->m_Version;
        //                //g_pGame->m_GameOptions.m_PuzzleDef.m_EpisodeIndex = pInfo->m_EpisodeIndex;
        //                //g_pGame->m_GameOptions.m_PuzzleDef.m_LevelNumber = pInfo->m_LevelNumber;
        //                //g_pGame->m_GameOptions.m_PuzzleDef.m_pPuzzleJSON = pInfo->m_LevelDesc;

        //                //return true;
        //            }
        //        }
        //    }
        //}
    }

    return false;
}

bool Screen_Leaderboards::OnButtons(GameCoreButtonActions action, GameCoreButtonIDs id)
{
    if( Screen_Base::OnButtons( action, id ) )
        return true;

    if( id == GCBI_Back && action == GCBA_Down )
    {
        //g_pGame->m_pGameAudio->Play( GameAudioCue_Explode );
        OnMenuAction( LeaderboardActions_Back );
        return true;
    }

    return false;
}

bool Screen_Leaderboards::OnMenuAction(int action)
{
    switch( action )
    {
    case LeaderboardActions_Login:
        SetScreenOverlayToShow( ScreenOverlay_Login );
        return true;

    case LeaderboardActions_SwitchBoard_Prev:
        m_Board = (LeaderboardBoards)(m_Board-1);
        if( m_Board < 0 )
            m_Board = (LeaderboardBoards)(Leaderboard_NumBoards-1);
        m_WaitingForInfo = true;
        return true;

    case LeaderboardActions_SwitchBoard_Next:
        m_Board = (LeaderboardBoards)(m_Board+1);
        if( m_Board >= Leaderboard_NumBoards )
            m_Board = (LeaderboardBoards)0;
        m_WaitingForInfo = true;
        return true;

    case LeaderboardActions_JumpToTopOrPlayer:
        m_FilterPlayers = (LeaderboardFilterPlayers)(m_FilterPlayers-1);
        if( m_FilterPlayers < 0 )
            m_FilterPlayers = (LeaderboardFilterPlayers)(LeaderboardFilterPlayers_NumOptions-1);
        m_WaitingForInfo = true;
        return true;

    case LeaderboardActions_Back:
        SetScreenOverlayToShow( ScreenOverlay_Destroy );
        return true;

    case LeaderboardActions_GameCenter:
#if MYFW_IOS
////        if( g_pGravityBallsGame == 0 )
////            ShowLeaderboard( "InfiniteModeID" );
////        else
//        if( m_Board == Leaderboard_OfflineInfinite )
//            ShowLeaderboard( "InfiniteModeID" );
//        else if( m_Board == Leaderboard_OfflineWordSearch )
//            ShowLeaderboard( "WordSearchModeID" );
//        else if( m_Board == Leaderboard_OfflineTimed )
//            ShowLeaderboard( "TimedModeID" );
//        else if( m_Board == Leaderboard_OfflineTurnBased )
//            ShowLeaderboard( "TurnBasedModeID" );
//        else if( m_Board == Leaderboard_OfflinePuzzle )
//            ShowLeaderboard( "PuzzleModeID" );
#endif
        return true;
    }

    return false;
}
