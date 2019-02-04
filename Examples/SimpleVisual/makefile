SimpleCTRNNMain: SimpleCTRNNMain.o TSearch.o random.o
		g++ -pthread -o SimpleCTRNNMain SimpleCTRNNMain.o TSearch.o random.o
TSearch.o: TSearch.cpp TSearch.h
		g++ -c -O3 -flto TSearch.cpp
random.o: random.cpp random.h VectorMatrix.h
		g++ -c -O3 -flto random.cpp
SimpleCTRNNMain.o: SimpleCTRNNMain.cpp TSearch.h
		g++ -c -O3 -flto SimpleCTRNNMain.cpp
clean:
		rm *.o SimpleCTRNNMain
