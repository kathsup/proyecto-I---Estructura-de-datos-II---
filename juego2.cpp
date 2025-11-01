#include "juego2.h"
#include "ui_juego2.h"
#include <QPropertyAnimation>
#include <QTimer>
#include <QRandomGenerator>
#include <QVariantAnimation>

juego2::juego2(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::juego2)
    , escenario(new Escenario(this))
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

void juego2::objetosInteractivos()
{
    QString ruta = "C:/Users/Lenovo/Downloads/burbuja.png";
    QList<QPointF> posiciones = {
        {150, 90}, {350, 150}, {550, 70}, {750, 180}
    };

    for (const QPointF &pos : posiciones) {
        QGraphicsPixmapItem* burbuja = escenario->scene->addPixmap(
            QPixmap(ruta).scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation)
            );
        burbuja->setPos(pos);
        animarBurbuja(burbuja, 20); // flotan 20 px arriba y abajo
    }
}



// 🔹 Animación para que las burbujas flotan suavemente
void juego2::animarBurbuja(QGraphicsPixmapItem* burbuja, int desplazamiento)
{
    if (!burbuja) return;

    QVariantAnimation* anim = new QVariantAnimation(this);
    anim->setDuration(2000 + QRandomGenerator::global()->bounded(2000)); // duración variable
    anim->setStartValue(-desplazamiento);
    anim->setEndValue(desplazamiento);
    anim->setEasingCurve(QEasingCurve::SineCurve);
    anim->setLoopCount(-1); // infinito

    QPointF posInicial = burbuja->pos();

    connect(anim, &QVariantAnimation::valueChanged, [burbuja, posInicial](const QVariant &value){
        qreal yOffset = value.toReal();
        burbuja->setPos(posInicial.x(), posInicial.y() + yOffset);
    });

    anim->start(QAbstractAnimation::DeleteWhenStopped);
}
