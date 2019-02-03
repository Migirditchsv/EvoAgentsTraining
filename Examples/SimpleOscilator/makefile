main: main.o CTRNN.o TSearch.o random.o
		g++ -pthread -o main main.o CTRNN.o TSearch.o random.o
random.o: random.cpp random.h VectorMatrix.h
		g++ -c -O3 -flto random.cpp
TSearch.o: TSearch.cpp TSearch.h
		g++ -c -O3 -flto TSearch.cpp
CTRNN.o: CTRNN.cpp CTRNN.h VectorMatrix.h random.h
		g++ -c -O3 -flto CTRNN.cpp
main.o: main.cpp TSearch.h
		g++ -c -O3 -flto main.cpp
clean:
		rm *.o main
