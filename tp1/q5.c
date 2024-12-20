
#include <unistd.h>     // For write, read
#include <string.h>     // For strcmp
#include <stdlib.h>     // For exit
#include <sys/wait.h>   // For wait and macros
#include <stdio.h>      // For snprintf
#include <time.h>       // For clock_gettime
#include "enseah.h"

#define MSGSIZE 64 // Buffer size for formatted messages
/////////////////////////////////////////////////////////////////////////////////////////
//                                 QUESTION 5                                         //
///////////////////////////////////////////////////////////////////////////////////////

int main() {
    char msg[MSGSIZE];           // Buffer to store formatted messages
    struct timespec start, end;  // To measure execution time
  while (1) {

        write(STDOUT_FILENO, prompt, sizeof(prompt) - 1);

        ssize_t letter = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
        if (letter <= 0) {
            write(STDOUT_FILENO, quitMessage, sizeof(quitMessage) - 1);
            break; 
        }
        buffer[letter - 1] = '\0'; 

        if (strcmp(buffer, exitCommand) == 0) {
            write(STDOUT_FILENO, quitMessage, sizeof(quitMessage) - 1);
            break;
        }

        // Start the timer
        clock_gettime(CLOCK_MONOTONIC, &start);
        pid_t pid = fork(); // Create a child process
        if (pid == 0) {

            execlp(buffer, buffer, NULL);
            write(STDOUT_FILENO, UnknownCommand, sizeof(UnknownCommand) - 1);
            exit(EXIT_FAILURE);

        } else {
            int status;
            wait(&status);
            //Stop the timer
            clock_gettime(CLOCK_MONOTONIC, &end); // Record the end time after the command execution

            // Calculate the elapsed time in milliseconds
            long elapsedTime = (end.tv_sec - start.tv_sec) * 1000 +
                               (end.tv_nsec - start.tv_nsec) / 1000000;

            if (WIFEXITED(status)) {
                int exit_code = WEXITSTATUS(status);

                int len = snprintf(msg, MSGSIZE, "%s%d|%ldms%s\n", exit_debut, exit_code, elapsedTime, exit_fin);
                write(STDOUT_FILENO, msg, len);
            }
            else if (WIFSIGNALED(status)) {
                int signal_num = WTERMSIG(status);

                // Format the message: "enseash [sign:<signal>|<time>ms]"
                int len = snprintf(msg, MSGSIZE, "%s%d|%ldms%s\n", signal_debut, signal_num, elapsedTime, signal_fin);
                write(STDOUT_FILENO, msg, len);
            }
        }
    }

    return 0;
}
