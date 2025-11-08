#include "npccasa.h"
#include <QGraphicsScene>
#include <QFont>
#include <QBrush>
#include <QPen>

npcCasa::npcCasa(const QPixmap &sprite, QObject *parent)
    : QObject(parent)
    , QGraphicsPixmapItem()
    , desplazamiento(0)
    , moviendoArriba(true)
    , velocidadFlotacion(0.5)
    , rangoFlotacion(15.0)
    , indiceDialogoActual(0)
    , usandoDialogosInicio(true)
    , cuadroDialogo(nullptr)
    , textoDialogo(nullptr)
{
    setPixmap(sprite);
    posicionInicialY = 0;

    // Timer para animación de flotación
    timerAnimacion = new QTimer(this);
    connect(timerAnimacion, &QTimer::timeout, this, &npcCasa::animarFlotacion);

    // Timer para cambiar diálogos automáticamente
    timerDialogo = new QTimer(this);
    connect(timerDialogo, &QTimer::timeout, this, &npcCasa::cambiarTextoDialogo);

    crearCuadroDialogo();
}

npcCasa::~npcCasa()
{
    if (timerAnimacion) {
        timerAnimacion->stop();
        delete timerAnimacion;
    }

    if (timerDialogo) {
        timerDialogo->stop();
        delete timerDialogo;
    }
}

void npcCasa::crearCuadroDialogo()
{
    // Crear rectángulo para el diálogo
    cuadroDialogo = new QGraphicsRectItem(0, 0, 300, 80);
    cuadroDialogo->setBrush(QBrush(QColor(255, 255, 255, 230)));
    cuadroDialogo->setPen(QPen(QColor(50, 50, 50), 3));
    cuadroDialogo->setZValue(2000);
    cuadroDialogo->setVisible(false);

    // Crear texto del diálogo
    textoDialogo = new QGraphicsTextItem();
    QFont font("Arial", 11);
    textoDialogo->setFont(font);
    textoDialogo->setDefaultTextColor(QColor(30, 30, 30));
    textoDialogo->setTextWidth(280);
    textoDialogo->setZValue(2001);
    textoDialogo->setVisible(false);
}

void npcCasa::iniciarAnimacion()
{
    posicionInicialY = y();
    timerAnimacion->start(30);
}

void npcCasa::detenerAnimacion()
{
    timerAnimacion->stop();
}

void npcCasa::animarFlotacion()
{
    if (moviendoArriba) {
        desplazamiento += velocidadFlotacion;
        if (desplazamiento >= rangoFlotacion) {
            moviendoArriba = false;
        }
    } else {
        desplazamiento -= velocidadFlotacion;
        if (desplazamiento <= 0) {
            moviendoArriba = true;
        }
    }

    setY(posicionInicialY - desplazamiento);
    actualizarPosicionDialogo();
}

void npcCasa::establecerDialogos(const QVector<QString> &dialogosInicio,
                                 const QVector<QString> &dialogosFinal)
{
    this->dialogosInicio = dialogosInicio;
    this->dialogosFinal = dialogosFinal;
    indiceDialogoActual = 0;
    usandoDialogosInicio = true;
}

QString npcCasa::obtenerDialogoActual(bool nivelCompletado)
{
    // Cambiar conjunto de diálogos si el nivel se completó
    if (nivelCompletado && usandoDialogosInicio) {
        usandoDialogosInicio = false;
        indiceDialogoActual = 0;
    }

    if (usandoDialogosInicio) {
        if (dialogosInicio.isEmpty()) return "Hola, aventurero.";
        return dialogosInicio[indiceDialogoActual];
    } else {
        if (dialogosFinal.isEmpty()) return "¡Felicidades!";
        return dialogosFinal[indiceDialogoActual];
    }
}

void npcCasa::cambiarTextoDialogo()
{
    // Avanzar al siguiente diálogo
    QVector<QString>& dialogosActuales = usandoDialogosInicio ? dialogosInicio : dialogosFinal;

    if (!dialogosActuales.isEmpty()) {
        indiceDialogoActual = (indiceDialogoActual + 1) % dialogosActuales.size();
        textoDialogo->setPlainText(dialogosActuales[indiceDialogoActual]);
    }
}

void npcCasa::mostrarDialogo(const QString &texto)
{
    if (!scene()) return;

    // Agregar elementos a la escena si no están
    if (cuadroDialogo->scene() == nullptr) {
        scene()->addItem(cuadroDialogo);
    }
    if (textoDialogo->scene() == nullptr) {
        scene()->addItem(textoDialogo);
    }

    // Configurar texto
    textoDialogo->setPlainText(texto);

    // Actualizar posición
    actualizarPosicionDialogo();

    // Mostrar
    cuadroDialogo->setVisible(true);
    textoDialogo->setVisible(true);

    // Iniciar rotación de diálogos cada 4 segundos
    timerDialogo->start(4000);
}

void npcCasa::ocultarDialogo()
{
    if (cuadroDialogo) cuadroDialogo->setVisible(false);
    if (textoDialogo) textoDialogo->setVisible(false);
    timerDialogo->stop();
    indiceDialogoActual = 0; // Reiniciar para la próxima vez
}

void npcCasa::actualizarPosicionDialogo()
{
    if (!cuadroDialogo || !textoDialogo) return;

    // Posicionar el cuadro encima del NPC
    qreal cuadroX = x() + (boundingRect().width() - 300) / 2;
    qreal cuadroY = y() - 90; // 90 píxeles arriba del NPC

    cuadroDialogo->setPos(cuadroX, cuadroY);
    textoDialogo->setPos(cuadroX + 10, cuadroY + 10);
}
