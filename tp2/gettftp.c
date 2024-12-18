#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  

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

return 0;
}