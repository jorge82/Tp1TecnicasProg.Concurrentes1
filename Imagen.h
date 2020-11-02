#if !defined IMAGEN_H
#define IMAGEN_H

#include <iostream>
#include <stdlib.h> 
#include <stdio.h> 
#include <unistd.h>
#include <time.h> 

using namespace std;

class Imagen {

    int ancho, alto;

    int** pixeles;



    public:

    Imagen(int, int);
    int generarImagenAleatoria();
    int mostrarImagen();
    int procesarImagen();
    int** getImagen();
    int sumarImagen(Imagen &imagen);
    int multiplicarImagen(float valor);
    string serializeImagen();
    int desSerializeImagen(string &serial);

    // Copy constructor
    Imagen(const Imagen& p2) {
        ancho= p2.ancho;
        alto = p2.alto;
        pixeles=p2.pixeles;
         pixeles= new int*[alto];
     

	// dynamically allocate memory of size N for each row
	for (int i = 0; i < alto; i++)
		pixeles[i] = new int[ancho];
    //inicializo la matriz 
    for (int i = 0; i < alto; i++)
        for (int j = 0; j < ancho; j++){
            pixeles[i][j] = p2.pixeles[i][j];
        }
        cout << ">> Copy constructor called.\n";
    }



    ~Imagen();


    private:

    int randomBetween0andX(int X);
    int procesarPixel(int pixel);
    int sumaPixeles(int &pixel1, int &pixel2 );
    int getValueFromSerial(string &serial, int &pos);

    





};
#endif // IMAGEN_H