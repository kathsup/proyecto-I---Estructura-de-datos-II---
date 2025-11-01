#ifndef PERSONAJE_H
#define PERSONAJE_H

#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QVector>
#include <QPixmap>
#include <QList>

class Personaje : public QGraphicsPixmapItem
{
public:
    Personaje(const QVector<QPixmap>& spritesDer,
              const QVector<QPixmap>& spritesIzq,
              const QVector<QPixmap>& spritesArriba,
              QPointF posicionInicial);

    void mover(QKeyEvent* event, const QList<QGraphicsItem*>& obstaculos, const QRectF& limites);

    //void setDireccionActual(const QString& direccion);

private:
    QVector<QPixmap> spritesDer;
    QVector<QPixmap> spritesIzq;
    QVector<QPixmap> spritesArriba;

    QString direccionActual = "derecha"; // por defecto
    int spriteActual = 0;
    int paso = 10; // velocidad del movimiento
};

#endif // PERSONAJE_H
