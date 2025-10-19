#include "nivel2ruleta.h"
#include "ui_nivel2ruleta.h"

nivel2Ruleta::nivel2Ruleta(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::nivel2Ruleta)
    ,escenario(new Escenario(this))//incluir el escenario al constructor
{
    ui->setupUi(this);
    setFocusPolicy(Qt::StrongFocus);
}

nivel2Ruleta::~nivel2Ruleta()
{
    delete ui;
}

void nivel2Ruleta::inicializarNivel()
{
    // Configurar el escenario dentro del QGraphicsView del UI
    QPixmap pixFondo("C:/Users/Lenovo/Downloads/fondo.png");
    escenario->inicializar(ui->graphicsViewRuleta, pixFondo);

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

    escenario->crearPersonaje(spritesDer, spritesIzq, spritesArriba, QPointF(300, 400));
    this->setFocus();


}

void nivel2Ruleta::keyPressEvent(QKeyEvent *event)
{
    if(escenario) {
        escenario->manejarTecla(event);
    }
}

void nivel2Ruleta::objetosInteractivos(){
    //puerta1
QPixmap pixPuertaArte("C:/Users/Lenovo/Downloads/puerta cerrada.png");
QGraphicsPixmapItem* puertaArteCerrada = escenario->scene->addPixmap(
    pixPuertaArte.scaled(260, 260, Qt::KeepAspectRatio, Qt::SmoothTransformation)
);
puertaArteCerrada->setPos(35, 210);

QPixmap pixPuertaHist("C:/Users/Lenovo/Downloads/puerta cerrada.png");
QGraphicsPixmapItem* puertaHistCerrada = escenario->scene->addPixmap(
    pixPuertaHist.scaled(260, 260, Qt::KeepAspectRatio, Qt::SmoothTransformation)
    );
puertaHistCerrada->setPos(280, 210);


QPixmap pixPuertaPoli("C:/Users/Lenovo/Downloads/puerta cerrada.png");
QGraphicsPixmapItem* puertaPoliCerrada = escenario->scene->addPixmap(
    pixPuertaPoli.scaled(260, 260, Qt::KeepAspectRatio, Qt::SmoothTransformation)
    );
puertaPoliCerrada->setPos(535, 210);

QPixmap pixPuertaCien("C:/Users/Lenovo/Downloads/puerta cerrada.png");
QGraphicsPixmapItem* puertaCienCerrada = escenario->scene->addPixmap(
    pixPuertaCien.scaled(260, 260, Qt::KeepAspectRatio, Qt::SmoothTransformation)
    );
puertaCienCerrada->setPos(775, 210);

// Lo agregas a la lista de interacciones
escenario->zonasInteractivas.append(puertaArteCerrada);
escenario->zonasInteractivas.append(puertaHistCerrada);
escenario->zonasInteractivas.append(puertaPoliCerrada);
escenario->zonasInteractivas.append(puertaCienCerrada);


}








