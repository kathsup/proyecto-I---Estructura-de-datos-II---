/*#ifndef FLOR_H
#define FLOR_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QString>

class Flor : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Flor(const QPixmap& pixmap, const QString& respuesta, bool esCorrecta, QGraphicsItem* parent = nullptr);

    QString obtenerRespuesta() const { return textoRespuesta; }
    bool esRespuestaCorrecta() const { return correcta; }
    bool estaRecolectada() const { return recolectada; }
    void marcarComoRecolectada() { recolectada = true; }
    void moverA(const QPointF& destino); // animación al moverse a la canasta

private:
    QString textoRespuesta;
    bool correcta;
    bool recolectada;
    bool enCanasta = false;
    QGraphicsTextItem* textoItem;
};

#endif // FLOR_H*/

#ifndef FLOR_H
#define FLOR_H

#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>

class Flor : public QGraphicsPixmapItem
{
public:
    Flor(const QPixmap& pixmap, const QString& respuesta, bool esCorrecta, QGraphicsItem* parent = nullptr);

    QString obtenerTexto() const { return textoRespuesta; }
    bool esRespuestaCorrecta() const { return correcta; }
    bool estaRecolectada() const { return recolectada; }
    void marcarComoRecolectada() { recolectada = true; }

    void moverA(const QPointF& destino);
    void mostrarTexto(bool mostrar);  // Nueva función para mostrar/ocultar texto

private:
    QString textoRespuesta;
    bool correcta;
    bool recolectada;
    bool enCanasta = false;
    QGraphicsTextItem* textoItem;
};

#endif // FLOR_H
