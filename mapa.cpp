#include "mapa.h"
#include "ui_mapa.h"
#include "nivel2ruleta.h"
#include "nivel1video.h"
#include "nivel3batalla.h"
#include "nivel4casa.h"


mapa::mapa(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::mapa)
    ,escenario(new Escenario(this))
{
    ui->setupUi(this);
    setFocusPolicy(Qt::StrongFocus);
}

mapa::~mapa()
{
    delete ui;
}

void mapa::inicializarNivel()
{
    // Configurar el escenario dentro del QGraphicsView del UI
    QPixmap pixFondo("C:/Users/Lenovo/Downloads/mapa.png");
    escenario->inicializar(ui->graphicsViewMapa, pixFondo);

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
        QPixmap("C:/Users/Lenovo/Downloads/hada4Ar.png").scaled(100,100, Qt::KeepAspectRatio, Qt::SmoothTransformation)
};
objetosInteractivos();

escenario->crearPersonaje(spritesDer, spritesIzq, spritesArriba, QPointF(450, 300));
this->setFocus();


}

/*void mapa::keyPressEvent(QKeyEvent *event)
{
    if(escenario) {
        escenario->manejarTecla(event);
    }

    if(event->key() == Qt::Key_G) {
        for(auto* obj : escenario->zonasInteractivas) {
            if(escenario->personaje->collidingItems().contains(obj)) {
                emit solicitarCambioNivel();
                break;
            }
        }
    }
}*/

void mapa::keyPressEvent(QKeyEvent *event)
{
    if (escenario) {
        escenario->manejarTecla(event);
    }
    if (event->key() == Qt::Key_G) {
        for (auto* obj : escenario->zonasInteractivas) {
            if (escenario->personaje->collidingItems().contains(obj)) {
                QString etiqueta = obj->data(0).toString();

                if (etiqueta == "nivel1") {
                    emit solicitarCambioNivel(1);
                } else if (etiqueta == "nivel2") {
                    emit solicitarCambioNivel(2);
                } else if (etiqueta == "nivel3") {
                    emit solicitarCambioNivel(3);
                } else if (etiqueta == "nivel4") {
                    emit solicitarCambioNivel(4);
                }
                break;
            }
        }
    }
}


/*void mapa::objetosInteractivos(){

    //nivel 1 video
    QPixmap pixVideo("C:/Users/Lenovo/Downloads/objetoVideoMapa.png");
    QGraphicsPixmapItem* video = escenario->scene->addPixmap(
        pixVideo.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    video->setPos(140, 320);

    //bandera video
    QPixmap pixBanderaVid("C:/Users/Lenovo/Downloads/banderaVid.png");
    QGraphicsPixmapItem* videoBandera = escenario->scene->addPixmap(
        pixBanderaVid.scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    videoBandera->setPos(260, 310);


    //nivel 2 ruleta
    QPixmap pixRuleta("C:/Users/Lenovo/Downloads/objetoRuletaMapa.png");
    QGraphicsPixmapItem* ruleta = escenario->scene->addPixmap(
        pixRuleta.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
            ruleta->setPos(250, 85);

    //bandera ruleta
    QPixmap pixRuletaVid("C:/Users/Lenovo/Downloads/banderaRuleta.png");
    QGraphicsPixmapItem* ruletaBandera = escenario->scene->addPixmap(
        pixRuletaVid.scaled(140, 140, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    ruletaBandera->setPos(220, 115);


    //nivel 3 batalla
    QPixmap pixBatalla("C:/Users/Lenovo/Downloads/objetoBatallaMapa.png");
    QGraphicsPixmapItem* batalla = escenario->scene->addPixmap(
        pixBatalla.scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    batalla->setPos(530, 430);

    //bandera batalla
    QPixmap pixBatallaVid("C:/Users/Lenovo/Downloads/banderaCombate.png");
    QGraphicsPixmapItem* batallaBandera = escenario->scene->addPixmap(
        pixBatallaVid.scaled(140, 140, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    batallaBandera->setPos(640, 420);


    //nivel 4 casa
    QPixmap pixCasa("C:/Users/Lenovo/Downloads/objetoCasaMapa.png");
    QGraphicsPixmapItem* casa = escenario->scene->addPixmap(
        pixCasa.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    casa->setPos(650, 140);

    //bandera casa
    QPixmap pixCasaVid("C:/Users/Lenovo/Downloads/banderaCasa.png");
    QGraphicsPixmapItem* casaBandera = escenario->scene->addPixmap(
        pixCasaVid.scaled(140, 140, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    casaBandera->setPos(800, 160);


    // agregar a lista de interacciones
    escenario->zonasInteractivas.append(video);
    escenario->zonasInteractivas.append(ruleta);
    escenario->zonasInteractivas.append(batalla);
    escenario->zonasInteractivas.append(casa);

}*/


void mapa::objetosInteractivos() {

    // ---- NIVEL 1: VIDEO ----
    QPixmap pixVideo("C:/Users/Lenovo/Downloads/objetoVideoMapa.png");
    QGraphicsPixmapItem* video = escenario->scene->addPixmap(
        pixVideo.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    video->setPos(140, 320);
    video->setData(0, "nivel1"); // 🔹 Etiqueta para identificarlo

    // Bandera del nivel 1
    QPixmap pixBanderaVid("C:/Users/Lenovo/Downloads/banderaVid.png");
    QGraphicsPixmapItem* videoBandera = escenario->scene->addPixmap(
        pixBanderaVid.scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    videoBandera->setPos(260, 310);


    // ---- NIVEL 2: RULETA ----
    QPixmap pixRuleta("C:/Users/Lenovo/Downloads/objetoRuletaMapa.png");
    QGraphicsPixmapItem* ruleta = escenario->scene->addPixmap(
        pixRuleta.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    ruleta->setPos(250, 85);
    ruleta->setData(0, "nivel2"); // 🔹 Etiqueta para identificarlo

    // Bandera del nivel 2
    QPixmap pixRuletaVid("C:/Users/Lenovo/Downloads/banderaRuleta.png");
    QGraphicsPixmapItem* ruletaBandera = escenario->scene->addPixmap(
        pixRuletaVid.scaled(140, 140, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    ruletaBandera->setPos(220, 115);


    // ---- NIVEL 3: BATALLA ----
    QPixmap pixBatalla("C:/Users/Lenovo/Downloads/objetoBatallaMapa.png");
    QGraphicsPixmapItem* batalla = escenario->scene->addPixmap(
        pixBatalla.scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    batalla->setPos(530, 430);
    batalla->setData(0, "nivel3"); // 🔹 Etiqueta para identificarlo

    // Bandera del nivel 3
    QPixmap pixBatallaVid("C:/Users/Lenovo/Downloads/banderaCombate.png");
    QGraphicsPixmapItem* batallaBandera = escenario->scene->addPixmap(
        pixBatallaVid.scaled(140, 140, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    batallaBandera->setPos(640, 420);


    // ---- NIVEL 4: CASA ----
    QPixmap pixCasa("C:/Users/Lenovo/Downloads/objetoCasaMapa.png");
    QGraphicsPixmapItem* casa = escenario->scene->addPixmap(
        pixCasa.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    casa->setPos(650, 140);
    casa->setData(0, "nivel4"); // 🔹 Etiqueta para identificarlo

    // Bandera del nivel 4
    QPixmap pixCasaVid("C:/Users/Lenovo/Downloads/banderaCasa.png");
    QGraphicsPixmapItem* casaBandera = escenario->scene->addPixmap(
        pixCasaVid.scaled(140, 140, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    casaBandera->setPos(800, 160);


    // ---- Agregar a la lista de interacciones ----
    escenario->zonasInteractivas.append(video);
    escenario->zonasInteractivas.append(ruleta);
    escenario->zonasInteractivas.append(batalla);
    escenario->zonasInteractivas.append(casa);
}



