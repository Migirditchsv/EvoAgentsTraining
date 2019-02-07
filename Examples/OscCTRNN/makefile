CTRNNOscilatorMain: CTRNNOscilatorMain.o TSearch.o random.o
		g++ -pthread -o CTRNNOscilatorMain CTRNNOscilatorMain.o TSearch.o random.o
TSearch.o: TSearch.cpp TSearch.h
		g++ -c -O3 -flto TSearch.cpp
random.o: random.cpp random.h VectorMatrix.h
		g++ -c -O3 -flto random.cpp
CTRNNOscilatorMain.o: CTRNNOscilatorMain.cpp TSearch.h
		g++ -c -O3 -flto CTRNNOscilatorMain.cpp
clean:
		rm *.o CTRNNOscilatorMain
