#include "base.h"
#include <assert.h>

DWORD LL::ReadNBytes(void* buffer, int32_t offset, int32_t n)
{
	auto buf = static_cast<const BYTE*>(buffer);
	DWORD ret = buf[0];
	for (int i = 1; i < n; i++)
	{
		ret |= (buf[i] << 8*i);
	}

	return ret;
}

LL::DecoderBase::~DecoderBase()
{
};

LL::StreamBase::~StreamBase()
{

};

LL::FileStream::FileStream(const char *fileName, const char* mode)
{
	Open(fileName, mode);
}

LL::FileStream::~FileStream()
{
	Close();
}

bool LL::FileStream::Open(const char *fileName, const char* mode)
{
	file_ = fopen(fileName, mode);
	if (file_ == NULL)
	{
		std::cout << "file not exist" << std::endl;
		fclose(file_);
		//todo,should let user to have a chance to input again
		exit(1);
	}
	return true;
}

void LL::FileStream::Close()
{
	if (file_)
	{
		fclose(file_);
		file_ = NULL;
	}

}

size_t LL::FileStream::Read(void *buffer, int32_t offset, int32_t size)
{
	//using namespace std;
	//long fileSize;
	//
	//assert(file_ != NULL);
	//fseek(file_, 0, SEEK_END);
	//fileSize = ftell(file_);
	//rewind(file_);

	//buffer = (BYTE*)malloc(sizeof(BYTE)*fileSize);

	//if (buffer == NULL) { exit(2); }

	//// copy the file into the buffer
	//size_t result = fread(buffer, 1, fileSize, file_);
	//if (result == 0) { exit(7); }
	//if (result != fileSize) { exit(6); }
	//fclose(file_);
	assert(size > 0);
	assert(offset < size);
	auto buf = static_cast<BYTE*>(buffer);
	return fread(buf + offset, 1, size, file_);
}

size_t LL::FileStream::Write(void *buffer, int32_t offset, int32_t size)
{
	//todo

	assert(size > 0);
	assert(offset < size);
	auto buf = static_cast<BYTE*>(buffer);
	return fwrite(buf + offset, 1, size, file_);
}