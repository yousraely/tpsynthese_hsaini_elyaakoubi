TP de Synthèse
README TP1


README TP2

FICHIER ADDRINFO.C

Nous avons écrit un code dans le fichier addrinfo.c  afin de tester la résolution d'adresse (getaddrinfo) et de créer un socket pour envoyer un message UDP.
Le programme prend un argument l'adresse IP du serveur. Si l'argument n'est pas fourni, il affiche un message d'usage et termine l'exécution.
Le programme initialise la structure addrinfo pour spécifier que l'on veut résoudre une adresse IPv4 (AF_INET) et un socket de type UDP (SOCK_DGRAM).
Nous avons alors utilisé deux éléments essentiels pour travailler avec les adresses IP sous forme lisible: INET_ADDRSTRLEN et inet_ntop

----- INET_ADDRSTRLEN : 
////////////////////////////////////////////////
![image](https://github.com/user-attachments/assets/b8b5ba36-ff96-45b1-ac98-0ea467a84aa2)

/////////////////////////////////////////////////
INET_ADDRSTRLEN est une constante définie dans le fichier d'en-tête <arpa/inet.h>. Elle spécifie la longueur maximale d'une chaîne de caractères pour une adresse IPv4
Ici, nous déclarons un tableau de caractères ip_str qui sera utilisé pour stocker l'adresse IP sous forme de chaîne lisible. La taille du tableau est définie par INET_ADDRSTRLEN pour garantir que nous avons suffisamment d'espace pour contenir l'adresse IPv4.

----- inet_stop :
/////////////////////////////////////////////////
inet_ntop(AF_INET, &addr->sin_addr, ip_str, sizeof(ip_str));
//////////////////////////////////////////////////
inet_ntop est une fonction qui permet de convertir une adresse IP binaire en une chaîne de caractères lisible.

Dans ce programme, nous avons utilisé la fonction socket() pour créer un socket, qui est essentiel pour établir une connexion réseau entre le client et le serveur. Bien que la fonction getaddrinfo() soit utilisée pour récupérer les informations d'adresse du serveur , ce n'est qu'en créant un socket que nous pouvons réellement établir une communication avec ce serveur. 
Pour cela noux avons utilsié cette fonction :  int sockfd = socket(r->ai_family, r->ai_socktype, r->ai_protocol);

Ici, un message générique, "Hello, Server!", est envoyé. Si l'envoi se fait correctement, un message de confirmation nous indique que le message a bien été envoyé, ce qui prouve que la connexion au serveur est établie avec succès et que le lien entre le client et le serveur fonctionne correctement : 
//////////////////////////////////////////////////////////////////////////////
 const char *test_msg = "Hello, Server!";
        ssize_t bytes_sent = sendto(sockfd, test_msg, strlen(test_msg), 0,
                                    r->ai_addr, r->ai_addrlen);
        if (bytes_sent < 0) {
            perror("Error: Unable to send test message");
        } else {
            write(STDOUT_FILENO, "Message sent successfully!\n", 27);
        }

  ////////////////////////////////////////////////////////////////////////////

On a fait des tests avec : ./addrinfo google.com
on obtient le résultat suivant :
216.58.214.78
Message sent successfully!


  FICHIER GETTFTP
  
  Ce programme implémente une requête RRQ pour interagir avec un serveur TFTP . 
  
1. Création de la fonction pour envoyer un RRQ :
Elle se compose en plusieurs parties 
- OpCode :  Pour une requête RRQ, l'OpCode est `0x0001`
  Donc à l'aide de ces lignes :
  ////////////////////////
    buffer[len++] = 0;
    buffer[len++] = 1;
  //////////////////////////
  on a buffer[0] = 0x00 et buffer[1] = 0x01
- Nom du fichier :Le nom du fichier que le client souhaite récupérer.
  Donc disons que le nom de fichier est "test.txt"on aura : buffer[2] = 't', buffer[3] = 'e', ... buffer[10] = '\0' (Fin du nom du fichier "test.txt")
  ////////////////////////////
  strcpy(&buffer[len], filename); // copy filename into buffer
   len += strlen(filename) + 1;  // filename + '\0'
  ////////////////////////////////
- Mode de transfert : Dans ce cas, le mode utilisé est "octet", qui indique que le fichier doit être transféré en mode binaire.
  On aura ainsi : buffer[11] = 'o',buffer[12] = 'c'... buffer[17] = '\0' (Fin de "octet")

 2. Fonction "send_rrq" :
Un fois la requête faite, cette fonction crée et envoie la requêteRRQ au serveur TFTP via la fonction sendto() qui permet d'envoyer des données par un socket UDP à l'adresse du serveur spécifiée.
////////////////////////////////////
 ssize_t bytes_sent = sendto(sockfd, buffer, len, 0, (struct sockaddr *)server_addr, sizeof(*server_addr));
 //////////////////////////////////////
 
3. Fonction "print_rrq_request" :
Cette fonction  n'est pas nécessaire mais sert à afficher la requête RRQ construite, sous forme hexadécimale. On voulait voir si notre RQQ était correctement écrite et à quoi cela ressemblait.Cela permet de vérifier que la requête a été correctement construite avant de l'envoyer. Cela permet aussi de comprendre la structure du message transmis dans le réseau.
On a testé avec le fichier "zeros256" et on obtient : 
Requête RRQ construite (en hexadécimal) :
00 01 7a 65 72 6f 73 32 35 36 00 6f 63 74 65 74 00 

4. Création du socket UDP :
On réutilise ce qu'on a mis dans le fichier "addrinfo.c"

FICHIER PUTTFTP

Dans ce fichier, nous avons réalisé un code pour lire le contenu d'un fichier et à afficher son contenu à l'écran.
Ce qui nous a été utile ici est  "O_RDONLY" qui nous permet d'ouvrir des ficheirs en lecture seule à l'aide de la fonction open : 
////////////////////////////////////////////
int file_fd = open(filename, O_RDONLY);
///////////////////////////////////////////
Nous avons fait un teste avec le fichier "motd.txt".Il faut faire attention à ne pas écrire dans la console : ./puttftp 127.0.0.1 motd.txt  mais remplacer "mots.txt" par le chemin complet qui est dans notre cas : ./puttftp 127.0.0.1 /home/ensea/Documents/tpsynthese_hsaini_elyaakoubi/tp2/serveur/motd.txt
Ainsi on obtient l'affichage suivant : 
Server IP: 127.0.0.
File to upload: Er/home/en
Reading file content:
Do, or do not; there is no try.
                -- Master Yoda
                

CONCLUSION TP 2 : 
Ce TP nous a permis de mieux comprendre et manipuler les différents aspects du protocole TFTP et des communications réseau en général. 
Cependant, plusieurs difficultés sont survenues tout au long du TP, principalement liées à la gestion des sockets et à la compréhension du fonctionnement des requêtes RRQ.
----------Création et utilisation des Sockets : La première difficulté a été de comprendre comment créer et utiliser un socket pour établir une connexion réseau. Le lien entre l'adresse récupérée via getaddrinfo() et le socket créé par la fonction socket() n'était pas immédiatement évident. En effet, bien que getaddrinfo() fournisse l'adresse du serveur, c’est le socket qui permet d'envoyer et de recevoir des données à travers cette adresse. Il a fallu bien saisir ce lien et la manière dont les données transitent à travers les sockets pour assurer la communication.
----------Structure de la requête RRQ : La construction de la requête RRQ elle-même, a été un autre défi. Comprendre la structure binaire de la requête, particulièrement la façon dont les données sont encodées et envoyées, nous a demandé un peu de temps c'est pourquoi afficher la requête en hexadécimal p nous a permis de mieux comprendre comment chaque élément de la requête est interprété.
----------Gestion des erreurs et des retours : Une autre difficulté a été liée à la gestion des erreurs.. fonction sendto() pour s'assurer que le message a bien été envoyé a nécessité de prêter une attention particulière aux messages d'erreur retournés.

En conclusion, ce TP nous a permis de mieux comprendre les fondements de la communication réseau et de la mise en œuvre du protocole TFTP. Bien que nous ayons rencontré quelques difficultés, elles nous ont poussés à approfondir nos connaissances, notamment en ce qui concerne les sockets, la gestion des requêtes réseau, et la construction des paquets TFTP.


