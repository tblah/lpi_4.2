COMPILE_OPTS=-O3 -g -Wall

all: cp sparse

sparse: sparse.c
	cc $(COMPILE_OPTS) -o sparse sparse.c

cp: cp.c
	cc $(COMPILE_OPTS) -o cp cp.c

clean:
	rm cp sparse
