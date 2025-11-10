#include "nivel1video.h"
#include "ui_nivel1video.h"
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QVBoxLayout>
#include <QAudioOutput>
#include <QMediaDevices>
#include <QAudioDevice>



nivel1Video::nivel1Video(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::nivel1Video)
    , escenario(new Escenario(this))
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

void nivel1Video::keyPressEvent(QKeyEvent *event)
{
    if (escenario) {
        escenario->manejarTecla(event);
    }

    if (event->key() == Qt::Key_Escape) {
        emit volverAlMapa();
    }

    if (event->key() == Qt::Key_E) {
        // Si el panel existe y está visible, lo ocultamos
        if (panelInstrucciones && panelInstrucciones->isVisible()) {
            panelInstrucciones->hide();
        }
        // Si está cerca del cuadro, lo mostramos
        else if (personajeCercaDeCuadro()) {
            mostrarPanelInstrucciones();
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

void nivel1Video::objetosInteractivos()
{
    cuadro = escenario->scene->addRect(400, 340, 150, 150, Qt::NoPen);
    cuadro->setBrush(Qt::transparent);
}


void nivel1Video::mostrarPanelInstrucciones()
{
    if (panelInstrucciones) {
        if (panelInstrucciones->isVisible()) {
            panelInstrucciones->hide();
        } else {
            panelInstrucciones->show();
        }
        return;
    }

    panelInstrucciones = new QFrame(this);
    panelInstrucciones->setGeometry(200, 80, 700, 500);
    panelInstrucciones->setStyleSheet(
        "background-color: rgba(255, 255, 255, 230);"
        "border: 3px solid #8B6F47;"
        "border-radius: 15px;"
        );

    QVBoxLayout* layout = new QVBoxLayout(panelInstrucciones);
    layout->setContentsMargins(10, 10, 10, 10);

    QVideoWidget* videoWidget = new QVideoWidget(panelInstrucciones);
    layout->addWidget(videoWidget);

    // 🔧 SOLUCIÓN: Configurar dispositivo de audio explícitamente
    QMediaPlayer* player = new QMediaPlayer(panelInstrucciones);
    QAudioOutput* audioOutput = new QAudioOutput(panelInstrucciones);

    // Obtener el dispositivo de audio por defecto
    QAudioDevice defaultDevice = QMediaDevices::defaultAudioOutput();
    if (defaultDevice.isNull()) {
        qWarning() << "No hay dispositivo de audio disponible";
    } else {
        audioOutput->setDevice(defaultDevice);
    }

    audioOutput->setVolume(1.0);
    player->setAudioOutput(audioOutput);
    player->setVideoOutput(videoWidget);
    player->setSource(QUrl::fromLocalFile("C:/Users/Lenovo/Downloads/videoNivel1.mp4"));
    player->play();

    panelInstrucciones->show();
}
