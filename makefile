all:
	gcc -g -std=c99 -o cpu cpuTime.c -lm && gcc -g -std=c99 -o read readCPU.c
clean:
	rm cpu read avg.dat child.dat total.dat

