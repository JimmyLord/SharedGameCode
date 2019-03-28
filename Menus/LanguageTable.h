//
// Copyright (c) 2012-2014 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#ifndef __LANGUAGETABLE_H__
#define __LANGUAGETABLE_H__

class LanguageTable;

extern LanguageTable* g_pLanguageTable;

class LanguageTable
{
public:
    MyFileObject* m_pFile;
    cJSON* m_pJSON;
    int m_ActiveLanguage;

public:
    LanguageTable();
    ~LanguageTable();

    void Initialize(FileManager* pFileManager, const char* filename);
    void SetActiveLanguage(int language);
    bool StringExists(const char* stringtofind, int languagetosearch = -1, ...);
    const char* LookUp(const char* stringtofind, int languagetosearch = -1, ...);
};

#endif //__LANGUAGETABLE_H__
