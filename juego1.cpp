#include "juego1.h"
#include "ui_juego1.h"
#include <QMessageBox>
#include <QTimer>
#include "preguntawidget.h"
#include <QPropertyAnimation>
#include "mensajewidget.h"

juego1::juego1(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::juego1)
    ,escenario(new Escenario(this))
{
    ui->setupUi(this);
    setFocusPolicy(Qt::StrongFocus);
}

juego1::~juego1()
{
    delete ui;
}


void juego1::inicializarNivel()
{
    // 🟢 Fondo
    QPixmap pixFondo("C:/Users/Lenovo/Downloads/laberinto.png");
    escenario->inicializar(ui->graphicsViewJ1, pixFondo);
    ui->graphicsViewJ1->setFocusPolicy(Qt::NoFocus);

    // 🟢 Reiniciar progreso
    llavesObtenidas = 0;
    cofreAbierto = false;
    npcsRespondidos.clear();

    // 🧚 Crear sprites del hada
    QVector<QPixmap> spritesDer = {
        QPixmap("C:/Users/Lenovo/Downloads/hada1AD.png").scaled(80,80),
        QPixmap("C:/Users/Lenovo/Pictures/hada2AD.png").scaled(80,80),
        QPixmap("C:/Users/Lenovo/Downloads/hada3AD.png").scaled(80,80)
    };

    QVector<QPixmap> spritesIzq = {
        QPixmap("C:/Users/Lenovo/Pictures/hada1Iz.png").scaled(80,80),
        QPixmap("C:/Users/Lenovo/Pictures/hada2Iz.png").scaled(80,80),
        QPixmap("C:/Users/Lenovo/Pictures/hada3Iz.png").scaled(80,80)
    };

    QVector<QPixmap> spritesArriba = {
        QPixmap("C:/Users/Lenovo/Downloads/hada4Ar.png").scaled(80,80),
        QPixmap("C:/Users/Lenovo/Downloads/hada5Ar.png").scaled(80,80)
    };

    // 🧭 Crear personaje (posición inicial)
    escenario->crearPersonaje(spritesDer, spritesIzq, spritesArriba, QPointF(85, 300));

    // 🗝️ Llaves y cofre
    inicializarLlaves();
    cofre = new QGraphicsPixmapItem(QPixmap("C:/Users/Lenovo/Downloads/cofreCerrado.png").scaled(70, 70));
    cofre->setPos(790, 200);
    escenario->scene->addItem(cofre);

    // 🤝 NPCs y colisiones
    objetosInteractivos();
    zonasColision();

    this->setFocus();
}

void juego1::keyPressEvent(QKeyEvent *event)
{
    if (escenario)
        escenario->manejarTecla(event);

    if (event->key() == Qt::Key_Escape)
        emit volverARuleta();

    verificarCofre();

    for (NPC* npc : npcs) {
        if (escenario->personaje->collidesWithItem(npc)) {
            if (npcsRespondidos.contains(npc->nombre)) continue;

            static QString ultimoNPC = "";
            if (ultimoNPC != npc->nombre) {
                ultimoNPC = npc->nombre;

                PreguntaWidget *p = nullptr;

                // 🧠 Preguntas
                if (npc->nombre == "npc1") {
                    p = new PreguntaWidget("Uno de los siguientes personajes fue\nel encargado de pintar la Capilla Sixtina:",
                                           {"Miguel Ángel", "Donatello", "Leonardo Da Vinci", "Francis Bacon"},
                                           "Miguel Ángel", escenario->scene, npc->nombre);
                }
                else if (npc->nombre == "npc2") {
                    p = new PreguntaWidget("Genio del renacimiento que esculpió\nel Moisés, el David y la Pietá:",
                                           {"Miguel Ángel Buonarroti", "Leonardo Da Vinci", "Rafael Sanzio", "Galileo Galilei"},
                                           "Miguel Ángel Buonarroti", escenario->scene, npc->nombre);
                }
                else if (npc->nombre == "npc3") {
                    p = new PreguntaWidget("Durante el renacimiento el estilo artístico\nque impregnó el arte, la filosofía,\nla pintura y la escritura fue el:",
                                           {"El Gótico", "El Barroco", "El Clasicismo", "El Romanticismo"},
                                           "El Clasicismo", escenario->scene, npc->nombre);
                }
                else if (npc->nombre == "npc4") {
                    p = new PreguntaWidget("Durante el renacimiento surge una nueva\nvisión del hombre reflejada en el arte,\nla política y las ciencias, llamada:",
                                           {"Antropocentrismo", "Humanismo", "Paradigma antropológico", "Teocentrismo"},
                                           "Humanismo", escenario->scene, npc->nombre);
                }
                else if (npc->nombre == "npc5") {
                    p = new PreguntaWidget("Cuatro genios del renacimiento fueron\nllevados a la pantalla en los comics de:",
                                           {"Las Tortugas Ninjas", "Los Caballeros del Zodiaco", "Los Cuatro Fantásticos", "Attack on Titan"},
                                           "Las Tortugas Ninjas", escenario->scene, npc->nombre);
                }

                if (p) {
                    connect(p, &PreguntaWidget::preguntaRespondida, this,
                            [this, npc](const QString &idNPC, bool esCorrecta) {
                                npcsRespondidos.insert(idNPC);
                                if (esCorrecta && llavesObtenidas < 5) {
                                    actualizarLlave(llavesObtenidas);
                                    llavesObtenidas++;
                                }
                                QPropertyAnimation *fadeOut = new QPropertyAnimation(npc, "opacity");
                                fadeOut->setDuration(500);
                                fadeOut->setStartValue(1.0);
                                fadeOut->setEndValue(0.0);
                                fadeOut->start(QAbstractAnimation::DeleteWhenStopped);
                                QTimer::singleShot(500, npc, [npc]() { npc->setVisible(false); });
                            });
                }

                QTimer::singleShot(1000, [=]() { ultimoNPC = ""; });
            }
        }
    }
}

void juego1::inicializarLlaves()
{
    // Crear 5 labels con iconos de llaves en blanco y negro
    for (int i = 0; i < 5; i++) {
        QLabel *llave = new QLabel(this);
        llave->setPixmap(QPixmap("C:/Users/Lenovo/Downloads/llaveGris.png").scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        llave->setGeometry(20 + (i * 50), 20, 40, 40); // Posición horizontal en la parte superior
        llave->setStyleSheet("background-color: transparent;"); // Fondo transparente
        llave->show();
        iconosLlaves.append(llave);
    }
}

// NUEVA FUNCIÓN: Actualizar una llave específica a dorado
void juego1::actualizarLlave(int indice)
{
    if (indice >= 0 && indice < iconosLlaves.size()) {
        iconosLlaves[indice]->setPixmap(
            QPixmap("C:/Users/Lenovo/Downloads/llaveColor.jpg").scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation)
            );
    }
}

// NUEVA FUNCIÓN: Verificar interacción con el cofre
void juego1::verificarCofre()
{
    if (!cofre || cofreAbierto) return;

    if (npcsRespondidos.size() < 5) {
        return; // No hacer nada si no ha terminado
    }

    // Verificar colisión con el cofre
    if (escenario->personaje->collidesWithItem(cofre)) {
        static bool yaInteractuo = false; // Evitar múltiples mensajes

        if (!yaInteractuo) {
            yaInteractuo = true;

            /*if (llavesObtenidas >= 5) {
                // ¡Éxito! Tiene las 5 llaves
                cofreAbierto = true;

                // Cambiar imagen del cofre a abierto
                cofre->setPixmap(QPixmap("C:/Users/Lenovo/Downloads/cofreAbierto.png").scaled(80, 80));

                // Mostrar mensaje de victoria
                QMessageBox::information(this, "¡Felicidades!",
                                         "¡Has obtenido las 5 llaves!\n¡Ganaste 1 punto!\n\n¡Nivel completado!");

                // Aquí puedes emitir una señal o volver a la ruleta
                emit volverARuleta();

            } else {
                // No tiene suficientes llaves
                QMessageBox::warning(this, "Cofre cerrado",
                                     QString("Necesitas 5 llaves para abrir el cofre.\n\nTienes: %1/5 llaves\n\nResponde correctamente todas las preguntas!").arg(llavesObtenidas));
                emit volverARuleta();
            }*/

            if (llavesObtenidas >= 5) {
                // ¡Éxito! Tiene las 5 llaves
                cofreAbierto = true;

                // Cambiar imagen del cofre a abierto
                cofre->setPixmap(QPixmap("C:/Users/Lenovo/Downloads/cofreAbierto.png").scaled(80, 80));

                // Mostrar mensaje de victoria con tu MensajeWidget
                MensajeWidget* mensaje = new MensajeWidget(
                    "¡FELICIDADES! Has obtenido las 5 llaves.\n\n"
                    "¡Ganaste 1 punto!\n\n¡Nivel completado!",
                    "C:/Users/Lenovo/Downloads/restanguloPreg.png",
                    this
                    );

                // Centrar el mensaje en pantalla
                mensaje->move((width() - mensaje->width()) / 2+250, (height() - mensaje->height()) / 2);
                mensaje->show();

                // Esperar unos segundos y luego volver a la ruleta
                QTimer::singleShot(2500, this, [this]() {
                    emit volverARuleta();
                });
            }
            else {
                // Mostrar mensaje personalizado en lugar de QMessageBox
                MensajeWidget* mensaje = new MensajeWidget(
                    QString("Necesitas 5 llaves para abrir el cofre.\n\nTienes: %1/5 llaves\n\nResponde correctamente todas las preguntas!").arg(llavesObtenidas),
                    "C:/Users/Lenovo/Downloads/restanguloPreg.png",
                    this
                    );

                // Centrar el mensaje en la pantalla (ajusta si quieres moverlo un poco)
                mensaje->move((width() - mensaje->width()) / 2+250, (height() - mensaje->height()) / 2);
                mensaje->show();

                // Llamar a volverARuleta después de un momento (por ejemplo, 2.5 segundos)
                QTimer::singleShot(2500, this, [this]() {
                    emit volverARuleta();
                });
            }

            // Resetear el flag después de un segundo
            QTimer::singleShot(1000, [&]() { yaInteractuo = false; });
        }
    }
}






void juego1::objetosInteractivos() {
    NPC* uno = new NPC("C:/Users/Lenovo/Downloads/npc1.png", "npc1");
    NPC* dos = new NPC("C:/Users/Lenovo/Downloads/npc2.png", "npc2");
    NPC* tres = new NPC("C:/Users/Lenovo/Downloads/npc3.png", "npc3");
    NPC* cuatro = new NPC("C:/Users/Lenovo/Downloads/npc4.png", "npc4");
    NPC* cinco = new NPC("C:/Users/Lenovo/Downloads/npc5.png", "npc5");

    // Posiciones dentro del laberinto
    uno->setPos(940, 310);
    dos->setPos(700, 550);
    tres->setPos(650, 300);
    cuatro->setPos(260, 150);
    cinco->setPos(260, 580);

    // Añadirlos a la escena
    escenario->scene->addItem(uno);
    escenario->scene->addItem(dos);
    escenario->scene->addItem(tres);
    escenario->scene->addItem(cuatro);
    escenario->scene->addItem(cinco);

    // Guardarlos si luego quieres comprobar colisiones o acceder a ellos
    npcs.append(uno);
    npcs.append(dos);
    npcs.append(tres);
    npcs.append(cuatro);
    npcs.append(cinco);

}

void juego1::zonasColision(){

    //colisión
    // zonas de colision
    ladoIzq = escenario->scene->addRect(0, 0, 100, 800, Qt::NoPen);
    ladoDer = escenario->scene->addRect(1000, 0, 80, 800, Qt::NoPen);
    arribaCuadro = escenario->scene->addRect(460, 0, 200, 300, Qt::NoPen);
    ladoArriba = escenario->scene->addRect(0, 0, 1000, 40, Qt::NoPen);
    ladoAbajo = escenario->scene->addRect(0, 645, 1000, 50, Qt::NoPen);
    abajoCuadro = escenario->scene->addRect(540, 460, 140, 300, Qt::NoPen);
    enmedio = escenario->scene->addRect(290, 210, 90, 260, Qt::NoPen);
    uno = escenario->scene->addRect(163, 115, 65, 205, Qt::NoPen);
    dos = escenario->scene->addRect(163, 115, 215, 30, Qt::NoPen);
    tres = escenario->scene->addRect(163, 390, 75, 185, Qt::NoPen);
    cuatro = escenario->scene->addRect(163, 545, 300, 30, Qt::NoPen);
    cinco = escenario->scene->addRect(447, 375, 30, 175, Qt::NoPen);

    seis = escenario->scene->addRect(447, 375, 215, 20, Qt::NoPen);

    siete = escenario->scene->addRect(735, 115, 65, 350, Qt::NoPen);
    ocho = escenario->scene->addRect(735, 115, 203, 30, Qt::NoPen);
    nueve = escenario->scene->addRect(870, 115, 65, 205, Qt::NoPen);
    diez = escenario->scene->addRect(860, 395, 75, 180, Qt::NoPen);
    once = escenario->scene->addRect(745, 554, 150, 25, Qt::NoPen);
    doce = escenario->scene->addRect(670, 460, 65, 20, Qt::NoPen);



    // Para que sean invisibles
    ladoIzq->setBrush(Qt::transparent);
    ladoDer->setBrush(Qt::transparent);
    arribaCuadro->setBrush(Qt::transparent);
    ladoArriba->setBrush(Qt::transparent);
    ladoAbajo->setBrush(Qt::transparent);
    abajoCuadro->setBrush(Qt::transparent);
    enmedio->setBrush(Qt::transparent);
    uno->setBrush(Qt::transparent);
    dos->setBrush(Qt::transparent);
    tres->setBrush(Qt::transparent);
    cuatro->setBrush(Qt::transparent);
    cinco->setBrush(Qt::transparent);
    seis->setBrush(Qt::transparent);
    siete->setBrush(Qt::transparent);
    ocho->setBrush(Qt::transparent);
    nueve->setBrush(Qt::transparent);
    diez->setBrush(Qt::transparent);
    once->setBrush(Qt::transparent);
    doce->setBrush(Qt::transparent);

    escenario->obstaculos = {ladoIzq, ladoDer, arribaCuadro, ladoArriba, ladoAbajo,
                             abajoCuadro, enmedio, uno, dos, tres, cuatro, cinco,
                             seis, siete, ocho, nueve, diez, once, doce};

}

