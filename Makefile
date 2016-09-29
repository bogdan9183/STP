build : main.o input-output.o graphOps.o
	gcc  $^ -o stp
main.o : main.c
	gcc -c -Wall -ggdb $^
input-output.o : input-output.c
	gcc -c -Wall -ggdb $^
graphOps.o : graphOps.c
	gcc -c -Wall -ggdb $^
clean :
	rm *.o stp
run : stp
	./stp initialise.in topology.in tasks.in stp.out
