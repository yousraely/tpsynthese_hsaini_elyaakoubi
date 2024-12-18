#include "stdlib.h"
#include "unistd.h"
#include "string.h"
#include "stdio.h"
#include "sys/wait.h"
#include <time.h>
;
int main(int argc, char *argv[]){
    // We define our different messages as character strings.
    char welcome[]="Bienvenue dans le shell ENSEA.\nPour quitter, tapez 'exit'.\n";
    char prompt[]="enseash%";
    char unknown[]="Commande inconnue .\n";
    char bye[]="Bye Bye ;).\n";
    int status;  
    char buffer[50];
    write(1, welcome, sizeof(welcome)); 

while(1){
    write(1, prompt, sizeof(prompt));

    int letter = read(0,buffer,sizeof(buffer)); // // Reads the data entered by the user and stores it in the buffer
    if (letter<=0) { write(1, bye, sizeof(bye)); exit(0);
    }  // If no word or letter is entered, the program ends with an exit
    buffer[letter-1]='\0'; //Replace the last character with a null character (to ignore the last character in the next read)

//Checks if the user typed "exit" by comparing each character with those in the string "exit"
    if (strcmp(buffer,"exit")==0){ 
    write(1, bye, sizeof(bye));
    exit(0); 
    }

// mesure tu temps

struct timespec start, end;
        clock_gettime(CLOCK_REALTIME, &start);


///Q4

// créer le processus
pid_t pid = fork();
if (pid==0){ // on est dans le processus fils
    char *args[] = {buffer,NULL};
    execvp(buffer,args);
    write(1,unknown,sizeof(unknown));
    exit(1);
}else if (pid>0){ // on est dans le processus père
    waitpid(pid, &status,0); // attends que le fils termine pour continuer l'éxecution

clock_gettime(CLOCK_REALTIME, &end);

//calcul
long milliseconds = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000;

    if (WIFEXITED(status)){ // si processus fils s'est terminé sans erreurs
        char exit_msg[50];
        int exit_code=WEXITSTATUS(status); // contient code du fils (=0 si tout bien passé sinon autre )

        char exit_debut[]="enseash[exit:";
        char exit_fin[]="]%";

                write(1, exit_debut, sizeof(exit_debut) - 1);

                char exit_code_str[10];
                snprintf(exit_code_str, sizeof(exit_code_str), "%d", exit_code); //convertit code en chaine de caractère

                write(1, exit_code_str, strlen(exit_code_str)); // Affichage du code de sortie
                write(1, "|", 1);
                write(1, &milliseconds, sizeof(milliseconds));
                write(1, exit_fin, sizeof(exit_fin) - 1);
          
            }

        else  if (WIFSIGNALED(status)){ 
        int signal_num=WTERMSIG(status); 

        char signal_debut[]="enseash[sign:";
        char signal_fin[]="]%";

                write(1, signal_debut, sizeof(signal_debut) - 1);

                char signal_str[10];
                snprintf(signal_str, sizeof(signal_str), "%d", signal_num);
                write(1,signal_str, strlen(signal_str)); 
                write(1, signal_fin, sizeof(signal_fin) - 1);
            }
    
        } else {
            // Si fork échoue
            perror("Erreur : fork");
            exit(1);
        }
    }

    return 0;
}

   