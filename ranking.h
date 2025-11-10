#ifndef RANKING_H
#define RANKING_H

#include <QString>
#include <QVector>
#include "jugador.h"

class Ranking {
private:
    // ELIMINAR: static const int MAX_JUGADORES = 10;
    QVector<Jugador> jugadores; // Cambiar nombre de topJugadores a jugadores
    QString archivoRanking;

public:
    Ranking();
    ~Ranking();

    bool cargarDesdeArchivo();
    bool guardarEnArchivo();
    void agregarPuntaje(QString nombre, int puntos); // Cambiar a void
    QVector<Jugador> obtenerRanking() const; // Cambiar nombre
    void limpiarRanking();
    void ordenarRanking();
    void quickSortDescendente(int bajo, int alto);
    int particion(int bajo, int alto);
};


#endif // RANKING_H
