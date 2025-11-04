#ifndef PERSONAJEBATALLA_H
#define PERSONAJEBATALLA_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QTimer>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>

class PersonajeBatalla : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)

public:
    enum Estado {
        IDLE,      // Animación de espera (saltando)
        ATACANDO,  // Lanzando fuego
        DERROTADO  // Perdió
    };

    enum Bando {
        ROJO,      // Empirista (izquierda)
        BLANCO     // Racionalista (derecha)
    };

    PersonajeBatalla(Bando bando, QGraphicsScene* scene, QObject* parent = nullptr);
    ~PersonajeBatalla();

    void iniciarAnimacionIdle();
    void detenerAnimacionIdle();
    void atacar(PersonajeBatalla* objetivo);
    void mostrarDerrota();

    Bando getBando() const { return bando; }
    Estado getEstado() const { return estado; }

private:
    Bando bando;
    Estado estado;
    QGraphicsScene* scene;

    // Sprites
    QVector<QPixmap> spritesDerecha;
    QVector<QPixmap> spritesIzquierda;
    QPixmap spriteAtaque;
    QPixmap spriteDerrota;
    QPixmap spriteFuego;

    // Animaciones
    QTimer* timerAnimacion;
    QPropertyAnimation* animacionSalto;
    // ELIMINADO: QPropertyAnimation* animacionFuego;

    int frameActual;

    // Posiciones
    QPointF posicionInicial;

    void cargarSprites();
    void actualizarFrame();
    void lanzarFuego(QPointF destino);
};

#endif // PERSONAJEBATALLA_H
