#if !defined IMAGEN_H
#define IMAGEN_H

#include <iostream>
#include <stdlib.h> 
#include <stdio.h> 
#include <unistd.h>
#include <time.h> 
#include <vector>

using namespace std;

class Imagen {

    int ancho, alto;
    vector<int> vector_pixeles;

    public:

    Imagen(int, int);
    int generarImagenAleatoria();
    int mostrarImagen();
    int procesarImagen();
    int** getImagen();

   vector<int> getImagenVector();

    int sumarImagen(Imagen &imagen);
    int multiplicarImagen(float valor);
    int getImagenFromVector(vector<int> &vect);

    // Copy constructor
    Imagen(const Imagen& p2) {
        ancho= p2.ancho;
        alto = p2.alto;
        vector_pixeles=p2.vector_pixeles;
	
    }


    ~Imagen();


    private:

    int randomBetween0andX(int X);
    int procesarPixel(int pixel);
    int sumaPixeles(int &pixel1, int &pixel2 );
    int getValueFromSerial(string &serial, int &pos);

    





};
#endif // IMAGEN_H