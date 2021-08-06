MATRIX_SUM_MIN_C_FLAGS=-c -O2 -Wall -Wextra -Waggregate-return -Wcast-align -Wcast-qual -Wconversion -Wformat=2 -Winline -Wmissing-prototypes -Wmissing-declarations -Wnested-externs -Wno-import -Wpointer-arith -Wredundant-decls -Wshadow -Wstrict-prototypes -Wwrite-strings

matrix_sum_min: matrix_sum_min.o
	gcc -o matrix_sum_min matrix_sum_min.o

matrix_sum_min.o: matrix_sum_min.c matrix_sum_min.make
	gcc ${MATRIX_SUM_MIN_C_FLAGS} -o matrix_sum_min.o matrix_sum_min.c

clean:
	rm -f matrix_sum_min matrix_sum_min.o
