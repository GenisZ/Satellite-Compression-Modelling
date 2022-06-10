#pragma once

#include "CompressionInstance.h"
#include <vector>

class CompressionManager 
{
	static CompressionInstance CompressRLE(int ImgId);
	static std::string CalculateRLE(std::string path);
	static CompressionInstance CompressLZW(int ImgId);
	static std::string CalculateLZW(std::string path);
public:
	class CompressionsPool {
		int m_count;
		std::vector<CompressionInstance> m_vecInsts;
	public:
		CompressionsPool() : m_count(0) {}
		int GetCount() { return m_count; }
		void Add(CompressionInstance& inst) { m_count++; m_vecInsts.push_back(inst); }
		CompressionInstance* GetInstance(int pos) { return &m_vecInsts[pos]; }
		CompressionInstance* GetInstance(eCompressionTypes type);
	};

	static CompressionInstance Compress(int ImgId, eCompressionTypes CompressType);
};

