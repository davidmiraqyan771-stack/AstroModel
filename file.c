#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
void terminalClear(char *prompt) {
    if(strcmp(prompt, "clear") == 0) {
        printf("\033[2J\033[H");
    }
}


int main (void) {
    char prompt[50];
    scanf("%s", prompt);
    terminalClear(prompt);
}