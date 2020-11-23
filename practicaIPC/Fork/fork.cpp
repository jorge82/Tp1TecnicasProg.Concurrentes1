#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <stdlib.h> 
#include <stdio.h> 

#include <sys/ipc.h>
#include <sys/shm.h>
#include <string>

#include <cstring>


#include <signal.h> 

#include <sys/types.h> 
#include <sys/wait.h>

  
#define Recieved 2
#define Ready 1 
#define NotReady -1 
#define Finished 3 

using namespace std;

struct memory { 
    char buff[100]; 
    int status, pid, parent_pid; 
  
};

struct memory* shmptr; 
void handler(int signum) 
{ 
    if (signum == SIGUSR1) { 
        
        cout<<" data recieved from child: "<<shmptr->buff<<endl<<endl;
        shmptr->status = Recieved;     
    } 
    if (signum == SIGUSR2) { 
        cout<<endl<<"here!!"<<endl;
         shmptr->status = Recieved; 
         cout<<" changing status  "<<shmptr->status<<endl;
     
    }
} 

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
   shmptr= (struct memory*) shmat(shmid, NULL,0);

    
    signal(SIGUSR1, handler); 
    signal(SIGUSR2, handler); 
    
 
  
    shmptr->status = NotReady; 

    int parent;

     for (int i=0; i < cant; ++i)
    {
        if(fork()==0){

             shmptr->pid = getpid(); 
             shmptr->parent_pid = getppid(); 
            signal(SIGUSR1, handler); 
            signal(SIGUSR2, handler); 
 
            
            
            char message[100]="helllo from child ";
            
            char buf[5];
            sprintf(buf, "%d",i);
           
            strcat(message,buf); 
        
                   
            memcpy(shmptr->buff, message, sizeof(message));


            shmptr->status = Ready; 
            
             kill(shmptr->parent_pid, SIGUSR1);

       
            cout<<"finsihing process "<<i<<endl;
            shmptr->status = NotReady; 
            shmdt(shmptr);
            return 1;
        }    
            
    }


    // int num=cant;
    // while (num>0) { 
        
        
    //      cout<<" father estatus is "<< shmptr->status<<endl;
    //     if(shmptr->status == Ready) {
    //         //cout<<"data reaqd from memory :"<< shmptr->buff<<endl;
    //         kill(shmptr->pid, SIGUSR2);
    //         num--;
            

    //     }
        

    // }
     cout << "Waiting childs to finishg" << endl;
  // Need to wait for all
  for(int i=0; i<cant; i++){
    wait(NULL);
    //cout << "Got " << i+1 << " done" << endl;
  }
   // cout<<" terminating"<<endl;
     shmdt(shmptr);
     shmctl(shmid, IPC_RMID, NULL);


   


    return 0;


}