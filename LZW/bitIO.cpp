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
		//�����ǰ�ֽ��б�д����bit���ͽ������ֽ������������
		if ( this->nBits < 8 ) {
			this->writeByte();
		}
		//�������������л������ݣ���Ӧ�������ܹر�
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
	//�Ӹ�λ����λ���ν��������
	unsigned char val;
	for ( int i = length - 1; i >= 0; i-- ) {
		val = ( data >> i ) & 1;
		writeBit( val );
	}
}

inline void bitIO::readByte()
{
	if ( this->pByte < this->BUF_END ) {
		//�����ǰ�������л������ݣ���ֱ�Ӷ�ȡ
		this->curByte = this->Buffer[pByte++];
	} else {
		//����������ļ���ȡ����
		this->read( reinterpret_cast<char *>(this->Buffer), sizeof(unsigned char)*BUF_SIZE );
		this->pByte = 0;
		this->curByte = this->Buffer[this->pByte++];
		this->BUF_END = int(this->gcount());
		//����Ƿ��Ѿ���ȡ�����ļ�β
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

