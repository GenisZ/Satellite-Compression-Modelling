#include "FileSystem.h"
#include "main.h"
#include <iostream>

Logger logger;

void PrintMetaInfo()
{
	for (int i = 0; i < FileSystem::GetNumImages(); i++)
	{
		logger << "Info for " << FileSystem::GetRawImageNames()[i] << ":\n";
		for (int j = 0; j < FileSystem::GetCompressionPool()[i].GetCount(); j++)
		{
			auto pInst = FileSystem::GetCompressionPool()[i].GetInstance(j);
			logger << "Compression Name: " << pInst->GetCompessionName() << logger.endl;
			logger << "Original Size: " << FileSystem::GetRawImageSizes()[i] << logger.endl;
			logger << "Compression Size: " << pInst->GetSize() << logger.endl;
			logger << "Compression Ratio: " << std::to_string(pInst->GetCoefficient()) << logger.endl << logger.endl;
		}
	}

	logger << "**************************************************************************" << logger.endl << logger.endl;
	logger << "Assembling a packet of images to send:" << logger.endl;

	unsigned PacketSize = 0;

	for (int i = 0; i < FileSystem::GetNumImages(); i++)
	{
		int Index = -1; unsigned IndexSize = 0xFFFFFFFF;
		for (int j = 0; j < FileSystem::GetCompressionPool()[i].GetCount(); j++)
		{
			auto pInst = FileSystem::GetCompressionPool()[i].GetInstance(j);
			if (pInst->GetSize() < IndexSize)
			{
				IndexSize = pInst->GetSize();
				Index = j;
			}
		}

		PacketSize += IndexSize;

		auto pInst = FileSystem::GetCompressionPool()[i].GetInstance(Index);
		logger << FileSystem::GetRawImageNames()[i] << ": " << pInst->GetCompessionName()
			<< " [" << pInst->GetSize() << " bytes]" << logger.endl;
	}

	float SendingTime = (float)PacketSize / 750000;

	logger << logger.endl;
	logger << "Sending a packet of " << PacketSize << " bytes was " << std::to_string(SendingTime) 
		<< " seconds" << logger.endl << logger.endl;
	
}

int main()
{
	FileSystem::Initialise();

	std::cout << "Current available " << FileSystem::GetNumImages() << " images:" << std::endl;
	for (int i = 0; i < FileSystem::GetNumImages(); i++)
	{
		logger << FileSystem::GetRawImageNames()[i] << ": " << FileSystem::GetRawImageSizes()[i] << " bytes" << logger.endl;
	}

	logger << "\nStart Images Compressing...\n";
	for (int i = 0; i < FileSystem::GetNumImages(); i++)
	{
		for (int j = 0; j < (int)eCompressionTypes::COMPRESSION_TYPE_NUM; j++)
		{
			auto inst = CompressionManager::Compress(i, (eCompressionTypes)j);
			FileSystem::GetCompressionPool()[i].Add(inst);
		}
	}
	logger << "Images Compressing Done\n\n";

	PrintMetaInfo();

	system("pause");
	return 0;
}