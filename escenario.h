/*#ifndef ESCENARIO_H
#define ESCENARIO_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QVector>
#include <QKeyEvent>

class Escenario : public QObject{
    Q_OBJECT
public:
    explicit Escenario(QObject *parent = nullptr);
    virtual ~Escenario();

    virtual void inicializar(QGraphicsView* vista, const QPixmap& pixFondo);   // Crea la escena y el fondo
    virtual void manejarTecla(QKeyEvent* event);      // Movimiento del personaje si existe

    // Opcional: si el escenario necesita un personaje
    void crearPersonaje(const QVector<QPixmap>& der,
                        const QVector<QPixmap>& izq,
                        const QVector<QPixmap>& arriba,
                        QPointF posicionInicial);

    QList<QGraphicsRectItem*> obstaculos;     // Colisiones
    QList<QGraphicsItem*> zonasInteractivas;  // Puertas, objetos
    QGraphicsScene *scene = nullptr;
    QGraphicsPixmapItem *fondo = nullptr;
    QGraphicsPixmapItem *personaje = nullptr;
    QVector<QPixmap> spritesDer, spritesIzq, spritesArriba;
    int currentSprite = 0;



    virtual void configurarColisiones() {}
    virtual void configurarInteracciones() {}
};

#endif // ESCENARIO_H
*/

#ifndef ESCENARIO_H
#define ESCENARIO_H

#include <QObject>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPixmap>
#include <QList>
#include "personaje.h"

class Escenario : public QObject
{
    Q_OBJECT
public:
    explicit Escenario(QObject* parent = nullptr);
    ~Escenario();

    void inicializar(QGraphicsView* vista, const QPixmap& pixFondo = QPixmap());
    void crearPersonaje(const QVector<QPixmap>& der,
                        const QVector<QPixmap>& izq,
                        const QVector<QPixmap>& arriba,
                        QPointF posicionInicial);
    void manejarTecla(QKeyEvent* event);

    QGraphicsScene* scene;
    Personaje* personaje = nullptr;
    QList<QGraphicsItem*> obstaculos;
    QList<QGraphicsItem*> zonasInteractivas;


private:
    QGraphicsPixmapItem* fondo = nullptr;
};

#endif // ESCENARIO_H
