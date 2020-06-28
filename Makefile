OPTIMIZATION_FLAG = -O3
CPP_VERSION_FLAG = c++17

findstr:	findstr.o
			g++ -o findstr findstr.o  -std=$(CPP_VERSION_FLAG) $(OPTIMIZATION_FLAG)

findstr.o:	findstr.cpp
			g++ -c findstr.cpp  -std=$(CPP_VERSION_FLAG) $(OPTIMIZATION_FLAG)  

clean:	
			rm -f findstr findstr.o
			
						