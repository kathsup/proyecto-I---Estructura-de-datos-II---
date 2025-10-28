#include "escenario.h"
#include <QGraphicsView>
#include <QGraphicsRectItem>

Escenario::Escenario(QObject* parent) : QObject(parent) {}
Escenario::~Escenario() {}

void Escenario::inicializar(QGraphicsView* vista, const QPixmap& pixFondo = QPixmap())
{
    scene = new QGraphicsScene();
    vista->setScene(scene);
    vista->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Si se pasó un fondo, agregarlo
    if(!pixFondo.isNull()) {
        fondo = scene->addPixmap(pixFondo.scaled(vista->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        fondo->setZValue(-1); // Para que quede detrás de todo
    }

    // Ajustar el rectángulo de la escena al tamaño del graphicsView
    scene->setSceneRect(0,0,vista->width(),vista->height());
}


void Escenario::crearPersonaje(const QVector<QPixmap>& der,
                               const QVector<QPixmap>& izq,
                               const QVector<QPixmap>& arriba,
                               QPointF posicionInicial)
{
    spritesDer = der;
    spritesIzq = izq;
    spritesArriba = arriba;

    personaje = scene->addPixmap(spritesDer[0]);
    personaje->setPos(posicionInicial);
}

void Escenario::manejarTecla(QKeyEvent* event) {
    if (!personaje) return;

    QPointF pos = personaje->pos();
    int step = 10;
    QVector<QPixmap>* spritesActuales = nullptr;

    switch(event->key()) {
    case Qt::Key_Left: pos.setX(pos.x()-step); spritesActuales=&spritesIzq; break;
    case Qt::Key_Right: pos.setX(pos.x()+step); spritesActuales=&spritesDer; break;
    case Qt::Key_Up: pos.setY(pos.y()-step); spritesActuales=&spritesArriba; break;
    case Qt::Key_Down: pos.setY(pos.y()+step); spritesActuales=&spritesDer; break;
    default: return;
    }



    //que no salga de la pantalla
    QRectF sceneRect = scene->sceneRect();
    QPointF sizePersonaje(personaje->pixmap().width(), personaje->pixmap().height());

    if (pos.x() < sceneRect.left()) pos.setX(sceneRect.left());
    if (pos.y() < sceneRect.top()) pos.setY(sceneRect.top());
    if (pos.x() + sizePersonaje.x() > sceneRect.right()) pos.setX(sceneRect.right() - sizePersonaje.x());
    if (pos.y() + sizePersonaje.y() > sceneRect.bottom()) pos.setY(sceneRect.bottom() - sizePersonaje.y());

    personaje->setPos(pos);

    // Colisiones
    for(auto* item : obstaculos) {
        if(personaje->collidingItems().contains(item)) {
            switch(event->key()) {
            case Qt::Key_Left: pos.setX(pos.x()+step); break;
            case Qt::Key_Right: pos.setX(pos.x()-step); break;
            case Qt::Key_Up: pos.setY(pos.y()+step); break;
            case Qt::Key_Down: pos.setY(pos.y()-step); break;
            }
            personaje->setPos(pos);
        }
    }

    // Animación
    if(spritesActuales && !spritesActuales->isEmpty()) {
        currentSprite = (currentSprite+1)%spritesActuales->size();
        personaje->setPixmap((*spritesActuales)[currentSprite]);
    }
}
