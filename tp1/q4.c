#include "enseah.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>

/////////////////////////////////////////////////////////////////////////////////////////
//                                 QUESTION 4                                         //
///////////////////////////////////////////////////////////////////////////////////////

#define MSGSIZE 64  // Buffer size for formatted messages
int main() {
    char msg[MSGSIZE];  // Buffer for formatted messages

    while (1) {
        // Display the prompt
        write(STDOUT_FILENO, prompt, sizeof(prompt) - 1);

        // Read user input
        int letter = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
        if (letter <= 0) {
            write(STDOUT_FILENO, quitMessage, sizeof(quitMessage));
            break; // Exit on Ctrl+D or read error
        }
buffer[letter - 1] = '\0'; // Remove the newline character

        // Check if the user entered "exit" to quit
        if (strcmp(buffer, exitCommand) == 0) {
            write(STDOUT_FILENO, "Bye bye ;) !\n", 13);
            break;
        }

        pid_t pid = fork(); // Create a child process
        if (pid == 0) {
            // In the child process, execute the command
            execlp(buffer, buffer, NULL);
  // If execlp fails, display an error message
            write(STDOUT_FILENO, UnknownCommand, sizeof(UnknownCommand) - 1);
            exit(EXIT_FAILURE);
        } else {
            // In the parent process
            int status;
            wait(&status); // wait for the child to finish

            // Check if the child terminated normally
            if (WIFEXITED(status)) {
                WEXITSTATUS(status);
            // Format the exit message as "enseash[exit:<code>]%"
                int len = snprintf(msg, MSGSIZE, "%s%d%s\n", exit_debut, WEXITSTATUS(status), exit_fin);
                write(STDOUT_FILENO, msg, len);
            }
            // Check if the child was terminated by a signal
            else if (WIFSIGNALED(status)) {
                WTERMSIG(status);
             // so format the signal message as "enseash[sign:<signal>]%"
                int len = snprintf(msg, MSGSIZE, "%s%d%s\n", signal_debut, WTERMSIG(status), signal_fin);
                write(STDOUT_FILENO, msg, len);
            }
        }
    }

    return 0;
}
