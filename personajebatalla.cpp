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

    // OPCIÓN 1: Usar solo 1 frame estático (MÁS LIMPIO)
    // Establecer frame inicial según el bando
   // /*if (bando == ROJO) {
     //   setPixmap(spritesDerecha[1]); // Frame del medio (posición neutral)
    //} else {
      //  setPixmap(spritesIzquierda[1]); // Frame del medio (posición neutral)
    //}

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
*/


#include "personajebatalla.h"
#include <QGraphicsScene>
#include <QDebug>
#include <QEasingCurve>
#include <QBrush>
#include <QPen>

PersonajeBatalla::PersonajeBatalla(Bando bando, QGraphicsScene* scene, QObject* parent)
    : QObject(parent)
    , bando(bando)
    , estado(IDLE)
    , scene(scene)
    , frameActual(0)
    , timerAnimacion(nullptr)
    , animacionSalto(nullptr)
    , barraDeFondo(nullptr)
    , barraDeVida(nullptr)
    , textoVidas(nullptr)
{
    cargarSprites();
    setPixmap(spritesDerecha[0]);

    // Posición inicial según el bando
    if (bando == ROJO) {
        posicionInicial = QPointF(150, 400);
    } else {
        posicionInicial = QPointF(750, 400);
    }

    setPos(posicionInicial);
    scene->addItem(this);

    // ✅ CREAR BARRA DE VIDA
    crearBarraVida();
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

    // ✅ LIMPIAR BARRA DE VIDA
    if (barraDeFondo && barraDeFondo->scene()) {
        scene->removeItem(barraDeFondo);
        delete barraDeFondo;
    }
    if (barraDeVida && barraDeVida->scene()) {
        scene->removeItem(barraDeVida);
        delete barraDeVida;
    }
    if (textoVidas && textoVidas->scene()) {
        scene->removeItem(textoVidas);
        delete textoVidas;
    }
}

void PersonajeBatalla::cargarSprites()
{
    if (bando == ROJO) {
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

// ✅ CREAR BARRA DE VIDA
void PersonajeBatalla::crearBarraVida()
{
    // Fondo de la barra (gris oscuro)
    barraDeFondo = new QGraphicsRectItem();
    barraDeFondo->setRect(0, 0, BARRA_ANCHO, BARRA_ALTO);
    barraDeFondo->setBrush(QBrush(QColor(50, 50, 50))); // Gris oscuro
    barraDeFondo->setPen(QPen(Qt::black, 2));
    scene->addItem(barraDeFondo);

    // Barra de vida (color según bando)
    barraDeVida = new QGraphicsRectItem();
    barraDeVida->setRect(0, 0, BARRA_ANCHO, BARRA_ALTO);

    if (bando == ROJO) {
        barraDeVida->setBrush(QBrush(QColor(231, 76, 60))); // Rojo
    } else {
        barraDeVida->setBrush(QBrush(QColor(236, 240, 241))); // Blanco/Gris claro
    }
    barraDeVida->setPen(Qt::NoPen);
    scene->addItem(barraDeVida);

    // Texto de vidas
    textoVidas = new QGraphicsTextItem();
    textoVidas->setFont(QFont("Arial", 10, QFont::Bold));
    textoVidas->setDefaultTextColor(Qt::white);
    scene->addItem(textoVidas);

    // Posicionar barra
    actualizarPosicionBarra();
}

// ✅ ACTUALIZAR POSICIÓN DE LA BARRA (sigue al personaje)
void PersonajeBatalla::actualizarPosicionBarra()
{
    if (!barraDeFondo || !barraDeVida || !textoVidas) return;

    QPointF posPersonaje = pos();
    qreal anchoPersonaje = pixmap().width();

    // Centrar barra sobre el personaje
    qreal barraX = posPersonaje.x() + (anchoPersonaje / 2) - (BARRA_ANCHO / 2);
    qreal barraY = posPersonaje.y() + BARRA_OFFSET_Y;

    barraDeFondo->setPos(barraX, barraY);
    barraDeVida->setPos(barraX, barraY);
    textoVidas->setPos(barraX + BARRA_ANCHO + 5, barraY - 5); // A la derecha de la barra
}

// ✅ INICIALIZAR BARRA (llamar desde nivel3batalla)
void PersonajeBatalla::inicializarBarraVida(int vidasIniciales, int vidasMaximas)
{
    actualizarBarraVida(vidasIniciales, vidasMaximas);
}

// ✅ ACTUALIZAR BARRA CUANDO PIERDE VIDA
void PersonajeBatalla::actualizarBarraVida(int vidasActuales, int vidasMaximas)
{
    if (!barraDeVida || !textoVidas) return;

    // Calcular porcentaje de vida
    float porcentaje = static_cast<float>(vidasActuales) / static_cast<float>(vidasMaximas);
    qreal nuevoAncho = BARRA_ANCHO * porcentaje;

    // Actualizar ancho de la barra
    QRectF rect = barraDeVida->rect();
    rect.setWidth(nuevoAncho);
    barraDeVida->setRect(rect);

    // Cambiar color según vida restante
    if (porcentaje > 0.6f) {
        // Verde/Color original
        if (bando == ROJO) {
            barraDeVida->setBrush(QBrush(QColor(231, 76, 60))); // Rojo
        } else {
            barraDeVida->setBrush(QBrush(QColor(236, 240, 241))); // Blanco
        }
    } else if (porcentaje > 0.3f) {
        // Naranja (advertencia)
        barraDeVida->setBrush(QBrush(QColor(243, 156, 18)));
    } else {
        // Rojo crítico
        barraDeVida->setBrush(QBrush(QColor(192, 57, 43)));
    }

    // Actualizar texto
    textoVidas->setPlainText(QString("%1/%2").arg(vidasActuales).arg(vidasMaximas));

    // Actualizar posición (por si el personaje se movió)
    actualizarPosicionBarra();
}

void PersonajeBatalla::iniciarAnimacionIdle()
{
    if (estado != IDLE) return;

    timerAnimacion = new QTimer(this);
    connect(timerAnimacion, &QTimer::timeout, this, &PersonajeBatalla::actualizarFrame);
    timerAnimacion->start(400);

    // Animación de salto
    animacionSalto = new QPropertyAnimation(this, "pos");
    animacionSalto->setDuration(1000);
    animacionSalto->setStartValue(posicionInicial);
    animacionSalto->setKeyValueAt(0.5, posicionInicial + QPointF(0, -30));
    animacionSalto->setEndValue(posicionInicial);
    animacionSalto->setEasingCurve(QEasingCurve::InOutQuad);
    animacionSalto->setLoopCount(-1);

    // ✅ ACTUALIZAR BARRA CUANDO EL PERSONAJE SALTA
    connect(animacionSalto, &QPropertyAnimation::valueChanged, this, [this]() {
        actualizarPosicionBarra();
    });

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
    frameActual = (frameActual == 0) ? 1 : 0;

    if (bando == ROJO) {
        setPixmap(spritesDerecha[frameActual]);
    } else {
        setPixmap(spritesIzquierda[frameActual]);
    }

    // ✅ ACTUALIZAR POSICIÓN DE BARRA AL CAMBIAR FRAME
    actualizarPosicionBarra();
}

void PersonajeBatalla::atacar(PersonajeBatalla* objetivo)
{
    if (!objetivo) return;

    detenerAnimacionIdle();
    estado = ATACANDO;
    setPixmap(spriteAtaque);

    // ✅ ACTUALIZAR BARRA AL CAMBIAR SPRITE
    actualizarPosicionBarra();

    QTimer::singleShot(300, this, [this, objetivo]() {
        lanzarFuego(objetivo->pos() + QPointF(50, 50));

        QTimer::singleShot(1000, this, [this]() {
            estado = IDLE;
            iniciarAnimacionIdle();
        });
    });
}

void PersonajeBatalla::lanzarFuego(QPointF destino)
{
    QGraphicsPixmapItem* fuego = new QGraphicsPixmapItem(spriteFuego);
    QPointF posInicial = pos() + QPointF(50, 30);
    fuego->setPos(posInicial);
    scene->addItem(fuego);

    const int duracionMs = 500;
    const int intervaloMs = 16;
    const int totalPasos = duracionMs / intervaloMs;
    QPointF delta = (destino - posInicial) / totalPasos;
    int* pasoActual = new int(0);

    QTimer* timerMovimiento = new QTimer(this);

    connect(timerMovimiento, &QTimer::timeout, this, [this, fuego, delta, totalPasos, timerMovimiento, pasoActual, destino]() {
        (*pasoActual)++;

        if (*pasoActual >= totalPasos) {
            timerMovimiento->stop();
            timerMovimiento->deleteLater();
            fuego->setPos(destino);
            delete pasoActual;

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
                    delete pasoExplosion;

                    if (fuego->scene()) {
                        fuego->scene()->removeItem(fuego);
                    }
                    delete fuego;
                } else {
                    qreal escalaActual = escalaInicial + (deltaEscala * (*pasoExplosion));
                    fuego->setScale(escalaActual);
                    fuego->setOpacity(1.0 - (static_cast<qreal>(*pasoExplosion) / totalPasosExplosion));
                }
            });

            timerExplosion->start(16);

        } else {
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

    // ✅ OCULTAR BARRA AL PERDER
    if (barraDeFondo) barraDeFondo->setVisible(false);
    if (barraDeVida) barraDeVida->setVisible(false);
    if (textoVidas) textoVidas->setVisible(false);

    QPropertyAnimation* caida = new QPropertyAnimation(this, "pos");
    caida->setDuration(500);
    caida->setStartValue(pos());
    caida->setEndValue(pos() + QPointF(0, 50));
    caida->setEasingCurve(QEasingCurve::InQuad);
    caida->start(QAbstractAnimation::DeleteWhenStopped);
}

