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

LL::ReadStreamBase::~ReadStreamBase()
{
};

LL::ReadCacheStream::ReadCacheStream(const char *fileName, const char* mode, int cacheSize)
:file_(0), buffer_(0), startPos_(0),curPos_(0),endPos_(0), cacheSize_(cacheSize)
{
	if (cacheSize_ < 0)
	{
		cacheSize_ = 65536;
	}
	buffer_ = new BYTE[cacheSize_];
	Open(fileName, mode);
}

LL::ReadCacheStream::~ReadCacheStream()
{
	delete[] buffer_;
	Close();
	file_ = 0;
	buffer_ = 0;
	startPos_ = endPos_ = 0;
	curPos_ = -1;
	cacheSize_ = -1;


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
	assert(size > 0);
	assert(offset < size);
	auto buf = static_cast<BYTE*>(buffer);
	return ReadFromCache(buf + offset, size);


}

size_t LL::ReadCacheStream::ReadToCache(int streamStartPos)
{
	assert(file_);
	int len = 0;
	if (ftell(file_) != streamStartPos)
	{
		fseek(file_, streamStartPos, SEEK_SET);
	}
	
	len = fread(buffer_, 1, cacheSize_, file_);
	startPos_ = curPos_;
	endPos_ = startPos_ + len;
	return len;
}

size_t LL::ReadCacheStream::ReadFromCache(void *buffer, int32_t size)
{
	assert(size > 0);
	int readed = 0;
	int len = 0;
	BYTE *data = static_cast<BYTE*>(buffer);
	while (size > 0)
	{
		if (curPos_ < endPos_)
		{
			len = endPos_ - curPos_ > size ? size : endPos_ - curPos_;
			memcpy(buffer, buffer_, len);
		}
		else
		{
			assert((curPos_ + len) % cacheSize_ == 0);
			int l = ReadToCache(curPos_ + len);
			//EOF reached
			if (feof(file_))
			{
				len = l > size ? size : l;
				readed += len;
				curPos_ += len;
				size -= len;
				data += len;
				memcpy(buffer, buffer_, len);
				return readed;
			}

		}

		readed += len;
		curPos_ += len;
		size -= len;
		data += len;
	}

	return readed;
}


BYTE LL::ReadCacheStream::ReadByte()
{
	DWORD ret;
	int cur = curPos_;
	int len = 0;
	if (cur >= endPos_)
	{
		len = ReadToCache(endPos_);
		cur = curPos_;
		if (len < 1)
		{
			exit(6);
		}
	}
	

	ret = buffer_[cur];
	curPos_ += 1;
	return ret;
}

WORD LL::ReadCacheStream::ReadWord(bool isLittleEndian)
{
	DWORD ret;
	int len = 0;
	int cur = curPos_;
	if (cur >= endPos_)
	{
		len = ReadToCache(endPos_);
		cur = curPos_;
		if (len < 2)
		{
			exit(6);
		}
	}
	
	if (isLittleEndian)
	{	
		ret = buffer_[cur] + (buffer_[cur + 1] << 8);
	}
	else
	{
		ret = (buffer_[cur]<<8) + buffer_[cur + 1];
	}
	curPos_ += 2;
	return ret;
}

DWORD LL::ReadCacheStream::ReadDWord(bool isLittleEndian)
{
	DWORD ret;
	int cur = curPos_;
	int len = 0;
	if (cur >= endPos_)
	{
		len = ReadToCache(endPos_);
		cur = curPos_;
		if (len < 4)
		{
			exit(6);
		}
	}
	if (isLittleEndian)
	{
		ret = buffer_[cur] + (buffer_[cur + 1] << 8) + (buffer_[cur + 2] << 16) + (buffer_[cur + 3] << 24);
	}
	else
	{
		ret = (buffer_[cur] << 24) + (buffer_[cur + 1] << 16) +( buffer_[cur + 2] << 8) + buffer_[cur + 3];
	}
	curPos_ += 4;
	return ret;
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
	return true;
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