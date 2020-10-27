#include "Imagen.h"


Imagen::Imagen(int anch, int alt){

    this->alto=alt;
    this->ancho=anch;
    pixeles= new int*[alt];
     /* initialize random seed: */
    srand (time(NULL));

	// dynamically allocate memory of size N for each row
	for (int i = 0; i < alt; i++)
		pixeles[i] = new int[anch];

   // cout<<"El alto es: "<<this->alto<<" el ancho es: "<<this->ancho<<endl;
 
}

int Imagen::generarImagenAleatoria(){

    int randomNumber;
    for (int i = 0; i < alto; i++)
        for (int j = 0; j < ancho; j++){
            randomNumber=randomBetween0andX(255);
            //scout<<" el numero generado es: "<< randomNumber<<endl;
            pixeles[i][j] = randomNumber;
        }
    return 0;
}

int Imagen::procesarImagen(){
    int randomSeconds=randomBetween0andX(10);
    cout<<"Comenzo a procesar "<< randomSeconds<<" segundos"<<endl;
    for (int i = 0; i < alto; i++)
        for (int j = 0; j < ancho; j++){
            pixeles[i][j] = procesarPixel(pixeles[i][j]);
        }
    sleep(randomSeconds);
    cout<<"Termino de procesar"<<endl;
    return 0;
}


int Imagen::procesarPixel(int pixel){
    
    
    return pixel/2 + 1;

}

int Imagen::mostrarImagen(){

    cout<<"Imagen"<<endl;
    
    for (int i = 0; i < alto; i++){
     //cout<<"i vale"<<i<<endl;
        for (int j = 0; j < ancho; j++){
         //   cout<<"j vale"<<j<<endl;
            cout<<" "<<pixeles[i][j]<<" "; 
        }
        cout<<endl;
    }
    return 0;
}

int Imagen::randomBetween0andX(int X){
    
    int random=  1 + (rand()% X);
    return random;

}

Imagen::~Imagen(){

       for (int i = 0; i < alto; i++)
		delete[] pixeles[i];

	    delete[] pixeles;
        


}