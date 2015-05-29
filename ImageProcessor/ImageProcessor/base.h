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


namespace Image
{
	DWORD ReadNBytes(BYTE* buf, int32_t n);

	class ImageProcessorBase abstract
	{
	public:
		virtual ~ImageProcessorBase();
		virtual void LoadImages(const char*) = 0;
		//virtual void SavaImages(const char*) = 0;

	};

	class Noncopyable
	{
	public:
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


	class FileBase : public Noncopyable
	{
	protected:
		FILE *file;
	//public:

	};
}

#endif // !_IMAGE_BASE_
