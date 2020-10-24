#if !defined IMAGEN_H
#define IMAGEN_H





#include <iostream>

using namespace std;


class Imagen {

    int ancho, alto;

    int** pixeles;



    public:

    Imagen(int, int);

    ~Imagen();



};
#endif // IMAGEN_H