#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <stdlib.h> 
#include <stdio.h> 

#include <sys/ipc.h>
#include <sys/shm.h>
#include <string>

using namespace std;
int main( int argc, char **argv){
   
     if (argc!=2){
        cout<<"Falta ingresar el número de cámaras"<<endl;
        return -1;
    }


    int cant= atoi(argv[1]);
   

    printf("--beginning of program\n");

 //genero una clave unica
    key_t key= ftok("sharedMem", 65);


    //shmget devuelve un indentiifcador unico
    int shmid= shmget(key, 1000, 0666|IPC_CREAT);

    // shamt para apuntar a la memoria compartida
    char *str= (char*) shmat(shmid, (void*)0,0);

  


    int parent;

     for (int i=0; i < cant; ++i)
    {
        if(fork()==0){
            cout<<"child number "<<getpid()<<" from parent "<<getppid()<<endl;
            char message[100]="helllo from child";
            
            char buf[5];
            sprintf(buf, "%d",i);
           
            strcat(message,buf); 
            cout<<"mensaje: "<<message<<endl; 
                   
            memcpy(str, message, sizeof(message));
            //printf("data reaqd from memory : %s\n", str);
             
            shmdt(str);
           sleep(1);

            return 1;

        }    
            
    }
         cout<<"child im your father: "<<getpid()<<endl;


     for (int i=0; i < cant; ++i)
    {
      
       printf("data reaqd from memory : %s\n", str);
       sleep(1);
            
    }


    

     

     shmdt(str);
     shmctl(shmid, IPC_RMID, NULL);


   


    return 0;


}