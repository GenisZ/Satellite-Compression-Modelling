#include "FileSystem.h"
#include <iostream>
#include <fstream>
#include <windows.h>

using namespace std;

int FileSystem::ms_NumImages = 0;
std::vector<std::string> FileSystem::ms_vecRawImagesPathes;
std::vector<std::string> FileSystem::ms_vecRawImagesNames;
std::vector<size_t> FileSystem::ms_vecRawImagesSizes;
std::vector<CompressionManager::CompressionsPool> FileSystem::ms_vecCompressionPool;

void FileSystem::GetRawImages()
{
    WIN32_FIND_DATA winFileData;
    HANDLE hFile;
    char szPath[MAX_PATH];
    if (GetCurrentDirectory(sizeof(szPath), szPath))
    {
        lstrcat(szPath, "\\Images\\*.*");
        hFile = FindFirstFile(szPath, &winFileData);
        if (hFile != INVALID_HANDLE_VALUE)
        {
            do
            {
                if (strlen(winFileData.cFileName) > 4)
                {
                    if (!strcmp(&winFileData.cFileName[strlen(winFileData.cFileName)-4], ".ppm"))
                    {
                        ms_vecRawImagesNames.push_back(std::string(winFileData.cFileName, strlen(winFileData.cFileName) - 4));
                        ms_vecRawImagesPathes.push_back("Images\\" + std::string(winFileData.cFileName));
                        ms_NumImages++;
                    }
                }
            } while (FindNextFile(hFile, &winFileData) != 0);
            FindClose(hFile);
        }
    }
}

void FileSystem::CalcuteRawImagesSizes()
{
    for (int i = 0; i < GetNumImages(); i++)
    {
        ms_vecRawImagesSizes.push_back(GetFileSize(GetRawImagePathes()[i]));
    }
}

void FileSystem::Initialise()
{
    GetRawImages();
    CalcuteRawImagesSizes();

    for (int i = 0; i < GetNumImages(); i++)
    {
        ms_vecCompressionPool.push_back(CompressionManager::CompressionsPool());
    }
}

size_t FileSystem::GetFileSize(std::string path)
{
    std::ifstream file(path, std::ios_base::binary);
    size_t nFileLen = 0;
    if (file)
    {
        file.seekg(0, std::ios_base::end);
        nFileLen = file.tellg();
        file.close();
    }
    return nFileLen;
}
