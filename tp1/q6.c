#include <unistd.h>   // For execvp, fork, write, read
#include <string.h>   // For strtok, strcmp
#include <stdlib.h>   // For exit, NULL
#include <sys/wait.h> // For wait
#include <stdio.h>    // For perror
#include "enseah.h"

/////////////////////////////////////////////////////////////////////////////////////////
//                                 QUESTION 6                                         //
///////////////////////////////////////////////////////////////////////////////////////

int main() {
    char buffer[BUFSIZE]; // Buffer for user input
    char *args[BUFSIZE / 2]; // Array to store command arguments

    while (1) {
       write(STDOUT_FILENO, prompt, sizeof(prompt));

        int letter = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
        if (letter <= 0) {
            write(STDOUT_FILENO, quitMessage, sizeof(quitMessage));
            break;
        }
        buffer[letter - 1] = '\0'; 

        if (strcmp(buffer, "exit") == 0) {
            write(STDOUT_FILENO, quitMessage, sizeof(quitMessage));
            break;
        }

        //Split the input command into tokens (arguments) using spaces

        char *token = strtok(buffer, " ");  
        int i = 0;
        while (token != NULL) {
            args[i] = token; // Store each argument
            i++;
            token = strtok(NULL, " "); // Get the next token

        }
        args[i] = NULL; // Null-terminate the array of arguments

        pid_t pid = fork(); // Create a child process
        if (pid == 0) {
            // In the child process, execute the command with arguments
            execvp(args[0], args);

            // If execvp fails, print an error message
            perror(UnknownCommand);
            exit(EXIT_FAILURE);
        } else {
            // In the parent process, wait for the child to finish
            int status;
            wait(&status);
        }
    }

    return 0;
}
