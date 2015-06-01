/*Interface and basic Utility for concrete iamge processor*/

#pragma once
#ifndef _IMAGE_BASE_
#define	_IMAGE_BASE_

#include <stdint.h>
#include <cstdio>

typedef unsigned long DWORD;
typedef unsigned short WORD;
typedef unsigned char BYTE;

#ifndef _MSC_VER
#define abstract
#endif

#if __cplusplus>=199711L 
#define DELETA_FUNCTION_SUPPORTED
#endif


namespace LL
{
	//DWORD ReadNBytes(BYTE* buf, int32_t n);

	class ImageProcessorBase abstract
	{
	public:
		virtual ~ImageProcessorBase();
		virtual bool LoadImage(const char*) = 0;
		virtual bool SaveImage(const char*) = 0;
		virtual void RotateImage(const char*) = 0;

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



	class FileBase abstract : private Noncopyable
	{
	
	public:
		virtual ~FileBase();
		virtual bool Open(const char *fileName, const char* mode) = 0;
		virtual void Close() = 0;
		virtual bool Read(void *buffer, int32_t size) = 0;
		virtual bool Write(void *buffer, int32_t size) = 0;
		
	};
}

#endif // !_IMAGE_BASE_
