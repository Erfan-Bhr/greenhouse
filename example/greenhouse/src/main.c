#include <tT.h>
#include <env.h>
#include <stdio.h>

void startup(void)
{
    printf("Greenhouse System Started\n");
}

ENV_STARTUP(startup);