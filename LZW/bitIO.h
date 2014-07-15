#ifndef BITIO_H_
#define BITIO_H_

#define _CRT_SECURE_NO_DEPRECATE
//#define DEBUG

#include <fstream>
#include <iostream>
#include <string>

using std::endl;
using std::cerr;
using std::string;
using std::ifstream;
using std::ofstream;

/*定义缓冲区大小*/
const int BUF_SIZE = 1048576;

class bitIO : public std::fstream
{
public:
	bitIO(const char * filename, unsigned mode);
	~bitIO();
	void close( );
	/*按位写入数据*/
	void writeBit( unsigned char bit );
	/*按长度写入整数*/
	void writeInt( unsigned data, int length );
	/*按位读取数据*/
	unsigned char readBit();
	/*按长度读取整数*/
	unsigned readInt( int length );
	/*返回是否读取到了文件末尾*/
	bool isEOF() { return this->isEOFbit; }
private:
	void writeByte();
	void readByte();
	unsigned char * Buffer;
	unsigned char curByte;
	unsigned mode;
	int BUF_END;
	int nBits;
	int pByte;
	bool isEOFbit;
};

/*函数声明*/
bool UnitTestCase_1();
bool UnitTestCase_2();
void LZW_Compress( ifstream & fin, bitIO & binfout );
void LZW_Decompress( ofstream & fout, bitIO & binfin );

#endif