CC=g++
CFLAGS=-std=c++11


all : procesadorSharedMem procesadorFifo
procesadorSharedMem: Imagen.h Imagen.cpp ProcesadorImagenes.h ProcesadorImagenes.cpp Logger.h Logger.cpp main.cpp
	$(CC) $(CFLAGS) Imagen.h Imagen.cpp ProcesadorImagenes.h ProcesadorImagenes.cpp Logger.h Logger.cpp main.cpp -o procesadorSharedMem

procesadorFifo: Imagen.h Imagen.cpp ProcesadorImagenes.h ProcesadorImagenes.cpp Logger.h Logger.cpp Fifo.cpp
	$(CC) $(CFLAGS) Imagen.h Imagen.cpp ProcesadorImagenes.h ProcesadorImagenes.cpp Logger.h Logger.cpp Fifo.cpp -o procesadorFifo

