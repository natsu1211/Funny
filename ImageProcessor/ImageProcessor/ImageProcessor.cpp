// ImageProcessor.cpp : 定义 DLL 应用程序的导出函数。
//

#include "ImageProcessor.h"
#include <math.h>
#include <assert.h>


//// 这是导出变量的一个示例
//IMAGEPROCESSOR_API int nImageProcessor=0;
//
//// 这是导出函数的一个示例。
//IMAGEPROCESSOR_API int fnImageProcessor(void)
//{
//	return 42;
//}

// 这是已导出类的构造函数。
// 有关类定义的信息，请参阅 ImageProcessor.h
using namespace LL;
BMPDecoder::BMPDecoder()
:fileHeaderHandle_(nullptr), infoHeaderHandle_(nullptr), palette_(nullptr), pixels_(nullptr), bmpfile_(nullptr)
{
	
}

BMPDecoder::~BMPDecoder()
{

}

void BMPDecoder::TraceFileHeader()
{
	//cout.setf(ios::hex);
	cout << "fileHeaderHandle->type: " << fileHeaderHandle_->type << '\n'
		<< "fileHeaderHandle->size: " << fileHeaderHandle_->size << '\n'
		<< "fileHeaderHandle->reserved1: " << fileHeaderHandle_->reserved1 << '\n'
		<< "fileHeaderHandle->reserved2: " << fileHeaderHandle_->reserved2 << '\n'
		<< "fileHeaderHandle->offBits: " << fileHeaderHandle_->offBits << endl;
}

void BMPDecoder::TraceInfoHeader()
{
	cout << "infoHeaderHandle->size: " << infoHeaderHandle_->size << '\n'
		<< "infoHeaderHandle->width: " << infoHeaderHandle_->width << '\n'
		<< "infoHeaderHandle->height: " << infoHeaderHandle_->height << '\n'
		<< "infoHeaderHandle->planes: " << infoHeaderHandle_->planes << '\n'
		<< "infoHeaderHandle->bits: " << infoHeaderHandle_->bits << '\n'
		<< "infoHeaderHandle->compression: " << infoHeaderHandle_->compression << '\n'
		<< "infoHeaderHandle->imageSize: " << infoHeaderHandle_->imageSize << '\n'
		<< "infoHeaderHandle->xResolution: " << infoHeaderHandle_->xResolution << '\n'
		<< "infoHeaderHandle->yResolution: " << infoHeaderHandle_->yResolution << '\n'
		<< "infoHeaderHandle->nColors: " << infoHeaderHandle_->nColors << '\n'
		<< "infoHeaderHandle->importantColors: " << infoHeaderHandle_->importantColors << endl;

}

bool BMPDecoder::LoadImage(const char *fileName)
{
	// bitmap header, 14 bytes
	if (fileHeaderHandle_ == nullptr)
	{
		fileHeaderHandle_ = make_unique<BitMapFileHeader>();
	}
	if (bmpfile_ == nullptr)
	{
		bmpfile_ = make_unique<ReadCacheStream>(fileName, "rb");
	}


	fileHeaderHandle_->type = bmpfile_->ReadWord(true);

	if (fileHeaderHandle_->type == 0x4d42 || fileHeaderHandle_->type == 0x4142 || fileHeaderHandle_->type == 0x4943
		|| fileHeaderHandle_->type == 0x5043 || fileHeaderHandle_->type == 0x4349 || fileHeaderHandle_->type == 0x5450)
	{
	}
	else
	{
		cerr << "not a bmp image" << endl;
		exit(5);
	}
	fileHeaderHandle_->size = bmpfile_->ReadDWord(true);
	fileHeaderHandle_->reserved1 = bmpfile_->ReadWord(true);
	fileHeaderHandle_->reserved2 = bmpfile_->ReadWord(true);
	fileHeaderHandle_->offBits = bmpfile_->ReadDWord(true);
	TraceFileHeader();

	//bitmap info
	infoHeaderHandle_ = make_unique<BitMapInfoHeader>();
	infoHeaderHandle_->size = bmpfile_->ReadDWord(true);
	//bmpfile_->Read(infoHeaderHandle_.get(), 14, 40);
	TraceInfoHeader();
	if (infoHeaderHandle_->size == 40)
	{
		/*infoHeaderHandle->width = ReadNBytes(buffer + 18, 4);
		infoHeaderHandle->height = ReadNBytes(buffer + 22, 4);
		infoHeaderHandle->planes = ReadNBytes(buffer + 26, 2);
		infoHeaderHandle->bits = ReadNBytes(buffer + 28, 2);
		infoHeaderHandle->compression = ReadNBytes(buffer + 30, 4);
		infoHeaderHandle->imageSize = ReadNBytes(buffer + 34, 4);
		infoHeaderHandle->xResolution = ReadNBytes(buffer + 38, 4);
		infoHeaderHandle->yResolution = ReadNBytes(buffer + 42, 4);
		infoHeaderHandle->nColors = ReadNBytes(buffer + 46, 4);
		infoHeaderHandle->importantColors = ReadNBytes(buffer + 50, 4);*/
		infoHeaderHandle_->width = bmpfile_->ReadDWord(true);
		infoHeaderHandle_->height = bmpfile_->ReadDWord(true);
		infoHeaderHandle_->planes = bmpfile_->ReadWord(true);
		infoHeaderHandle_->bits = bmpfile_->ReadWord(true);
		infoHeaderHandle_->compression = bmpfile_->ReadDWord(true);
		infoHeaderHandle_->imageSize = bmpfile_->ReadDWord(true);
		infoHeaderHandle_->xResolution = bmpfile_->ReadDWord(true);
		infoHeaderHandle_->yResolution = bmpfile_->ReadDWord(true);
		infoHeaderHandle_->nColors = bmpfile_->ReadDWord(true);
		infoHeaderHandle_->importantColors = bmpfile_->ReadDWord(true);
		TraceInfoHeader();
	}

	else
	{
		cout << "not a supported bmp image" << endl;
		exit(4);
	}

	if (fileHeaderHandle_->offBits == (14 + infoHeaderHandle_->size) || infoHeaderHandle_->bits >= 24)
	{
		palette_ = nullptr;
	}
	else
	{

		if (infoHeaderHandle_->nColors == 0)
		{
			palette_ = make_unique<DWORD>(pow(2, infoHeaderHandle_->bits));
			//todo
			//memcpy(palette.get(), buffer + 14 + infoHeaderHandle->size, fileHeaderHandle->size - fileHeaderHandle->offBits);
		}

	}

	pixels_ = make_unique<BYTE>(fileHeaderHandle_->size - fileHeaderHandle_->offBits);
	//memcpy(pixels.get(), buffer + fileHeaderHandle->offBits + 1, fileHeaderHandle->size - fileHeaderHandle->offBits);



};

ImageType LL::TypeofImage(const char *fileName)
{
	//just for test, todo
	ImageType type = ImageType::BMP;
	return type;
}

struct ImageProcessorHandle
{
	BMPDecoder *decoder;
	ImageProcessorHandle(BMPDecoder *p = nullptr)
	{
		decoder = p;
	}
};

extern "C"
{

	IMAGEPROCESSOR_API void __stdcall OpenImage(ImageProcessorHandle *handle, const char *fileName)
	{
		//todo
		switch (TypeofImage(fileName))
		{
		case ImageType::BMP:
			handle->decoder = new BMPDecoder();
			handle->decoder->LoadImage(fileName);
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