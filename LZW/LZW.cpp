#include "bitIO.h"
#include <map>
#include <climits>

using std::map;

/*定义字典的初始大小*/
const unsigned DICT_SIZE = 256;
/*定义字典的起始编号*/
const unsigned CODE_START = 256;

/*定义压缩码的最大长度*/
const int MAX_CODE_LEN = 14;
/*定义压缩码的最小长度*/
const int MIN_CODE_LEN = 9;
/*定义清空标记*/
const int SIG_CLEAR = (1 << MAX_CODE_LEN) - 1;

void LZW_Compress( ifstream & fin, bitIO & binfout )
{
	map<string, unsigned> Dict;
	for ( unsigned i = 0; i < DICT_SIZE; i++ ) Dict[string(1, i)] = i;
	
	int CodeLength = MIN_CODE_LEN;
	unsigned CodeIndex  = CODE_START;

	int CH;
	string STR;

	while ( 1 ) {
		CH = fin.get();
		if ( fin.eof() ) break;
		if ( Dict.find( STR + char(CH) ) != Dict.end() ) {
			STR += char(CH);
		} else {
			unsigned Code = Dict[STR];
			/*检查并增加编码长度*/
			while ( Code >= (1 << CodeLength) - 1 && CodeLength <= MAX_CODE_LEN ) {
				binfout.writeInt( (1 << CodeLength) - 1, CodeLength );
				CodeLength++;
			}
			binfout.writeInt( Code, CodeLength );
			Dict[STR + char(CH)] = CodeIndex++;
			/*如果字典已满*/
			if ( CodeIndex >= SIG_CLEAR ) {
				/*重建字典*/
				Dict.clear();
				for ( int i = 0; i < DICT_SIZE; i++ ) Dict[string( 1, i )] = i;
				/*写入CLEAR标记*/
				binfout.writeInt( SIG_CLEAR, MAX_CODE_LEN );
				//cerr << "Write CLEAR signal, rebuild dictionary!" << endl;
				/*状态变量重新初始化*/
				CodeLength = MIN_CODE_LEN;
				CodeIndex  = CODE_START;
			}
			STR = char(CH);
		}
	}
	if ( !STR.empty( ) ) binfout.writeInt( Dict[STR], CodeLength );
}

void LZW_Decompress( ofstream & fout, bitIO & binfin )
{
	map<int, string> Dict;
	map<int, string> ::iterator iter;
	for ( unsigned i = 0; i < DICT_SIZE; i++ ) Dict[i] = string( 1, i );

	unsigned Code;
	int CodeLength = MIN_CODE_LEN;
	unsigned CodeIndex = CODE_START;

	string OldString, NewString;

	Code = binfin.readInt( CodeLength );
	if ( !binfin.isEOF() ) {
		OldString = Dict[Code];
		fout << OldString;
	}

	while ( true ) {
		/*读入一个编码，如果无效则退出*/
		Code = binfin.readInt( CodeLength );
		if ( binfin.isEOF( ) ) break;

		/*首先检查是否需要重建字典*/
		if ( Code == SIG_CLEAR ) {
			//cerr << "Found CLEAR signal, rebuild dictionary!" << endl;
			/*重建字典*/
			Dict.clear();
			for ( unsigned i = 0; i < DICT_SIZE; i++ ) Dict[i] = string( 1, i );
			/*状态变量重新初始化*/
			OldString = NewString = "";
			CodeLength = MIN_CODE_LEN;
			CodeIndex = CODE_START;
			/*读取第一个编码*/
			Code = binfin.readInt( CodeLength );
			if ( !binfin.isEOF() ) {
				OldString = Dict[Code];
				fout << OldString;
			}
			continue;
		}

		/*其次检查是否是一个增加编码长度的标志*/
		while ( Code == (1 << CodeLength) - 1 && 
				CodeLength <= MAX_CODE_LEN && !binfin.isEOF() ) {
			CodeLength++;
			Code = binfin.readInt( CodeLength );
		}

		if ( ( iter = Dict.find(Code) ) != Dict.end() ) {
			NewString = iter->second;
			Dict[CodeIndex++] = OldString + NewString[0];
		} else {
			NewString = OldString + OldString[0];
			Dict[CodeIndex++] = NewString;
		}

		fout << NewString;
		OldString = NewString;
	}
}