/*Interface and basic Utility for concrete iamge processor*/

#pragma once
#ifndef _IMAGE_BASE_
#define	_IMAGE_BASE_

#include <stdint.h>


typedef unsigned long DWORD;
typedef unsigned short WORD;
typedef unsigned char BYTE;

#ifndef _MSC_VER
#define abstract
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
}

#endif // !_IMAGE_BASE_
