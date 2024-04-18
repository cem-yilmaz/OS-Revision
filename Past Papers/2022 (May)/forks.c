#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define LOOPS 5

int main()
{
    int ret, i, j = -1;
    for (i = 0; i < LOOPS; i++) {
        ret = fork();
        if (ret > 0) { //parent
            wait(0);
            break;
        }
    }
    printf("ret is: %d %d\n", ret, j);
    j = i;
    return 0;
}