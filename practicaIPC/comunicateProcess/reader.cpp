#include <iostream> 
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h> 
using namespace std; 
  
int main() 
{ 
    // ftok to generate unique key 
    key_t key = ftok("shmfile",65); 
  
    // shmget returns an identifier in shmid 
    int shmid = shmget(key,300,0666|IPC_CREAT); 
  
    // shmat to attach to shared memory 
    char *str = (char*) shmat(shmid,(void*)0,0); 
    char salida = 'x';
    char nuevaEntrada= 'N';
    bool exit=false;
    char entrada[200]="";
  
  while (!exit){
    //Retrieve the stored information, form the shared location.
    

    if (strcmp(entrada, str)!=0) {
        printf("Data read from memory: %s\n",str); 
        memcpy(entrada,str,sizeof(entrada));
    
        }
      
    
    if (*str==salida){
            exit=true;
        }
        

  }
  //detach from shared memory  
    shmdt(str); 
    
    // destroy the shared memory 
    shmctl(shmid,IPC_RMID,NULL); 
     
    return 0; 
} 