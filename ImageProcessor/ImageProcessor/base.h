/*Interface and basic Utility for concrete iamge processor*/

#pragma once
#ifndef _IMAGE_BASE_
#define	_IMAGE_BASE_

#include <stdint.h>
#include <cstdio>
#include <iostream>
#include <memory>

typedef unsigned long DWORD;
typedef unsigned short WORD;
typedef unsigned char BYTE;

#ifndef _MSC_VER
#define abstract
#define override
#endif

#if __cplusplus>=199711L 
#define DELETA_FUNCTION_SUPPORTED
#endif


namespace LL
{
	

	class DecoderBase abstract
	{
	public:
		virtual ~DecoderBase();
		virtual bool LoadImage(const char*) = 0;
		//virtual bool SaveImage(const char*) = 0;
		//virtual void RotateImage(const char*) = 0;

	};

	class Noncopyable
	{
	protected:
		Noncopyable(){};
	#ifdef DELETA_FUNCTION_SUPPORTED
		Noncopyable(const Noncopyable&) = delete ;
		Noncopyable& operator=(const Noncopyable&) = delete;
	#else
	private:
		Noncopyable(const Noncopyable&){};
		Noncopyable& operator=(const Noncopyable&){return *this};
	#endif
	};



	class ReadStreamBase abstract : private Noncopyable
	{
	
	public:
		virtual ~ReadStreamBase();
		virtual bool Open(const char *fileName, const char* mode) = 0;
		virtual void Close() = 0;
		virtual size_t Read(void *buffer, int32_t offset, int32_t size) = 0;
		//virtual size_t Write(void *buffer, int32_t offset, int32_t size) = 0;
		//virtual bool Peek(void *buffer, int32_t size) = 0;
		//n should be a intergal <=4
		virtual DWORD ReadByte() = 0;
		virtual DWORD ReadWord(bool isLittleEndian) = 0;
		virtual DWORD ReadDWord(bool isLittleEndian) = 0;
	};

	class WriteStreamBase abstract : private Noncopyable
	{

	public:
		virtual ~WriteStreamBase();
		virtual bool Open(const char *fileName, const char* mode) = 0;
		virtual void Close() = 0;
		//virtual size_t Read(void *buffer, int32_t offset, int32_t size) = 0;
		virtual size_t Write(void *buffer, int32_t offset, int32_t size) = 0;
		//virtual bool Peek(void *buffer, int32_t size) = 0;
		virtual DWORD WriteByte() = 0;
		virtual DWORD WriteWord(bool isLittleEndian) = 0;
		virtual DWORD WriteDWord(bool isLittleEndian) = 0;
	};


	class ReadCacheStream : public ReadStreamBase
	{
	protected:
		FILE *file_;
		BYTE *buffer_;
	public:
		ReadCacheStream(const char *fileName, const char* mode);
		virtual ~ReadCacheStream();
		virtual bool Open(const char *fileName, const char* mode) override;
		virtual void Close() override;
		virtual size_t Read(void *buffer, int32_t offset, int32_t size) override;
		//virtual size_t Write(void *buffer, int32_t offset, int32_t size) override;
		virtual DWORD ReadByte() override;
		virtual DWORD ReadWord(bool isLittleEndian) override;
		virtual DWORD ReadDWord(bool isLittleEndian) override;

	};

	class WriteCacheStream : public WriteStreamBase
	{
	protected:
		FILE *file_;
		BYTE *buffer_;
	public:
		WriteCacheStream(const char *fileName, const char* mode);
		virtual ~WriteCacheStream();
		virtual bool Open(const char *fileName, const char* mode) override;
		virtual void Close() override;
		//virtual size_t Read(void *buffer, int32_t offset, int32_t size) override;
		virtual size_t Write(void *buffer, int32_t offset, int32_t size) override;
		virtual DWORD WriteByte() override;
		virtual DWORD WriteWord(bool isLittleEndian) override;
		virtual DWORD WriteDWord(bool isLittleEndian) override;

	};
}

#endif // !_IMAGE_BASE_
