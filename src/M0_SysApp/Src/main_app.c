#include "stdio.h"
#include "main_app.h"
void app_main(void)
{
    printf("Hello from app_main!\n");
}

uint32_t main_app_dummy(uint32_t param)
{
    return param + 1;
}