#pragma once

#include <string>

enum class eCompressionTypes {
	COMPRESSION_TYPE_RLE,
	COMPRESSION_TYPE_LZW,
	COMPRESSION_TYPE_NUM
};

class CompressionInstance
{
	eCompressionTypes m_Type;
	std::string m_CompessionName;
	std::string m_FilePath;
	size_t m_Size;
	float m_Coefficient;
public:
	CompressionInstance() : m_Type(eCompressionTypes::COMPRESSION_TYPE_NUM), m_Size(0), m_Coefficient(1.0f) {}
	eCompressionTypes GetType() { return m_Type; }
	std::string GetCompessionName() { return m_CompessionName; }
	std::string GetFilePath() { return m_FilePath; }
	size_t GetSize() { return m_Size; }
	float GetCoefficient() { return m_Coefficient; }
	void SetType(eCompressionTypes type) { m_Type = type; }
	void SetCompessionName(std::string name) { m_CompessionName = name; }
	void SetFilePath(std::string path) { m_FilePath = path; }
	void SetSize(size_t size) { m_Size = size; }
	void SetCoefficient(float coefficient) { m_Coefficient = coefficient; }
};

