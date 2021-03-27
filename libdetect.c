/*
    About:
        Detect usage of, and input to, "dangerous" functions at runtime.
    Compile: 
        gcc -shared -fPIC -ldl -nostartfiles libdetect.c -o libdetect.so
    Usage:
        LD_PRELOAD="<path>/libdetect.so" <elf/command>
        or
        patchelf --add-needed <path>/libdetect.so <target_elf>
    TODO:
        * Add support for a lot more dangerous functions
        * Add support to filter for specific input
        * Add support to print to file instead of stdout
     
*/
#define _GNU_SOURCE
#include <stdio.h>
#include <dlfcn.h>

char *(*___strcpy)(char *dest, const char *src);
char *(*___strcat)(char *dest, const char *src);

void _init()
{
    ___strcpy = dlsym(RTLD_NEXT, "strcpy");
    ___strcat = dlsym(RTLD_NEXT, "strcat");
}

char *strcpy(char *dest, const char *src)
{
    printf("\nlibdetect strcpy: %s\n", src);
    return ___strcpy(dest, src);
}

char *strcat(char *dest, const char *src)
{
    printf("\nlibdetect strcat: %s\n", src);
    return ___strcat(dest, src);
}
