#include "mapa.h"
#include "ui_mapa.h"
#include "nivel2ruleta.h"


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

void mapa::keyPressEvent(QKeyEvent *event)
{
    if(escenario) {
        escenario->manejarTecla(event);
    }

    if(event->key() == Qt::Key_G) {
        for(auto* obj : escenario->zonasInteractivas) {
            if(escenario->personaje->collidingItems().contains(obj)) {
                emit solicitarCambioNivel(); // sin pasar nada
                break;
            }
        }
    }
}


void mapa::objetosInteractivos(){

    //nivel 1 video
    QPixmap pixVideo("C:/Users/Lenovo/Downloads/objetoVideoMapa.png");
    QGraphicsPixmapItem* video = escenario->scene->addPixmap(
        pixVideo.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    video->setPos(140, 300);


    //nivel 2 ruleta
    QPixmap pixRuleta("C:/Users/Lenovo/Downloads/objetoRuletaMapa.png");
    QGraphicsPixmapItem* ruleta = escenario->scene->addPixmap(
        pixRuleta.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
            ruleta->setPos(250, 85);


    //nivel 3 batalla
    QPixmap pixBatalla("C:/Users/Lenovo/Downloads/objetoBatallaMapa.png");
    QGraphicsPixmapItem* batalla = escenario->scene->addPixmap(
        pixBatalla.scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    batalla->setPos(530, 430);


    //nivel 4 casa
    QPixmap pixCasa("C:/Users/Lenovo/Downloads/objetoCasaMapa.png");
    QGraphicsPixmapItem* casa = escenario->scene->addPixmap(
        pixCasa.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    casa->setPos(650, 140);


    // agregar a lista de interacciones
    escenario->zonasInteractivas.append(video);
    escenario->zonasInteractivas.append(ruleta);
    escenario->zonasInteractivas.append(batalla);
    escenario->zonasInteractivas.append(casa);



}



