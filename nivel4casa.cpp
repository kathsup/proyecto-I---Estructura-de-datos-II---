#include "nivel4casa.h"
#include "ui_nivel4casa.h"

nivel4Casa::nivel4Casa(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::nivel4Casa)
    ,escenario(new Escenario(this))
{
    ui->setupUi(this);
    setFocusPolicy(Qt::StrongFocus);
}

nivel4Casa::~nivel4Casa()
{
    delete ui;
}

void nivel4Casa::inicializarNivel()
{
    // Configurar el escenario dentro del QGraphicsView del UI
    QPixmap pixFondo("C:/Users/Lenovo/Downloads/lobby.png");
    escenario->inicializar(ui->graphicsViewCasa, pixFondo);

    ui->graphicsViewCasa->setFocusPolicy(Qt::NoFocus);

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

void nivel4Casa::keyPressEvent(QKeyEvent *event)
{
    if(escenario) {
        escenario->manejarTecla(event);
    }

    if (event->key() == Qt::Key_G) {
        for (auto* obj : escenario->zonasInteractivas) {
            if (escenario->personaje->collidingItems().contains(obj)) {
                QString etiqueta = obj->data(0).toString();

                if (etiqueta == "cuarto") {
                    emit solicitarCambioHabitacion("cuarto");
                } else if (etiqueta == "laboratorio") {
                    emit solicitarCambioHabitacion("laboratorio");
                }
                break;
            }
        }
    }

    if (event->key() == Qt::Key_Escape) {
        emit volverAlMapa();
    }
}

/*void nivel4Casa::objetosInteractivos() {
    QPixmap pixPuertaCuarto("C:/Users/Lenovo/Downloads/puertaCasa.png");
    QGraphicsPixmapItem* cuarto = escenario->scene->addPixmap(
        pixPuertaCuarto.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    cuarto->setPos(270, 330);

    QPixmap pixPuertaLab("C:/Users/Lenovo/Downloads/puertaCasa.png");
    QGraphicsPixmapItem* lab = escenario->scene->addPixmap(
        pixPuertaLab.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    lab->setPos(660, 330);


    // ---- Agregar a la lista de interacciones ----
    escenario->zonasInteractivas.append(cuarto);
    escenario->zonasInteractivas.append(lab);
}*/

void nivel4Casa::objetosInteractivos() {
    QPixmap pixPuertaCuarto("C:/Users/Lenovo/Downloads/puertaCasa.png");
    QGraphicsPixmapItem* cuarto = escenario->scene->addPixmap(
        pixPuertaCuarto.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    cuarto->setPos(270, 330);
    cuarto->setData(0, "cuarto"); // 🔹 Etiqueta identificadora

    QPixmap pixPuertaLab("C:/Users/Lenovo/Downloads/puertaCasa.png");
    QGraphicsPixmapItem* lab = escenario->scene->addPixmap(
        pixPuertaLab.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    lab->setPos(660, 330);
    lab->setData(0, "laboratorio"); // 🔹 Etiqueta identificadora

    // Agregar a la lista de interacciones
    escenario->zonasInteractivas.append(cuarto);
    escenario->zonasInteractivas.append(lab);
}


