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

using namespace std;

#define BUFFSIZE 1000
#define ERROR -1
#define OK 0


using namespace std::chrono; 

using namespace std;


bool logToFile=true;


void sendBytes(int &fd, string &message);

void readBytes(int &fd, string &mensaje);

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
    int status=Ready;

    if(logToFile){
        log= "Comienza el programa con " + to_string(cant) +" imagenes de " + to_string(n) + "x" +to_string(n)+ " pixeles";   ;
        logger->writeToLogFile(log);
    }

    // path del fifo
    char  myfifo[] = "/tmp/myfifo"; 
  
    // Creo el FIFO si o existe
    mkfifo(myfifo, 0666); 

   
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
            
            while(status != Ready){
               
            }    
            status = NotReady; 

            sendBytes(fd,serial);
 
            // declaring character array
            char char_array[n + 1];
        
            // copying the contents of the
            // string to char array

            strcpy(char_array, serial.c_str());
              if(logToFile){
                log= "Process id " + to_string(process_id) + " envia la imagen procesada al padre :" + serial ;
                logger->writeToLogFile(log);
            }
            //  cout<<"bytes sent should send "<< strlen(char_array)<<endl;
            //int bytes_sent=write(fd, char_array, strlen(char_array)); 
            //cout<<"bytes sent "<<bytes_sent<<" should send "<< strlen(char_array)<<endl;
        
            close(fd); 
            return OK;
        }else if (pid<0){
            perror ("Error in creating child process");
            return ERROR;

        }   
            
    }



// Open FIFO for Read only 
    fd = open(myfifo, O_RDONLY);
    

    if(logToFile){
        log= "Process id " + to_string(parent_id) + " :esperando a los procesos hijos que terminen";
        logger->writeToLogFile(log);
    }
    //Espero  que todos los hijos terminen
        // Need to wait for all
        for(int i=0; i<cant; i++){


        char data_in[BUFFSIZE];
        
        //read(fd, data_in, sizeof(data_in));
        

        string data="";
        readBytes(fd, data); 

        //cout<<endl<<" data recieved from child: "<<data<<endl;
          

            //string data= data_in;
               if(logToFile){
            log= "Proceso padre recibe la imagen " + data;
            logger->writeToLogFile(log);
        }   
            imagen.desSerializeImagen(data);
            imagen.mostrarImagen();
            imagenes.push_back(imagen);
            status=Ready;
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


void sendBytes(int &fd, string &message){

    int enviado =0;
    int inicio=0;
    int chunk_size=1000;
      //cout<<"mensaje original"<<message<<endl;

    int bytes_sent;  
    string tam=to_string(message.length());
    //envio el tamaño primero;
    write(fd, static_cast<const void*>(tam.c_str()), tam.length());
         cout<<"tam a enviar "<<tam<<endl;

    while(enviado <=message.size()){
        string mensaje=message.substr(inicio, chunk_size);
        
        //cout<<"mensaje a enviar"<<mensaje<<endl;

        bytes_sent=write(fd, static_cast<const void*>(mensaje.c_str()), mensaje.length()); 

        enviado+=chunk_size;
        inicio+=chunk_size;
        
    }
}

void readBytes(int &fd,string  &mensaje){
    
    int recibido =0;
    int inicio=0;
    int chunk_size=1000;
    int bytes_sent;  
    char tamChar[100];
    memset(tamChar, 0, sizeof(tamChar));

    //long n;
    //read(fd, &n, sizeof(n));
    int bytes=read(fd, tamChar, sizeof(tamChar));
    std::string tamanio = tamChar;
	tamanio.resize (bytes);
    cout<<"tam recibido"<<tamanio<<endl;

    long tam= stol(tamanio);   
    cout<<"tam recibido en int "<<tam<<endl;;
     
   
    while(recibido <=tam){

         char data_in[BUFFSIZE];
        
        read(fd, data_in, sizeof(data_in));

        //cout<<"mensaje recibido "<<data_in<<endl;;
        mensaje= mensaje + data_in;

        recibido+=chunk_size;
    
        
    }
    //cout<<"mensaje recibido 2 "<<mensaje;
}