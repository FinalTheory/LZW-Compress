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

/*���建������С*/
const int BUF_SIZE = 1048576;

class bitIO : public std::fstream
{
public:
	bitIO(const char * filename, unsigned mode);
	~bitIO();
	void close( );
	/*��λд������*/
	void writeBit( unsigned char bit );
	/*������д������*/
	void writeInt( unsigned data, int length );
	/*��λ��ȡ����*/
	unsigned char readBit();
	/*�����ȶ�ȡ����*/
	unsigned readInt( int length );
	/*�����Ƿ��ȡ�����ļ�ĩβ*/
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

/*��������*/
bool UnitTestCase_1();
bool UnitTestCase_2();
void LZW_Compress( ifstream & fin, bitIO & binfout );
void LZW_Decompress( ofstream & fout, bitIO & binfin );

#endif