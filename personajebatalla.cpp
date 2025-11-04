/*#include "personajebatalla.h"
#include <QGraphicsScene>
#include <QDebug>
#include <QEasingCurve>

PersonajeBatalla::PersonajeBatalla(Bando bando, QGraphicsScene* scene, QObject* parent)
    : QObject(parent)
    , bando(bando)
    , estado(IDLE)
    , scene(scene)
    , frameActual(0)
    , timerAnimacion(nullptr)
    , animacionSalto(nullptr)
{
    cargarSprites();
    setPixmap(spritesDerecha[0]); // Frame inicial

    // Posición inicial según el bando
    if (bando == ROJO) {
        // Rojo a la izquierda
        posicionInicial = QPointF(150, 400);
    } else {
        // Blanco a la derecha
        posicionInicial = QPointF(750, 400);
    }

    setPos(posicionInicial);
    scene->addItem(this);
}

PersonajeBatalla::~PersonajeBatalla()
{
    if (timerAnimacion) {
        timerAnimacion->stop();
        delete timerAnimacion;
    }
    if (animacionSalto) {
        animacionSalto->stop();
        delete animacionSalto;
    }
}

void PersonajeBatalla::cargarSprites()
{
    if (bando == ROJO) {
        // Cargar sprites del personaje ROJO (Empirista)
        spritesDerecha = {
            QPixmap("C:/Users/Lenovo/Downloads/rojo1AD.png").scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation),
            QPixmap("C:/Users/Lenovo/Downloads/rojo2AD.png").scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation),
            QPixmap("C:/Users/Lenovo/Downloads/rojo3AD.png").scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        };

        spritesIzquierda = {
            QPixmap("C:/Users/Lenovo/Downloads/rojo1Iz.png").scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation),
            QPixmap("C:/Users/Lenovo/Downloads/rojo2Iz.png").scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation),
            QPixmap("C:/Users/Lenovo/Downloads/rojo3Iz.png").scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        };

        spriteAtaque = QPixmap("C:/Users/Lenovo/Downloads/rojoAtaque.png").scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        spriteDerrota = QPixmap("C:/Users/Lenovo/Downloads/finalRojo.png").scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        spriteFuego = QPixmap("C:/Users/Lenovo/Downloads/FuegoRojoo.png").scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    } else { // BLANCO
        // Cargar sprites del personaje BLANCO (Racionalista)
        spritesDerecha = {
            QPixmap("C:/Users/Lenovo/Downloads/blanco1AD.png").scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation),
            QPixmap("C:/Users/Lenovo/Downloads/blanco2AD.png").scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation),
            QPixmap("C:/Users/Lenovo/Downloads/blanco3AD.png").scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        };

        spritesIzquierda = {
            QPixmap("C:/Users/Lenovo/Downloads/blanco1Iz.png").scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation),
            QPixmap("C:/Users/Lenovo/Downloads/blanco2Iz.png").scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation),
            QPixmap("C:/Users/Lenovo/Downloads/blanco3Iz.png").scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        };

        spriteAtaque = QPixmap("C:/Users/Lenovo/Downloads/blancoAtaque.png").scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        spriteDerrota = QPixmap("C:/Users/Lenovo/Downloads/finalBlanco.png").scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        spriteFuego = QPixmap("C:/Users/Lenovo/Downloads/FuegoBlancoo.png").scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }
}

void PersonajeBatalla::iniciarAnimacionIdle()
{
    if (estado != IDLE) return;

    // Timer para cambiar frames (animación de caminar/saltar)
    timerAnimacion = new QTimer(this);
    connect(timerAnimacion, &QTimer::timeout, this, &PersonajeBatalla::actualizarFrame);
    timerAnimacion->start(300); // Cambiar frame cada 300ms

    // Animación de salto (movimiento vertical)
    animacionSalto = new QPropertyAnimation(this, "pos");
    animacionSalto->setDuration(800);
    animacionSalto->setStartValue(posicionInicial);
    animacionSalto->setKeyValueAt(0.5, posicionInicial + QPointF(0, -20)); // Salta 20px
    animacionSalto->setEndValue(posicionInicial);
    animacionSalto->setEasingCurve(QEasingCurve::InOutQuad);
    animacionSalto->setLoopCount(-1); // Infinito
    animacionSalto->start();
}

void PersonajeBatalla::detenerAnimacionIdle()
{
    if (timerAnimacion) {
        timerAnimacion->stop();
    }
    if (animacionSalto) {
        animacionSalto->stop();
    }
}

void PersonajeBatalla::actualizarFrame()
{
    frameActual = (frameActual + 1) % 3; // Ciclar entre 0, 1, 2

    // Usar sprites de derecha o izquierda según el bando
    if (bando == ROJO) {
        setPixmap(spritesDerecha[frameActual]); // Rojo mira a la derecha
    } else {
        setPixmap(spritesIzquierda[frameActual]); // Blanco mira a la izquierda
    }
}

void PersonajeBatalla::atacar(PersonajeBatalla* objetivo)
{
    if (!objetivo) return;

    // Detener animación idle
    detenerAnimacionIdle();
    estado = ATACANDO;

    // Cambiar a sprite de ataque
    setPixmap(spriteAtaque);

    // Esperar un momento antes de lanzar el fuego
    QTimer::singleShot(300, this, [this, objetivo]() {
        lanzarFuego(objetivo->pos() + QPointF(50, 50)); // Centro del objetivo

        // Volver a idle después del ataque
        QTimer::singleShot(1000, this, [this]() {
            estado = IDLE;
            iniciarAnimacionIdle();
        });
    });
}

void PersonajeBatalla::lanzarFuego(QPointF destino)
{
    // Crear el proyectil de fuego
    QGraphicsPixmapItem* fuego = new QGraphicsPixmapItem(spriteFuego);
    QPointF posInicial = pos() + QPointF(50, 30); // Desde la mano del personaje
    fuego->setPos(posInicial);
    scene->addItem(fuego);

    // Variables para la animación manual
    const int duracionMs = 500;
    const int intervaloMs = 16; // ~60 FPS
    const int totalPasos = duracionMs / intervaloMs;

    // Calcular delta de movimiento por paso
    QPointF delta = (destino - posInicial) / totalPasos;

    // Crear contador de pasos (usar puntero para captura en lambda)
    int* pasoActual = new int(0);

    // Timer para animar el movimiento
    QTimer* timerMovimiento = new QTimer(this);

    connect(timerMovimiento, &QTimer::timeout, this, [this, fuego, delta, totalPasos, timerMovimiento, pasoActual, destino]() {
        (*pasoActual)++;

        if (*pasoActual >= totalPasos) {
            // Llegó al destino
            timerMovimiento->stop();
            timerMovimiento->deleteLater();
            fuego->setPos(destino);
            delete pasoActual; // Liberar contador

            // Iniciar explosión
            const int duracionExplosion = 200;
            const int totalPasosExplosion = duracionExplosion / 16;
            qreal escalaInicial = 1.0;
            qreal escalaFinal = 2.5;
            qreal deltaEscala = (escalaFinal - escalaInicial) / totalPasosExplosion;

            int* pasoExplosion = new int(0);
            QTimer* timerExplosion = new QTimer(this);

            connect(timerExplosion, &QTimer::timeout, this, [this, fuego, timerExplosion, pasoExplosion, totalPasosExplosion, escalaInicial, deltaEscala]() {
                (*pasoExplosion)++;

                if (*pasoExplosion >= totalPasosExplosion) {
                    timerExplosion->stop();
                    timerExplosion->deleteLater();
                    delete pasoExplosion; // Liberar contador

                    // Eliminar el fuego
                    if (fuego->scene()) {
                        fuego->scene()->removeItem(fuego);
                    }
                    delete fuego;
                } else {
                    // Escalar el fuego
                    qreal escalaActual = escalaInicial + (deltaEscala * (*pasoExplosion));
                    fuego->setScale(escalaActual);

                    // Hacer fade out
                    fuego->setOpacity(1.0 - (static_cast<qreal>(*pasoExplosion) / totalPasosExplosion));
                }
            });

            timerExplosion->start(16);

        } else {
            // Mover el fuego
            QPointF nuevaPos = fuego->pos() + delta;
            fuego->setPos(nuevaPos);
        }
    });

    timerMovimiento->start(intervaloMs);
}

void PersonajeBatalla::mostrarDerrota()
{
    detenerAnimacionIdle();
    estado = DERROTADO;
    setPixmap(spriteDerrota);

    // Animación de caída
    QPropertyAnimation* caida = new QPropertyAnimation(this, "pos");
    caida->setDuration(500);
    caida->setStartValue(pos());
    caida->setEndValue(pos() + QPointF(0, 50)); // Cae un poco
    caida->setEasingCurve(QEasingCurve::InQuad);
    caida->start(QAbstractAnimation::DeleteWhenStopped);
}
*/


#include "personajebatalla.h"
#include <QGraphicsScene>
#include <QDebug>
#include <QEasingCurve>

PersonajeBatalla::PersonajeBatalla(Bando bando, QGraphicsScene* scene, QObject* parent)
    : QObject(parent)
    , bando(bando)
    , estado(IDLE)
    , scene(scene)
    , frameActual(0)
    , timerAnimacion(nullptr)
    , animacionSalto(nullptr)
{
    cargarSprites();
    setPixmap(spritesDerecha[0]); // Frame inicial

    // Posición inicial según el bando
    if (bando == ROJO) {
        // Rojo a la izquierda
        posicionInicial = QPointF(150, 400);
    } else {
        // Blanco a la derecha
        posicionInicial = QPointF(750, 400);
    }

    setPos(posicionInicial);
    scene->addItem(this);
}

PersonajeBatalla::~PersonajeBatalla()
{
    if (timerAnimacion) {
        timerAnimacion->stop();
        delete timerAnimacion;
    }
    if (animacionSalto) {
        animacionSalto->stop();
        delete animacionSalto;
    }
}

void PersonajeBatalla::cargarSprites()
{
    if (bando == ROJO) {
        // Cargar sprites del personaje ROJO (Empirista)
        spritesDerecha = {
            QPixmap("C:/Users/Lenovo/Downloads/rojo1AD.png").scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation),
            QPixmap("C:/Users/Lenovo/Downloads/rojo2AD.png").scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation),
            QPixmap("C:/Users/Lenovo/Downloads/rojo3AD.png").scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        };

        spritesIzquierda = {
            QPixmap("C:/Users/Lenovo/Downloads/rojo1Iz.png").scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation),
            QPixmap("C:/Users/Lenovo/Downloads/rojo2Iz.png").scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation),
            QPixmap("C:/Users/Lenovo/Downloads/rojo3Iz.png").scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        };

        spriteAtaque = QPixmap("C:/Users/Lenovo/Downloads/rojoAtaque.png").scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        spriteDerrota = QPixmap("C:/Users/Lenovo/Downloads/finalRojo.png").scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        spriteFuego = QPixmap("C:/Users/Lenovo/Downloads/FuegoRojoo.png").scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    } else { // BLANCO
        // Cargar sprites del personaje BLANCO (Racionalista)
        spritesDerecha = {
            QPixmap("C:/Users/Lenovo/Downloads/blanco1AD.png").scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation),
            QPixmap("C:/Users/Lenovo/Downloads/blanco2AD.png").scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation),
            QPixmap("C:/Users/Lenovo/Downloads/blanco3AD.png").scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        };

        spritesIzquierda = {
            QPixmap("C:/Users/Lenovo/Downloads/blanco1Iz.png").scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation),
            QPixmap("C:/Users/Lenovo/Downloads/blanco2Iz.png").scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation),
            QPixmap("C:/Users/Lenovo/Downloads/blanco3Iz.png").scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        };

        spriteAtaque = QPixmap("C:/Users/Lenovo/Downloads/blancoAtaque.png").scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        spriteDerrota = QPixmap("C:/Users/Lenovo/Downloads/finalBlanco.png").scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        spriteFuego = QPixmap("C:/Users/Lenovo/Downloads/FuegoBlancoo.png").scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }
}

void PersonajeBatalla::iniciarAnimacionIdle()
{
    if (estado != IDLE) return;

    // OPCIÓN 1: Usar solo 1 frame estático (MÁS LIMPIO)
    // Establecer frame inicial según el bando
    /*if (bando == ROJO) {
        setPixmap(spritesDerecha[1]); // Frame del medio (posición neutral)
    } else {
        setPixmap(spritesIzquierda[1]); // Frame del medio (posición neutral)
    }*/

    // OPCIÓN 2: Alternar entre 2 frames (DESCOMENTA ESTO SI PREFIERES MOVIMIENTO)

    timerAnimacion = new QTimer(this);
    connect(timerAnimacion, &QTimer::timeout, this, &PersonajeBatalla::actualizarFrame);
    timerAnimacion->start(400); // Más lento para que no parezca que camina


    // Animación de salto (movimiento vertical)
    animacionSalto = new QPropertyAnimation(this, "pos");
    animacionSalto->setDuration(1000); // Un poco más lento para mejor efecto
    animacionSalto->setStartValue(posicionInicial);
    animacionSalto->setKeyValueAt(0.5, posicionInicial + QPointF(0, -30)); // Salta 30px
    animacionSalto->setEndValue(posicionInicial);
    animacionSalto->setEasingCurve(QEasingCurve::InOutQuad);
    animacionSalto->setLoopCount(-1); // Infinito
    animacionSalto->start();
}

void PersonajeBatalla::detenerAnimacionIdle()
{
    if (timerAnimacion) {
        timerAnimacion->stop();
    }
    if (animacionSalto) {
        animacionSalto->stop();
    }
}

void PersonajeBatalla::actualizarFrame()
{
    // OPCIÓN 2: Alternar solo entre frame 0 y 1 (sin el frame 2)
    frameActual = (frameActual == 0) ? 1 : 0; // Alterna entre 0 y 1

    // Usar sprites de derecha o izquierda según el bando
    if (bando == ROJO) {
        setPixmap(spritesDerecha[frameActual]); // Rojo mira a la derecha
    } else {
        setPixmap(spritesIzquierda[frameActual]); // Blanco mira a la izquierda
    }
}

void PersonajeBatalla::atacar(PersonajeBatalla* objetivo)
{
    if (!objetivo) return;

    // Detener animación idle
    detenerAnimacionIdle();
    estado = ATACANDO;

    // Cambiar a sprite de ataque
    setPixmap(spriteAtaque);

    // Esperar un momento antes de lanzar el fuego
    QTimer::singleShot(300, this, [this, objetivo]() {
        lanzarFuego(objetivo->pos() + QPointF(50, 50)); // Centro del objetivo

        // Volver a idle después del ataque
        QTimer::singleShot(1000, this, [this]() {
            estado = IDLE;
            iniciarAnimacionIdle();
        });
    });
}

void PersonajeBatalla::lanzarFuego(QPointF destino)
{
    // Crear el proyectil de fuego
    QGraphicsPixmapItem* fuego = new QGraphicsPixmapItem(spriteFuego);
    QPointF posInicial = pos() + QPointF(50, 30); // Desde la mano del personaje
    fuego->setPos(posInicial);
    scene->addItem(fuego);

    // Variables para la animación manual
    const int duracionMs = 500;
    const int intervaloMs = 16; // ~60 FPS
    const int totalPasos = duracionMs / intervaloMs;

    // Calcular delta de movimiento por paso
    QPointF delta = (destino - posInicial) / totalPasos;

    // Crear contador de pasos (usar puntero para captura en lambda)
    int* pasoActual = new int(0);

    // Timer para animar el movimiento
    QTimer* timerMovimiento = new QTimer(this);

    connect(timerMovimiento, &QTimer::timeout, this, [this, fuego, delta, totalPasos, timerMovimiento, pasoActual, destino]() {
        (*pasoActual)++;

        if (*pasoActual >= totalPasos) {
            // Llegó al destino
            timerMovimiento->stop();
            timerMovimiento->deleteLater();
            fuego->setPos(destino);
            delete pasoActual; // Liberar contador

            // Iniciar explosión
            const int duracionExplosion = 200;
            const int totalPasosExplosion = duracionExplosion / 16;
            qreal escalaInicial = 1.0;
            qreal escalaFinal = 2.5;
            qreal deltaEscala = (escalaFinal - escalaInicial) / totalPasosExplosion;

            int* pasoExplosion = new int(0);
            QTimer* timerExplosion = new QTimer(this);

            connect(timerExplosion, &QTimer::timeout, this, [this, fuego, timerExplosion, pasoExplosion, totalPasosExplosion, escalaInicial, deltaEscala]() {
                (*pasoExplosion)++;

                if (*pasoExplosion >= totalPasosExplosion) {
                    timerExplosion->stop();
                    timerExplosion->deleteLater();
                    delete pasoExplosion; // Liberar contador

                    // Eliminar el fuego
                    if (fuego->scene()) {
                        fuego->scene()->removeItem(fuego);
                    }
                    delete fuego;
                } else {
                    // Escalar el fuego
                    qreal escalaActual = escalaInicial + (deltaEscala * (*pasoExplosion));
                    fuego->setScale(escalaActual);

                    // Hacer fade out
                    fuego->setOpacity(1.0 - (static_cast<qreal>(*pasoExplosion) / totalPasosExplosion));
                }
            });

            timerExplosion->start(16);

        } else {
            // Mover el fuego
            QPointF nuevaPos = fuego->pos() + delta;
            fuego->setPos(nuevaPos);
        }
    });

    timerMovimiento->start(intervaloMs);
}

void PersonajeBatalla::mostrarDerrota()
{
    detenerAnimacionIdle();
    estado = DERROTADO;
    setPixmap(spriteDerrota);

    // Animación de caída
    QPropertyAnimation* caida = new QPropertyAnimation(this, "pos");
    caida->setDuration(500);
    caida->setStartValue(pos());
    caida->setEndValue(pos() + QPointF(0, 50)); // Cae un poco
    caida->setEasingCurve(QEasingCurve::InQuad);
    caida->start(QAbstractAnimation::DeleteWhenStopped);
}
