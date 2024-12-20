#include <unistd.h> // For fork, execlp, write, read
#include <string.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "enseah.h"

/////////////////////////////////////////////////////////////////////////////////////////
//                                 QUESTION 1 à 3                                     //
///////////////////////////////////////////////////////////////////////////////////////

// Buffer user input
char buffer[BUFSIZE];

int main() {
    // Display the welcome message
    write(STDOUT_FILENO, welcome, sizeof(welcome));

    while (1) {
        // Display the shell prompt
        write(STDOUT_FILENO, prompt, sizeof(prompt));

        // Read user input into the buffer
        int letter = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);

     // Handle errors or Ctrl+D
        if (letter <= 0) {
            if (letter == 0) {
                break; 
            }
            perror("Erreur de lecture");
            break; // Exit on read error 
        }
        
        buffer[letter - 1] = '\0'; // Remove the newline character at the end of the input
        // Compare the input with "exit" to terminate the shell
        if (strcmp(buffer, exitCommand) == 0) {
            break;    //Exit the shell
        }
    }

    // Exit message before leaving the shell
    write(STDOUT_FILENO, quitMessage, sizeof(quitMessage));
    return 0;
}