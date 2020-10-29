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



    ~Imagen();


    private:

    int randomBetween0andX(int X);
    int procesarPixel(int pixel);
    int sumaPixeles(int &pixel1, int &pixel2 );





};
#endif // IMAGEN_H