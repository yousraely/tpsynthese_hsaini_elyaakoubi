#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>      // Pour getaddrinfo, freeaddrinfo, gai_strerror
#include <sys/socket.h> // Pour socket, sendto
#include <arpa/inet.h>  // Pour inet_ntop
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

//addr info
struct addrinfo hints, *res;
memset(&hints, 0, sizeof(hints));
hints.ai_family = AF_INET;      
hints.ai_socktype = SOCK_DGRAM;

// adresse du serveur
    int ret = getaddrinfo(argv[1], "1069", &hints, &res);
    if (ret != 0) {
        fprintf(stderr, "Error: Unable to resolve server address: %s\n", gai_strerror(ret));
        exit(1);
    }

    // création du socket UDP
    int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd < 0) {
        perror("Error: Unable to create socket");
        freeaddrinfo(res);  
        exit(1);
    }

    // Ouvrir le fichier à envoyer
    int file_fd = open(argv[2], O_RDONLY);
    if (file_fd < 0) {
        perror("Error: Unable to open file");
        close(sockfd);   // Fermer le socket avant de quitter
        freeaddrinfo(res);  // Libérer la mémoire allouée par getaddrinfo
        exit(1);
    }

// Envoyer le fichier
    char buffer[MAX_STR_LEN];
    ssize_t bytes_read;
    while ((bytes_read = read(file_fd, buffer, sizeof(buffer))) > 0) {
        ssize_t bytes_sent = sendto(sockfd, buffer, bytes_read, 0,
                                     res->ai_addr, res->ai_addrlen);
     if (bytes_sent < 0) {
            perror("Error: Unable to send data");
            close(file_fd);   // Fermer le fichier
            close(sockfd);    // Fermer le socket
            freeaddrinfo(res);  // Libérer la mémoire
            exit(1);
        }
    }    
      // Fermer le fichier et le socket
    close(file_fd);
    close(sockfd);

    // Libérer la mémoire allouée par getaddrinfo
    freeaddrinfo(res);

    return 0;
}                          

