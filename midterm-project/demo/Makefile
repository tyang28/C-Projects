CC=gcc
CFLAGS=-std=c99 -pedantic -Wall -Wextra -g


project: project.o ppm_io.o imageManip.o
	$(CC) project.o ppm_io.o imageManip.o -lm -o project

demo_ppm.o: demo_ppm.c ppm_io.h imageManip.h imageManip.c
	$(CC) $(CFLAGS) -c demo_ppm.c

ppm_io.o: ppm_io.c ppm_io.h
	$(CC) $(CFLAGS) -c ppm_io.c

imageManip.o: imageManip.c imageManip.h
	$(CC) $(CFLAGS) -c  imageManip.c

clean:
	rm -f *.o project
