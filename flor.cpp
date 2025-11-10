/*#include <QPropertyAnimation>
#include "flor.h"
#include <QFont>
#include <QBrush>
#include <QPen>
#include <QColor>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QGraphicsDropShadowEffect>


Flor::Flor(const QPixmap& pixmap, const QString& respuesta, bool esCorrecta, QGraphicsItem* parent)
    : QGraphicsPixmapItem(pixmap, parent)
    , textoRespuesta(respuesta)
    , correcta(esCorrecta)
    , recolectada(false)
{
    // Crear rectángulo de fondo blanco
   // /*QGraphicsRectItem* fondo = new QGraphicsRectItem(this);
   // fondo->setRect(0, 0, 150, 30);
   // fondo->setBrush(QBrush(QColor(255, 255, 255, 220)));
    //fondo->setPen(QPen(Qt::black, 2));

    // Crear el texto
    textoItem = new QGraphicsTextItem(this);

    QFont font;
    font.setFamily("Comic Sans MS");   // más acorde al estilo del hada
    font.setPointSize(8);
    font.setBold(true);
    textoItem->setFont(font);
    textoItem->setPlainText(respuesta);
    textoItem->setDefaultTextColor(QColor("#4a148c"));

    // Centrar todo debajo de la flor
    qreal anchoFlor = pixmap.width();
    qreal altoFlor = pixmap.height();

    qreal anchoTexto = textoItem->boundingRect().width();
    qreal altoTexto = textoItem->boundingRect().height();

    textoItem->setPos((anchoFlor - anchoTexto) / 2, (altoFlor - altoTexto) / 2);

    // Efecto de sombra suave para mejor visibilidad
    QGraphicsDropShadowEffect* sombra = new QGraphicsDropShadowEffect();
    sombra->setColor(Qt::white);
    sombra->setBlurRadius(5);
    sombra->setOffset(0);
    textoItem->setGraphicsEffect(sombra);
}

void Flor::moverA(const QPointF& destino) {
    QPropertyAnimation* anim = new QPropertyAnimation(this, "pos");
    anim->setDuration(500);
    anim->setStartValue(pos());
    anim->setEndValue(destino);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
    enCanasta = true;
}*/


#include "flor.h"
#include <QTimer>
#include <QFont>
#include <QBrush>
#include <QPen>
#include <QColor>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QGraphicsDropShadowEffect>

Flor::Flor(const QPixmap& pixmap, const QString& respuesta, bool esCorrecta, QGraphicsItem* parent)
    : QGraphicsPixmapItem(pixmap, parent)
    , textoRespuesta(respuesta)
    , correcta(esCorrecta)
    , recolectada(false)
    , enCanasta(false)
    , textoItem(nullptr)
{
    // Crear el texto de la respuesta
    textoItem = new QGraphicsTextItem(this);

    // Configurar fuente del texto - estilo mágico para el hada
    QFont font;
    font.setFamily("Arial");  // Arial es más compatible
    font.setPointSize(10);
    font.setBold(true);
    textoItem->setFont(font);
    textoItem->setPlainText(respuesta);
    textoItem->setDefaultTextColor(QColor("#2c0e4a"));  // Morado muy oscuro

    // Calcular dimensiones
    qreal anchoFlor = pixmap.width();
    qreal altoFlor = pixmap.height();
    qreal anchoTexto = textoItem->boundingRect().width();
    qreal altoTexto = textoItem->boundingRect().height();

    // Posicionar el texto justo arriba de la flor, centrado
    //textoItem->setPos((anchoFlor - anchoTexto) / 2, altoFlor + 8);
    textoItem->setPos((anchoFlor - anchoTexto) / 2, -altoTexto - 8);

    // Agregar sombra blanca al texto para que sea legible
    QGraphicsDropShadowEffect* sombra = new QGraphicsDropShadowEffect();
    sombra->setColor(QColor(255, 255, 255, 230));  // Blanco semi-transparente
    sombra->setBlurRadius(15);
    sombra->setOffset(0, 0);
    textoItem->setGraphicsEffect(sombra);

    // El texto es visible por defecto cuando la flor está en el césped
    textoItem->setVisible(true);
}

void Flor::mostrarTexto(bool mostrar)
{
    // Controlar la visibilidad del texto
    if (textoItem) {
        textoItem->setVisible(mostrar);
    }
}

void Flor::moverA(const QPointF& destino)
{
    // SOLUCIÓN SIMPLE: Movimiento inmediato (sin animación para evitar crashes)
    setPos(destino);
    enCanasta = true;
}
