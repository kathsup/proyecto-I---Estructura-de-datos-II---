#include "juego4.h"
#include "ui_juego4.h"

juego4::juego4(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::juego4)
    ,escenario(new Escenario(this))
{
    ui->setupUi(this);
    setFocusPolicy(Qt::StrongFocus);
}

juego4::~juego4()
{
    delete ui;
}

void juego4::inicializarNivel()
{
    // Configurar el escenario dentro del QGraphicsView del UI
    QPixmap pixFondo("C:/Users/Lenovo/Downloads/fondoMini4.png");
    escenario->inicializar(ui->graphicsViewJ4, pixFondo);

    ui->graphicsViewJ4->setFocusPolicy(Qt::NoFocus);

    // Crear el personaje
    QVector<QPixmap> spritesDer = {
        QPixmap("C:/Users/Lenovo/Downloads/hada1AD.png").scaled(90,90, Qt::KeepAspectRatio, Qt::SmoothTransformation),
        QPixmap("C:/Users/Lenovo/Pictures/hada2AD.png").scaled(90,90, Qt::KeepAspectRatio, Qt::SmoothTransformation),
        QPixmap("C:/Users/Lenovo/Downloads/hada3AD.png").scaled(90,90, Qt::KeepAspectRatio, Qt::SmoothTransformation)
    };

    QVector<QPixmap> spritesIzq = {
        QPixmap("C:/Users/Lenovo/Pictures/hada1Iz.png").scaled(90,90, Qt::KeepAspectRatio, Qt::SmoothTransformation),
        QPixmap("C:/Users/Lenovo/Pictures/hada2Iz.png").scaled(90,90, Qt::KeepAspectRatio, Qt::SmoothTransformation),
        QPixmap("C:/Users/Lenovo/Pictures/hada3Iz.png").scaled(90,90, Qt::KeepAspectRatio, Qt::SmoothTransformation)
    };

    QVector<QPixmap> spritesArriba = {
        QPixmap("C:/Users/Lenovo/Downloads/hada4Ar.png").scaled(90,90, Qt::KeepAspectRatio, Qt::SmoothTransformation),
        QPixmap("C:/Users/Lenovo/Downloads/hada5Ar.png").scaled(90,90, Qt::KeepAspectRatio, Qt::SmoothTransformation)
    };
    objetosInteractivos();
    zonasColision();


    escenario->crearPersonaje(spritesDer, spritesIzq, spritesArriba, QPointF(370, 510));
    this->setFocus();


}

void juego4::keyPressEvent(QKeyEvent *event)
{
    if(escenario) {
        escenario->manejarTecla(event);
    }

    if (event->key() == Qt::Key_Escape) {
        emit volverARuleta();
    }

}

void juego4::objetosInteractivos() {
    QPixmap pixFlor("C:/Users/Lenovo/Downloads/flor.png");
    QGraphicsPixmapItem* flor = escenario->scene->addPixmap(
        pixFlor.scaled(480, 480, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    flor->setPos(350, 115);
}


void juego4::zonasColision(){

    uno = escenario->scene->addRect(410, 590, 10, 5, Qt::NoPen);
    dos = escenario->scene->addRect(510, 530, 10, 5, Qt::NoPen);
    tres = escenario->scene->addRect(635, 450, 10, 5, Qt::NoPen);
    cuatro = escenario->scene->addRect(520, 350, 10, 5, Qt::NoPen);
    cinco = escenario->scene->addRect(650, 265, 10, 5, Qt::NoPen);
    seis = escenario->scene->addRect(560, 180, 10, 5, Qt::NoPen);

    uno->setBrush(Qt::red);
    dos->setBrush(Qt::red);
    tres->setBrush(Qt::red);
    cuatro->setBrush(Qt::red);
    cinco->setBrush(Qt::red);
    seis->setBrush(Qt::red);



}
