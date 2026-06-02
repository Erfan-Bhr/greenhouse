#include <tT.h>
#include <env.h>
#include <stdio.h>

void startup(void)
{
    printf("=================================\n");
    printf(" Greenhouse Monitoring System\n");
    printf(" TinyTimber Started Successfully\n");
    printf("=================================\n");

    printf("Startup Function Executed\n");
}

ENV_STARTUP(startup);