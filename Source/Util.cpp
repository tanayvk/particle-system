#include "Util.h"
#include <iostream>

// read the contents of a file and save them to a string
char* readFileToString(char* filename)
{
    FILE* f = fopen(filename, "rb");
    fseek (f, 0, SEEK_END);
    long length = ftell (f);
    fseek (f, 0, SEEK_SET);
    char* string = (char*)malloc (length);
    fread (string, 1, length, f);
    fclose (f);
    return string;
}

