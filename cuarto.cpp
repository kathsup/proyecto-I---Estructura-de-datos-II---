#include "cuarto.h"
#include "ui_cuarto.h"

Cuarto::Cuarto(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Cuarto)
    ,escenario(new Escenario(this))
{
    ui->setupUi(this);
    setFocusPolicy(Qt::StrongFocus);
}

Cuarto::~Cuarto()
{
    delete ui;
}

void Cuarto::inicializarNivel()
{
    // Configurar el escenario dentro del QGraphicsView del UI
    QPixmap pixFondo("C:/Users/Lenovo/Downloads/cuarto.png");
    escenario->inicializar(ui->graphicsViewCuarto, pixFondo);

    ui->graphicsViewCuarto->setFocusPolicy(Qt::NoFocus);

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

 //personaje = new Personaje(spritesDer, spritesIzq, spritesArriba, QPointF(350, 450));

    // ✅ Agregarlo al escenario
    //escenario->agregarPersonaje(personaje);
    escenario->crearPersonaje(spritesDer, spritesIzq, spritesArriba, QPointF(650, 490));

    objetosInteractivos();

    this->setFocus();


}

void Cuarto::keyPressEvent(QKeyEvent *event)
{
    if(escenario) {
        escenario->manejarTecla(event);
    }

    if (event->key() == Qt::Key_Escape || event->key() == Qt::Key_E) {
        emit volverAlLobby();
    }
}

void Cuarto::objetosInteractivos() {
    QPixmap pixLibrero("C:/Users/Lenovo/Downloads/librero.png");
    librero = escenario->scene->addPixmap(
        pixLibrero.scaled(250, 250, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    librero->setPos(0, 190);

    QPixmap pixPlantaIzq("C:/Users/Lenovo/Downloads/plantaTecho.png");
    plantaIzq = escenario->scene->addPixmap(
        pixPlantaIzq.scaled(250, 250, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    plantaIzq->setPos(145, 0);

    QPixmap pixPizarra("C:/Users/Lenovo/Downloads/pizarraa.png");
    pizarra = escenario->scene->addPixmap(
        pixPizarra.scaled(250, 250, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    pizarra->setPos(390, 65);

    QPixmap pixPlantaDer("C:/Users/Lenovo/Downloads/plantaTecho.png");
    plantaDer = escenario->scene->addPixmap(
        pixPlantaDer.scaled(220, 220, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    plantaDer->setPos(705, 0);

    QPixmap pixSolar("C:/Users/Lenovo/Downloads/sistema solar.png");
    sisSolar = escenario->scene->addPixmap(
        pixSolar.scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    sisSolar->setPos(890, 60);

    QPixmap pixMesaIzq("C:/Users/Lenovo/Downloads/mesaCuarto.png");
    mesaIzq = escenario->scene->addPixmap(
        pixMesaIzq.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    mesaIzq->setPos(130, 500);

    QPixmap pixMesaDer("C:/Users/Lenovo/Downloads/mesaCuarto.png");
    mesaDer = escenario->scene->addPixmap(
        pixMesaDer.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    mesaDer->setPos(360, 500);

    QPixmap pixPlantaSuelo("C:/Users/Lenovo/Downloads/plantaSuelo.png");
    plantaSuelo = escenario->scene->addPixmap(
        pixPlantaSuelo.scaled(110, 110, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    plantaSuelo->setPos(650, 330);

    QPixmap pixPedestal("C:/Users/Lenovo/Downloads/pedestal.png");
    pedestal = escenario->scene->addPixmap(
        pixPedestal.scaled(115, 115, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    pedestal->setPos(880, 510);

    QPixmap pixEstelar("C:/Users/Lenovo/Downloads/mapaEstelar.png");
    estelarCompleto = escenario->scene->addPixmap(
        pixEstelar.scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    estelarCompleto->setPos(875, 440);

    QPixmap pixKant("C:/Users/Lenovo/Downloads/kant.png");
    kant = escenario->scene->addPixmap(
        pixKant.scaled(90, 90, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    kant->setPos(780, 490);



}


