#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

#define MAX_STR_LEN 512
#define ERROR_MSG_SIZE 256

//function to send a RRQ to the server 
void send_rrq(int sockfd, struct sockaddr_in *server_addr, const char *filename) {
    char buffer[MAX_STR_LEN];
    int len = 0;

    //Opcode RRQ (0x0001)
    buffer[len++] = 0;
    buffer[len++] = 1;

    //addinf the file name (null terminated)
    strcpy(&buffer[len], filename); // copy filename into buffer
    len += strlen(filename) + 1;  // filename + '\0'

    //adding transfer mode "octet" (null terminated)
    strcpy(&buffer[len], "octet"); //copy 'octet' into the  buffer
    len += strlen("octet") + 1;  // add "octet" + '\0'

     //sending the rrq packet to the server
    ssize_t bytes_sent = sendto(sockfd, buffer, len, 0, (struct sockaddr *)server_addr, sizeof(*server_addr));
    if (bytes_sent < 0) { //if nothing was received.
        perror("Erreur lors de l'envoi de la requête RRQ");
    } else {
        const char *msg = "Requête RRQ envoyée\n";
        write(STDOUT_FILENO, msg, strlen(msg));  
    }
}

  //function to display the RRQ request (in hexadecimal) : to see if it works properly
void print_rrq_request(const char *filename) {
    char buffer[MAX_STR_LEN];
    int len = 0;

    buffer[len++] = 0;
    buffer[len++] = 1;

    strcpy(&buffer[len], filename);
    len += strlen(filename) + 1;  

    strcpy(&buffer[len], "octet");
    len += strlen("octet") + 1;

    //// Displaying the request in hexadecimal
    const char *msg = "Requête RRQ construite (en hexadécimal) :\n";
    write(STDOUT_FILENO, msg, strlen(msg));  
     //The loop goes through each byte of the buffer, converts it into a 2-character hexadecimal representation, and then displays it on the console.
for (int i = 0; i < len; i++) {
        char hex_byte[4];   
        snprintf(hex_byte, sizeof(hex_byte), "%02x ", (unsigned char)buffer[i]);
        write(STDOUT_FILENO, hex_byte, strlen(hex_byte));  
    }

    write(STDOUT_FILENO, "\n", 1);
}


//Reading arguments and displaying IP address

int main(int argc, char *argv[]) {
    if (argc != 3) {
        const char *usage_message = "Usage: gettftp <server_ip> <filename>\n";
    write(STDOUT_FILENO, usage_message,strlen(usage_message));
    exit(1);
    }

// arguments
    char *server_ip = argv[1];
    char *filename = argv[2];

//display
//server IP
write(STDOUT_FILENO, "Server IP: ", strlen("Server IP: "));
write(STDOUT_FILENO, server_ip, strlen(server_ip));
write(STDOUT_FILENO, "\n", 1);

//filename
write(STDOUT_FILENO, "File to upload: ", strlen("File to download: "));  
write(STDOUT_FILENO, filename, strlen(filename)); 
write(STDOUT_FILENO, "\n", 1);  

    print_rrq_request(filename);

      //  getaddrinfo
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET; //for the IPv4 adress
    hints.ai_socktype = SOCK_DGRAM; //for the  UDP socket

    int ret = getaddrinfo(server_ip, "69", &hints, &res);  // Port TFTP is  generally 69,  the res variable then contains a list of address results.
    if (ret != 0) { // error detection (if ret is 0, it means the call to getaddrinfo() was successful)
        char error_msg[ERROR_MSG_SIZE];
        snprintf(error_msg, strlen(error_msg), "Erreur de résolution DNS : %s\n", gai_strerror(ret));
        write(STDOUT_FILENO, error_msg, strlen(error_msg));
        return 1;
    }

    //socket UDP (Tused to create a socket that will allow communication between the client and the server)
    int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd < 0) { //only positive values 
        perror("Erreur de création du socket");
        freeaddrinfo(res);
        return 1;
    }
// Cast sockaddr structur
    struct sockaddr_in *server_addr = (struct sockaddr_in *)res->ai_addr;
    server_addr->sin_port = htons(69);  // htons(host to network short) converts the port number 69  to "network byte order" (network format).
    send_rrq(sockfd, server_addr, filename);

    freeaddrinfo(res);

    close(sockfd);

    return 0;
}