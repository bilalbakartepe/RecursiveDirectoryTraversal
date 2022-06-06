#include <windows.h>
#include <tchar.h> 
#include <stdio.h>
#include <strsafe.h>
#include <Shlwapi.h>
#pragma comment(lib, "User32.lib")
#pragma comment(lib, "Shlwapi.lib")

void FindFilesRecursively(TCHAR lpFolder[MAX_PATH], LPCTSTR lpFilePattern);


int main()
{
    TCHAR szDir[MAX_PATH];

    StringCchCopy(szDir, MAX_PATH, L"C:\\"); //Give a root path

    FindFilesRecursively(szDir, L"*");

    return 0;
}



void FindFilesRecursively(TCHAR lpFolder[MAX_PATH], LPCTSTR lpFilePattern)
{
    TCHAR szFullPattern[MAX_PATH];
    WIN32_FIND_DATA FindFileData;
    HANDLE hFindFile;
    PathCombine(szFullPattern, lpFolder, _T("*"));
    try {
        hFindFile = FindFirstFile(szFullPattern, &FindFileData);
    }
    catch (HANDLE myHindleFile) {
        printf("ERROR");
    }

    // first we are going to process any subdirectories


    if (hFindFile != INVALID_HANDLE_VALUE)
    {
        do
        {
            if ((wcscmp(FindFileData.cFileName, L".")) && (wcscmp(FindFileData.cFileName, L".."))) { //this directories are not sub directory

                if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                {
                    // found a subdirectory; recurse into it
                    _tprintf_s(_T("%s <DIR>\n"), szFullPattern);
                    PathCombine(szFullPattern, lpFolder, FindFileData.cFileName);
                    FindFilesRecursively(szFullPattern, lpFilePattern);
                }
            }

        } while (FindNextFile(hFindFile, &FindFileData));
        FindClose(hFindFile);
    }

    // Now we are going to look for the matching files
    PathCombine(szFullPattern, lpFolder, lpFilePattern);
    hFindFile = FindFirstFile(szFullPattern, &FindFileData);
    if (hFindFile != INVALID_HANDLE_VALUE)
    {
        do
        {
            if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            {
                // found a file; do something with it
                PathCombine(szFullPattern, lpFolder, FindFileData.cFileName);
                _tprintf_s(_T("%s File\n"), szFullPattern);
            }
            else {
                PathCombine(szFullPattern, lpFolder, FindFileData.cFileName);
                _tprintf_s(_T("%s <DIR>\n"), szFullPattern);
            }
        } while (FindNextFile(hFindFile, &FindFileData));
        FindClose(hFindFile);
    }
}
