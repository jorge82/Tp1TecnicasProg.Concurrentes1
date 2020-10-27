#if !defined IMAGEN_H
#define IMAGEN_H





#include <iostream>
#include <stdlib.h> 
#include <stdio.h> 
#include <unistd.h>

using namespace std;


class Imagen {

    int ancho, alto;

    int** pixeles;



    public:

    Imagen(int, int);

    int generarImagenAleatoria();
    int mostrarImagen();
    int procesarImagen();

    ~Imagen();


    private:
    int randomBetween0andX(int X);
    int procesarPixel(int pixel);



};
#endif // IMAGEN_H