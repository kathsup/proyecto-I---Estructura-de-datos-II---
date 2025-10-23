#include "nivel3batalla.h"
#include "ui_nivel3batalla.h"

nivel3Batalla::nivel3Batalla(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::nivel3Batalla)
    ,escenario(new Escenario(this))
{
    ui->setupUi(this);
    setFocusPolicy(Qt::StrongFocus);
}

nivel3Batalla::~nivel3Batalla()
{
    delete ui;
}

void nivel3Batalla::inicializarNivel()
{
    // Configurar el escenario dentro del QGraphicsView del UI
    QPixmap pixFondo("C:/Users/Lenovo/Downloads/fondo.png");
    escenario->inicializar(ui->graphicsViewBatalla, pixFondo);

    ui->graphicsViewBatalla->setFocusPolicy(Qt::NoFocus);

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



  //  escenario->crearPersonaje(spritesDer, spritesIzq, spritesArriba, QPointF(350, 450));
    this->setFocus();


}

void nivel3Batalla::keyPressEvent(QKeyEvent *event)
{
    if(escenario) {
        escenario->manejarTecla(event);
    }

    if (event->key() == Qt::Key_Escape) {
        emit volverAlMapa();
    }
}

void nivel3Batalla::objetosInteractivos() {
    QPixmap pixGatoRojo("C:/Users/Lenovo/Downloads/gatoRojo.png");
    QGraphicsPixmapItem* gatoRojo = escenario->scene->addPixmap(
        pixGatoRojo.scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    gatoRojo->setPos(350, 450);

    QPixmap pixGatoBlanco("C:/Users/Lenovo/Downloads/gatoBlanco.png");
    QGraphicsPixmapItem* gatoBlanco = escenario->scene->addPixmap(
        pixGatoBlanco.scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    gatoBlanco->setPos(650, 450);
}


