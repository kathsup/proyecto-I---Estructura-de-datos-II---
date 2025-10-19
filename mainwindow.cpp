#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <QMessageBox>

#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>//para el cambio de niveles


#include <QTimer>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    iniciarConfiguracion();

    //agregar nivel 2
    nivel2 = new nivel2Ruleta(this);
    //nivel2->setObjectName("nivel2");
    ui->stackedWidget->addWidget(nivel2);

    Mapa = new mapa(this);
    ui->stackedWidget->addWidget(Mapa);

    connect(Mapa, &mapa::solicitarCambioNivel, this, [this](){
        cambiarDeNivel(nivel2); // MainWindow sabe cuál nivel mostrar
    });



}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::iniciarConfiguracion(){
    //FONDO
    QGraphicsScene *scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);


    QPixmap pixFondo("C:/Users/Lenovo/Downloads/inicioProv.jpg");

    // Escalar la imagen al tamaño del graphicsView
    pixFondo = pixFondo.scaled(ui->graphicsView->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    // Poner la imagen como fondo de la escena
    scene->setBackgroundBrush(pixFondo);

    // Ajustar el rectángulo de la escena al tamaño del graphicsView
    scene->setSceneRect(0, 0,ui->graphicsView->width(),ui->graphicsView->height());

    //quitar las barras
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //BOTON
    QPixmap pix("C:/Users/Lenovo/Pictures/btnComenzar.png");
    pix = pix.scaled(ui->btnComenzar->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    // Crear un icono y ponerlo en el botón
    ui->btnComenzar->setIcon(QIcon(pix));
    ui->btnComenzar->setIconSize(ui->btnComenzar->size());

    // Opcional: quitar texto si solo quieres la imagen
    ui->btnComenzar->setText("");

    ui->btnComenzar->setStyleSheet(
        "QPushButton {"
        "border-radius: 20px;"               // esquinas redondeadas
        "border: 4px solid #e5989e;"            // borde opcional
        "background-color: white;"            // color de fondo si quieres
        "}"
        "QPushButton:hover {"
        "border: 4px solid #b84d5e;"            // cambio de borde al pasar mouse
        "background-color: #e0e0e0;"         // cambio de fondo al pasar mouse
        "}"
        );

    //line edit
    ui->lineNombre->setPlaceholderText("                           INGRESA TU USUARIO");
}

void MainWindow::on_btnComenzar_clicked()
{
    QString usser = ui->lineNombre->text();

    // 1) Validar que no esté vacío
    if (usser.isEmpty()) {
        QMessageBox::warning(this, "Nombre requerido", "Por favor ingresa un nombre de usuario.");
        ui->lineNombre->setFocus();
        return;
    }

    // 2) Buscar si ya existe
    Jugador* encontrado = buscarJugador(usser);

    if (encontrado) {
        // 3a) Si existe, lo cargamos como jugadorActual
        jugadorActual = *encontrado; // copia simple (puedes guardar índice si prefieres)
        QMessageBox::information(this, "Bienvenido", "Jugador cargado: " + jugadorActual.nombre
                                                         + "\nPuntos: " + QString::number(jugadorActual.puntos));
    } else {
        // 3b) Si no existe, lo creamos y lo agregamos
        agregarJugador(usser);
        // obtener puntero al nuevo jugador (último elemento)
        jugadorActual = jugadores.last();
        QMessageBox::information(this, "Nuevo jugador", "Se ha creado el jugador: " + jugadorActual.nombre);
    }

    // 4) Limpiar input, ir al mapa (ejemplo) y mantener foco si quieres
    ui->lineNombre->clear();
    ui->stackedWidget->setCurrentWidget(Mapa);
    Mapa->inicializarNivel();
}

Jugador* MainWindow::buscarJugador(const QString &nombre) {
    for (auto &j : jugadores) {            // auto & para no copiar
        if (j.nombre == nombre) {
            return &j;
        }
    }
    return nullptr;
}

void MainWindow::agregarJugador(const QString &nombre) {
    // Antes de agregar, puedes volver a verificar que no exista (seguridad)
    if (buscarJugador(nombre) != nullptr) return;
    jugadores.append(Jugador(nombre, 0));
}


void MainWindow::cambiarDeNivel(QWidget* nuevoNivel)
{
    QWidget *overlay = new QWidget(this);
    overlay->setStyleSheet("background-color: black;");
    overlay->setGeometry(ui->stackedWidget->geometry());
    overlay->raise();
    overlay->show();

    // Fade in (a negro)
    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(overlay);
    overlay->setGraphicsEffect(effect);

    QPropertyAnimation *fadeOut = new QPropertyAnimation(effect, "opacity");
    fadeOut->setDuration(400);
    fadeOut->setStartValue(0);
    fadeOut->setEndValue(1);

    // Cuando termina el fade a negro, cambiamos la escena
    connect(fadeOut, &QPropertyAnimation::finished, this, [=]() {
        ui->stackedWidget->setCurrentWidget(nuevoNivel);

        if (auto* nivel = qobject_cast<nivel2Ruleta*>(nuevoNivel)) {
            nivel->inicializarNivel();
        }

        // Fade out (de negro al juego)
        QPropertyAnimation *fadeIn = new QPropertyAnimation(effect, "opacity");
        fadeIn->setDuration(400);
        fadeIn->setStartValue(1);
        fadeIn->setEndValue(0);
        connect(fadeIn, &QPropertyAnimation::finished, overlay, &QWidget::deleteLater);
        fadeIn->start(QAbstractAnimation::DeleteWhenStopped);
    });

    fadeOut->start(QAbstractAnimation::DeleteWhenStopped);
}
