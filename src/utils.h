#pragma once
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

void die(const char *format, ...);
void readfile(const char* fname, char** buffer);

