#include "juego1.h"
#include "ui_juego1.h"
#include <QMessageBox>
#include <QTimer>
#include "preguntawidget.h"

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
    // Configurar el escenario dentro del QGraphicsView del UI
    QPixmap pixFondo("C:/Users/Lenovo/Downloads/laberinto.png");
    escenario->inicializar(ui->graphicsViewJ1, pixFondo);

    ui->graphicsViewJ1->setFocusPolicy(Qt::NoFocus);

    // Crear el personaje
    QVector<QPixmap> spritesDer = {
        QPixmap("C:/Users/Lenovo/Downloads/hada1AD.png").scaled(80,80, Qt::KeepAspectRatio, Qt::SmoothTransformation),
        QPixmap("C:/Users/Lenovo/Pictures/hada2AD.png").scaled(80,80, Qt::KeepAspectRatio, Qt::SmoothTransformation),
        QPixmap("C:/Users/Lenovo/Downloads/hada3AD.png").scaled(80,80, Qt::KeepAspectRatio, Qt::SmoothTransformation)
    };

    QVector<QPixmap> spritesIzq = {
        QPixmap("C:/Users/Lenovo/Pictures/hada1Iz.png").scaled(80,80, Qt::KeepAspectRatio, Qt::SmoothTransformation),
        QPixmap("C:/Users/Lenovo/Pictures/hada2Iz.png").scaled(80,80, Qt::KeepAspectRatio, Qt::SmoothTransformation),
        QPixmap("C:/Users/Lenovo/Pictures/hada3Iz.png").scaled(80,80, Qt::KeepAspectRatio, Qt::SmoothTransformation)
    };

    QVector<QPixmap> spritesArriba = {
        QPixmap("C:/Users/Lenovo/Downloads/hada4Ar.png").scaled(80,80, Qt::KeepAspectRatio, Qt::SmoothTransformation),
        QPixmap("C:/Users/Lenovo/Downloads/hada5Ar.png").scaled(80,80, Qt::KeepAspectRatio, Qt::SmoothTransformation)
    };
    objetosInteractivos();
    zonasColision();


    escenario->crearPersonaje(spritesDer, spritesIzq, spritesArriba, QPointF(85, 300));
    this->setFocus();


}



void juego1::keyPressEvent(QKeyEvent *event)
{
    if(escenario) {
        escenario->manejarTecla(event);
    }

    if (event->key() == Qt::Key_Escape) {
        emit volverARuleta();
    }

    for (NPC* npc : npcs) {
        if (escenario->personaje->collidesWithItem(npc)) {
            // Verificar si ya fue respondido
            if (npcsRespondidos.contains(npc->nombre)) {
                continue; // Saltar este NPC
            }

            // Evita que dispare el evento varias veces seguidas
            static QString ultimoNPC = "";
            if (ultimoNPC != npc->nombre) {
                ultimoNPC = npc->nombre;

                PreguntaWidget *p = nullptr;

                if (npc->nombre == "npc1") {
                    QString pregunta = "Uno de los siguientes personajes fue\nel encargado de pintar la Capilla Sixtina:";
                    QStringList opciones = {"Miguel Ángel", "Donatello", "Leonardo Da Vinci", "Francis Bacon"};
                    QString correcta = "Miguel Ángel";
                    p = new PreguntaWidget(pregunta, opciones, correcta, escenario->scene, npc->nombre);
                }
                else if (npc->nombre == "npc2") {
                    QString pregunta = "Genio del renacimiento que esculpió\nel Moisés, el David y la Pietá:";
                    QStringList opciones = {"Miguel Ángel Buonarroti", "Leonardo Da Vinci", "Rafael Sanzio", "Galileo Galilei"};
                    QString correcta = "Miguel Ángel Buonarroti";
                    p = new PreguntaWidget(pregunta, opciones, correcta, escenario->scene, npc->nombre);
                }
                else if (npc->nombre == "npc3") {
                    QString pregunta = "Durante el renacimiento el estilo artístico\nque impregnó el arte, la filosofía,\nla pintura y la escritura fue el:";
                    QStringList opciones = {"El Gótico", "El Barroco", "El Clasicismo", "El Romanticismo"};
                    QString correcta = "El Clasicismo";
                    p = new PreguntaWidget(pregunta, opciones, correcta, escenario->scene, npc->nombre);
                }
                else if (npc->nombre == "npc4") {
                    QString pregunta = "Durante el renacimiento surge una nueva\nvisión del hombre reflejada en el arte,\nla política y las ciencias, llamada:";
                    QStringList opciones = {"Antropocentrismo", "Humanismo", "Paradigma antropológico", "Teocentrismo"};
                    QString correcta = "Humanismo";
                    p = new PreguntaWidget(pregunta, opciones, correcta, escenario->scene, npc->nombre);
                }
                else if (npc->nombre == "npc5") {
                    QString pregunta = "Cuatro genios del renacimiento (Leonardo,\nDonatello, Rafael y Michelangelo) han sido\nllevados a la pantalla en los comics de:";
                    QStringList opciones = {"Las Tortugas Ninjas", "Los Caballeros del Zodiaco", "Los Cuatro Fantásticos", "Los antagonistas de Attack Titan"};
                    QString correcta = "Las Tortugas Ninjas";
                    p = new PreguntaWidget(pregunta, opciones, correcta, escenario->scene, npc->nombre);
                }

                // Conectar la señal para marcar como respondido
                if (p) {
                    connect(p, &PreguntaWidget::preguntaRespondida, this, [this](QString idNPC) {
                        npcsRespondidos.insert(idNPC);
                    });
                }

                // Si quieres que se vuelva a poder hablar después de moverse un poco
                QTimer::singleShot(1000, [=]() { ultimoNPC = ""; });
            }
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
    uno->setPos(800, 200);
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

