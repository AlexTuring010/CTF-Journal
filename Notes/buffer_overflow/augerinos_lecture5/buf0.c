#include <string.h>
#include <unistd.h>
#include <stdio.h>

void print(char *message)
{
    char buffer[64];
    strcpy(buffer, message);
    printf("%s\n", buffer);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <name>\n", argv[0]);
        return 1;
    }
    print(argv[1]);
    return 0;
}