genOscMain: genOscMain.o CTRNN.o TSearch.o random.o
		g++ -pthread -o genOscMain genOscMain.o CTRNN.o TSearch.o random.o
CRTNN.o: CTRNN.cpp CTRNN.h random.h
	g++ -c --O3 -flto CTRNN.cpp
TSearch.o: TSearch.cpp TSearch.h
		g++ -c -O3 -flto TSearch.cpp
random.o: random.cpp random.h VectorMatrix.h
		g++ -c -O3 -flto random.cpp
genOscMain.o: genOscMain.cpp TSearch.h
		g++ -c -O3 -flto genOscMain.cpp
clean:
		rm *.o genOscMain
