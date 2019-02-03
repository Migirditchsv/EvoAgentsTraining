SimpleSearchMain: SimpleSearchMain.o TSearch.o random.o
		g++ -pthread -o SimpleSearchMain SimpleSearchMain.o TSearch.o random.o
TSearch.o: TSearch.cpp TSearch.h
		g++ -c -O3 -flto TSearch.cpp
random.o: random.cpp random.h VectorMatrix.h
		g++ -c -O3 -flto random.cpp
SimpleSearchMain.o: SimpleSearchMain.cpp TSearch.h
		g++ -c -O3 -flto SimpleSearchMain.cpp
clean:
		rm *.o SimpleSearchMain
