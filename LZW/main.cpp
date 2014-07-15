#include "bitIO.h"

int main(int argc, char * argv[])
{

#ifdef DEBUG

	if ( UnitTestCase_1() ) {
		cerr << "Test case 1 passed!" << endl;
	} else {
		cerr << "Test case 1 not pass!" << endl;
	}
	if ( UnitTestCase_2() ) {
		cerr << "Test case 2 passed!" << endl;
	} else {
		cerr << "Test case 2 not pass!" << endl;
	}
	exit( 0 );

#endif

	if ( argc != 4 ) {
		cerr << "Unknown parameters!" << endl;
		exit( 0 );
	}

	string paraType = argv[1];
	string Src = argv[2];
	string Dst = argv[3];

	if ( paraType == "-compress" ) {
		cerr << "Now start compressing!" << endl;
		ifstream fin(Src.c_str(), std::ios::binary);
		bitIO binfout(Dst.c_str(), std::ios::out);
		LZW_Compress( fin, binfout );
		fin.close();
		binfout.close();
		cerr << "Compressing Finished!" << endl;
	} else if ( paraType == "-decompress" ) {
		cerr << "Now start decompressing!" << endl;
		ofstream fout( Dst.c_str( ), std::ios::binary );
		bitIO binfin( Src.c_str(), std::ios::in );
		LZW_Decompress( fout, binfin );
		fout.close();
		binfin.close();
		cerr << "Decompressing Finished!" << endl;
	} else {
		cerr << "Invalid parameter!" << endl;
		return -1;
	}

	return 0;
}	