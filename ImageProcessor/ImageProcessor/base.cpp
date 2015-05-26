#include "base.h"

DWORD Image::ReadNBytes(BYTE* buf, int32_t n)
{
	//auto buf = static_cast<const BYTE*>(buffer);
	DWORD ret = buf[0];
	for (int i = 1; i < n; i++)
	{
		ret |= (buf[i] << 8*i);
	}

	return ret;
}

Image::ImageProcessorBase::~ImageProcessorBase()
{
};