#include "juego2.h"
#include "ui_juego2.h"

juego2::juego2(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::juego2)
    ,escenario(new Escenario(this))
{
    ui->setupUi(this);
    setFocusPolicy(Qt::StrongFocus);
}

juego2::~juego2()
{
    delete ui;
}

void juego2::inicializarNivel()
{
    // Configurar el escenario dentro del QGraphicsView del UI
    QPixmap pixFondo("C:/Users/Lenovo/Downloads/fondo.png");
    escenario->inicializar(ui->graphicsViewJ2, pixFondo);

    ui->graphicsViewJ2->setFocusPolicy(Qt::NoFocus);

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

void juego2::keyPressEvent(QKeyEvent *event)
{
    if(escenario) {
        escenario->manejarTecla(event);
    }

    if (event->key() == Qt::Key_Escape) {
        emit volverARuleta();
    }
}

void juego2::objetosInteractivos() {
    QPixmap burbuja1("C:/Users/Lenovo/Downloads/burbuja.png");
    QGraphicsPixmapItem* b1 = escenario->scene->addPixmap(
        burbuja1.scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    b1->setPos(150, 90);

    QPixmap burbuja2("C:/Users/Lenovo/Downloads/burbuja.png");
    QGraphicsPixmapItem* b2 = escenario->scene->addPixmap(
        burbuja2.scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    b2->setPos(350, 150);

    QPixmap burbuja3("C:/Users/Lenovo/Downloads/burbuja.png");
    QGraphicsPixmapItem* b3 = escenario->scene->addPixmap(
        burbuja3.scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    b3->setPos(550, 70);

    QPixmap burbuja4("C:/Users/Lenovo/Downloads/burbuja.png");
    QGraphicsPixmapItem* b4 = escenario->scene->addPixmap(
        burbuja4.scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    b4->setPos(750, 180);
}

