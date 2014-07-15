#include "bitIO.h"

bitIO :: bitIO ( const char * filename, unsigned mode )
: std :: fstream( filename, mode | std::ios::binary )
{
	std::ios::sync_with_stdio( false );
	this->mode = mode;
	this->isEOFbit = false;
	this->Buffer = new unsigned char[BUF_SIZE];
	if ( (this->mode & std::ios::in) && (this->mode & std::ios::out) ) {
		std::cerr << "Wrong mode, exit!" << std::endl;
		exit( 0 );
	} else if ( this->mode & std::ios::in ) {
		this->nBits = 0;
		this->pByte = 0;
		this->curByte = 0;
		this->BUF_END = 0;
	} else if ( this->mode & std::ios::out ) {
		this->nBits = 8;
		this->pByte = 0;
		this->curByte = 0;
		this->BUF_END = -1;
	}
}

void bitIO::close()
{
	if ( this->mode & std::ios::out ) {
		//如果当前字节中被写入了bit，就将整个字节输出到缓冲区
		if ( this->nBits < 8 ) {
			this->writeByte();
		}
		//如果输出缓冲区中还有数据，则应输出后才能关闭
		if ( this->pByte > 0 ) {
			this->write( (char *)this->Buffer, sizeof(unsigned char)*pByte );
		}
	}
	std::fstream::close();
}

bitIO::~bitIO()
{
	delete[] this->Buffer;
}

inline void bitIO::writeByte()
{
	if (this->pByte < BUF_SIZE) {
		this->Buffer[pByte++] = this->curByte;
	} else {
		this->write((char *)this->Buffer, sizeof(unsigned char)*BUF_SIZE);
		this->pByte = 0;
		this->Buffer[this->pByte++] = this->curByte;
	}
	this->curByte = 0;
}


inline void bitIO::writeBit( unsigned char bit )
{
	if ( this->nBits <= 0 ) {
		this->writeByte();
		this->nBits = 8;
	}
	this->nBits--;
	this->curByte |= (bit << this->nBits);
}

void bitIO::writeInt( unsigned data, int length )
{
	//从高位到低位依次将数据输出
	unsigned char val;
	for ( int i = length - 1; i >= 0; i-- ) {
		val = ( data >> i ) & 1;
		writeBit( val );
	}
}

inline void bitIO::readByte()
{
	if ( this->pByte < this->BUF_END ) {
		//如果当前缓冲区中还有数据，则直接读取
		this->curByte = this->Buffer[pByte++];
	} else {
		//否则继续从文件读取数据
		this->read( reinterpret_cast<char *>(this->Buffer), sizeof(unsigned char)*BUF_SIZE );
		this->pByte = 0;
		this->curByte = this->Buffer[this->pByte++];
		this->BUF_END = int(this->gcount());
		//检查是否已经读取到了文件尾
		if ( this->BUF_END == 0 ) {
			this->isEOFbit = true;
		}
	}
}

inline unsigned char bitIO::readBit()
{
	if ( this->nBits <= 0 ) {
		this->readByte();
		this->nBits = 8;
	}
	this->nBits--;
	return ( ( this->curByte >> this->nBits ) & (unsigned char)1 );
}

unsigned bitIO::readInt( int length )
{
	unsigned val = 0;
	while ( length-- ) {
		val |= readBit();
		if ( length > 0 ) val <<= 1;
	}
	return val;
}

