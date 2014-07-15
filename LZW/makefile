CC = g++
OBJS = main.o LZW.o bitIO.o UnitTest.o
EXEC = LZW
FLAG = -O2

$(EXEC): $(OBJS)
	$(CC) -o $@ $^ $(FLAG)
main.o: main.cpp bitIO.h
	$(CC) -c $< $(FLAG)
LZW.o: LZW.cpp bitIO.h
	$(CC) -c $< $(FLAG)
bitIO.o: bitIO.cpp bitIO.h
	$(CC) -c $< $(FLAG)
UnitTest.o: UnitTest.cpp bitIO.h
	$(CC) -c $< $(FLAG)
clean:
	rm -rf $(EXEC) *.o
