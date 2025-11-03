/*#include "juego2.h"
#include "ui_juego2.h"
#include "mensajewidget.h"
#include <QPropertyAnimation>
#include <QTimer>
#include <QRandomGenerator>
#include <QVariantAnimation>
#include <QPen>
#include <QPainterPath>
#include <QGraphicsOpacityEffect>

juego2::juego2(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::juego2)
    , escenario(new Escenario(this))
    , preguntaActual(0)
    , puntosObtenidos(0)
    , preguntaRespondida(false)
    , textoPregunta(nullptr)
    , fondoPregunta(nullptr)
    , textoPuntuacion(nullptr)
{
    ui->setupUi(this);
    setFocusPolicy(Qt::StrongFocus);
}

juego2::~juego2()
{
    delete ui;
}

void juego2::inicializarNivel()
{
    // Configurar el escenario dentro del QGraphicsView del UI
    QPixmap pixFondo("C:/Users/Lenovo/Downloads/fondo.png");
    escenario->inicializar(ui->graphicsViewJ2, pixFondo);
    ui->graphicsViewJ2->setFocusPolicy(Qt::NoFocus);

    // Crear el personaje
    QVector<QPixmap> spritesDer = {
        QPixmap("C:/Users/Lenovo/Downloads/hada1AD.png").scaled(100,100, Qt::KeepAspectRatio, Qt::SmoothTransformation),
        QPixmap("C:/Users/Lenovo/Pictures/hada2AD.png").scaled(100,100, Qt::KeepAspectRatio, Qt::SmoothTransformation),
        QPixmap("C:/Users/Lenovo/Downloads/hada3AD.png").scaled(100,100, Qt::KeepAspectRatio, Qt::SmoothTransformation)
    };

    QVector<QPixmap> spritesIzq = {
        QPixmap("C:/Users/Lenovo/Pictures/hada1Iz.png").scaled(100,100, Qt::KeepAspectRatio, Qt::SmoothTransformation),
        QPixmap("C:/Users/Lenovo/Pictures/hada2Iz.png").scaled(100,100, Qt::KeepAspectRatio, Qt::SmoothTransformation),
        QPixmap("C:/Users/Lenovo/Pictures/hada3Iz.png").scaled(100,100, Qt::KeepAspectRatio, Qt::SmoothTransformation)
    };

    QVector<QPixmap> spritesArriba = {
        QPixmap("C:/Users/Lenovo/Downloads/hada4Ar.png").scaled(100,100, Qt::KeepAspectRatio, Qt::SmoothTransformation),
        QPixmap("C:/Users/Lenovo/Downloads/hada5Ar.png").scaled(100,100, Qt::KeepAspectRatio, Qt::SmoothTransformation)
    };

    // Crear objetos interactivos (burbujas)
    objetosInteractivos();

    // Crear personaje
    escenario->crearPersonaje(spritesDer, spritesIzq, spritesArriba, QPointF(350, 450));

    // ⭐ Inicializar sistema de preguntas
    inicializarPreguntas();
    mostrarPuntuacion();
    cargarPregunta();

    // Seleccionar la primera burbuja automáticamente
    if (!burbujas.isEmpty()) {
        seleccionarSiguienteBurbuja();
    }

    this->setFocus();
}

void juego2::keyPressEvent(QKeyEvent *event)
{
    // Tecla TAB para cambiar de burbuja
    if (event->key() == Qt::Key_Tab) {
        seleccionarSiguienteBurbuja();
        return;
    }

    // Tecla E para explotar la burbuja seleccionada
    if (event->key() == Qt::Key_E) {
        explotarBurbuja();
        return;
    }

    // Manejar movimiento del personaje
    if(escenario) {
        escenario->manejarTecla(event);

        // Actualizar línea punteada después de mover el personaje
        if (burbujaSeleccionada) {
            dibujarLineaPunteada();
        }
    }

    // Tecla ESC para volver a la ruleta
    if (event->key() == Qt::Key_Escape) {
        emit volverARuleta();
    }
}

void juego2::objetosInteractivos()
{
    QString ruta = "C:/Users/Lenovo/Downloads/burbuja.png";
    QList<QPointF> posiciones = {
        {150, 90}, {350, 150}, {550, 70}, {750, 180}
    };

    for (const QPointF &pos : posiciones) {
        QGraphicsPixmapItem* burbuja = escenario->scene->addPixmap(
            QPixmap(ruta).scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation)
            );
        burbuja->setPos(pos);
        burbujas.append(burbuja);
        animarBurbuja(burbuja, 20);
    }
}

void juego2::animarBurbuja(QGraphicsPixmapItem* burbuja, int desplazamiento)
{
    if (!burbuja) return;

    QVariantAnimation* anim = new QVariantAnimation(this);
    anim->setDuration(2000 + QRandomGenerator::global()->bounded(2000));
    anim->setStartValue(-desplazamiento);
    anim->setEndValue(desplazamiento);
    anim->setEasingCurve(QEasingCurve::SineCurve);
    anim->setLoopCount(-1);

    QPointF posInicial = burbuja->pos();

    // ⭐ Guardamos la animación como propiedad del objeto para poder detenerla/reactivarla
    burbuja->setData(0, QVariant::fromValue((void*)anim));

    connect(anim, &QVariantAnimation::valueChanged, [this, burbuja, posInicial](const QVariant &value){
        if (!burbuja->isVisible()) return; // No animar si está oculta

        qreal yOffset = value.toReal();
        burbuja->setPos(posInicial.x(), posInicial.y() + yOffset);

        if (burbuja == burbujaSeleccionada) {
            dibujarLineaPunteada();

            if (selectorBurbuja) {
                QPointF centro = burbuja->pos() + QPointF(40, 40);
                selectorBurbuja->setPos(centro.x() - 30, centro.y() - 30);
            }
        }
    });

    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void juego2::seleccionarSiguienteBurbuja()
{
    if (burbujas.isEmpty()) return;

    burbujaIndex = (burbujaIndex + 1) % burbujas.size();
    burbujaSeleccionada = burbujas[burbujaIndex];

    if (!selectorBurbuja) {
        selectorBurbuja = escenario->scene->addEllipse(0, 0, 60, 60);
        QPen pen;
        pen.setColor(QColor(200, 150, 255, 180));
        pen.setStyle(Qt::DashDotLine);
        pen.setWidth(4);
        selectorBurbuja->setPen(pen);
        selectorBurbuja->setBrush(QBrush(QColor(255, 255, 255, 30)));
        selectorBurbuja->setZValue(2);
    }

    QPointF centroBurbuja = burbujaSeleccionada->pos() + QPointF(40, 40);
    selectorBurbuja->setPos(centroBurbuja.x() - 50, centroBurbuja.y() - 50);
    selectorBurbuja->setVisible(true);

    dibujarLineaPunteada();
}

void juego2::dibujarLineaPunteada()
{
    if (!burbujaSeleccionada || !escenario->personaje) return;

    if (lineaPunteada) {
        escenario->scene->removeItem(lineaPunteada);
        delete lineaPunteada;
        lineaPunteada = nullptr;
    }

    QPointF posPersonaje = escenario->personaje->pos() + QPointF(50, 50);
    QPointF posBurbuja = burbujaSeleccionada->pos() + QPointF(40, 40);

    QPainterPath path;
    path.moveTo(posPersonaje);
    path.lineTo(posBurbuja);

    lineaPunteada = escenario->scene->addPath(path);

    QPen pen;
    pen.setColor(QColor(200, 150, 255, 180));
    pen.setStyle(Qt::DashDotLine);
    pen.setWidth(3);
    pen.setCapStyle(Qt::RoundCap);

    lineaPunteada->setPen(pen);
    lineaPunteada->setZValue(1.5);
}

void juego2::explotarBurbuja()
{
    if (!burbujaSeleccionada || preguntaRespondida) return;

    // ⭐ Verificar respuesta ANTES de explotar
    verificarRespuesta(burbujaIndex);

    // Crear y lanzar el proyectil
    QPixmap pixProyectil("C:/Users/Lenovo/Downloads/proyectil.png");
    QGraphicsPixmapItem* proyectil = escenario->scene->addPixmap(
        pixProyectil.scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );

    QPointF posPersonaje = escenario->personaje->pos() + QPointF(50, 50);
    proyectil->setPos(posPersonaje);
    proyectil->setZValue(3);

    QPointF posBurbuja = burbujaSeleccionada->pos() + QPointF(40, 40);

    QVariantAnimation* animProyectil = new QVariantAnimation(this);
    animProyectil->setDuration(500);
    animProyectil->setStartValue(posPersonaje);
    animProyectil->setEndValue(posBurbuja);
    animProyectil->setEasingCurve(QEasingCurve::Linear);

    connect(animProyectil, &QVariantAnimation::valueChanged, [proyectil](const QVariant &value){
        proyectil->setPos(value.toPointF());
    });

    connect(animProyectil, &QVariantAnimation::finished, [this, proyectil](){
        escenario->scene->removeItem(proyectil);
        delete proyectil;
        realizarExplosion();
    });

    animProyectil->start(QAbstractAnimation::DeleteWhenStopped);
}

void juego2::realizarExplosion()
{
    if (!burbujaSeleccionada) return;

    QVariantAnimation* animExplosion = new QVariantAnimation(this);
    animExplosion->setDuration(300);
    animExplosion->setStartValue(0.0);
    animExplosion->setEndValue(1.0);
    animExplosion->setEasingCurve(QEasingCurve::OutQuad);

    qreal escalaInicial = 1.0;
    qreal opacidadInicial = 1.0;

    connect(animExplosion, &QVariantAnimation::valueChanged, [this, escalaInicial, opacidadInicial](const QVariant &value){
        if (!burbujaSeleccionada) return;

        qreal progress = value.toDouble();
        qreal nuevaEscala = escalaInicial + (0.5 * progress);
        burbujaSeleccionada->setScale(nuevaEscala);

        qreal nuevaOpacidad = opacidadInicial - (opacidadInicial * progress);
        burbujaSeleccionada->setOpacity(nuevaOpacidad);
    });

    connect(animExplosion, &QVariantAnimation::finished, [this](){
        if (burbujaSeleccionada) {
            // ⭐ NO eliminamos la burbuja, solo la ocultamos temporalmente
            burbujaSeleccionada->setVisible(false);
            burbujaSeleccionada = nullptr;

            if (selectorBurbuja) {
                selectorBurbuja->setVisible(false);
            }
            if (lineaPunteada) {
                escenario->scene->removeItem(lineaPunteada);
                delete lineaPunteada;
                lineaPunteada = nullptr;
            }

            // ⭐ Después de explotar, avanzar a la siguiente pregunta
            QTimer::singleShot(500, this, &juego2::avanzarSiguientePregunta);
        }
    });

    animExplosion->start(QAbstractAnimation::DeleteWhenStopped);
}

// ⭐⭐⭐ NUEVOS MÉTODOS PARA SISTEMA DE PREGUNTAS ⭐⭐⭐

void juego2::inicializarPreguntas()
{
    preguntas = {
        PreguntaBurbuja(
            "Después del feudalismo medieval acudimos al surgimiento de una nueva clase social conocida como la:",
            {"La monarquía", "El mercantilismo", "La burguesía", "El proletariado"},
            2
            ),
        PreguntaBurbuja(
            "El renacimiento supone una época de absolutismo y nacionalismos, como el nacimiento de fuertes\nmonarquías europeas centralizadas como:",
            {"Grecia", "Inglaterra", "Yugoslavia", "Egipto"},
            1
            ),
        PreguntaBurbuja(
            "Antes de la consolidación del estado moderno, Italia estuvo divida en pequeñas ciudades-estado\nnormalmente enfrentadas entre si, como es el caso de:",
            {" Florencia-\nNapoli", "Ámsterdam-\nCracovia", "Reims-\nColonia", "Milán-\nLourdes"},
            0
            ),
        PreguntaBurbuja(
            "La toma de Constantinopla supone un bloqueo comercial entre Europa y Asia (la ruta de la seda) y\nocurrióen lo que hoy es actualmente:",
            {"Eslovaquia", "Estambul en\nTurquía", "Mesopotamia", "Jerusalén"},
            1
            ),
        PreguntaBurbuja(
            "Resurge el interés por Grecia y Roma, junto al declive del sistema feudal, el crecimiento\ndel comercio e innovaciones entre las que mencionamos:",
            {"La imprenta y\nla brújula", "La rueda y\nla escritura", "Las máquinas de vapor y\nla producción en masa", " La pólvora y\nLa rueda "},
            0
            )
    };

    preguntaActual = 0;
    puntosObtenidos = 0;
}

void juego2::cargarPregunta()
{
    if (preguntaActual >= preguntas.size()) {
        finalizarJuego();
        return;
    }

    preguntaRespondida = false;

    // Limpiar textos de respuestas anteriores
    for (QGraphicsTextItem* texto : textosRespuestas) {
        escenario->scene->removeItem(texto);
        delete texto;
    }
    textosRespuestas.clear();

    // Mostrar pregunta en la escena
    if (!textoPregunta) {
        // Crear fondo para la pregunta
        fondoPregunta = escenario->scene->addRect(0, 0, 800, 50);
        fondoPregunta->setBrush(QBrush(QColor(255, 193, 227, 200))); // #ffc1e3
        fondoPregunta->setPen(QPen(QColor(44, 14, 74), 2)); // #2c0e4a
        fondoPregunta->setPos(50, 10);
        fondoPregunta->setZValue(5);

        textoPregunta = escenario->scene->addText("");
        QFont font("Arial", 12, QFont::Bold);
        textoPregunta->setFont(font);
        textoPregunta->setDefaultTextColor(QColor(44, 14, 74)); // #2c0e4a
        textoPregunta->setZValue(6);
    }

    PreguntaBurbuja& pregunta = preguntas[preguntaActual];
    textoPregunta->setPlainText(pregunta.textoPregunta);
    textoPregunta->setPos(60, 20);

    // Mostrar respuestas en las burbujas
    for (int i = 0; i < burbujas.size() && i < pregunta.opciones.size(); i++) {
        QGraphicsTextItem* textoResp = escenario->scene->addText(pregunta.opciones[i]);
        QFont fontResp("Arial", 10, QFont::Bold);
        textoResp->setFont(fontResp);
        textoResp->setDefaultTextColor(QColor(44, 14, 74));
        textoResp->setZValue(3);

        // Centrar texto en la burbuja
        QPointF posBurbuja = burbujas[i]->pos();
        qreal anchoTexto = textoResp->boundingRect().width();
        qreal altoTexto = textoResp->boundingRect().height();
        textoResp->setPos(
            posBurbuja.x() + 40 - anchoTexto / 2,
            posBurbuja.y() + 40 - altoTexto / 2
            );

        textosRespuestas.append(textoResp);
    }
}

void juego2::verificarRespuesta(int indiceBurbuja)
{
    if (preguntaRespondida || preguntaActual >= preguntas.size()) return;

    preguntaRespondida = true;

    PreguntaBurbuja& pregunta = preguntas[preguntaActual];

    if (indiceBurbuja == pregunta.indiceCorrecta) {
        // ✅ Respuesta correcta
        puntosObtenidos++;

        MensajeWidget* mensaje = new MensajeWidget(
            "¡Correcto! +1 punto",
            "C:/Users/Lenovo/Downloads/restanguloPreg.png",
            this
            );
        mensaje->move((width() - mensaje->width()) / 2, (height() - mensaje->height()) / 2);
        mensaje->show();
    } else {
        // ❌ Respuesta incorrecta
        MensajeWidget* mensaje = new MensajeWidget(
            "Respuesta incorrecta.\nLa correcta era: " + pregunta.opciones[pregunta.indiceCorrecta],
            "C:/Users/Lenovo/Downloads/restanguloPreg.png",
            this
            );
        mensaje->move((width() - mensaje->width()) / 2, (height() - mensaje->height()) / 2);
        mensaje->show();
    }

    mostrarPuntuacion();
}

void juego2::avanzarSiguientePregunta()
{
    preguntaActual++;

    if (preguntaActual >= preguntas.size()) {
        finalizarJuego();
        return;
    }

    // Limpiar textos de respuestas
    for (QGraphicsTextItem* texto : textosRespuestas) {
        escenario->scene->removeItem(texto);
        delete texto;
    }
    textosRespuestas.clear();

    // ⭐ Restaurar TODAS las burbujas para la nueva pregunta
    for (QGraphicsPixmapItem* burbuja : burbujas) {
        burbuja->setVisible(true);
        burbuja->setOpacity(1.0);
        burbuja->setScale(1.0);
        burbuja->setZValue(1); // Asegurar que estén visibles
    }

    burbujaIndex = -1; // Resetear índice

    // Cargar nueva pregunta
    cargarPregunta();

    // Seleccionar primera burbuja
    if (!burbujas.isEmpty()) {
        seleccionarSiguienteBurbuja();
    }
}

void juego2::finalizarJuego()
{
    QString mensaje;
    if (puntosObtenidos == preguntas.size()) {
        mensaje = QString("¡PERFECTO! %1/%2 puntos\n\n¡Has ganado el minijuego!")
                      .arg(puntosObtenidos).arg(preguntas.size());
    } else {
        mensaje = QString("Juego terminado\n\nPuntuación final: %1/%2")
                      .arg(puntosObtenidos).arg(preguntas.size());
    }

    MensajeWidget* mensajeFinal = new MensajeWidget(
        mensaje,
        "C:/Users/Lenovo/Downloads/restanguloPreg.png",
        this
        );
    mensajeFinal->move((width() - mensajeFinal->width()) / 2, (height() - mensajeFinal->height()) / 2);
    mensajeFinal->show();

    // Volver a la ruleta después de 3 segundos
    QTimer::singleShot(3000, this, &juego2::volverARuleta);
}

void juego2::mostrarPuntuacion()
{
    if (!textoPuntuacion) {
        textoPuntuacion = escenario->scene->addText("");
        QFont font("Arial", 14, QFont::Bold);
        textoPuntuacion->setFont(font);
        textoPuntuacion->setDefaultTextColor(QColor(255, 215, 0)); // Dorado
        textoPuntuacion->setZValue(10);
    }

    QString puntos = QString("Puntos: %1/%2").arg(puntosObtenidos).arg(preguntas.size());
    textoPuntuacion->setPlainText(puntos);
    textoPuntuacion->setPos(700, 550);
} //ESTE ES EL CORRECTO SOLO QUIERO QUE SE ANIMEN LAS PREGUNTAS CON LA BURBUJA
*/


#include "juego2.h"
#include "ui_juego2.h"
#include "mensajewidget.h"
#include <QPropertyAnimation>
#include <QTimer>
#include <QRandomGenerator>
#include <QVariantAnimation>
#include <QPen>
#include <QPainterPath>
#include <QGraphicsOpacityEffect>

juego2::juego2(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::juego2)
    , escenario(new Escenario(this))
    , preguntaActual(0)
    , puntosObtenidos(0)
    , preguntaRespondida(false)
    , textoPregunta(nullptr)
    , fondoPregunta(nullptr)
    , textoPuntuacion(nullptr)
{
    ui->setupUi(this);
    setFocusPolicy(Qt::StrongFocus);
}

juego2::~juego2()
{
    delete ui;
}

void juego2::inicializarNivel()
{
    // Configurar el escenario dentro del QGraphicsView del UI
    QPixmap pixFondo("C:/Users/Lenovo/Downloads/fondo.png");
    escenario->inicializar(ui->graphicsViewJ2, pixFondo);
    ui->graphicsViewJ2->setFocusPolicy(Qt::NoFocus);

    // Crear el personaje
    QVector<QPixmap> spritesDer = {
        QPixmap("C:/Users/Lenovo/Downloads/hada1AD.png").scaled(100,100, Qt::KeepAspectRatio, Qt::SmoothTransformation),
        QPixmap("C:/Users/Lenovo/Pictures/hada2AD.png").scaled(100,100, Qt::KeepAspectRatio, Qt::SmoothTransformation),
        QPixmap("C:/Users/Lenovo/Downloads/hada3AD.png").scaled(100,100, Qt::KeepAspectRatio, Qt::SmoothTransformation)
    };

    QVector<QPixmap> spritesIzq = {
        QPixmap("C:/Users/Lenovo/Pictures/hada1Iz.png").scaled(100,100, Qt::KeepAspectRatio, Qt::SmoothTransformation),
        QPixmap("C:/Users/Lenovo/Pictures/hada2Iz.png").scaled(100,100, Qt::KeepAspectRatio, Qt::SmoothTransformation),
        QPixmap("C:/Users/Lenovo/Pictures/hada3Iz.png").scaled(100,100, Qt::KeepAspectRatio, Qt::SmoothTransformation)
    };

    QVector<QPixmap> spritesArriba = {
        QPixmap("C:/Users/Lenovo/Downloads/hada4Ar.png").scaled(100,100, Qt::KeepAspectRatio, Qt::SmoothTransformation),
        QPixmap("C:/Users/Lenovo/Downloads/hada5Ar.png").scaled(100,100, Qt::KeepAspectRatio, Qt::SmoothTransformation)
    };

    // Crear objetos interactivos (burbujas)
    objetosInteractivos();

    // Crear personaje
    escenario->crearPersonaje(spritesDer, spritesIzq, spritesArriba, QPointF(350, 450));

    // ⭐ Inicializar sistema de preguntas
    inicializarPreguntas();
    mostrarPuntuacion();
    cargarPregunta();

    // Seleccionar la primera burbuja automáticamente
    if (!burbujas.isEmpty()) {
        seleccionarSiguienteBurbuja();
    }

    this->setFocus();
}

void juego2::keyPressEvent(QKeyEvent *event)
{
    // Tecla TAB para cambiar de burbuja
    if (event->key() == Qt::Key_Tab) {
        seleccionarSiguienteBurbuja();
        return;
    }

    // Tecla E para explotar la burbuja seleccionada
    if (event->key() == Qt::Key_E) {
        explotarBurbuja();
        return;
    }

    // Manejar movimiento del personaje
    if(escenario) {
        escenario->manejarTecla(event);

        // Actualizar línea punteada después de mover el personaje
        if (burbujaSeleccionada) {
            dibujarLineaPunteada();
        }
    }

    // Tecla ESC para volver a la ruleta
    if (event->key() == Qt::Key_Escape) {
        emit volverARuleta();
    }
}

void juego2::objetosInteractivos()
{
    QString ruta = "C:/Users/Lenovo/Downloads/burbuja.png";
    QList<QPointF> posiciones = {
        {150, 90}, {350, 150}, {550, 70}, {750, 180}
    };

    // ⭐ Guardar las posiciones originales
    posicionesOriginalesBurbujas = posiciones;

    for (const QPointF &pos : posiciones) {
        QGraphicsPixmapItem* burbuja = escenario->scene->addPixmap(
            QPixmap(ruta).scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation)
            );
        burbuja->setPos(pos);
        burbujas.append(burbuja);
        animarBurbuja(burbuja, 20);
    }
}

void juego2::animarBurbuja(QGraphicsPixmapItem* burbuja, int desplazamiento)
{
    if (!burbuja) return;

    // Buscar la posición original de esta burbuja
    int index = burbujas.indexOf(burbuja);
    if (index < 0 || index >= posicionesOriginalesBurbujas.size()) return;

    QPointF posInicial = posicionesOriginalesBurbujas[index];

    QVariantAnimation* anim = new QVariantAnimation(this);
    anim->setDuration(2000 + QRandomGenerator::global()->bounded(2000));
    anim->setStartValue(-desplazamiento);
    anim->setEndValue(desplazamiento);
    anim->setEasingCurve(QEasingCurve::SineCurve);
    anim->setLoopCount(-1);

    connect(anim, &QVariantAnimation::valueChanged, [this, burbuja, posInicial, index](const QVariant &value){
        if (!burbuja || !burbuja->isVisible()) return; // No animar si está oculta

        qreal yOffset = value.toReal();
        burbuja->setPos(posInicial.x(), posInicial.y() + yOffset);

        // ⭐ Mover el texto de respuesta junto con la burbuja
        if (index < textosRespuestas.size() && textosRespuestas[index]) {
            QGraphicsTextItem* textoResp = textosRespuestas[index];
            QPointF posBurbuja = burbuja->pos();
            qreal anchoTexto = textoResp->boundingRect().width();
            qreal altoTexto = textoResp->boundingRect().height();
            textoResp->setPos(
                posBurbuja.x() + 40 - anchoTexto / 2,
                posBurbuja.y() + 40 - altoTexto / 2
                );
        }

        if (burbuja == burbujaSeleccionada) {
            dibujarLineaPunteada();

            if (selectorBurbuja) {
                QPointF centro = burbuja->pos() + QPointF(40, 40);
                selectorBurbuja->setPos(centro.x() - 30, centro.y() - 30);
            }
        }
    });

    anim->start();
}

void juego2::seleccionarSiguienteBurbuja()
{
    if (burbujas.isEmpty()) return;

    burbujaIndex = (burbujaIndex + 1) % burbujas.size();
    burbujaSeleccionada = burbujas[burbujaIndex];

    if (!selectorBurbuja) {
        selectorBurbuja = escenario->scene->addEllipse(0, 0, 60, 60);
        QPen pen;
        pen.setColor(QColor(200, 150, 255, 180));
        pen.setStyle(Qt::DashDotLine);
        pen.setWidth(4);
        selectorBurbuja->setPen(pen);
        selectorBurbuja->setBrush(QBrush(QColor(255, 255, 255, 30)));
        selectorBurbuja->setZValue(2);
    }

    QPointF centroBurbuja = burbujaSeleccionada->pos() + QPointF(40, 40);
    selectorBurbuja->setPos(centroBurbuja.x() - 50, centroBurbuja.y() - 50);
    selectorBurbuja->setVisible(true);

    dibujarLineaPunteada();
}

void juego2::dibujarLineaPunteada()
{
    if (!burbujaSeleccionada || !escenario->personaje) return;

    if (lineaPunteada) {
        escenario->scene->removeItem(lineaPunteada);
        delete lineaPunteada;
        lineaPunteada = nullptr;
    }

    QPointF posPersonaje = escenario->personaje->pos() + QPointF(50, 50);
    QPointF posBurbuja = burbujaSeleccionada->pos() + QPointF(40, 40);

    QPainterPath path;
    path.moveTo(posPersonaje);
    path.lineTo(posBurbuja);

    lineaPunteada = escenario->scene->addPath(path);

    QPen pen;
    pen.setColor(QColor(200, 150, 255, 180));
    pen.setStyle(Qt::DashDotLine);
    pen.setWidth(3);
    pen.setCapStyle(Qt::RoundCap);

    lineaPunteada->setPen(pen);
    lineaPunteada->setZValue(1.5);
}

void juego2::explotarBurbuja()
{
    if (!burbujaSeleccionada || preguntaRespondida) return;

    // ⭐ Verificar respuesta ANTES de explotar
    verificarRespuesta(burbujaIndex);

    // Crear y lanzar el proyectil
    QPixmap pixProyectil("C:/Users/Lenovo/Downloads/proyectil.png");
    QGraphicsPixmapItem* proyectil = escenario->scene->addPixmap(
        pixProyectil.scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );

    QPointF posPersonaje = escenario->personaje->pos() + QPointF(50, 50);
    proyectil->setPos(posPersonaje);
    proyectil->setZValue(3);

    QPointF posBurbuja = burbujaSeleccionada->pos() + QPointF(40, 40);

    QVariantAnimation* animProyectil = new QVariantAnimation(this);
    animProyectil->setDuration(500);
    animProyectil->setStartValue(posPersonaje);
    animProyectil->setEndValue(posBurbuja);
    animProyectil->setEasingCurve(QEasingCurve::Linear);

    connect(animProyectil, &QVariantAnimation::valueChanged, [proyectil](const QVariant &value){
        proyectil->setPos(value.toPointF());
    });

    connect(animProyectil, &QVariantAnimation::finished, [this, proyectil](){
        escenario->scene->removeItem(proyectil);
        delete proyectil;
        realizarExplosion();
    });

    animProyectil->start(QAbstractAnimation::DeleteWhenStopped);
}

void juego2::realizarExplosion()
{
    if (!burbujaSeleccionada) return;

    QVariantAnimation* animExplosion = new QVariantAnimation(this);
    animExplosion->setDuration(300);
    animExplosion->setStartValue(0.0);
    animExplosion->setEndValue(1.0);
    animExplosion->setEasingCurve(QEasingCurve::OutQuad);

    qreal escalaInicial = 1.0;
    qreal opacidadInicial = 1.0;

    connect(animExplosion, &QVariantAnimation::valueChanged, [this, escalaInicial, opacidadInicial](const QVariant &value){
        if (!burbujaSeleccionada) return;

        qreal progress = value.toDouble();
        qreal nuevaEscala = escalaInicial + (0.5 * progress);
        burbujaSeleccionada->setScale(nuevaEscala);

        qreal nuevaOpacidad = opacidadInicial - (opacidadInicial * progress);
        burbujaSeleccionada->setOpacity(nuevaOpacidad);
    });

    connect(animExplosion, &QVariantAnimation::finished, [this](){
        if (burbujaSeleccionada) {
            // ⭐ NO eliminamos la burbuja, solo la ocultamos temporalmente
            burbujaSeleccionada->setVisible(false);
            burbujaSeleccionada = nullptr;

            if (selectorBurbuja) {
                selectorBurbuja->setVisible(false);
            }
            if (lineaPunteada) {
                escenario->scene->removeItem(lineaPunteada);
                delete lineaPunteada;
                lineaPunteada = nullptr;
            }

            // ⭐ Después de explotar, avanzar a la siguiente pregunta
            QTimer::singleShot(500, this, &juego2::avanzarSiguientePregunta);
        }
    });

    animExplosion->start(QAbstractAnimation::DeleteWhenStopped);
}

// ⭐⭐⭐ NUEVOS MÉTODOS PARA SISTEMA DE PREGUNTAS ⭐⭐⭐

void juego2::inicializarPreguntas()
{
    preguntas = {
        PreguntaBurbuja(
            "Después del feudalismo medieval acudimos al surgimiento de una nueva clase social conocida como la:",
            {"La monarquía", "El mercantilismo", "La burguesía", "El proletariado"},
            2
            ),
        PreguntaBurbuja(
            "El renacimiento supone una época de absolutismo y nacionalismos, como el nacimiento de fuertes\nmonarquías europeas centralizadas como:",
            {"Grecia", "Inglaterra", "Yugoslavia", "Egipto"},
            1
            ),
        PreguntaBurbuja(
            "Antes de la consolidación del estado moderno, Italia estuvo divida en pequeñas ciudades-estado\nnormalmente enfrentadas entre si, como es el caso de:",
            {" Florencia-\nNapoli", "Ámsterdam-\nCracovia", "Reims-\nColonia", "Milán-\nLourdes"},
            0
            ),
        PreguntaBurbuja(
            "La toma de Constantinopla supone un bloqueo comercial entre Europa y Asia (la ruta de la seda) y\nocurrióen lo que hoy es actualmente:",
            {"Eslovaquia", "Estambul en\nTurquía", "Mesopotamia", "Jerusalén"},
            1
            ),
        PreguntaBurbuja(
            "Resurge el interés por Grecia y Roma, junto al declive del sistema feudal, el crecimiento\ndel comercio e innovaciones entre las que mencionamos:",
            {"La imprenta y\nla brújula", "La rueda y\nla escritura", "Las máquinas de vapor y\nla producción en masa", " La pólvora y\nLa rueda "},
            0
            )
    };

    preguntaActual = 0;
    puntosObtenidos = 0;
}

void juego2::cargarPregunta()
{
    if (preguntaActual >= preguntas.size()) {
        finalizarJuego();
        return;
    }

    preguntaRespondida = false;

    // Limpiar textos de respuestas anteriores
    for (QGraphicsTextItem* texto : textosRespuestas) {
        escenario->scene->removeItem(texto);
        delete texto;
    }
    textosRespuestas.clear();

    // Mostrar pregunta en la escena
    if (!textoPregunta) {
        // Crear fondo para la pregunta
        fondoPregunta = escenario->scene->addRect(0, 0, 800, 50);
        fondoPregunta->setBrush(QBrush(QColor(255, 193, 227, 200))); // #ffc1e3
        fondoPregunta->setPen(QPen(QColor(44, 14, 74), 2)); // #2c0e4a
        fondoPregunta->setPos(50, 10);
        fondoPregunta->setZValue(5);

        textoPregunta = escenario->scene->addText("");
        QFont font("Arial", 12, QFont::Bold);
        textoPregunta->setFont(font);
        textoPregunta->setDefaultTextColor(QColor(44, 14, 74)); // #2c0e4a
        textoPregunta->setZValue(6);
    }

    PreguntaBurbuja& pregunta = preguntas[preguntaActual];
    textoPregunta->setPlainText(pregunta.textoPregunta);
    textoPregunta->setPos(60, 20);

    // Mostrar respuestas en las burbujas
    for (int i = 0; i < burbujas.size() && i < pregunta.opciones.size(); i++) {
        QGraphicsTextItem* textoResp = escenario->scene->addText(pregunta.opciones[i]);
        QFont fontResp("Arial", 10, QFont::Bold);
        textoResp->setFont(fontResp);
        textoResp->setDefaultTextColor(QColor(44, 14, 74));
        textoResp->setZValue(3);

        // Centrar texto en la burbuja
        QPointF posBurbuja = burbujas[i]->pos();
        qreal anchoTexto = textoResp->boundingRect().width();
        qreal altoTexto = textoResp->boundingRect().height();
        textoResp->setPos(
            posBurbuja.x() + 40 - anchoTexto / 2,
            posBurbuja.y() + 40 - altoTexto / 2
            );

        textosRespuestas.append(textoResp);
    }
}

void juego2::verificarRespuesta(int indiceBurbuja)
{
    if (preguntaRespondida || preguntaActual >= preguntas.size()) return;

    preguntaRespondida = true;

    PreguntaBurbuja& pregunta = preguntas[preguntaActual];

    if (indiceBurbuja == pregunta.indiceCorrecta) {
        // ✅ Respuesta correcta
        puntosObtenidos++;

        MensajeWidget* mensaje = new MensajeWidget(
            "¡Correcto! +1 punto",
            "C:/Users/Lenovo/Downloads/restanguloPreg.png",
            this
            );
        mensaje->move((width() - mensaje->width()) / 2, (height() - mensaje->height()) / 2);
        mensaje->show();
    } else {
        // ❌ Respuesta incorrecta
        MensajeWidget* mensaje = new MensajeWidget(
            "Respuesta incorrecta.\nLa correcta era: " + pregunta.opciones[pregunta.indiceCorrecta],
            "C:/Users/Lenovo/Downloads/restanguloPreg.png",
            this
            );
        mensaje->move((width() - mensaje->width()) / 2, (height() - mensaje->height()) / 2);
        mensaje->show();
    }

    mostrarPuntuacion();
}

void juego2::avanzarSiguientePregunta()
{
    preguntaActual++;

    if (preguntaActual >= preguntas.size()) {
        finalizarJuego();
        return;
    }

    // Limpiar textos de respuestas
    for (QGraphicsTextItem* texto : textosRespuestas) {
        escenario->scene->removeItem(texto);
        delete texto;
    }
    textosRespuestas.clear();

    // ⭐ Restaurar TODAS las burbujas para la nueva pregunta
    for (QGraphicsPixmapItem* burbuja : burbujas) {
        burbuja->setVisible(true);
        burbuja->setOpacity(1.0);
        burbuja->setScale(1.0);
        burbuja->setZValue(1); // Asegurar que estén visibles
    }

    burbujaIndex = -1; // Resetear índice

    // Cargar nueva pregunta
    cargarPregunta();

    // Seleccionar primera burbuja
    if (!burbujas.isEmpty()) {
        seleccionarSiguienteBurbuja();
    }
}

void juego2::finalizarJuego()
{
    QString mensaje;
    if (puntosObtenidos == preguntas.size()) {

        emit estrellaObtenida();

        mensaje = QString("¡PERFECTO! %1/%2 puntos\n\n¡Has ganado el minijuego!")
                      .arg(puntosObtenidos).arg(preguntas.size());
    } else {
        mensaje = QString("Juego terminado\n\nPuntuación final: %1/%2")
                      .arg(puntosObtenidos).arg(preguntas.size());
    }

    MensajeWidget* mensajeFinal = new MensajeWidget(
        mensaje,
        "C:/Users/Lenovo/Downloads/restanguloPreg.png",
        this
        );
    mensajeFinal->move((width() - mensajeFinal->width()) / 2, (height() - mensajeFinal->height()) / 2);
    mensajeFinal->show();

    // Volver a la ruleta después de 3 segundos
    QTimer::singleShot(3000, this, &juego2::volverARuleta);
}

void juego2::mostrarPuntuacion()
{
    if (!textoPuntuacion) {
        textoPuntuacion = escenario->scene->addText("");
        QFont font("Arial", 14, QFont::Bold);
        textoPuntuacion->setFont(font);
        textoPuntuacion->setDefaultTextColor(QColor(255, 215, 0)); // Dorado
        textoPuntuacion->setZValue(10);
    }

    QString puntos = QString("Puntos: %1/%2").arg(puntosObtenidos).arg(preguntas.size());
    textoPuntuacion->setPlainText(puntos);
    textoPuntuacion->setPos(700, 550);
}
