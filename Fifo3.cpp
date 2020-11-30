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

#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <chrono> 

  
#define Recieved 2
#define Ready 1 
#define NotReady -1 
#define Finished 3 
#define BUFFSIZE 64000
#define ERROR -1
#define OK 0

using namespace std::chrono; 
using namespace std;


/*  
    Ejecucion: /procesadorFifo  <tam_imagen> <num_camaras>
*/
/* false si no se quiere log*/
bool logToFile=true;


int main(int argc, const char** argv) {
    
    if (argc!=3){
       perror ("Faltan argumentos. Debe ingresar N(ancho y alto de las imagenes) y C(numero de camaras)");
       return ERROR;
    }
    
    // comienzo a contar el tiempo
    auto start = high_resolution_clock::now(); 
   
    int n= atoi(argv[1]); 
    int cant= atoi(argv[2]);   
    int alto=n;
    int ancho=n;

    vector <Imagen> imagenes;
    Imagen imagen(ancho,alto);
    Logger* logger=Logger::getLogger("Log.txt");
    string log;


    if(logToFile){
        log= "Comienza el programa con " + to_string(cant) +" imagenes de " + to_string(n) + "x" +to_string(n)+ " pixeles";   ;
        logger->writeToLogFile(log);
    }
    // path del fifo
    char  myfifo[] = "/tmp/myfifo"; 
  
    // Creo el FIFO si o existe
    mkfifo(myfifo, 0666); 
    //obtengo el pid del padre
    int parent_id=getppid();
    int fd;

     for (int i=0; i < cant; ++i)
    {   
        int pid=fork();
        //Proceso hijo
        if(pid==0){
            // Open FIFO for write only 
             fd = open(myfifo, O_WRONLY);

            int process_id= getpid();
        
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
            //Imagen.mostrarImagen();
 
            string serial=Imagen.serializeImagen();
            int n = serial.length();
    
 
            char char_array[n + 1];
        
            strcpy(char_array, serial.c_str());
              if(logToFile){
                log= "Process id " + to_string(process_id) + " envia la imagen procesada al padre :" + serial ;
                logger->writeToLogFile(log);
            }
            
            write(fd, char_array, strlen(char_array)); 

            close(fd); 
            return OK;
        }else if (pid<0){
            perror ("Error in creating child process");
            return ERROR;

        }   
            
    }

    // abro FIFO para lectura 
    fd = open(myfifo, O_RDONLY);

    if(logToFile){
        log= "Process id " + to_string(parent_id) + " :esperando a los procesos hijos que terminen";
        logger->writeToLogFile(log);
    }
    //Espero  que todos los hijos terminen
    for(int i=0; i<cant; i++){

    char data_in[BUFFSIZE];

    read(fd, data_in, sizeof(data_in));

    string data= data_in;
    
    if(logToFile){
    log= "Proceso padre recibe la imagen " + data;
    logger->writeToLogFile(log);
    }   
        imagen.desSerializeImagen(data);
        imagen.mostrarImagen();
        imagenes.push_back(imagen);
        
        wait(NULL);
        
        if(logToFile){
            log= "Process id " + to_string(parent_id) + " :termino hijo " + to_string(i+1) + " de " + to_string(cant);
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

  
    close(fd); 
   
    // Calculo la duracion de ejecuciòn
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(stop - start); 
    if(logToFile){
        log= "Se termina el programa , tiempo de ejecucion: " + to_string(duration.count())+ " segundos";
        logger->writeToLogFile(log);
    }
    return OK;
}


