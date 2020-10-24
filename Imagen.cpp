#include "Imagen.h"


Imagen::Imagen(int anch, int alt){

    this->alto=alt;
    this->ancho=anch;
    pixeles= new int*[alt];


	// dynamically allocate memory of size N for each row
	for (int i = 0; i < alt; i++)
		pixeles[i] = new int[anch];




    cout<<"El alto es: "<<this->alto<<" el ancho es: "<<this->ancho<<endl;
 
}


Imagen::~Imagen(){

       for (int i = 0; i < alto; i++)
		delete[] pixeles[i];

	    delete[] pixeles;
        


}