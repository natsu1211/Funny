#include "../ImageProcessor/ImageProcessor.h"

#pragma comment(lib,"ImageProcessor.lib")

int main()
{
	ImageProcessorHandle *handle = CreateImageProcessor("test.bmp");
	OpenImage(handle, "test.bmp");
	return 0;
}
