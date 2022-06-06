#include <windows.h>
#include <tchar.h> 
#include <stdio.h>
#include <strsafe.h>
#include <Shlwapi.h>
#include <limits.h>
#pragma comment(lib, "User32.lib")
#pragma comment(lib, "Shlwapi.lib")

DWORD WINAPI FindFilesRecursively(void* param);
void createThreadInTheDir(TCHAR mainDir[MAX_PATH]);

const wchar_t* blackPathTmp[] = { L".",L"..",L".vs",L".gradle"}; //you can add blacklist

int amountOfDir = 0;
int main()
{

    TCHAR mainDir[MAX_PATH];
    const wchar_t* szDir[]{ L"D:",L"C:",L"A:",
                            L"E:",L"F:",L"B:",
                            L"G:",L"H:",L"I:",
                            L"J:",L"K:",L"L:",
                            L"M:",L"N:",L"O:",
                            L"P:",L"R:",L"S:",
                            L"T:",L"U:",L"V:",
                            L"Y:",L"Z:",L"X:",
                            L"W:",L"Q:"};
    
    

    for (int i = 0; i < 26; i++) {

        StringCchCopy(mainDir, MAX_PATH, szDir[i]);
        createThreadInTheDir(mainDir);

    }

    return 0;

}

void createThreadInTheDir(TCHAR mainDir[MAX_PATH]) {
    LPCTSTR lpFilePattern = L"*";
    TCHAR szFullPattern[MAX_PATH];
    HANDLE hFindFile;
    WIN32_FIND_DATA FindFileData;

    HANDLE h[MAX_PATH];

    PathCombine(szFullPattern, mainDir, lpFilePattern);
    hFindFile = FindFirstFile(szFullPattern, &FindFileData);
    if (hFindFile != INVALID_HANDLE_VALUE)
    {
        do
        {
            if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            {

                PathCombine(szFullPattern, mainDir, FindFileData.cFileName);
                _tprintf_s(_T("%s File\n"), szFullPattern);
            }
            else {
                PathCombine(szFullPattern, mainDir, FindFileData.cFileName);

                TCHAR tmp[MAX_PATH];
                StringCchCopy(tmp, MAX_PATH, szFullPattern);

                h[amountOfDir] = CreateThread(
                    NULL,                   // default security attributes
                    0,                      // use default stack size  
                    FindFilesRecursively,       // thread function name
                    &tmp,          // argument to thread function 
                    0,                      // use default creation flags 
                    0);   // returns the thread identifier 


                _tprintf_s(_T("%s <DIR>\n"), szFullPattern);

                amountOfDir++;

            }
        } while (FindNextFile(hFindFile, &FindFileData));
        FindClose(hFindFile);

        WaitForMultipleObjects(amountOfDir, h, TRUE, INFINITE);

        for (int i = 0; i < amountOfDir; i++) {
            if((h[i]!=0)||(h[i]!=INVALID_HANDLE_VALUE)||(h[i]!=NULL))
                CloseHandle(h[i]);

        }
    }
}



DWORD WINAPI FindFilesRecursively(void* param)
{

    TCHAR lpFolder[MAX_PATH];

    wcscpy(lpFolder, (TCHAR*)param);
    LPCTSTR lpFilePattern = L"*";
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
    _tprintf_s(_T("%s File\n"), szFullPattern);

    if (hFindFile != INVALID_HANDLE_VALUE)
    {
        do
        {
            if ((wcscmp(FindFileData.cFileName, blackPathTmp[0])) && (wcscmp(FindFileData.cFileName, blackPathTmp[1]))) {

                if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                {

                    PathCombine(szFullPattern, lpFolder, FindFileData.cFileName);

                    FindFilesRecursively(&szFullPattern);

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
    return 0;
}
