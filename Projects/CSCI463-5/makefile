rv32i: main.cpp memory.cpp hex.cpp rv32i_decode.cpp rv32i_hart.cpp registerfile.cpp cpu_single_hart.cpp memory.h hex.h rv32i_decode.h rv32i_hart.h registerfile.h cpu_single_hart.h
	g++ -g -ansi -pedantic -Wall -Werror -std=c++14 -c -o main.o main.cpp
	g++ -g -ansi -pedantic -Wall -Werror -std=c++14 -c -o rv32i_decode.o rv32i_decode.cpp
	g++ -g -ansi -pedantic -Wall -Werror -std=c++14 -c -o memory.o memory.cpp
	g++ -g -ansi -pedantic -Wall -Werror -std=c++14 -c -o hex.o hex.cpp
	g++ -g -ansi -pedantic -Wall -Werror -std=c++14 -c -o registerfile.o registerfile.cpp
	g++ -g -ansi -pedantic -Wall -Werror -std=c++14 -c -o rv32i_hart.o rv32i_hart.cpp
	g++ -g -ansi -pedantic -Wall -Werror -std=c++14 -c -o cpu_single_hart.o cpu_single_hart.cpp
	g++ -g -ansi -pedantic -Wall -Werror -std=c++14 -o rv32i main.o rv32i_decode.o memory.o hex.o registerfile.o rv32i_hart.o cpu_single_hart.o
clean:
	rm -f *.o rv32i
