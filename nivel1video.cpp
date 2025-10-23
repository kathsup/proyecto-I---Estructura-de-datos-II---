#include "nivel1video.h"
#include "ui_nivel1video.h"

nivel1Video::nivel1Video(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::nivel1Video)
    ,escenario(new Escenario(this))
{
    ui->setupUi(this);
    setFocusPolicy(Qt::StrongFocus);
}

nivel1Video::~nivel1Video()
{
    delete ui;
}

void nivel1Video::inicializarNivel()
{
    // Configurar el escenario dentro del QGraphicsView del UI
    QPixmap pixFondo("C:/Users/Lenovo/Downloads/fondo.png");
    escenario->inicializar(ui->graphicsViewVideo, pixFondo);

    ui->graphicsViewVideo->setFocusPolicy(Qt::NoFocus);

    // Crear el personaje
   /* QVector<QPixmap> spritesDer = {
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
    };*/
    objetosInteractivos();



    //escenario->crearPersonaje(spritesDer, spritesIzq, spritesArriba, QPointF(350, 450));
    this->setFocus();


    //objetosInteractivos();
}

void nivel1Video::keyPressEvent(QKeyEvent *event)
{
    if(escenario) {
        escenario->manejarTecla(event);
    }

    if (event->key() == Qt::Key_Escape) {
        emit volverAlMapa();
    }
}

void nivel1Video::objetosInteractivos() {

        QPixmap pixBaseVid("C:/Users/Lenovo/Downloads/video.png");
        QGraphicsPixmapItem* baseVideo = escenario->scene->addPixmap(
            pixBaseVid.scaled(500, 500, Qt::KeepAspectRatio, Qt::SmoothTransformation)
            );
        baseVideo->setPos(300, 160);
        escenario->zonasInteractivas.append(baseVideo);
    }



