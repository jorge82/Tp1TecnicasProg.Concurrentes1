#include "Imagen.h"
#include "ProcesadorImagenes.h"

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

  
#define Recieved 2
#define Ready 1 
#define NotReady -1 
#define Finished 3 

using namespace std;

struct memory { 
    char buff[1000]; 
    int status, pid, parent_pid;  
};



char  data_in[1000];
vector <Imagen> imagenes;

Imagen imagen(8,8);
int fd; 

void handler(int signum) 
{ 
    if (signum == SIGUSR1) { 
       
        
        
        
        read(fd, data_in, sizeof(data_in));

       // cout<<" data recieved from child: "<<data_in<<endl<<endl;

        string data= data_in;
        imagen.desSerializeImagen(data);
        imagen.mostrarImagen();
        imagenes.push_back(imagen);
        
    } 
}

int main(int argc, const char** argv) {
    int alto=8;
    int ancho=8;
    
    if (argc!=2){
        cout<<"Falta ingresar el número de cámaras"<<endl;
        return -1;
    }


    int cant= atoi(argv[1]);   

    printf("--beginning of program\n");
    
  
    // FIFO file path 
    char  myfifo[] = "/tmp/myfifo"; 
  
    // Creating the named file(FIFO) 
    // mkfifo(<pathname>, <permission>) 
 
    mkfifo(myfifo, 0666); 
    

    
    signal(SIGUSR1, handler); 
    //signal(SIGUSR2, handler); 

    


    int parent;


     for (int i=0; i < cant; ++i)
    {
        if(fork()==0){
            // Open FIFO for write only 
       
             fd = open(myfifo, O_WRONLY);


             int pid = getpid(); 
             int parent_pid = getppid(); 
            
            srand (time(NULL)+i);
            Imagen Imagen(ancho, alto);
            Imagen.generarImagenAleatoria();
            Imagen.procesarImagen();
            Imagen.mostrarImagen();
 
            string serial=Imagen.serializeImagen();
            int n = serial.length();
 
            // declaring character array
            char char_array[n + 1];
        
            // copying the contents of the
            // string to char array
            strcpy(char_array, serial.c_str());


            //cout<<"iamgen a envia "<<serial.c_str()<<endl;
            //cout<<"iamgen a envia 22222"<<char_array<<endl;
            
            
         
            //memset(shmptr->buff,' ', strlen(shmptr->buff));
     
            //memcpy(shmptr->buff, char_array, sizeof(char_array));
            write(fd, char_array, strlen(char_array)); 
        
            
            
            kill(parent_pid, SIGUSR1);

            cout<<"finsihing process "<<i<<endl;
           
            close(fd); 

            return 1;
        }    
            
    }



// Open FIFO for Read only 
    fd = open(myfifo, O_RDONLY);
    


       cout << "Waiting childs to finishg" << endl;
        // Need to wait for all
        for(int i=0; i<cant; i++){
            wait(NULL);
            cout << "Got " << i+1 << " done" << endl;
        }

         ProcesadorImagenes procesador(cant, ancho, alto);
         procesador.aplanarImagenes(imagenes);

         procesador.devolverImagen()->mostrarImagen();

    cout<<" terminating"<<endl;
   close(fd); 
    // vector <Imagen> imagenes;
    // int cant= atoi(argv[1]);

    // for (int i; i<cant; i++){
        
    //     cout<<"Generando la imagen "<<i<<endl;
    //     // Imagen *imagen= new Imagen(ancho, alto);
    //     // imagen->generarImagenAleatoria();
        
    //     //imagenes.push_back(*(new Imagen(ancho, alto)));
    //       imagenes.emplace_back(ancho,alto);

        
    // }
    // for (int i; i<imagenes.size(); i++){
    //     cout<<"Procesando la imagen "<<i<<endl;
    //     imagenes[i].procesarImagen();
    //     imagenes[i].mostrarImagen();

    // }

    // Imagen imagen(6, 8);

    // imagen.generarImagenAleatoria();
    // //imagen.mostrarImagen();
    // //imagen.procesarImagen();
    // imagen.mostrarImagen();
    // string serial=imagen.serializeImagen();
    // cout<<endl<<"imagen serializada"<<serial<<endl;

    // imagen.desSerializeImagen(serial);
    // imagen.mostrarImagen();

    // Imagen imagen2(6, 8);

    // imagen2.generarImagenAleatoria();
    // //imagen2.mostrarImagen();
    // imagen2.procesarImagen();
    // imagen2.mostrarImagen();


//    // imagen.sumarImagen(imagen2);
//     //imagen.mostrarImagen();

    

  
//     imagenes.push_back(&imagen2);

    // ProcesadorImagenes procesador(imagenes,cant, ancho, alto);
    // procesador.aplanarImagenes();

    // procesador.devolverImagen().mostrarImagen();

    return 0;
}