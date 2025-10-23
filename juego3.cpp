#include "juego3.h"
#include "ui_juego3.h"

juego3::juego3(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::juego3)
    ,escenario(new Escenario(this))
{
    ui->setupUi(this);
    setFocusPolicy(Qt::StrongFocus);
}

juego3::~juego3()
{
    delete ui;
}

void juego3::inicializarNivel()
{
    // Configurar el escenario dentro del QGraphicsView del UI
    QPixmap pixFondo("C:/Users/Lenovo/Downloads/fondo.png");
    escenario->inicializar(ui->graphicsViewJ3, pixFondo);

    ui->graphicsViewJ3->setFocusPolicy(Qt::NoFocus);

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

    objetosInteractivos();

    escenario->crearPersonaje(spritesDer, spritesIzq, spritesArriba, QPointF(350, 450));
    this->setFocus();

}

void juego3::keyPressEvent(QKeyEvent *event)
{
    if(escenario) {
        escenario->manejarTecla(event);
    }

    if (event->key() == Qt::Key_Escape) {
        emit volverARuleta();
    }

}

void juego3::objetosInteractivos() {
    QPixmap pixcanasta1("C:/Users/Lenovo/Downloads/canasta.png");
    QGraphicsPixmapItem* c1 = escenario->scene->addPixmap(
        pixcanasta1.scaled(250, 250, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    c1->setPos(220, 450);

    QPixmap pixcanasta2("C:/Users/Lenovo/Downloads/canasta.png");
    QGraphicsPixmapItem* c2 = escenario->scene->addPixmap(
        pixcanasta2.scaled(250, 250, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    c2->setPos(650, 450);
}

