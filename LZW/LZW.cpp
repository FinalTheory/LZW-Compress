#include "bitIO.h"
#include <map>
#include <climits>

using std::map;

/*�����ֵ�ĳ�ʼ��С*/
const unsigned DICT_SIZE = 256;
/*�����ֵ����ʼ���*/
const unsigned CODE_START = 256;

/*����ѹ�������󳤶�*/
const int MAX_CODE_LEN = 14;
/*����ѹ�������С����*/
const int MIN_CODE_LEN = 9;
/*������ձ��*/
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
			/*��鲢���ӱ��볤��*/
			while ( Code >= (1 << CodeLength) - 1 && CodeLength <= MAX_CODE_LEN ) {
				binfout.writeInt( (1 << CodeLength) - 1, CodeLength );
				CodeLength++;
			}
			binfout.writeInt( Code, CodeLength );
			Dict[STR + char(CH)] = CodeIndex++;
			/*����ֵ�����*/
			if ( CodeIndex >= SIG_CLEAR ) {
				/*�ؽ��ֵ�*/
				Dict.clear();
				for ( int i = 0; i < DICT_SIZE; i++ ) Dict[string( 1, i )] = i;
				/*д��CLEAR���*/
				binfout.writeInt( SIG_CLEAR, MAX_CODE_LEN );
				//cerr << "Write CLEAR signal, rebuild dictionary!" << endl;
				/*״̬�������³�ʼ��*/
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
		/*����һ�����룬�����Ч���˳�*/
		Code = binfin.readInt( CodeLength );
		if ( binfin.isEOF( ) ) break;

		/*���ȼ���Ƿ���Ҫ�ؽ��ֵ�*/
		if ( Code == SIG_CLEAR ) {
			//cerr << "Found CLEAR signal, rebuild dictionary!" << endl;
			/*�ؽ��ֵ�*/
			Dict.clear();
			for ( unsigned i = 0; i < DICT_SIZE; i++ ) Dict[i] = string( 1, i );
			/*״̬�������³�ʼ��*/
			OldString = NewString = "";
			CodeLength = MIN_CODE_LEN;
			CodeIndex = CODE_START;
			/*��ȡ��һ������*/
			Code = binfin.readInt( CodeLength );
			if ( !binfin.isEOF() ) {
				OldString = Dict[Code];
				fout << OldString;
			}
			continue;
		}

		/*��μ���Ƿ���һ�����ӱ��볤�ȵı�־*/
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