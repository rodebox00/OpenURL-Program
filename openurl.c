/**
*   @author rodebox00
*/

#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


int main(int argc, char **argv)
{
    int opt;
    char *navegador  = NULL;


    if(argc==1){    //Entra si no se reciben argumentos/Enter if no arguments are received
        fprintf(stderr, "Uso: %s -n NAVEGADOR [URLs] \n", argv[0]);
        exit(EXIT_FAILURE);
    }

    while ((opt = getopt(argc, argv, "n:")) != -1)
    {
        switch (opt)
        {
        case 'n':
            navegador = optarg;
            break;

        default:
            fprintf(stderr, "Uso: %s -n NAVEGADOR [URLs] \n", argv[0]);
            exit(EXIT_FAILURE);
            break;
        }
    }

    int nurl = argc-3;
    
    if(nurl==0) nurl=1;

    pid_t pid[nurl];

    for (int i=0;i<nurl;i++){
         switch (pid[i] = fork())
        {
        case -1:
            perror("fork()");
            exit(EXIT_FAILURE);
        case 0:
            if ((argc-3)==0) execlp(navegador,"--new-window","www.github.com/rodebox00", NULL);
            else execlp(navegador,"--new-window",argv[3+i], NULL);
            fprintf(stderr, "Error: NAVEGADOR no encontrado\n");
            exit(EXIT_FAILURE);
        }
    }

    int status;
     for (int i=0;i<nurl;i++){
         waitpid(pid[i], &status, 0);
         if (WIFEXITED(status)) {   //Entra si el hijo no terminó correctamente/Enter if the son did not finish correctly
            if ((argc-3)==0) printf("./openurl: URL: www.github.com/rodebox00, STATUS: %d\n",WEXITSTATUS(status));
            else printf("./openurl: URL: %s, STATUS: %d\n",argv[3+i],WEXITSTATUS(status));

        }
         else if ((argc-3)==0) printf("./openurl: URL: www.github.com/rodebox00, STATUS: %d\n",status);
         else printf("./openurl: URL: %s, STATUS: %d\n",argv[3+i],status);
     }

    return EXIT_SUCCESS;

}