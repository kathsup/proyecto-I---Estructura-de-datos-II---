#include "personaje.h"

#include <QGraphicsScene>

Personaje::Personaje(const QVector<QPixmap>& der,
                     const QVector<QPixmap>& izq,
                     const QVector<QPixmap>& arriba,
                     QPointF posicionInicial)
    : spritesDer(der), spritesIzq(izq), spritesArriba(arriba)
{
    // Establecer sprite inicial
    setPixmap(spritesDer[0]);
    setPos(posicionInicial);
    setZValue(1); // para que quede encima del fondo
}

void Personaje::mover(QKeyEvent* event, const QList<QGraphicsItem*>& obstaculos, const QRectF& limites)
{
    QPointF nuevaPos = pos();

    switch (event->key()) {
    case Qt::Key_Left:
        nuevaPos.setX(nuevaPos.x() - paso);
        direccionActual = "izquierda";
        break;
    case Qt::Key_Right:
        nuevaPos.setX(nuevaPos.x() + paso);
        direccionActual = "derecha";
        break;
    case Qt::Key_Up:
        nuevaPos.setY(nuevaPos.y() - paso);
        direccionActual = "arriba";
        break;
    case Qt::Key_Down:
        nuevaPos.setY(nuevaPos.y() + paso);
        direccionActual = "derecha"; // puedes cambiar a "abajo" si tienes sprites abajo
        break;
    default:
        return;
    }

    // Evitar salir del escenario
    if (!limites.contains(QRectF(nuevaPos, boundingRect().size()))) {
        return;
    }

    // Guardar posición previa
    QPointF posAnterior = pos();
    setPos(nuevaPos);

    // Verificar colisiones
    for (auto* item : collidingItems()) {
        if (obstaculos.contains(item)) {
            setPos(posAnterior);
            return;
        }
    }

    // Animación
    QVector<QPixmap>* spritesActuales = nullptr;
    if (direccionActual == "derecha") spritesActuales = &spritesDer;
    else if (direccionActual == "izquierda") spritesActuales = &spritesIzq;
    else if (direccionActual == "arriba") spritesActuales = &spritesArriba;

    if (spritesActuales && !spritesActuales->isEmpty()) {
        spriteActual = (spriteActual + 1) % spritesActuales->size();
        setPixmap((*spritesActuales)[spriteActual]);
    }
}


/*void Personaje::setDireccionActual(const QString& direccion)
{
    direccionActual = direccion;
}*/
