#ifndef PREGUNTA_H
#define PREGUNTA_H

#include <QString>
#include <QVector>

struct Respuesta {
    QString texto;
    bool esCorrecta;
};

struct Pregunta {
    QString texto;
    QVector<Respuesta> respuestas;
};


#endif // PREGUNTA_H
