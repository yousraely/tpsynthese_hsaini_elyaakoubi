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
    if (argc != 2) {
        const char *usage_message = "Usage: gettftp <server_ip> <filename>\n";
    write(STDOUT_FILENO, usage_message, sizeof(usage_message));
    exit(1);
    }

// on récupère les arguments
    char *server_ip = argv[1];
   // char *filename = argv[2];

//addr info
struct addrinfo hints, *res,*r;
memset(&hints, 0, sizeof(hints));
hints.ai_family = AF_INET;      
hints.ai_socktype = SOCK_DGRAM;

// adresse du serveur
    int e= getaddrinfo(argv[1],"1069", &hints, &res);
    if (e!= 0) {
        fprintf(stderr, "Error: Unable to resolve server address: %s\n", gai_strerror(e));
        exit(1);
    }

    for (r = res; r != NULL; r = r->ai_next) {
        char ip_str[INET_ADDRSTRLEN];

    // Conversion de l'adresse en chaîne lisible
        struct sockaddr_in *addr = (struct sockaddr_in *)r->ai_addr;
        inet_ntop(AF_INET, &addr->sin_addr, ip_str, sizeof(ip_str));

        // Affichage de l'adresse IP
        write(STDOUT_FILENO, ip_str, strlen(ip_str));
        write(STDOUT_FILENO, "\n", 1);
    
    // Création du socket
        int sockfd = socket(r->ai_family, r->ai_socktype, r->ai_protocol);
        if (sockfd < 0) {
            perror("Error: Unable to create socket");
            continue; // Essayer l'adresse suivante si échec
        }
         // Test de connexion au serveur : Envoi d'un message UDP
        const char *test_msg = "Hello, Server!";
        ssize_t bytes_sent = sendto(sockfd, test_msg, strlen(test_msg), 0,
                                    r->ai_addr, r->ai_addrlen);
        if (bytes_sent < 0) {
            perror("Error: Unable to send test message");
        } else {
            write(STDOUT_FILENO, "Message sent successfully!\n", 27);
        }
         // Fermeture du socket après le test
        close(sockfd);
    }
     // Libérer la mémoire allouée par getaddrinfo
    freeaddrinfo(res);

    return 0;
}