#include "nivel1video.h"
#include "ui_nivel1video.h"
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QVBoxLayout>


nivel1Video::nivel1Video(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::nivel1Video)
    ,escenario(new Escenario(this))
{
    ui->setupUi(this);
    setFocusPolicy(Qt::StrongFocus);
    inicializarVideoPanel();
}

nivel1Video::~nivel1Video()
{
    delete ui;
}

void nivel1Video::inicializarNivel()
{
    // Configurar el escenario dentro del QGraphicsView del UI
    QPixmap pixFondo("C:/Users/Lenovo/Downloads/fondoVideo.png");
    escenario->inicializar(ui->graphicsViewVideo, pixFondo);

    ui->graphicsViewVideo->setFocusPolicy(Qt::NoFocus);

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



    escenario->crearPersonaje(spritesDer, spritesIzq, spritesArriba, QPointF(270, 450));
    this->setFocus();



}

/*void nivel1Video::keyPressEvent(QKeyEvent *event)
{
    if(escenario) {
        escenario->manejarTecla(event);
    }

    if (event->key() == Qt::Key_Escape) {
        emit volverAlMapa();
    }
}
*/

void nivel1Video::keyPressEvent(QKeyEvent *event)
{
    if (escenario) {
        escenario->manejarTecla(event);
    }

    if (event->key() == Qt::Key_Escape) {
        emit volverAlMapa();
    }

    // 🟢 Si presiona E, verificar si está cerca del cuadro
    if (event->key() == Qt::Key_E) {
        if (personajeCercaDeCuadro()) {
            mostrarPanelVideo();
        }
    }
}

bool nivel1Video::personajeCercaDeCuadro()
{
    if (!escenario || !escenario->personaje || !cuadro) return false;

    QPointF posJugador = escenario->personaje->pos();
    QRectF areaCuadro = cuadro->rect().translated(cuadro->pos());

    return areaCuadro.adjusted(-50, -50, 50, 50).contains(posJugador);
}


void nivel1Video::objetosInteractivos() {

    cuadro = escenario->scene->addRect(400, 340, 150, 150, Qt::NoPen);
    cuadro->setBrush(Qt::transparent);

}


/*
void nivel1Video::inicializarVideoPanel()
{
    // 🧱 Panel contenedor
    panelVideo = new QFrame(this);
    panelVideo->setGeometry(0, 0, 1914, 1080);
    panelVideo->setStyleSheet(
        "background-color: rgba(0, 0, 0, 200);"  // fondo oscuro semitransparente
        "border: 3px solid #fff;"
        "border-radius: 15px;"
        );
    panelVideo->hide();
    panelVideo->raise();

    // 🎬 Crear el widget de video
    QVideoWidget *videoWidget = new QVideoWidget(panelVideo);
    videoWidget->setGeometry(100, 100, 500, 300); // centrado aproximadamente

    // 🎵 Crear el reproductor
    player = new QMediaPlayer(panelVideo);
    player->setVideoOutput(videoWidget);

    // Ruta del video
    player->setSource(QUrl::fromLocalFile("C:/Users/Lenovo/Downloads/videoNivel1.mp4"));

    // 🧭 Opcional: diseño para centrar el video con layout
    QVBoxLayout *layout = new QVBoxLayout(panelVideo);
    layout->addWidget(videoWidget, 0, Qt::AlignCenter);
    panelVideo->setLayout(layout);
}*/

void nivel1Video::inicializarVideoPanel()
{
    // 🧱 Panel contenedor
    panelVideo = new QFrame(this);
    panelVideo->setGeometry(0, 0, 1914, 1080);
    panelVideo->setStyleSheet(
        "background-color: rgba(0, 0, 0, 200);"
        "border: 3px solid #fff;"
        "border-radius: 15px;"
        );
    panelVideo->hide();
    panelVideo->raise();

    // 🎬 Crear el widget de video
    QVideoWidget *videoWidget = new QVideoWidget(panelVideo);
    videoWidget->setGeometry(707, 290, 500, 300);

    // 🔊 Crear salida de audio
    audioOutput = new QAudioOutput(panelVideo);
    audioOutput->setVolume(0.9);  // volumen 90%

    // 🎵 Crear reproductor
    player = new QMediaPlayer(panelVideo);
    player->setVideoOutput(videoWidget);
    player->setAudioOutput(audioOutput);   // ✅ Conectar audio aquí

    player->setSource(QUrl::fromLocalFile("C:/Users/Lenovo/Downloads/videoNivel1.mp4"));

    // 🧭 Centrar con layout opcional
    QVBoxLayout *layout = new QVBoxLayout(panelVideo);
    layout->addWidget(videoWidget, 0, Qt::AlignCenter);
    panelVideo->setLayout(layout);
}


void nivel1Video::mostrarPanelVideo()
{
    if (!panelVideo || !player) return;

    if (panelVideo->isVisible()) {
        player->stop();       // Detener video si se vuelve a presionar
        panelVideo->hide();
    } else {
        panelVideo->show();   // Mostrar el panel
        panelVideo->raise();
        player->play();       // Reproducir el video
    }
}

