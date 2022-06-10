#include "CompressionManager.h"
#include "FileSystem.h"

#include <iostream>
#include <sstream>
#include <fstream>

#include <bitset>
#include <string>
#include <unordered_map>

using namespace std;

CompressionInstance* CompressionManager::CompressionsPool::GetInstance(eCompressionTypes type)
{
    for (int i = 0; i < m_count; i++)
    {
        if (m_vecInsts[i].GetType() == type)
            return &m_vecInsts[i];
    }
    return nullptr;
}

CompressionInstance CompressionManager::CompressRLE(int ImgId)
{
    CompressionInstance inst;
    inst.SetFilePath(CalculateRLE(FileSystem::GetRawImagePathes()[ImgId]));
    inst.SetCompessionName("Run-Length Encoding (RLE)");
    inst.SetType(eCompressionTypes::COMPRESSION_TYPE_RLE);
    inst.SetSize(FileSystem::GetFileSize(inst.GetFilePath()));
    inst.SetCoefficient((float)FileSystem::GetRawImageSizes()[ImgId] / (float)inst.GetSize());
    return inst;
}

unsigned char* readPPM(string fileName, char* pSix, int& width, int& height, int& maximum) {
    FILE* fr = fopen(fileName.c_str(), "rb");

    char line[256];
    fgets(line, 256, fr);
    sscanf(line, "%s", pSix);

    fgets(line, 256, fr);
    sscanf(line, "%d %d", &width, &height);
    fgets(line, 256, fr);
    sscanf(line, "%d", &maximum);

    int size = width * height * 3;

    unsigned char* pixels = new unsigned char[size];

    fread(pixels, 1, size, fr);

    fclose(fr);

    return pixels;
}

std::string CompressionManager::CalculateRLE(std::string path)
{
    char pSix[10];		
    int width = 0;		
    int height = 0;		
    int maximum = 0;	

    unsigned char* pixelArray = readPPM(path, pSix, width, height, maximum);
    std::vector <unsigned char> encodedPixels;
    std::vector <char> occurence;

    bool first = true;

    for (int i = 0; i < width * height * 3; i += 3)
    {
        if (first)
        {
            encodedPixels.push_back(pixelArray[i]);
            encodedPixels.push_back(pixelArray[i + 1]);
            encodedPixels.push_back(pixelArray[i + 2]);
            occurence.push_back(0);
            first = false;
            continue;
        }

        bool same = pixelArray[i] == encodedPixels[encodedPixels.size() - 3]
            && pixelArray[i + 1] == encodedPixels[encodedPixels.size() - 2]
            && pixelArray[i + 2] == encodedPixels[encodedPixels.size() - 1];

        if (same)
        {
            if (occurence.back() <= 0)
            {
                if (occurence.back() == -127)
                {
                    encodedPixels.push_back(pixelArray[i]);
                    encodedPixels.push_back(pixelArray[i + 1]);
                    encodedPixels.push_back(pixelArray[i + 2]);
                    occurence.push_back(0);
                }
                else
                    occurence.back()--;
            }
            else
            {
                occurence.back()--;
                occurence.push_back(-1);
            }
        }
        else
        {
            encodedPixels.push_back(pixelArray[i]);
            encodedPixels.push_back(pixelArray[i + 1]);
            encodedPixels.push_back(pixelArray[i + 2]);

            if (occurence.back() < 0 || occurence.back() == 127)
                occurence.push_back(0);
            else
                occurence.back()++;
        }
    }
    occurence.push_back(-128);

    string outputPath = path.substr(0, path.size() - 4) + ".rle";
    FILE* fp = fopen(outputPath.c_str(), "wb");
    fwrite(&width, 1, sizeof(int), fp);
    fwrite(&height, 1, sizeof(int), fp);

    int pixelCounter = 0;
    for (int i = 0; i < occurence.size(); i++)
    {
        fwrite(&occurence[i], 1, 1, fp);

        if (occurence[i] < 0 && occurence[i] != -128)
        {
            fwrite(&encodedPixels[pixelCounter], 1, 3, fp);
            pixelCounter += 3;
        }
        else if (occurence[i] >= 0)
        {
            for (int k = 0; k < occurence[i] + 1; k++)
            {
                fwrite(&encodedPixels[pixelCounter], 1, 3, fp);
                pixelCounter += 3;
            }
        }
        else
        {
            break;
        }
    }
    fclose(fp);

    return outputPath;
}

CompressionInstance CompressionManager::CompressLZW(int ImgId)
{
    CompressionInstance inst;
    inst.SetFilePath(CalculateLZW(FileSystem::GetRawImagePathes()[ImgId]));
    inst.SetCompessionName("Lempel-Ziv-Welch Encoding (LZW)");
    inst.SetType(eCompressionTypes::COMPRESSION_TYPE_LZW);
    inst.SetSize(FileSystem::GetFileSize(inst.GetFilePath()) / 2 * 12 / 8);
    inst.SetCoefficient((float)FileSystem::GetRawImageSizes()[ImgId] / (float)inst.GetSize());
    return inst;
}

string convert_char_to_string(const char* pCh, int arraySize) {
    string str;
    if (pCh[arraySize - 1] == '\0') str.append(pCh);
    else for (int i = 0; i < arraySize; i++) str.append(1, pCh[i]);
    return str;
}

std::string CompressionManager::CalculateLZW(std::string path)
{
    int size; char* memblock;
    ifstream file(path, ios::in | ios::binary | ios::ate);
    if (file.is_open())
    {
        size = file.tellg();
        memblock = new char[size];
        file.seekg(0, ios::beg);
        file.read(memblock, size);
        file.close();
        string input = convert_char_to_string(memblock, size);
        
        unordered_map<string, int> compress_dictionary(4096);

        for (int unsigned i = 0; i < 256; i++) {
            compress_dictionary[string(1, i)] = i;
        }

        string current_string;
        unsigned int code;
        unsigned int next_code = 256;

        string filename = path.substr(0, path.size() - 4) + ".lzw";

        ofstream outputFile;
        outputFile.open(filename, ios_base::out | ios_base::binary);

        for (char& c : input) {
            current_string = current_string + c;
            if (compress_dictionary.find(current_string) == compress_dictionary.end()) {
                if (next_code <= 4096)
                    compress_dictionary.insert(make_pair(current_string, next_code++));
                current_string.erase(current_string.size() - 1);
                outputFile.write((char*)&compress_dictionary[current_string], sizeof(short));
                current_string = c;
            }
        }
        if (current_string.size())
            outputFile.write((char*)&compress_dictionary[current_string], sizeof(int));
        outputFile.close();

        return filename;
    }

    return path;
}

CompressionInstance CompressionManager::Compress(int ImgId, eCompressionTypes CompressType)
{
    switch (CompressType)
    {
    case eCompressionTypes::COMPRESSION_TYPE_RLE:
        return CompressRLE(ImgId);
    case eCompressionTypes::COMPRESSION_TYPE_LZW:
        return CompressLZW(ImgId);
    default:
        return CompressionInstance();
    }
}
