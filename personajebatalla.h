/*#ifndef PERSONAJEBATALLA_H
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
*/

#ifndef PERSONAJEBATALLA_H
#define PERSONAJEBATALLA_H

#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
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
        IDLE,
        ATACANDO,
        DERROTADO
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

    // ✅ NUEVAS FUNCIONES PARA BARRA DE VIDA
    void actualizarBarraVida(int vidasActuales, int vidasMaximas);
    void inicializarBarraVida(int vidasIniciales, int vidasMaximas);

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
    int frameActual;
    QPointF posicionInicial;

    // ✅ BARRA DE VIDA
    QGraphicsRectItem* barraDeFondo;        // Fondo gris
    QGraphicsRectItem* barraDeVida;         // Barra de color
    QGraphicsTextItem* textoVidas;          // Texto "3/4"

    // Constantes de la barra
    static constexpr int BARRA_ANCHO = 100;
    static constexpr int BARRA_ALTO = 12;
    static constexpr int BARRA_OFFSET_Y = -20; // Arriba del personaje

    void cargarSprites();
    void actualizarFrame();
    void lanzarFuego(QPointF destino);
    void crearBarraVida(); // ✅ NUEVA
    void actualizarPosicionBarra(); // ✅ NUEVA
};

#endif // PERSONAJEBATALLA_H
