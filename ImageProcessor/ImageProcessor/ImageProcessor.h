// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� IMAGEPROCESSOR_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// IMAGEPROCESSOR_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
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
			WORD type;//λͼ�ļ�������1-2�ֽ�
			DWORD size;//λͼ�ļ��Ĵ�С�����ֽ�Ϊ��λ��3-6�ֽڣ���λ��ǰ��
			WORD reserved1;//λͼ�ļ������֣�����Ϊ0(7-8�ֽڣ�
			WORD reserved2;//λͼ�ļ������֣�����Ϊ0(9-10�ֽڣ�
			DWORD offBits;//λͼ���ݵ���ʼλ�ã��������λͼ��11-14�ֽڣ���λ��ǰ��
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


