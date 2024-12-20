TP de Synthèse

README TP1
Dans le cadre de notre TP de synthèse intitulé "ENSEA in the Shell", nous avons développé un micro-shell en C. Ce shell simplifié, que nous avons nommé enseash, est capable d'exécuter des commandes simples, d'afficher les codes de retour des programmes lancés, et de gérer proprement leur terminaison, qu'elle soit normale ou due à un signal.

Programme des questions 1 à 3 :
Dans cette première partie du TP, nous avons travaillé sur la création d'un mini-shell nommé "enseash". L'idée était de poser les bases d'un interpréteur de commandes simple et interactif. Voici comment nous avons procédé et ce que nous avons appris : 
Afficher le message d'accueil :
Dès que le programme démarre, un message d'accueil est affiché dans le terminal, indiquant à l'utilisateur qu'il est dans le shell et qu'il peut quitter avec la commande exit.
Pour cela, nous avons utilisé la fonction système write, qui permet d'écrire directement dans le terminal.
Créer un prompt interactif :
Après l'affichage du message d'accueil, un prompt (enseash%) est affiché pour indiquer que le shell est prêt à recevoir des commandes.
Le prompt s'affiche en boucle, grâce à une structure répétitive ( boucle while(1)), jusqu'à ce que l'utilisateur quitte le shell.
Nous avons utilisé "strcmp" pour comparer les chaines de caractères à "exit". 
Pour permettre à l'utilisateur de quitter le shell, nous avons implémenté une commande spéciale : exit.
Lorsque l'utilisateur saisit "exit", le programme affiche un message de sortie (« Bye bye ;) ! » dans notre cas) et termine le shell.
Par exemple avec "hello" la boucle continue et avec "exit" elle s'arrête
![image](https://github.com/user-attachments/assets/2ffb2ea6-5587-4b31-bb02-d06b060774ed)


Difficultés rencontrées
STDXX_FILENO
STDIN_FILENO (entrée) et STDOUT_FILENO (sortie) sont utilisés dans read et write pour lire depuis l'entrée  ou écrire vers la sortie. Ces sont des descripteurs de fichier en Linux
Gestion des erreurs et des signaux :
Lors de la lecture des commandes avec read, il a fallu gérer le cas où Ctrl+D est pressé (indiquant un EOF) ou lorsqu'une erreur se produit.
Suppression du caractère de retour à la ligne :
La commande saisie contient toujours un \n final qui doit être retiré avant de comparer la chaîne avec "exit". Cela a été réalisé avec la ligne du code suivant : 

Questions  4 :

Dans cette question nous avons ajouté la fonctionnalité suivante à notre shell : l'exécution des commandes utilisateur et l'affichage des codes de retour ou des signaux si un processus est interrompu. 
Pour cela nous avons créé le processus enfant (fork) : cette méthode permet  d’exécuter  la commande saisie par l'utilisateur dans un processus séparé.
![image](https://github.com/user-attachments/assets/1ad186b3-2798-4b4b-945b-900ead7593f0)

fork() duplique le processus actuel en deux :
Le processus parent : continue d'exécuter le code principal (le shell). (Si pid>0)
Le processus enfant : exécute la commande utilisateur. (Si pid=0)
Dans le processus enfant, on utilise la fonction execlp pour exécuter la commande stockée dans buffer avec execlp(buffer, buffer, NULL)
Si execlp réussit, il remplace le contenu du processus enfant par le programme de la commande.
Si la commande est invalide ou introuvable, execlp échoue. Dans ce cas, le code suivant execlp est exécuté pour afficher un message d'erreur :
Et on quitte le processus enfant avec « exit(EXIT_FAILURE) ». 

Maintenant on gère le child process dans le Parent process: 
Le parent utilise « wait(&status) » pour suspendre son exécution jusqu'à ce que le processus enfant se termine.
La fonction wait renvoie des informations sur la manière dont le processus enfant s’est terminé (succès, échec, ou signal).
Une fois le processus enfant fini, le parent analyse son statut avec les macros WIFEXITED et WIFSIGNALED.

Si la terminaison est normal : 
WIFEXITED(status) : Retourne true si le processus enfant s’est terminé normalement.
WEXITSTATUS(status) : Retourne le code de retour du processus enfant (généralement 0 pour succès).
On construit alors un message formaté de la manière suivante : enseash[exit:<code>]%

Si la terminaison se fait par un signal 
WIFSIGNALED(status) : Retourne true si le processus enfant a été interrompu par un signal.
WTERMSIG(status) : Retourne le numéro du signal qui a causé l’interruption.
On construit alors un message formaté de la manière suivante : enseash[sign:<code>]%

On a pu tester uniquement si la terminaison est normal : 
![image](https://github.com/user-attachments/assets/a9e2e713-ab7c-44d3-8f15-9cd1f95f3b4f)

Question 5 : 

Dans cette question, on souhaite afficher le temps d'exécution des commandes utilisateur.
Pour mesurer la durée d’exécution d’une commande, nous avons utilisé la fonction clock_gettime de la bibliothèque <time.h>. Cette fonction est idéale car elle utilise une horloge appelée CLOCK_MONOTONIC. Avant l'exécution de la commande, nous enregistrons l'heure de début avec la commande clock_gettime(CLOCK_MONOTONIC, &start). Après l'exécution, nous enregistrons l'heure de fin avec clock_gettime(CLOCK_MONOTONIC, &end). 
![image](https://github.com/user-attachments/assets/3b3531db-a28d-4f9c-be8c-645891f8d737)
Dans timespec, le temps est séparé en secondes et nanosecondes pour être précis et facile à utiliser. On utilise les secondes pour mesurer les durées longues, comme plusieurs minutes ou heures, et les nanosecondes pour obtenir une précision très fine On convertit ces deux valeurs en millisecondes dans notre code pour afficher clairement le temps que met chaque commande à s’exécuter. 
De cette manière notre message formaté sera maintenant :  [sign:<signal>|<temps>]% ou [exit:<code>|<temps>]%

![image](https://github.com/user-attachments/assets/8368146b-caeb-4ed4-bb41-5facbcf474d3)

En testant avec la chaîne de caractères "hello", nous obtenons une durée de 0 ms. Ce résultat reste identique lorsque nous utilisons d'autres chaînes de caractères, ce qui soulève une interrogation. Nous ne savons pas si cela est dû à la rapidité de l'exécution, rendant la précision en millisecondes insuffisante pour capturer une durée aussi courte, ou si cela résulte d'un problème sous-jacent dans notre implémentation ou dans la manière dont la durée est mesurée.

Question 6 :
Dans cette question, on écrit un programme qui va nous permettre d’exécuter des commandes avec arguments.
On utilise strtok pour découper l'entrée utilisateur en plusieurs morceaux (qui sont les arguments), en se basant sur les espaces comme séparateurs. Chaque morceau est stocké dans un tableau appelé args. Ce tableau est ensuite utilisé pour exécuter la commande avec ses arguments.
Par exemple , ls -l /home sera découpé de la manière suivante : args[0] = "ls", args[1] = "-l", args[2] = "/home" et args[3] = NULL (pour signaler la fin des arguments)
On utilisera toujours fork pour créer un processus enfant qui exécute la commande de la même manière que la question 5


CONCLUSION TP1 : 
Ce TP nous a permis de construire pas à pas un shell fonctionnel. On a commencé avec un prompt simple capable de gérer la commande exit, puis on a ajouté l’exécution de commandes, le code de retour, les signaux, et enfin la gestion des arguments.
Ce qu’on a trouvé intéressant, c’est de manipuler des processus avec fork et wait, d’utiliser execvp pour exécuter des commandes, et de mesurer leur temps d’exécution avec clock_gettime par exemple. Grâce à ce TP, on a mieux compris comment fonctionne un shell et les bases des systèmes Linux, tout en construisant un programme qui ressemble à un vrai terminal.

README TP2

FICHIER ADDRINFO.C

Nous avons écrit un code dans le fichier addrinfo.c  afin de tester la résolution d'adresse (getaddrinfo) et de créer un socket pour envoyer un message UDP.
Le programme prend un argument l'adresse IP du serveur. Si l'argument n'est pas fourni, il affiche un message d'usage et termine l'exécution.
Le programme initialise la structure addrinfo pour spécifier que l'on veut résoudre une adresse IPv4 (AF_INET) et un socket de type UDP (SOCK_DGRAM).
Nous avons alors utilisé deux éléments essentiels pour travailler avec les adresses IP sous forme lisible: INET_ADDRSTRLEN et inet_ntop

----- INET_ADDRSTRLEN : 
![image](https://github.com/user-attachments/assets/b8b5ba36-ff96-45b1-ac98-0ea467a84aa2)

INET_ADDRSTRLEN est une constante définie dans le fichier d'en-tête <arpa/inet.h>. Elle spécifie la longueur maximale d'une chaîne de caractères pour une adresse IPv4
Ici, nous déclarons un tableau de caractères ip_str qui sera utilisé pour stocker l'adresse IP sous forme de chaîne lisible. La taille du tableau est définie par INET_ADDRSTRLEN pour garantir que nous avons suffisamment d'espace pour contenir l'adresse IPv4.

----- inet_stop :
![image](https://github.com/user-attachments/assets/bd73751e-8acf-4519-a917-5ef770079950)

inet_ntop est une fonction qui permet de convertir une adresse IP binaire en une chaîne de caractères lisible.

Dans ce programme, nous avons utilisé la fonction socket() pour créer un socket, qui est essentiel pour établir une connexion réseau entre le client et le serveur. Bien que la fonction getaddrinfo() soit utilisée pour récupérer les informations d'adresse du serveur , ce n'est qu'en créant un socket que nous pouvons réellement établir une communication avec ce serveur. 
Pour cela noux avons utilsié cette fonction :  int sockfd = socket(r->ai_family, r->ai_socktype, r->ai_protocol);

Ici, un message générique, "Hello, Server!", est envoyé. Si l'envoi se fait correctement, un message de confirmation nous indique que le message a bien été envoyé, ce qui prouve que la connexion au serveur est établie avec succès et que le lien entre le client et le serveur fonctionne correctement : 
![image](https://github.com/user-attachments/assets/737f41f6-cade-49ea-ba54-4d3e73cb0b68)

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
![image](https://github.com/user-attachments/assets/d5c341cc-9456-477e-ba75-5154bbb5343c)

  on a buffer[0] = 0x00 et buffer[1] = 0x01
- Nom du fichier :Le nom du fichier que le client souhaite récupérer.
  Donc disons que le nom de fichier est "test.txt"on aura : buffer[2] = 't', buffer[3] = 'e', ... buffer[10] = '\0' (Fin du nom du fichier "test.txt")
![image](https://github.com/user-attachments/assets/0fbddf9f-c888-43b9-827a-ed9da31e43ee)

- Mode de transfert : Dans ce cas, le mode utilisé est "octet", qui indique que le fichier doit être transféré en mode binaire.
  On aura ainsi : buffer[11] = 'o',buffer[12] = 'c'... buffer[17] = '\0' (Fin de "octet")

 2. Fonction "send_rrq" :
Un fois la requête faite, cette fonction crée et envoie la requêteRRQ au serveur TFTP via la fonction sendto() qui permet d'envoyer des données par un socket UDP à l'adresse du serveur spécifiée.
![image](https://github.com/user-attachments/assets/88b48f8a-51ff-4051-b4c8-7e63816a2ead)

 
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
![image](https://github.com/user-attachments/assets/bef3761e-fb13-4983-a4d0-e7b429923b22)

Nous avons fait un teste avec le fichier "motd.txt".Il faut faire attention à ne pas écrire dans la console : ./puttftp 127.0.0.1 motd.txt  mais remplacer "mots.txt" par le chemin complet qui est dans notre cas : ./puttftp 127.0.0.1 /home/ensea/Documents/tpsynthese_hsaini_elyaakoubi/tp2/serveur/motd.txt
Ainsi on obtient l'affichage suivant : 
![image](https://github.com/user-attachments/assets/ae878534-d0e6-417c-98ef-437184505644)

                
CONCLUSION TP 2 : 
Ce TP nous a permis de mieux comprendre et manipuler les différents aspects du protocole TFTP et des communications réseau en général. 
Cependant, plusieurs difficultés sont survenues tout au long du TP, principalement liées à la gestion des sockets et à la compréhension du fonctionnement des requêtes RRQ.
La première difficulté a été de comprendre comment créer et utiliser un socket pour établir une connexion réseau. Le lien entre l'adresse récupérée via getaddrinfo() et le socket créé par la fonction socket() n'était pas immédiatement évident. En effet, bien que getaddrinfo() fournisse l'adresse du serveur, c’est le socket qui permet d'envoyer et de recevoir des données à travers cette adresse. Il a fallu bien saisir ce lien et la manière dont les données transitent à travers les sockets pour assurer la communication.
La construction de la requête RRQ elle-même, a été un autre défi. Comprendre la structure binaire de la requête, particulièrement la façon dont les données sont encodées et envoyées, nous a demandé un peu de temps c'est pourquoi afficher la requête en hexadécimal p nous a permis de mieux comprendre comment chaque élément de la requête est interprété.
Une autre difficulté a été liée à la gestion des erreurs.. fonction sendto() pour s'assurer que le message a bien été envoyé a nécessité de prêter une attention particulière aux messages d'erreur retournés.




