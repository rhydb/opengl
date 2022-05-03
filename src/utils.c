#include "utils.h"

void
die(const char *format, ...)
{
	va_list vl;
	va_start(vl, format);
	vfprintf(stderr, format, vl);
	va_end(vl);
	exit(EXIT_FAILURE);
}

void
readfile(const char* fname, char** buffer)
{
    char *fbuffer = *buffer;
    FILE *fptr = fopen(fname, "r");
    long flen;
    if (!fptr) {
        die("Failed to open %s\n", fname);
    }
    if ((fseek(fptr, 0, SEEK_END)) == -1) {
		die("Error with fseek() SEEK_END\n");
	}
    if ((flen = ftell(fptr)) == -1) {
		die("Error with ftell()\n");
	}
    if ((fseek(fptr, 0, SEEK_SET)) == -1) {
		die("Error with fseek() SEEK_SET\n");
	}
    fbuffer = (char*)malloc(flen + 1);
    if (fbuffer) {
        fread(fbuffer, 1, flen, fptr);
		fbuffer[flen] = '\0';
    } else {
		printf("Failed to malloc file buffer!\n");
	}
    fclose(fptr);
	*buffer = fbuffer;
}

