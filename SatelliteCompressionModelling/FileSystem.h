#pragma once

#include <string>
#include <vector>
#include "CompressionManager.h"

class FileSystem
{
	static void GetRawImages();
	static void CalcuteRawImagesSizes();

	static int ms_NumImages;
	static std::vector<std::string> ms_vecRawImagesPathes;
	static std::vector<std::string> ms_vecRawImagesNames;
	static std::vector<size_t> ms_vecRawImagesSizes;
	static std::vector<CompressionManager::CompressionsPool> ms_vecCompressionPool;
public:
	static void Initialise();
	static size_t GetFileSize(std::string path);
	static int GetNumImages() { return ms_NumImages; }
	static std::vector<std::string>& GetRawImagePathes() { return ms_vecRawImagesPathes; }
	static std::vector<std::string>& GetRawImageNames() { return ms_vecRawImagesNames; }
	static std::vector<size_t>& GetRawImageSizes() { return ms_vecRawImagesSizes; }
	static std::vector<CompressionManager::CompressionsPool>& GetCompressionPool() { return ms_vecCompressionPool; }
};

