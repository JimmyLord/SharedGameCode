//
// Copyright (c) 2012-2014 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "GameCommonHeader.h"
#include "Core/ProfileManager.h"
#include "LanguageTable.h"

LanguageTable* g_pLanguageTable = 0;

LanguageTable::LanguageTable()
{
    m_pFile = 0;
    m_pJSON = 0;

    m_ActiveLanguage = -1;
}

LanguageTable::~LanguageTable()
{
    if( m_pFile )
        g_pFileManager->FreeFile( m_pFile );
    cJSON_Delete( m_pJSON );
}

void LanguageTable::Initialize(const char* filename)
{
    m_pFile = RequestFile( filename );
}

void LanguageTable::SetActiveLanguage(int language)
{
    m_ActiveLanguage = language;
}

bool LanguageTable::StringExists(const char* stringtofind, int languagetosearch, ...)
{
#define MAX_MESSAGE 1024
    char fullstring[MAX_MESSAGE];
    va_list arg;
    va_start(arg, languagetosearch);
    vsnprintf_s( fullstring, sizeof(fullstring), _TRUNCATE, stringtofind, arg );
    va_end(arg);

    if( m_pJSON == 0 )
    {
        MyAssert( m_pFile ); // don't call StringExists before requesting the file.
        if( m_pFile == 0 || m_pFile->m_FileLoadStatus != FileLoadStatus_Success )
            return false;

        // parse file.
        m_pJSON = cJSON_Parse( m_pFile->m_pBuffer );
        MyAssert( m_pJSON );
        g_pFileManager->FreeFile( m_pFile ); // we don't need the file anymore.
        m_pFile = 0;
    }

    int lang = languagetosearch;
    if( languagetosearch == -1 )
        lang = m_ActiveLanguage;

    cJSON* languageblock = 0;
    
    if( lang == 0 )
        languageblock = cJSON_GetObjectItem( m_pJSON, "English" );
    if( lang == 1 )
        languageblock = cJSON_GetObjectItem( m_pJSON, "French" );

    if( languageblock )
    {
        cJSON* string = cJSON_GetObjectItem( languageblock, (const char*)fullstring );

        if( string )
        {
            return true;
        }
    }
    else
    {
        return false;
    }

    return false;
}

const char* LanguageTable::LookUp(const char* stringtofind, int languagetosearch, ...)
{
    //return "---";

#define MAX_MESSAGE 1024
    char fullstring[MAX_MESSAGE];
    va_list arg;
    va_start(arg, languagetosearch);
    vsnprintf_s( fullstring, sizeof(fullstring), _TRUNCATE, stringtofind, arg );
    va_end(arg);

    if( m_pJSON == 0 )
    {
        MyAssert( m_pFile ); // don't call LookUp before requesting the file.
        if( m_pFile == 0 || m_pFile->m_FileLoadStatus != FileLoadStatus_Success )
            return "!language file not ready!";

        // parse file.
        m_pJSON = cJSON_Parse( m_pFile->m_pBuffer );
        MyAssert( m_pJSON );
        g_pFileManager->FreeFile( m_pFile ); // we don't need the file anymore.
        m_pFile = 0;
    }

    int lang = languagetosearch;
    if( languagetosearch == -1 )
        lang = m_ActiveLanguage;

    cJSON* languageblock = 0;
    
    if( lang == 0 )
        languageblock = cJSON_GetObjectItem( m_pJSON, "English" );
    if( lang == 1 )
        languageblock = cJSON_GetObjectItem( m_pJSON, "French" );

    if( languageblock )
    {
        cJSON* string = cJSON_GetObjectItem( languageblock, (const char*)fullstring );

        if( string )
        {
            return string->valuestring;
        }
    }
    else
    {
        languageblock = cJSON_GetObjectItem( m_pJSON, "English" );

        cJSON* string = cJSON_GetObjectItem( languageblock, (const char*)fullstring );

        if( string )
        {
            return string->valuestring;
        }

        return "!invalid language set!";
    }

    return "!string not found!";
}
