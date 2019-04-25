intc: int.c mymalloc.o
	gcc -I. int.c mymalloc.o -o intc
# mymalloc.o: mymalloc.c
#	gcc -DCOMPILETIME -c mymalloc.c

intl: int.o mymalloc.o
	gcc -Wl,--wrap,malloc -Wl,--wrap,free int.o mymalloc.o -o intl
int.o: int.c
	gcc -c int.c
mymalloc.o: mymalloc.c
	gcc -DLINKTIME -c mymalloc.c

intr: int.c
	gcc -DRUNTIME -shared -fpic mymalloc.c -ldl -o mymalloc.so	
	gcc int.c -o intr

