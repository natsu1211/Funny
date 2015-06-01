// ImageProcessor.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "ImageProcessor.h"
#include <math.h>
#include <assert.h>


//// ���ǵ���������һ��ʾ��
//IMAGEPROCESSOR_API int nImageProcessor=0;
//
//// ���ǵ���������һ��ʾ����
//IMAGEPROCESSOR_API int fnImageProcessor(void)
//{
//	return 42;
//}

// �����ѵ�����Ĺ��캯����
// �й��ඨ�����Ϣ������� ImageProcessor.h
using namespace LL;
BMPImageProcessor::BMPImageProcessor()
:fileHeaderHandle(nullptr), infoHeaderHandle(nullptr), palette(nullptr), pixels(nullptr), bmpfile(nullptr)
{

}

BMPImageProcessor::~BMPImageProcessor()
{

}

void BMPImageProcessor::TraceFileHeader()
{
	//cout.setf(ios::hex);
	cout << "fileHeaderHandle->type: " << fileHeaderHandle->type << '\n'
		<< "fileHeaderHandle->size: " << fileHeaderHandle->size << '\n'
		<< "fileHeaderHandle->reserved1: " << fileHeaderHandle->reserved1 << '\n'
		<< "fileHeaderHandle->reserved2: " << fileHeaderHandle->reserved2 << '\n'
		<< "fileHeaderHandle->offBits: " << fileHeaderHandle->offBits << endl;
}

void BMPImageProcessor::TraceInfoHeader()
{
	cout << "infoHeaderHandle->size: " << infoHeaderHandle->size << '\n'
		<< "infoHeaderHandle->width: " << infoHeaderHandle->width << '\n'
		<< "infoHeaderHandle->height: " << infoHeaderHandle->height << '\n'
		<< "infoHeaderHandle->planes: " << infoHeaderHandle->planes << '\n'
		<< "infoHeaderHandle->bits: " << infoHeaderHandle->bits << '\n'
		<< "infoHeaderHandle->compression: " << infoHeaderHandle->compression << '\n'
		<< "infoHeaderHandle->imageSize: " << infoHeaderHandle->imageSize << '\n'
		<< "infoHeaderHandle->xResolution: " << infoHeaderHandle->xResolution << '\n'
		<< "infoHeaderHandle->yResolution: " << infoHeaderHandle->yResolution << '\n'
		<< "infoHeaderHandle->nColors: " << infoHeaderHandle->nColors << '\n'
		<< "infoHeaderHandle->importantColors: " << infoHeaderHandle->importantColors << endl;

}

bool BMPImageProcessor::LoadImage(const char *fileName)
{
	

};

ImageType LL::TypeofImage(const char *fileName)
{
	//just for test, todo
	ImageType type = ImageType::BMP;
	return type;
}

struct ImageProcessorHandle
{
	ImageProcessorBase *processor;
	ImageProcessorHandle(ImageProcessorBase *p = nullptr)
	{
		processor = p;
	}
};

extern "C"
{

	IMAGEPROCESSOR_API void __stdcall OpenImages(ImageProcessorHandle *handle, const char *fileName)
	{
		//todo
		switch (TypeofImage(fileName))
		{
		case ImageType::BMP:
			handle->processor = new BMPImageProcessor();
			handle->processor->LoadImage(fileName);
			break;
		default:


			break;


		}
		
	}

	IMAGEPROCESSOR_API ImageProcessorHandle* __stdcall CreateImageProcessor(const char *fileName)
	{
		return new ImageProcessorHandle();
	}

	IMAGEPROCESSOR_API void __stdcall DestoryImageProcessor(ImageProcessorHandle *handle)
	{
		delete handle;
	}
}