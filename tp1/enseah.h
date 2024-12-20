#define BUFSIZE 128

char welcome[]="Bienvenue dans le shell ENSEA.\nPour quitter, tapez 'exit'.\n";
char prompt[] = "enseash% ";
char exitCommand[]= "exit"; 
char UnknownCommand[] = "Commande inconnue.\n";
char quitMessage[] = "Bye bye ;) !\n";
char exit_debut[] = "enseash[exit:";
char exit_fin[] = "]%";
char signal_debut[] = "enseash[sign:";
char signal_fin[] = "]%";
char buffer[BUFSIZE];
