#include "juego4.h"
#include "ui_juego4.h"
#include <QTimeLine>
#include <QTimer>

juego4::juego4(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::juego4)
    ,escenario(new Escenario(this))
    , posicionActual(0)//nuevo
    , preguntaActual(0)
    , respuestasCorrectas(0)
    , textoContador(nullptr)
{
    ui->setupUi(this);
    setFocusPolicy(Qt::StrongFocus);

    preguntas = {
        {"Entre los siguientes renacentistas seleccione, uno de los precursores filósofo-científico\ndel heliocentrismo (teoría que afirma que el sol es el centro del universo): ",
         {"Tomas Moro", "Galileo", "Platón", "Arquimedes"},
         "Galileo"},

        {"El método científico se introduce por el interés de tres filósofos. Entre los siguientes uno\nde los mencionados no es precursor del método científico:",
         {"Francis Bacon", "Galileo Galilei", "Nicolas Maquiavelo", "René Descartes"},
         "Nicolas Maquiavelo"},

        {"Es uno de los precursores del pensamiento Moderno:",
         {"Isaac Newton", "René Descartes", "Erasmo de Roterdam", "Francis Bacon"},
         "René Descartes"},

        {"De los siguientes filósofos niega el geocentrismo (teoría que afirma\nque el centro de nuestro sistema solar es la tierra): ",
         {"Aristóteles", "Nicolás Copérnico", "Tomás de Aquino", "Isaac Newton"},
         "Nicolás Copérnico"},

        {"Uno de los inventos que suscitó un conocimiento ilimitado, fue el de Gutenberg:",
         {"El astrolabio", "La imprenta", "La Nao y la Carabela", "El Telescopio"},
         "La imprenta"}
    };
}

juego4::~juego4()
{
    delete ui;
}

void juego4::inicializarNivel()
{
    // Configurar el escenario dentro del QGraphicsView del UI
    QPixmap pixFondo("C:/Users/Lenovo/Downloads/fondoMini4.png");
    escenario->inicializar(ui->graphicsViewJ4, pixFondo);

    ui->graphicsViewJ4->setFocusPolicy(Qt::NoFocus);

    // Crear el personaje
    QVector<QPixmap> spritesDer = {
        QPixmap("C:/Users/Lenovo/Downloads/hada1AD.png").scaled(90,90, Qt::KeepAspectRatio, Qt::SmoothTransformation),
        QPixmap("C:/Users/Lenovo/Pictures/hada2AD.png").scaled(90,90, Qt::KeepAspectRatio, Qt::SmoothTransformation),
        QPixmap("C:/Users/Lenovo/Downloads/hada3AD.png").scaled(90,90, Qt::KeepAspectRatio, Qt::SmoothTransformation)
    };

    QVector<QPixmap> spritesIzq = {
        QPixmap("C:/Users/Lenovo/Pictures/hada1Iz.png").scaled(90,90, Qt::KeepAspectRatio, Qt::SmoothTransformation),
        QPixmap("C:/Users/Lenovo/Pictures/hada2Iz.png").scaled(90,90, Qt::KeepAspectRatio, Qt::SmoothTransformation),
        QPixmap("C:/Users/Lenovo/Pictures/hada3Iz.png").scaled(90,90, Qt::KeepAspectRatio, Qt::SmoothTransformation)
    };

    QVector<QPixmap> spritesArriba = {
        QPixmap("C:/Users/Lenovo/Downloads/hada4Ar.png").scaled(90,90, Qt::KeepAspectRatio, Qt::SmoothTransformation),
        QPixmap("C:/Users/Lenovo/Downloads/hada5Ar.png").scaled(90,90, Qt::KeepAspectRatio, Qt::SmoothTransformation)
    };
    objetosInteractivos();
    //zonasColision();


    posiciones = {
        QPointF(410 - 45, 590 - 70),
        QPointF(510 - 45, 530 - 70),
        QPointF(635 - 45, 450 - 70),
        QPointF(520 - 45, 350 - 70),
        QPointF(650 - 45, 265 - 70),
        QPointF(560 - 45, 180 - 70)
    };


    escenario->crearPersonaje(spritesDer, spritesIzq, spritesArriba, posiciones[0]);

    textoContador = new QGraphicsTextItem();
    textoContador->setFont(QFont("Arial", 16, QFont::Bold));
    textoContador->setDefaultTextColor(Qt::white);
    textoContador->setPos(50, 50);
    escenario->scene->addItem(textoContador);
    actualizarContador();

    this->setFocus();

    QTimer::singleShot(500, this, &juego4::mostrarPregunta);
}

void juego4::mostrarPregunta()
{
    // Verificar si ya se respondieron todas las preguntas
    if (preguntaActual >= preguntas.size()) {
        // Fin del juego
        QGraphicsTextItem* mensajeFinal = new QGraphicsTextItem();
        if (respuestasCorrectas == 5) {
            mensajeFinal->setPlainText("¡FELICIDADES!\n¡Llegaste a la cima!");
            mensajeFinal->setDefaultTextColor(QColor("#4CAF50"));
        } else {
            mensajeFinal->setPlainText(QString("¡Juego terminado!\nRespuestas correctas: %1/5").arg(respuestasCorrectas));
            mensajeFinal->setDefaultTextColor(Qt::white);
        }
        mensajeFinal->setFont(QFont("Arial", 18, QFont::Bold));
        mensajeFinal->setPos(400, 300);
        escenario->scene->addItem(mensajeFinal);

        // Volver a la ruleta después de 3 segundos
        QTimer::singleShot(3000, this, [this]() {
            emit volverARuleta();
        });
        return;
    }

    // Obtener la pregunta actual
    Pregunta& p = preguntas[preguntaActual];

    // Crear el widget de pregunta
    PreguntaWidget* pregunta = new PreguntaWidget(
        p.pregunta,
        p.opciones,
        p.respuestaCorrecta,
        escenario->scene,
        QString::number(preguntaActual)
        );

    // Conectar la señal de respuesta
    connect(pregunta, &PreguntaWidget::preguntaRespondida,
            this, &juego4::onPreguntaRespondida);
}

void juego4::onPreguntaRespondida(QString npcId, bool correcta)
{
    if (correcta) {
        respuestasCorrectas++;
        // Si responde correctamente, sube de posición
        if (posicionActual < posiciones.size() - 1) {
            moverPersonajeAPosicion(posicionActual + 1);
        }
    }
    // Si no responde correctamente, se queda en la misma posición

    // Actualizar contador
    actualizarContador();

    // Pasar a la siguiente pregunta
    preguntaActual++;

    // Mostrar siguiente pregunta después de un delay
    QTimer::singleShot(2500, this, &juego4::mostrarPregunta);
}

void juego4::actualizarContador()
{
    if (textoContador) {
        textoContador->setPlainText(QString("Correctas: %1/5").arg(respuestasCorrectas));
    }
}

void juego4::moverPersonajeAPosicion(int numeroPosicion)
{
    // Verificar que el número de posición sea válido
    if (numeroPosicion < 0 || numeroPosicion >= posiciones.size()) {
        return;
    }

    // Obtener posiciones
    QPointF posInicial = escenario->personaje->pos();
    QPointF posFinal = posiciones[numeroPosicion];

    // Crear timeline para la animación
    QTimeLine *timeLine = new QTimeLine(800); // 800ms
    timeLine->setFrameRange(0, 100);

    // Conectar la timeline al movimiento
    connect(timeLine, &QTimeLine::frameChanged, this, [=](int frame) {
        qreal progreso = frame / 100.0;

        // Aplicar easing (OutQuad)
        qreal t = progreso;
        qreal easedProgress = -t * (t - 2); // Fórmula OutQuad

        // Calcular posición interpolada
        QPointF nuevaPos = posInicial + (posFinal - posInicial) * easedProgress;
        escenario->personaje->setPos(nuevaPos);
    });

    // Cuando termine, eliminar el timeline
    connect(timeLine, &QTimeLine::finished, timeLine, &QTimeLine::deleteLater);

    // Actualizar posición actual
    posicionActual = numeroPosicion;

    // Iniciar animación
    timeLine->start();
}


void juego4::keyPressEvent(QKeyEvent *event)
{
    if(escenario) {
        escenario->manejarTecla(event);
    }

    if (event->key() == Qt::Key_Escape) {
        emit volverARuleta();
    }

    //nuevo
    if (event->key() == Qt::Key_1) moverPersonajeAPosicion(0);
    if (event->key() == Qt::Key_2) moverPersonajeAPosicion(1);
    if (event->key() == Qt::Key_3) moverPersonajeAPosicion(2);
    if (event->key() == Qt::Key_4) moverPersonajeAPosicion(3);
    if (event->key() == Qt::Key_5) moverPersonajeAPosicion(4);
    if (event->key() == Qt::Key_6) moverPersonajeAPosicion(5);
}

void juego4::objetosInteractivos() {
    QPixmap pixFlor("C:/Users/Lenovo/Downloads/flor.png");
    QGraphicsPixmapItem* flor = escenario->scene->addPixmap(
        pixFlor.scaled(480, 480, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    flor->setPos(350, 115);
}

