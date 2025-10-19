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

escenario->crearPersonaje(spritesDer, spritesIzq, spritesArriba, QPointF(300, 400));
this->setFocus();


}

void mapa::keyPressEvent(QKeyEvent *event)
{
    if(escenario) {
        escenario->manejarTecla(event);
    }

    // Suponiendo que 'G' es la tecla para entrar al nivel
    if(event->key() == Qt::Key_G) {
        // Verificar colisión con algún objeto interactivo
        for(auto* obj : escenario->zonasInteractivas) {
            if(escenario->personaje->collidingItems().contains(obj)) {
                emit solicitarCambioNivel();
                break;
            }
        }
    }
}

void mapa::objetosInteractivos(){
    //puerta1
    QPixmap pixRuleta("C:/Users/Lenovo/Downloads/objetoRuletaMapa.png");
    QGraphicsPixmapItem* ruleta = escenario->scene->addPixmap(
        pixRuleta.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
            ruleta->setPos(250, 85);


    // Lo agregas a la lista de interacciones
    escenario->zonasInteractivas.append(ruleta);


}



