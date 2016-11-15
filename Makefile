all: Main

program: Main.o
	g++ -std=c++11 -o lapDiff -L/usr/local/cuda/lib64 -lcuda -lcudart Main.cpp lapDiffCu.o

lapDiffCu.o:
	nvcc -c lapDiffCu.cu 

clean: rm -f *o lapDiff
