#ifndef JUGADOR_H
#define JUGADOR_H

#include<QString>

class Jugador
{
public:
    Jugador(QString nombre, int puntos = 0);
    Jugador() : nombre(""), puntos(0) {}

    //getter
    QString getNombre()const;
    int getPuntos()const;

    //setter
    void setPuntos(int nuevosPuntos);
    QString nombre;
    int puntos;

};

#endif // JUGADOR_H
