#include "stdlib.h"
#include "unistd.h"
#include "string.h"
#include "stdio.h"
;
int main(int argc, char *argv[]){
    // We define our different messages as character strings.
    char welcome[]="Bienvenue dans le shell ENSEA.\nPour quitter, tapez 'exit'.\n";
    char prompt[]="enseash%";
    char unknown[]="Commande inconnue .\n";
    char bye[]="Bye Bye ;).\n";
    char buffer[50];
    write(1, welcome, sizeof(welcome)); 

while(1){
    write(1, prompt, sizeof(prompt));
    int letter = read(0,buffer,sizeof(buffer)); // // Reads the data entered by the user and stores it in the buffer
    if (letter<=0) exit(0); // If no word or letter is entered, the program ends with an exit
    buffer[letter-1]='\0'; //Replace the last character with a null character (to ignore the last character in the next read)

//Checks if the user typed "exit" by comparing each character with those in the string "exit"
    if (buffer[0] == 'e' && buffer[1] == 'x' && buffer[2] == 'i' && buffer[3] == 't' && buffer[4] == '\0'){ 
    write(1, bye, sizeof(bye));
    exit(0); 
    }

    write(1,unknown,sizeof(unknown) );
}
}

