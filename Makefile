default: prodcons.c prodcons.h
	gcc -m32 -o prodcons -I /u/OSLab/USERNAME/linux-2.6.23.1/include/ prodcons.c
clean:
	rm prodcons
