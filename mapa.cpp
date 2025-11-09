#include "mapa.h"
#include "ui_mapa.h"
#include "nivel2ruleta.h"
#include "nivel1video.h"
#include "nivel3batalla.h"
#include "nivel4casa.h"


mapa::mapa(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::mapa)
    ,escenario(new Escenario(this))
{
    ui->setupUi(this);
    botones();
    crearPanelRanking();
    crearPanelInstrucciones();
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


escenario->crearPersonaje(spritesDer, spritesIzq, spritesArriba, QPointF(450, 300));
this->setFocus();


}


void mapa::botones()
{
    // 🔹 Botón de Ranking
    ui->btnRanking->setStyleSheet(
        "QPushButton {"
        "   border-image: url('C:/Users/Lenovo/Downloads/btnRanking.png');"
        "   border-radius: 50px;"       // Ajusta según el tamaño del botón
        "   background-color: transparent;"
        "}"
        "QPushButton:hover {"
        "   border-image: url('C:/Users/Lenovo/Downloads/btnRanking.png');"
        "   opacity: 0.4;"
        "}"
        );

    // 🔹 Botón de Instrucciones
    ui->btnInstrucciones->setStyleSheet(
        "QPushButton {"
        "   border-image: url('C:/Users/Lenovo/Downloads/btnInstruccion.png');"
        "   border-radius: 50px;"
        "   background-color: transparent;"
        "}"
        "QPushButton:hover {"
        "   border-image: url('C:/Users/Lenovo/Downloads/btnInstruccion.png');"
        "   opacity: 0.4;"
        "}"
        );
}


void mapa::keyPressEvent(QKeyEvent *event)
{
    if (escenario) {
        escenario->manejarTecla(event);
    }
    if (event->key() == Qt::Key_G) {
        for (auto* obj : escenario->zonasInteractivas) {
            if (escenario->personaje->collidingItems().contains(obj)) {
                QString etiqueta = obj->data(0).toString();

                if (etiqueta == "nivel1") {
                    emit solicitarCambioNivel(1);
                } else if (etiqueta == "nivel2") {
                    emit solicitarCambioNivel(2);
                } else if (etiqueta == "nivel3") {
                    emit solicitarCambioNivel(3);
                } else if (etiqueta == "nivel4") {
                    emit solicitarCambioNivel(4);
                }
                break;
            }
        }
    }
}



void mapa::objetosInteractivos() {

    // ---- NIVEL 1: VIDEO ----
    QPixmap pixVideo("C:/Users/Lenovo/Downloads/objetoVideoMapa.png");
    objetoNivel1 = escenario->scene->addPixmap(
        pixVideo.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    objetoNivel1->setPos(140, 320);
    objetoNivel1->setData(0, "nivel1");
    objetoNivel1->setOpacity(1.0);  // Siempre visible

    // Bandera del nivel 1
    QPixmap pixBanderaVid("C:/Users/Lenovo/Downloads/banderaVid.png");
    QGraphicsPixmapItem* videoBandera = escenario->scene->addPixmap(
        pixBanderaVid.scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    videoBandera->setPos(260, 310);


    // ---- NIVEL 2: RULETA ----
    QPixmap pixRuleta("C:/Users/Lenovo/Downloads/objetoRuletaMapa.png");
    objetoNivel2 = escenario->scene->addPixmap(
        pixRuleta.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    objetoNivel2->setPos(250, 85);
    objetoNivel2->setData(0, "nivel2");
    objetoNivel2->setOpacity(1.0);  // Siempre visible

    // Bandera del nivel 2
    QPixmap pixRuletaVid("C:/Users/Lenovo/Downloads/banderaRuleta.png");
    QGraphicsPixmapItem* ruletaBandera = escenario->scene->addPixmap(
        pixRuletaVid.scaled(140, 140, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    ruletaBandera->setPos(220, 115);


    // ---- NIVEL 3: BATALLA ----
    QPixmap pixBatalla("C:/Users/Lenovo/Downloads/objetoBatallaMapa.png");
    objetoNivel3 = escenario->scene->addPixmap(
        pixBatalla.scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    objetoNivel3->setPos(530, 430);
    objetoNivel3->setData(0, "nivel3");

    // Establecer opacidad según si está desbloqueado
    if (grafoNiveles && grafoNiveles->estaDesbloqueado(3)) {
        objetoNivel3->setOpacity(1.0);  // Totalmente visible
    } else {
        objetoNivel3->setOpacity(0.4);  // Oscurecido
    }

    // Bandera del nivel 3
    QPixmap pixBatallaVid("C:/Users/Lenovo/Downloads/banderaCombate.png");
    QGraphicsPixmapItem* batallaBandera = escenario->scene->addPixmap(
        pixBatallaVid.scaled(140, 140, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    batallaBandera->setPos(640, 420);


    // ---- NIVEL 4: CASA ----
    QPixmap pixCasa("C:/Users/Lenovo/Downloads/objetoCasaMapa.png");
    objetoNivel4 = escenario->scene->addPixmap(
        pixCasa.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    objetoNivel4->setPos(650, 140);
    objetoNivel4->setData(0, "nivel4");

    // Establecer opacidad según si está desbloqueado
    if (grafoNiveles && grafoNiveles->estaDesbloqueado(4)) {
        objetoNivel4->setOpacity(1.0);  // Totalmente visible
    } else {
        objetoNivel4->setOpacity(0.4);  // Oscurecido
    }

    // Bandera del nivel 4
    QPixmap pixCasaVid("C:/Users/Lenovo/Downloads/banderaCasa.png");
    QGraphicsPixmapItem* casaBandera = escenario->scene->addPixmap(
        pixCasaVid.scaled(140, 140, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    casaBandera->setPos(800, 160);


    // ---- Agregar a la lista de interacciones ----
    escenario->zonasInteractivas.append(objetoNivel1);
    escenario->zonasInteractivas.append(objetoNivel2);
    escenario->zonasInteractivas.append(objetoNivel3);
    escenario->zonasInteractivas.append(objetoNivel4);
}

//=====botones========0

void mapa::on_btnInstrucciones_clicked() {
    if (panelRanking) panelRanking->hide();

    if (panelInstrucciones) {
        bool estabaVisible = panelInstrucciones->isVisible();
        panelInstrucciones->setVisible(!estabaVisible);

        // ✅ Si se cierra el panel, restaurar el foco
        if (estabaVisible) {
            this->setFocus();
        }
    }
}

void mapa::crearPanelInstrucciones() {
    panelInstrucciones = new QFrame(this);
    panelInstrucciones->setGeometry(150, 100, 500, 400);
    panelInstrucciones->setStyleSheet(
        "background-color: rgba(255, 255, 255, 220);"
        "border: 3px solid #8B6F47;"
        "border-radius: 15px;"
        );

    QLabel* lblTitulo = new QLabel("📖 Instrucciones", panelInstrucciones);
    lblTitulo->setGeometry(120, 20, 260, 50);
    lblTitulo->setAlignment(Qt::AlignCenter);
    lblTitulo->setStyleSheet("font-size: 22px; font-weight: bold; color: #8B6F47;");

    panelInstrucciones->hide();
}


void mapa::crearPanelRanking() {
    panelRanking = new QFrame(this);
    panelRanking->setGeometry(200, 80, 600, 500); // ⬅️ MÁS GRANDE
    panelRanking->setStyleSheet(
        "background-color: rgba(255, 255, 255, 240);"
        "border: 3px solid #8B6F47;"
        "border-radius: 15px;"
        );

    QLabel* lblTitulo = new QLabel("🏆 RANKING GENERAL", panelRanking);
    lblTitulo->setGeometry(150, 15, 300, 40);
    lblTitulo->setAlignment(Qt::AlignCenter);
    lblTitulo->setStyleSheet("font-size: 22px; font-weight: bold; color: #8B6F47;");

    // ⭐ CREAR SCROLL AREA PARA TODOS LOS JUGADORES
    QScrollArea* scrollArea = new QScrollArea(panelRanking);
    scrollArea->setGeometry(20, 70, 560, 400);
    scrollArea->setStyleSheet("background-color: transparent; border: none;");

    QWidget* contenedor = new QWidget();
    scrollArea->setWidget(contenedor);
    scrollArea->setWidgetResizable(true);

    panelRanking->hide();
}

void mapa::actualizarRanking(const QVector<Jugador>& ranking) {
    // Buscar el scroll area
    QScrollArea* scrollArea = panelRanking->findChild<QScrollArea*>();
    if (!scrollArea) return;

    // Limpiar contenido anterior
    QWidget* contenedor = new QWidget();
    scrollArea->setWidget(contenedor);

    // ⭐ MOSTRAR TODOS LOS JUGADORES
    int y = 10;
    for (int i = 0; i < ranking.size(); i++) {
        QString texto = QString("%1. %2 - %3 pts")
        .arg(i + 1)
            .arg(ranking[i].getNombre())
            .arg(ranking[i].getPuntos());

        QLabel* lblRank = new QLabel(texto, contenedor);
        lblRank->setGeometry(20, y, 500, 30);

        // ⭐ RESALTAR AL JUGADOR ACTUAL
        if (ranking[i].getNombre() == nombreJugadorActual) {
            lblRank->setStyleSheet(
                "font-size: 16px; "
                "color: white; "
                "background-color: #f39c12; "
                "font-weight: bold; "
                "padding: 5px; "
                "border-radius: 5px;"
                );
        } else {
            lblRank->setStyleSheet("font-size: 16px; color: #2c3e50;");
        }

        lblRank->show();
        y += 40;
    }

    // Ajustar tamaño del contenedor
    contenedor->setMinimumHeight(y);
}

void mapa::on_btnRanking_clicked() {
    if (panelInstrucciones) panelInstrucciones->hide();

    if (panelRanking->isVisible()) {
        panelRanking->hide();
        // ✅ Restaurar el foco al cerrar
        this->setFocus();
    } else {
        if (rankingRef) {
            actualizarRanking(rankingRef->obtenerRanking());
        }
        panelRanking->show();
    }
}

void mapa::actualizarVisualesNiveles() {
    if (!grafoNiveles) return;

    // Actualizar nivel 3
    if (objetoNivel3) {
        if (grafoNiveles->estaDesbloqueado(3)) {
            objetoNivel3->setOpacity(1.0);  // Ahora se ve completamente
        } else {
            objetoNivel3->setOpacity(0.4);  // Oscurecido
        }
    }

    // Actualizar nivel 4
    if (objetoNivel4) {
        if (grafoNiveles->estaDesbloqueado(4)) {
            objetoNivel4->setOpacity(1.0);  // Ahora se ve completamente
        } else {
            objetoNivel4->setOpacity(0.4);  // Oscurecido
        }
    }
}
