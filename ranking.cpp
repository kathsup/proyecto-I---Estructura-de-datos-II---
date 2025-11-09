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

// ⭐ FUNCIÓN DE ORDENAMIENTO (Bubble Sort simple)
void Ranking::ordenarRanking() {
    // Ordenar de mayor a menor usando Bubble Sort
    for (int i = 0; i < jugadores.size() - 1; i++) {
        for (int j = 0; j < jugadores.size() - i - 1; j++) {
            if (jugadores[j].getPuntos() < jugadores[j + 1].getPuntos()) {
                // Intercambiar
                Jugador temp = jugadores[j];
                jugadores[j] = jugadores[j + 1];
                jugadores[j + 1] = temp;
            }
        }
    }
}

// ⭐ SIMPLIFICADO: Solo agregar y ordenar
void Ranking::agregarPuntaje(QString nombre, int puntos) {
    jugadores.append(Jugador(nombre, puntos));

    // Ordenar después de agregar
    ordenarRanking();

    guardarEnArchivo();
    qDebug() << "🏆" << nombre << "agregado con" << puntos << "puntos. Total:" << jugadores.size();
}

QVector<Jugador> Ranking::obtenerRanking() const {
    return jugadores;
}

void Ranking::limpiarRanking() {
    jugadores.clear();
    guardarEnArchivo();
}
