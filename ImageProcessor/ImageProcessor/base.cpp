#include "base.h"
#include <assert.h>

//DWORD LL::ReadNBytes(void* buffer, int32_t offset, int32_t n)
//{
//	auto buf = static_cast<const BYTE*>(buffer);
//	DWORD ret = buf[0];
//	for (int i = 1; i < n; i++)
//	{
//		ret |= (buf[i] << 8*i);
//	}
//
//	return ret;
//}

LL::DecoderBase::~DecoderBase()
{
};

LL::ReadCacheStream ::~ReadCacheStream()
{

};

LL::ReadCacheStream::ReadCacheStream(const char *fileName, const char* mode, int cacheSize)
:file_(0), buffer_(0), startPos_(0),curPos_(0),endPos_(0), cacheSize_(cacheSize)
{
	if (cacheSize_ < 0)
	{
		cacheSize_ = 65536;
	}
	Open(fileName, mode);
}

LL::ReadCacheStream::~ReadCacheStream()
{
	Close();
}

bool LL::ReadCacheStream::Open(const char *fileName, const char* mode)
{
	file_ = fopen(fileName, mode);
	if (file_ == 0)
	{
		std::cout << "file not exist" << std::endl;
		fclose(file_);
		//todo,should let user to have a chance to input again
		exit(1);
	}
	return true;
}

void LL::ReadCacheStream::Close()
{
	if (file_)
	{
		fclose(file_);
		file_ = 0;
	}

}

//return the byte number been read
size_t LL::ReadCacheStream::Read(void *buffer, int32_t offset, int32_t size)
{
	/*assert(size > 0);
	assert(offset < size);
	auto buf = static_cast<BYTE*>(buffer);
	return fread(buf + offset, 1, size, file_);*/


}

size_t LL::ReadCacheStream::ReadToCache(int bufStartPos)
{
	startPos_ = bufStartPos;
	return fread(buffer_, 1, cacheSize_, file_);
}


DWORD LL::ReadCacheStream::ReadByte()
{
	//todo,add pos member to indicate the current position of file stream
}


/*Write Stream*/

LL::WriteCacheStream::WriteCacheStream(const char *fileName, const char* mode)
{
	Open(fileName, mode);
}


LL::WriteCacheStream::~WriteCacheStream()
{
	Close();
}

bool LL::WriteCacheStream::Open(const char *fileName, const char* mode)
{
	//todo
}

void LL::WriteCacheStream::Close()
{
	if (file_)
	{
		fclose(file_);
		file_ = 0;
	}

}

size_t LL::WriteCacheStream::Write(void *buffer, int32_t offset, int32_t size)
{
	assert(size > 0);
	assert(offset < size);
	auto buf = static_cast<BYTE*>(buffer);
	return fwrite(buf + offset, 1, size, file_);
}