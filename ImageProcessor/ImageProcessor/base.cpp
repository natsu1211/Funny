#include "base.h"
#include <assert.h>

DWORD LL::ReadNBytes(BYTE* buf, int32_t n)
{
	//auto buf = static_cast<const BYTE*>(buffer);
	DWORD ret = buf[0];
	for (int i = 1; i < n; i++)
	{
		ret |= (buf[i] << 8*i);
	}

	return ret;
}

LL::ImageProcessorBase::~ImageProcessorBase()
{
};

LL::BMPFile::BMPFile(const char *fileName, const char* mode)
{
	Open(fileName, mode);
}

LL::BMPFile::~BMPFile()
{
	Close();
}

bool LL::BMPFile::Open(const char *fileName, const char* mode)
{
	file_ = fopen(fileName, mode);
	if (file_ == NULL)
	{
		std::cout << "file not exist" << std::endl;
		fclose(file_);
		//todo,should let user to have a chance to input again
		exit(1);
	}
}

void LL::BMPFile::Close()
{
	if (file_)
	{
		fclose(file_);
		file_ = NULL;
	}

	/*if (buffer)
	{
		delete buffer;
	}*/
}

bool LL::BMPFile::Read(void *buffer, int32_t size)
{
	using namespace std;
	long fileSize;
	//BYTE* buffer = NULL;
	assert(file_ != NULL);
	fseek(file_, 0, SEEK_END);
	fileSize = ftell(file_);
	rewind(file_);

	buffer = (BYTE*)malloc(sizeof(BYTE)*fileSize);

	if (buffer == NULL) { exit(2); }

	// copy the file into the buffer
	size_t result = fread(buffer, 1, fileSize, file_);
	if (result == 0) { exit(7); }
	if (result != fileSize) { exit(6); }
	fclose(file_);

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
		infoHeaderHandle->width = ReadNBytes(buffer + 18, 4);
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


}

bool LL::BMPFile::Write(void *buffer, int32_t size)
{
	//todo
}