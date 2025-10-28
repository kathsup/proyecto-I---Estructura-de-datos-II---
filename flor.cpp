#include <QPropertyAnimation>
#include "flor.h"

Flor::Flor(const QPixmap& pixmap, const QString& respuesta, bool esCorrecta, QGraphicsItem* parent)
    : QGraphicsPixmapItem(pixmap, parent)
    , textoRespuesta(respuesta)
    , correcta(esCorrecta)
    , recolectada(false)
{}

void Flor::moverA(const QPointF& destino) {
    QPropertyAnimation* anim = new QPropertyAnimation(this, "pos");
    anim->setDuration(500);
    anim->setStartValue(pos());
    anim->setEndValue(destino);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
    enCanasta = true;
}
