#include "bitIO.h"
#include <climits>
#include <ctime>

using std::ofstream;
using std::ifstream;

bool UnitTestCase_1()
{
	srand( time( NULL ) );

	ofstream fout1( "test.len" );
	ofstream fout2( "test.ans" );

	bitIO bfout( "test.bin", std::ios::out );
	
	int TEST_NUM = rand() % 100000 + 10000;
	unsigned num;
	int len;

	for ( int i = 0; i < TEST_NUM; i++ ) {
		num = rand() % UINT_MAX;
		for ( int i = 31; i >= 0; i-- ) {
			if ( (num >> i) & 1 ) {
				len = i + 1;
				break;
			}
		}
		fout1 << len << '\n';
		fout2 << num << '\n';
		bfout.writeInt( num, len );
	}

	fout1.close();
	fout2.close( );
	bfout.close();

	ifstream fin1( "test.len" );
	ifstream fin2( "test.ans" );

	bitIO bfin( "test.bin", std::ios::in );

	for ( int i = 0; i < TEST_NUM; i++ ) {
		fin1 >> len;
		fin2 >> num;
		if ( bfin.readInt( len ) != num )
			return false;
	}

	fin1.close();
	fin2.close( );
	bfin.close( );

	remove( "test.len" );
	remove( "test.ans" );
	remove( "test.bin" );

	return true;
}

bool UnitTestCase_2()
{
	srand( time( NULL ) );
	
	/*创建测试输入文件*/
	ofstream fout( "test.in", std::ios::binary );
	for ( int i = 0; i < 2 * BUF_SIZE + BUF_SIZE / 2; i++ ) {
		fout << (unsigned char)(rand() % 256);
	}
	fout.close();

	/*将输入文件压缩*/
	ifstream fin( "test.in", std::ios::binary );
	bitIO binfout( "test.lzw", std::ios::out );
	LZW_Compress( fin, binfout );
	fin.close();
	binfout.close();

	/*将压缩文件解压缩*/
	fout.open( "test.out", std::ios::binary );
	bitIO binfin( "test.lzw", std::ios::in );
	LZW_Decompress( fout, binfin );
	fout.close();
	binfin.close();

	/*比较解压缩文件与原始文件*/
	ifstream fin1( "test.in", std::ios::binary );
	ifstream fin2( "test.out", std::ios::binary );

	int ch1, ch2;
	for ( int i = 0; i < 2 * BUF_SIZE + BUF_SIZE / 2; i++ ) {
		ch1 = fin1.get();
		ch2 = fin2.get();
		if ( ch1 != ch2 ) return false;
	}
	if ( fin1.eof() != fin2.eof() ) return false;

	fin1.close();
	fin2.close( );

	remove( "test.in" );
	remove( "test.out" );
	remove( "test.lzw" );

	return true;
}