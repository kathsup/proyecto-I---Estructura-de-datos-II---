#include "juego3.h"
#include "ui_juego3.h"
#include "mensajewidget.h"
#include <QDebug>
#include <QKeyEvent>
#include <QPropertyAnimation>

juego3::juego3(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::juego3)
    , escenario(new Escenario(this))
{
    ui->setupUi(this);
    setFocusPolicy(Qt::StrongFocus);
}

juego3::~juego3()
{
    delete ui;
}

/*void juego3::inicializarNivel()
{
    // Configurar el escenario dentro del QGraphicsView del UI
    QPixmap pixFondo("C:/Users/Lenovo/Downloads/fondo.png");
    escenario->inicializar(ui->graphicsViewJ3, pixFondo);

    ui->graphicsViewJ3->setFocusPolicy(Qt::NoFocus);



    QFont font;
    font.setFamily("Arial");
    font.setPointSize(8);
    font.setBold(true);
    ui->labelPregunta->setFont(font);
    ui->labelPregunta->setStyleSheet(
        "background-image: url('C:/Users/Lenovo/Downloads/preguntaNivel3.png');"
        "background-repeat: no-repeat;"
        "background-position: center;"
        "background-size: cover;"
        "color: #2c0e4a;"
        "font-weight: bold;"
        "border: none;"
        );

    // Crear el personaje con sus sprites
    QVector<QPixmap> spritesDer = {
        QPixmap("C:/Users/Lenovo/Downloads/hada1AD.png").scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation),
        QPixmap("C:/Users/Lenovo/Pictures/hada2AD.png").scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation),
        QPixmap("C:/Users/Lenovo/Downloads/hada3AD.png").scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation)
    };

    QVector<QPixmap> spritesIzq = {
        QPixmap("C:/Users/Lenovo/Pictures/hada1Iz.png").scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation),
        QPixmap("C:/Users/Lenovo/Pictures/hada2Iz.png").scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation),
        QPixmap("C:/Users/Lenovo/Pictures/hada3Iz.png").scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation)
    };

    QVector<QPixmap> spritesArriba = {
        QPixmap("C:/Users/Lenovo/Downloads/hada4Ar.png").scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation),
        QPixmap("C:/Users/Lenovo/Downloads/hada5Ar.png").scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation)
    };

    objetosInteractivos();
    escenario->crearPersonaje(spritesDer, spritesIzq, spritesArriba, QPointF(350, 530));

    inicializarPreguntas();
    cargarPregunta();

    this->setFocus();
}*/

void juego3::inicializarNivel()
{
    // Configurar el escenario dentro del QGraphicsView del UI
    QPixmap pixFondo("C:/Users/Lenovo/Downloads/fondo.png");
    escenario->inicializar(ui->graphicsViewJ3, pixFondo);

    ui->graphicsViewJ3->setFocusPolicy(Qt::NoFocus);

    // MÉTODO QPALETTE - Más confiable que stylesheet
    QPixmap fondoPregunta("C:/Users/Lenovo/Downloads/preguntaNivel3.png");

    if (!fondoPregunta.isNull()) {
        // Escalar la imagen al tamaño exacto del label
        QPixmap fondoEscalado = fondoPregunta.scaled(
            ui->labelPregunta->size(),
            Qt::IgnoreAspectRatio,
            Qt::SmoothTransformation
            );

        QPalette palette;
        palette.setBrush(QPalette::Window, QBrush(fondoEscalado));
        ui->labelPregunta->setAutoFillBackground(true);
        ui->labelPregunta->setPalette(palette);
    } else {
        qDebug() << "ERROR: No se pudo cargar preguntaNivel3.png";
    }

    // Configurar fuente del texto
    QFont font;
    font.setFamily("Arial");
    font.setPointSize(8);
    font.setBold(true);
    ui->labelPregunta->setFont(font);
    ui->labelPregunta->setAlignment(Qt::AlignCenter);
    ui->labelPregunta->setWordWrap(true);

    // Establecer color del texto
    QPalette textPalette = ui->labelPregunta->palette();
    textPalette.setColor(QPalette::WindowText, QColor("#2c0e4a"));
    ui->labelPregunta->setPalette(textPalette);

    // Crear el personaje con sus sprites
    QVector<QPixmap> spritesDer = {
        QPixmap("C:/Users/Lenovo/Downloads/hada1AD.png").scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation),
        QPixmap("C:/Users/Lenovo/Pictures/hada2AD.png").scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation),
        QPixmap("C:/Users/Lenovo/Downloads/hada3AD.png").scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation)
    };

    QVector<QPixmap> spritesIzq = {
        QPixmap("C:/Users/Lenovo/Pictures/hada1Iz.png").scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation),
        QPixmap("C:/Users/Lenovo/Pictures/hada2Iz.png").scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation),
        QPixmap("C:/Users/Lenovo/Pictures/hada3Iz.png").scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation)
    };

    QVector<QPixmap> spritesArriba = {
        QPixmap("C:/Users/Lenovo/Downloads/hada4Ar.png").scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation),
        QPixmap("C:/Users/Lenovo/Downloads/hada5Ar.png").scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation)
    };

    objetosInteractivos();
    escenario->crearPersonaje(spritesDer, spritesIzq, spritesArriba, QPointF(350, 530));

    inicializarPreguntas();
    cargarPregunta();

    this->setFocus();
}

void juego3::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Escape) {
        emit volverARuleta();
    }
    else if (event->key() == Qt::Key_E) {
        if (florEnMano) {
            depositarFlor();
        } else {
            for (Flor* f : flores) {
                if (escenario->personaje->collidesWithItem(f)) {
                    recogerFlor(f);
                    break;
                }
            }
        }
    }
    else if (escenario) {
        escenario->manejarTecla(event);
        if (florEnMano) {
            actualizarPosicionFlor();
        }
    }
}

void juego3::objetosInteractivos()
{
    QPixmap pixcanasta("C:/Users/Lenovo/Downloads/canasta.png");
    canasta = escenario->scene->addPixmap(
        pixcanasta.scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    canasta->setPos(550, 530);
}

void juego3::inicializarPreguntas()
{
    preguntas = {
        {"Durante el renacimiento, el modelo de gobierno es uno de los siguientes:",
         {{"Monarquía absoluta", true},
          {"Tiranía republicana", false},
          {"Democracia participativa", false},
          {"Liberalismo político", false}}},

        {"De los siguientes acontecimientos, seleccione el que inicia el período moderno:",
         {{"Toma de Constantinopla", false},
          {"Tratado de paz de Westfalia", true},
          {"Toma de la Bastilla", false},
          {"La ruta de la seda", false}}},

        {"Durante el siglo XV, la sociedad se estratifica en tres estamentos definidos:",
         {{"Clase media, baja y alta", false},
          {"Nobleza, clero y estado llano", true},
          {"Artesanos y guardianes", false},
          {"Gobernantes", false}}},

        {"¿Quién aparece como exponente del realismo político?",
         {{"Tomás Moro", false},
          {"Jean Bodín", false},
          {"Nicolás Maquiavelo", true},
          {"Erasmo de Rotterdam", false}}},

        {"Terminada la edad media, en el contexto de la política resulta que:",
         {{"La Iglesia\nresalta su poder", false},
          {"La Iglesia pierde\nel papel rector\nen la política", true},
          {"La Iglesia evangélica\nse posiciona\nen la política", false},
          {"La política desaparece", false}}}
    };
}

void juego3::cargarPregunta()
{
    for (Flor* f : flores) {
        escenario->scene->removeItem(f);
        delete f;
    }
    flores.clear();
    florEnMano = nullptr;

    ui->labelPregunta->setText(preguntas[preguntaActual].texto);

    QVector<QString> rutasFlores = {
        "C:/Users/Lenovo/Downloads/flor1.png",
        "C:/Users/Lenovo/Downloads/flor2.png",
        "C:/Users/Lenovo/Downloads/flor3.png",
        "C:/Users/Lenovo/Downloads/flor4.png"
    };

    int posicionY = 390;
    int inicioX = 150;
    int espaciado = 200;

    for (int i = 0; i < preguntas[preguntaActual].respuestas.size() && i < rutasFlores.size(); i++) {
        auto& resp = preguntas[preguntaActual].respuestas[i];
        QPixmap pixFlor(rutasFlores[i]);

        if (pixFlor.isNull()) {
            qDebug() << "ERROR: No se pudo cargar la imagen:" << rutasFlores[i];
            continue;
        }

        Flor* f = new Flor(
            pixFlor.scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation),
            resp.texto,
            resp.esCorrecta
            );

        f->setPos(inicioX + (i * espaciado), posicionY);
        f->setZValue(1);
        escenario->scene->addItem(f);
        flores.push_back(f);
    }
}

void juego3::recogerFlor(Flor* flor)
{
    if (!flor) return;

    florEnMano = flor;
    flor->setZValue(10);
    flor->mostrarTexto(false);
    actualizarPosicionFlor();
}

void juego3::depositarFlor()
{
    if (!florEnMano) return;

    QPointF posPersonaje = escenario->personaje->pos();
    QPointF posCanasta = canasta->pos();
    qreal distancia = QLineF(posPersonaje, posCanasta).length();

    if (distancia < 200) {
        if (florEnMano->esRespuestaCorrecta()) {
            floresCorrectas++;
            QPointF posicionEnCanasta = canasta->pos() + QPointF(-5 + (floresCorrectas * 30), 20);
            florEnMano->moverA(posicionEnCanasta);
            florEnMano->setZValue(5);
            florEnMano->mostrarTexto(false);
            florEnMano->marcarComoRecolectada();
            flores.removeOne(florEnMano);
            florEnMano = nullptr;

            preguntaActual++;
            if (preguntaActual < preguntas.size()) {
                cargarPregunta();
            } else {
                finalizarJuego();
            }
        } else {
            MensajeWidget* mensaje = new MensajeWidget(
                "Esa no es la respuesta correcta.",

                this,200,100,3000
                );
            mensaje->move(650,300);
            mensaje->show();

            florEnMano->mostrarTexto(true);
            florEnMano->setZValue(1);
            florEnMano = nullptr;

            preguntaActual++;
            if (preguntaActual < preguntas.size()) {
                cargarPregunta();
            } else {
                finalizarJuego();
            }
        }
    } else {
        MensajeWidget* mensaje = new MensajeWidget(
            "Debes estar más cerca de la canasta\npara depositar la flor.",
            this,200,100,3000
            );
        mensaje->move(650,300);
        mensaje->show();
    }
}

void juego3::finalizarJuego()
{
    if (floresCorrectas == 5) {
        emit estrellaObtenida();

        MensajeWidget* mensaje = new MensajeWidget(
            "¡FELICIDADES! Has completado la canasta con las 5 flores correctas.\n\n"
            "¡Has ganado el punto del minijuego!",
            this,200,100,3000
            );
        mensaje->move(650,300);
        mensaje->show();
    } else {
        MensajeWidget* mensaje = new MensajeWidget(
            "Has recolectado " + QString::number(floresCorrectas) + " de 5 flores correctas.\n\n"
                                                                    "Necesitas las 5 flores correctas para ganar el punto.",
            this,200,100,3000
            );
        mensaje->move(650,300);
        mensaje->show();
    }

    preguntaActual = 0;
    floresCorrectas = 0;
    emit volverARuleta();
}

void juego3::actualizarPosicionFlor()
{
    if (florEnMano && escenario->personaje) {
        QPointF posPersonaje = escenario->personaje->pos();
        florEnMano->setPos(posPersonaje.x() + 60, posPersonaje.y() + 50);
    }
}
