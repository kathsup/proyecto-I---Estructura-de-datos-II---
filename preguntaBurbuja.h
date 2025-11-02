#ifndef PREGUNTABURBUJA_H
#define PREGUNTABURBUJA_H

#include <QString>
#include <QVector>

struct PreguntaBurbuja {
    QString textoPregunta;          // El texto de la pregunta
    QVector<QString> opciones;      // Las respuestas posibles
    int indiceCorrecta;             // Índice de la respuesta correcta (0, 1, 2, 3...)

    PreguntaBurbuja() {}
    PreguntaBurbuja(QString pregunta, QVector<QString> respuestas, int correcta)
        : textoPregunta(pregunta), opciones(respuestas), indiceCorrecta(correcta) {}
};

#endif // PREGUNTABURBUJA_H
