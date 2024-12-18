#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  
#include <string.h>  
#include <fcntl.h>

#define MAX_STR_LEN 512

int main(int argc, char *argv[]) {
    if (argc != 3) {
        const char *usage_message = "Usage: gettftp <server_ip> <filename>\n";
    write(STDOUT_FILENO, usage_message, sizeof(usage_message));
    exit(1);
    }

// on récupère les arguments
    char *server_ip = argv[1];
    char *filename = argv[2];

//affichage 
//serveur IP
write(STDOUT_FILENO, "Server IP: ", sizeof("Server IP: "));
write(STDOUT_FILENO, server_ip, sizeof(server_ip));
write(STDOUT_FILENO, "\n", 1);

//nom du fichier
write(STDOUT_FILENO, "File to upload: ", sizeof("File to download: "));  
write(STDOUT_FILENO, filename, sizeof(filename)); 
write(STDOUT_FILENO, "\n", 1);  

// Ouvrir le fichier en lecture
    int file_fd = open(filename, O_RDONLY);
    if (file_fd < 0) {
        const char *error_message = "Error: Unable to open file.\n";
        write(STDERR_FILENO, error_message, strnlen(error_message, MAX_STR_LEN));
        exit(1);
    }

    // Simulation : Lecture du fichier et affichage du contenu
    char buffer[512];
    ssize_t bytes_read;
    write(STDOUT_FILENO, "Reading file content:\n", strnlen("Reading file content:\n", MAX_STR_LEN));
    while ((bytes_read = read(file_fd, buffer, sizeof(buffer))) > 0) {
        write(STDOUT_FILENO, buffer, bytes_read);
    }

    if (bytes_read < 0) {
        const char *error_message = "Error: Unable to read file.\n";
        write(STDERR_FILENO, error_message, strnlen(error_message, MAX_STR_LEN));
        close(file_fd);
        exit(1);
    }
 // Fermeture du fichier
    close(file_fd);
    write(STDOUT_FILENO, "\nFile reading complete.\n", strnlen("\nFile reading complete.\n", MAX_STR_LEN));

    return 0;
}