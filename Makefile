FLAGS=-Wall -Wextra -Wpedantic
LIBS=-lhipblas -lhiprand

all:
	hipcc ${FLAGS} ${LIBS} bug.cpp -o bug

clean:
	rm *.csv bug
