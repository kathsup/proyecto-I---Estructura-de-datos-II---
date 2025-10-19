#include "jugador.h"

Jugador::Jugador(QString nombre,int puntos):nombre(nombre), puntos(puntos){}

QString Jugador::getNombre()const{
    return nombre;
}

int Jugador::getPuntos()const{
    return puntos;
}

void Jugador::setPuntos(int nuevosPuntos){
    puntos = nuevosPuntos;
}



