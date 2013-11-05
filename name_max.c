#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <limits.h>

int main(int argc, char* argv[])
{
   printf("Max path is %d\n", PATH_MAX);
   printf("File max is %d\n", NAME_MAX);
   return 0;
}
