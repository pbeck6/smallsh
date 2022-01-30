#include "shell.h"
#include "parse.h"

int main() {
    int status;
    char *prompt = ": ";

    // Set variables for getline()
    char *command = NULL;
    size_t bufsize = 0;
    ssize_t characters;

    while (1) {
        printf("%s", prompt);
        fflush(stdout);
        characters = getline(&command, &bufsize, stdin);
    }

    free(command);

    return 0;
}
//