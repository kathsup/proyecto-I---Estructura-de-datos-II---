#include "ranking.h"
#include <QFile>
#include <QDataStream>
#include <QDebug>
#include <algorithm> // Para sorting

Ranking::Ranking() {
    archivoRanking = "ranking.dat";
    cargarDesdeArchivo();
}

Ranking::~Ranking() {
    guardarEnArchivo();
}

bool Ranking::cargarDesdeArchivo() {
    QFile archivo(archivoRanking);

    if (!archivo.exists()) {
        qDebug() << "📁 Archivo de ranking no existe, se creará uno nuevo";
        return false;
    }

    if (!archivo.open(QIODevice::ReadOnly)) {
        qDebug() << "❌ Error al abrir archivo para lectura";
        return false;
    }

    QDataStream in(&archivo);
    jugadores.clear();

    while (!in.atEnd()) {
        char nombre[50];
        int puntos;

        in.readRawData(nombre, 50);
        in >> puntos;

        jugadores.append(Jugador(QString::fromUtf8(nombre), puntos));
    }

    archivo.close();

    // ⭐ ORDENAR DESPUÉS DE CARGAR (de mayor a menor)
    ordenarRanking();

    qDebug() << "✅ Ranking cargado:" << jugadores.size() << "jugadores";
    return true;
}

bool Ranking::guardarEnArchivo() {
    QFile archivo(archivoRanking);

    if (!archivo.open(QIODevice::WriteOnly)) {
        qDebug() << "❌ Error al abrir archivo para escritura";
        return false;
    }

    QDataStream out(&archivo);

    for (const Jugador& j : jugadores) {
        char nombre[50] = {0};
        QByteArray nombreBytes = j.getNombre().toUtf8();
        qstrncpy(nombre, nombreBytes.constData(), 50);

        out.writeRawData(nombre, 50);
        out << j.getPuntos();
    }

    archivo.close();
    qDebug() << "💾 Ranking guardado:" << jugadores.size() << "jugadores";
    return true;
}

// ⭐ QUICK SORT DESCENDENTE (de mayor a menor)
void Ranking::ordenarRanking() {
    if (jugadores.size() <= 1) return;
    quickSortDescendente(0, jugadores.size() - 1);
}

// Función auxiliar de partición
int Ranking::particion(int bajo, int alto) {
    int pivote = jugadores[alto].getPuntos();
    int i = bajo - 1;

    for (int j = bajo; j < alto; j++) {
        // Para orden DESCENDENTE: si es MAYOR que el pivote
        if (jugadores[j].getPuntos() > pivote) {
            i++;
            // Intercambiar jugadores[i] y jugadores[j]
            Jugador temp = jugadores[i];
            jugadores[i] = jugadores[j];
            jugadores[j] = temp;
        }
    }

    // Intercambiar jugadores[i+1] y jugadores[alto] (pivote)
    Jugador temp = jugadores[i + 1];
    jugadores[i + 1] = jugadores[alto];
    jugadores[alto] = temp;

    return i + 1;
}

// Función recursiva de Quick Sort
void Ranking::quickSortDescendente(int bajo, int alto) {
    if (bajo < alto) {
        int pi = particion(bajo, alto);
        quickSortDescendente(bajo, pi - 1);
        quickSortDescendente(pi + 1, alto);
    }
}

// ⭐ SIMPLIFICADO: Solo agregar y ordenar
/*void Ranking::agregarPuntaje(QString nombre, int puntos) {
    jugadores.append(Jugador(nombre, puntos));

    // Ordenar después de agregar
    ordenarRanking();

    guardarEnArchivo();
    qDebug() << "🏆" << nombre << "agregado con" << puntos << "puntos. Total:" << jugadores.size();
}*/

// ⭐ MODIFICADO: Actualizar si existe, agregar si no existe
void Ranking::agregarPuntaje(QString nombre, int puntos) {
    // Buscar si el jugador ya existe
    bool encontrado = false;
    for (int i = 0; i < jugadores.size(); i++) {
        if (jugadores[i].getNombre() == nombre) {
            // ⭐ ACTUALIZAR puntaje existente (sumar o reemplazar)
            // Opción 1: SUMAR puntos
            // jugadores[i].setPuntos(jugadores[i].getPuntos() + puntos);

            // Opción 2: REEMPLAZAR solo si es mayor (guardar mejor puntaje)
            if (puntos > jugadores[i].getPuntos()) {
                jugadores[i].setPuntos(puntos);
                qDebug() << "🏆 Nuevo récord para" << nombre << ":" << puntos << "pts";
            } else {
                qDebug() << "📊" << nombre << "ya tiene un mejor puntaje:" << jugadores[i].getPuntos();
            }

            encontrado = true;
            break;
        }
    }

    // Si no existe, agregarlo
    if (!encontrado) {
        jugadores.append(Jugador(nombre, puntos));
        qDebug() << "🆕" << nombre << "agregado al ranking con" << puntos << "puntos";
    }

    // Ordenar y guardar
    ordenarRanking();
    guardarEnArchivo();
    qDebug() << "💾 Ranking actualizado. Total:" << jugadores.size() << "jugadores";
}

QVector<Jugador> Ranking::obtenerRanking() const {
    return jugadores;
}

void Ranking::limpiarRanking() {
    jugadores.clear();
    guardarEnArchivo();
}
