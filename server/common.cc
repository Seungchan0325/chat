#include "common.h"

#include <stdio.h>
#include <stdlib.h> //exit

void ErrorHandling(char* message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}