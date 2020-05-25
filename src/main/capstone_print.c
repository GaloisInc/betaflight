/*
 * This file is temp for debugging purpose - riscv team
 */
#pragma once
#include <stdio.h>

#define NO_DEBUG_CAP
void print_my_msg(char * msg, const char * func, char * file, int line){
#ifdef NO_DEBUG_CAP
    printf("*** %s *** \n\n", msg);
#else
    printf("*** %s *** \n\n[func] %s() | [file] %s:%d\n\n", msg, func, file, line);
#endif
    return;
}
