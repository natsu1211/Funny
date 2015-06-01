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
BMPImageProcessor::BMPImageProcessor()
:fileHeaderHandle(nullptr), infoHeaderHandle(nullptr), palette(nullptr), pixels(nullptr), file(NULL), buffer(NULL)
{

}

BMPImageProcessor::~BMPImageProcessor()
{
	if (file )
	{
		fclose(file);
		file = NULL;
	}

	if (buffer)
	{
		delete buffer;
	}
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

void BMPImageProcessor::LoadImages(const char *fileName)
{
	//using namespace std;
	long fileSize;
	//BYTE* buffer = NULL;
	file = fopen(fileName, "rb");
	if ( file == NULL)
	{
		cerr << "file not exist" << endl;
		fclose(file);
		exit(1);
	}
	fseek(file, 0, SEEK_END);
	fileSize = ftell(file);
	rewind(file);

	buffer = (BYTE*)malloc(sizeof(BYTE)*fileSize);

	if (buffer == NULL) {  exit(2); }

	// copy the file into the buffer:
	size_t result = fread(buffer, 1, fileSize, file);
	if (result == 0) { exit(7); }
	if (result != fileSize) { exit(6); }
	fclose(file);
	// bitmap header, 14 bytes
	fileHeaderHandle = make_shared<BitMapFileHeader>();
	fileHeaderHandle->type = (ReadNBytes(buffer, 2));
	
	//check the file type
	if (fileHeaderHandle->type == 0x4d42 || fileHeaderHandle->type == 0x4142 || fileHeaderHandle->type == 0x4943
		|| fileHeaderHandle->type == 0x5043 || fileHeaderHandle->type == 0x4349 || fileHeaderHandle->type == 0x5450)
	{
	}
	else
	{
		cerr << "not a bmp image" << endl;
		exit(5);
	}
	fileHeaderHandle->size = ReadNBytes(buffer + 2, 4);
	fileHeaderHandle->reserved1 = ReadNBytes(buffer + 6, 2);
	fileHeaderHandle->reserved2 = ReadNBytes(buffer + 8, 2);
	fileHeaderHandle->offBits = ReadNBytes(buffer + 10, 4);
	TraceFileHeader();

	//bitmap info
	infoHeaderHandle = make_shared<BitMapInfoHeader>();
	infoHeaderHandle->size = ReadNBytes(&buffer[14], 4);
	if (infoHeaderHandle->size == 40)
	{	
		infoHeaderHandle->width =ReadNBytes(buffer + 18, 4);
		infoHeaderHandle->height = ReadNBytes(buffer + 22, 4);
		infoHeaderHandle->planes = ReadNBytes(buffer + 26, 2);
		infoHeaderHandle->bits = ReadNBytes(buffer + 28, 2);
		infoHeaderHandle->compression = ReadNBytes(buffer + 30, 4);
		infoHeaderHandle->imageSize = ReadNBytes(buffer + 34, 4);
		infoHeaderHandle->xResolution = ReadNBytes(buffer + 38, 4);
		infoHeaderHandle->yResolution = ReadNBytes(buffer + 42, 4);
		infoHeaderHandle->nColors = ReadNBytes(buffer + 46, 4);
		infoHeaderHandle->importantColors = ReadNBytes(buffer + 50, 4);
		TraceInfoHeader();
	}
	
	else
	{
		cout << "not a supported bmp image" << endl;
		exit(4);
	}

	if (fileHeaderHandle->offBits == (14 + infoHeaderHandle->size) || infoHeaderHandle->bits >= 24)
	{
		palette = nullptr;
	}
	else
	{

		if (infoHeaderHandle->nColors == 0)
		{
			palette = make_shared<DWORD>(pow(2, infoHeaderHandle->bits));
			//todo
			//memcpy(palette.get(), buffer + 14 + infoHeaderHandle->size, fileHeaderHandle->size - fileHeaderHandle->offBits);
		}

	}

	pixels = make_shared<BYTE>(fileHeaderHandle->size - fileHeaderHandle->offBits);
	//memcpy(pixels.get(), buffer + fileHeaderHandle->offBits + 1, fileHeaderHandle->size - fileHeaderHandle->offBits);

	

};

ImageType Image::TypeofImage(const char *fileName)
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
			handle->processor->LoadImages(fileName);
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