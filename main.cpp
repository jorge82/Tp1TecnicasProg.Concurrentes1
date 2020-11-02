#include "Imagen.h"
#include "ProcesadorImagenes.h"
#include "Logger.h"

#include <string>
#include <cstring>
#include <signal.h> 
#include <sys/types.h> 
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <stdlib.h> 
#include <sys/ipc.h>
#include <sys/shm.h>


  
#define Recieved 2
#define Ready 1 
#define NotReady -1 
#define Finished 3 

#define BUFFSIZE 1000
#define ERROR -1
#define OK 0

#include <chrono> 
using namespace std::chrono; 

using namespace std;


struct memory { 
    char buff[BUFFSIZE]; 
    int status, pid, parent_pid;  
};

struct memory* shmptr; 

string data_in;

vector <Imagen> imagenes;

Imagen imagen(8,8);

Logger* logger=Logger::getLogger("Log.txt");
string log;
bool logToFile=true;

void handler(int signum) 
{ 
    if (signum == SIGUSR1) { 
        //cout<<" data recieved from child: "<<shmptr->buff<<endl<<endl;
        data_in=shmptr->buff;
        
        imagen.desSerializeImagen(data_in);
        // Use auto keyword to avoid typing long 
            // type definitions to get the timepoint 
            // at this instant use function now() 
            auto start = high_resolution_clock::now(); 
        if(logToFile){

            log= "Proceso padre recibe de " + to_string(shmptr->pid) + " la imagen " + data_in;
            logger->writeToLogFile(log);
  
        }    

        imagen.mostrarImagen();
        imagenes.push_back(imagen);
        shmptr->status = Recieved;     
    } 
    if (signum == SIGUSR2) { 
        cout<<endl<<"here!!"<<endl;
         shmptr->status = Recieved; 
         cout<<" changing status  "<<shmptr->status<<endl;
    }
}

int main(int argc, const char** argv) {
  
    
    if (argc!=3){
       perror ("Faltan argumentos. Debe ingresar N(ancho y alto de las imagenes) y C(numero de camaras)");
       return ERROR;
    }
    // Use auto keyword to avoid typing long 
// type definitions to get the timepoint 
// at this instant use function now() 
auto start = high_resolution_clock::now(); 
   

    
    


    int n= atoi(argv[1]); 
    int cant= atoi(argv[2]);   
    int alto=n;
    int ancho=n;


    


    if(logToFile){
        log= "Comienza el programa con " + to_string(cant) +" imagenes de " + to_string(n) + "x" +to_string(n)+ " pixeles";   ;
        logger->writeToLogFile(log);
    }

     //genero una clave unica
    key_t key= ftok("sharedMem", 65);

    //shmget devuelve un indentiifcador unico
    int shmid= shmget(key, 10000, 0666|IPC_CREAT);

    // shamt para apuntar a la memoria compartida
    shmptr= (struct memory*) shmat(shmid, NULL,0);

    
    signal(SIGUSR1, handler); 

    

    shmptr->status = NotReady; 

    int parent_pid, pid;

    parent_pid=getpid();


    for (int i=0; i < cant; ++i)
    {
        pid=fork();
        if(pid==0){

             int process_id= getpid();
             shmptr->pid = process_id; 
             shmptr->parent_pid = parent_pid; 
            //signal(SIGUSR1, handler); 
            
            srand (time(NULL)+i);
            Imagen Imagen(ancho, alto);
            Imagen.generarImagenAleatoria();
            if(logToFile){
                log= "Proceso id " + to_string(process_id) + " comienza a procesar su imagen";
                logger->writeToLogFile(log);
            }
            Imagen.procesarImagen();
              if(logToFile){
                log= "Proceso id " + to_string(process_id) + " Termina de  procesar su imagen";
                logger->writeToLogFile(log);
            }
            
 
            string serial=Imagen.serializeImagen();
            int n = serial.length();
 
            // declaring character array
            char char_array[n + 1];
        
            // copying the contents of the
            // string to char array
            strcpy(char_array, serial.c_str());

            if(logToFile){
                log= "Process id " + to_string(process_id) + " envia la imagen procesada al padre :" + serial ;
                logger->writeToLogFile(log);
            }


    
            memcpy(shmptr->buff, char_array, sizeof(char_array));

            shmptr->status = Ready; 
            
            kill(shmptr->parent_pid, SIGUSR1);

           
            shmptr->status = NotReady; 
            shmdt(shmptr);
            if(logToFile){
                log= "Process id " + to_string(process_id) + " termino";
                logger->writeToLogFile(log);
            }
            return OK;
        }else if (pid<0){
            perror ("Error in creating child process");
            return ERROR;

        }     
            
    }

    if(logToFile){
        log= "Process id " + to_string(pid) + " :esperando a los procesos hijos que terminen";
        logger->writeToLogFile(log);
    }
    
    for(int i=0; i<cant; i++){
        wait(NULL);
        if(logToFile){
        log= "Process id " + to_string(pid) + " :termino hijo " + to_string(i+1) + " de " + to_string(cant);
        logger->writeToLogFile(log);
         }   
    }


    ProcesadorImagenes procesador(cant, ancho, alto);
    if(logToFile){
        log= "Se comienza el aplanado de imagenes ";
        logger->writeToLogFile(log);
    }

    procesador.aplanarImagenes(imagenes);
    procesador.devolverImagen()->mostrarImagen();

     
    shmdt(shmptr);
    shmctl(shmid, IPC_RMID, NULL);

    // After function call 
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(stop - start); 
    if(logToFile){
        log= "Se termina el programa , tiempo de ejecucion: " + to_string(duration.count())+ " segundos";
        logger->writeToLogFile(log);
    }
    return OK;
}