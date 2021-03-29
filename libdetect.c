/*
    About:
        Detect usage of, and input to, "dangerous" functions.
    Compile: 
        gcc -shared -fPIC -ldl -nostartfiles libdetect.c -o libdetect.so
    Usage:
        LD_PRELOAD="<path>/libdetect.so" <elf/command>
        or
        patchelf --add-needed <path>/libdetect.so <target_elf>
    TODO:
        * Add support for a lot more dangerous functions
        * Add support to filter for specific input
*/
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <dlfcn.h>

char *(*___strcpy)(char *dest, const char *src);
char *(*___strcat)(char *dest, const char *src);
char *(*___gets)(char *s);
//int   (*___sprintf)(char *str, const char *format, ...);

void _init()
{
	___strcpy   = dlsym(RTLD_NEXT, "strcpy");
	___strcat   = dlsym(RTLD_NEXT, "strcat");
	___gets     = dlsym(RTLD_NEXT, "gets");
	//___sprintf  = dlsym(RTLD_NEXT, "sprintf");
}

void log_call(const char *func, const char *value)
{
    FILE *logfile = fopen("/tmp/libdetect.log", "a+");
    fprintf(logfile, "libdetect (%u) %4s: %s\n", getpid(), func, value);
    fclose(logfile);
}

char *strcpy(char *dest, char *src)
{
    log_call("strcpy", src);
	return ___strcpy(dest, src);
}

char *strcat(char *dest, const char *src)
{
    log_call("strcat", src);
	return ___strcat(dest, src);
}

// implemented full sprintf function due to va_list problems
int sprintf(char *buf, const char *fmt, ...)
{
    va_list args;
    int i;

    va_start(args, fmt);
    i = vsprintf(buf, fmt, args);
    va_end(args);
    log_call("sprintf", buf);
    return i;
}

char *gets(char *s)
{
    char *c;
    c = ___gets(s);
    log_call("gets", s);
    return c;
}
