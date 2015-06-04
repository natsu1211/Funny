// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 IMAGEPROCESSOR_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// IMAGEPROCESSOR_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef IMAGEPROCESSOR_EXPORTS
#define IMAGEPROCESSOR_API __declspec(dllexport)
#else
#define IMAGEPROCESSOR_API __declspec(dllimport)
#endif

#include "base.h"


namespace LL
{
	using namespace std;

	enum class ImageType
	{
		BMP,
		JPG,
		PNG,
		UNKNOWN,
	};

	ImageType TypeofImage(const char* fileName);
	
	class BMPDecoder : public DecoderBase
	{
		
		struct BitMapFileHeader
		{
			WORD type;//位图文件的类型1-2字节
			DWORD size;//位图文件的大小，以字节为单位（3-6字节，低位在前）
			WORD reserved1;//位图文件保留字，必须为0(7-8字节）
			WORD reserved2;//位图文件保留字，必须为0(9-10字节）
			DWORD offBits;//位图数据的起始位置，以相对于位图（11-14字节，低位在前）
		} ;
		struct BitMapInfoHeader
		{
			DWORD size;/* Info Header size in bytes */
			int32_t width, height;/* Width and height of image */
			WORD planes;/* Number of colour planes */
			WORD bits; /* Bits per pixel */
			DWORD compression; /* Compression type */
			DWORD imageSize; /* Image size in bytes */
			int32_t xResolution, yResolution; /* Pixels per meter */
			DWORD nColors; /* Number of colors */
			DWORD importantColors; /* Important colours */
		};

	public:
		BMPDecoder();
		virtual ~BMPDecoder();
		virtual bool LoadImage(const char *fileName) override;	
		//virtual void SavaImage(const char *fileName) override;

	protected:
		shared_ptr<BitMapFileHeader> fileHeaderHandle;
		shared_ptr<BitMapInfoHeader> infoHeaderHandle;
		shared_ptr<DWORD> palette;
		shared_ptr<BYTE> pixels;
		shared_ptr<ReadStreamBase> bmpfile;
		//BYTE *buffer;
		//for debug
		void TraceFileHeader();
		void TraceInfoHeader();
		
		
	};
	
	

}

struct ImageProcessorHandle;

extern "C"
{
	extern IMAGEPROCESSOR_API void __stdcall OpenImage(ImageProcessorHandle *handle, const char *fileName);
	extern IMAGEPROCESSOR_API ImageProcessorHandle* __stdcall CreateImageProcessor(const char *fileName);
}


