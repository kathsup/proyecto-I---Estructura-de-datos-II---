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

    ranking = new Ranking();

    //nivel 1
    nivel1 = new nivel1Video(this);
    ui->stackedWidget->addWidget(nivel1);

    //agregar nivel 2
    nivel2 = new nivel2Ruleta(this);
    ui->stackedWidget->addWidget(nivel2);

    //agregar nivel 3
    nivel3 = new nivel3Batalla(this);
    ui->stackedWidget->addWidget(nivel3);

    //agregar nivel 4
    nivel4 = new nivel4Casa(this);
    ui->stackedWidget->addWidget(nivel4);

    //habitaciones
    lab = new Laboratorio(this);
    ui->stackedWidget->addWidget(lab);

    room = new Cuarto(this);
    ui->stackedWidget->addWidget(room);

    //juegos de la ruleta
    juegoArte = new juego1(this);
    ui->stackedWidget->addWidget(juegoArte);

    juegoHistoria = new juego2(this);
    ui->stackedWidget->addWidget(juegoHistoria);

    juegoPolitica = new juego3(this);
    ui->stackedWidget->addWidget(juegoPolitica);

    juegoCiencia = new juego4(this);
    ui->stackedWidget->addWidget(juegoCiencia);

    //vidas
    connect(juegoArte, &juego1::estrellaObtenida, nivel2, &nivel2Ruleta::marcarEstrellaObtenida);
    connect(juegoHistoria, &juego2::estrellaObtenida, nivel2, &nivel2Ruleta::marcarEstrellaObtenida);
    connect(juegoPolitica, &juego3::estrellaObtenida, nivel2, &nivel2Ruleta::marcarEstrellaObtenida);
    connect(juegoCiencia, &juego4::estrellaObtenida, nivel2, &nivel2Ruleta::marcarEstrellaObtenida);


    Mapa = new mapa(this);
    ui->stackedWidget->addWidget(Mapa);
    Mapa->setRanking(ranking);
    grafoNiveles = new GrafoNiveles(this);
    Mapa->setGrafoNiveles(grafoNiveles);

    connect(grafoNiveles, &GrafoNiveles::nivelDesbloqueado, this,
            [this](int nivelId) {
                Mapa->actualizarVisualesNiveles();
            });

    connect(Mapa, &mapa::solicitarCambioNivel, this, [this](int nivel){

        if (!grafoNiveles->estaDesbloqueado(nivel)) {
            QMessageBox::warning(this, "Nivel Bloqueado",
                                 QString("Este nivel aún está bloqueado.\n\n"
                                         "Completa los niveles anteriores para desbloquearlo."));
            return;
        }

        if (nivel == 1) {
            cambiarDeNivel(nivel1);
        } else if (nivel == 2) {
            cambiarDeNivel(nivel2);
        } else if (nivel == 3){
            cambiarDeNivel(nivel3);
        }
        else if (nivel == 4){
            cambiarDeNivel(nivel4);
        }
    });

    // Después de crear nivel4
    connect(nivel4, &nivel4Casa::solicitarCambioHabitacion, this, [this](const QString& habitacion){
        if (habitacion == "cuarto") {
            cambiarDeNivel(room); // cuando lo tengas creado
        } else if (habitacion == "laboratorio") {
            cambiarDeNivel(lab); // cuando lo tengas creado
        }
    });

    //de la ruleta a los juegos
    connect(nivel2, &nivel2Ruleta::solicitarCambioJuego, this, [this](const QString& juego){
        if (juego == "Arte") {
            cambiarDeNivel(juegoArte);
        } else if (juego == "Historia") {
            cambiarDeNivel(juegoHistoria);
        } else if (juego == "Política") {
            cambiarDeNivel(juegoPolitica);
        } else if (juego == "Ciencia") {
            cambiarDeNivel(juegoCiencia);
        }
    });

    // Conexiones para regresar al lobby
    connect(room, &Cuarto::volverAlLobby, this, [this](){
        cambiarDeNivel(nivel4);
    });

    connect(lab, &Laboratorio::volverAlLobby, this, [this](){
        cambiarDeNivel(nivel4);
    });

    //regresar al mapa
    connect(nivel1, &nivel1Video::volverAlMapa, this, [this](){
        grafoNiveles->completarNivel(1);
        cambiarDeNivel(Mapa);
    });

    connect(nivel2, &nivel2Ruleta::volverAlMapa, this, [this](){
        grafoNiveles->completarNivel(2);
        cambiarDeNivel(Mapa);
    });

    connect(nivel3, &nivel3Batalla::volverAlMapa, this, [this](){
        cambiarDeNivel(Mapa);
    });

    connect(nivel4, &nivel4Casa::volverAlMapa, this, [this](){
        grafoNiveles->completarNivel(4);
        cambiarDeNivel(Mapa);
    });

    //regresar a la ruleta desde el juego
    /*connect(juegoArte, &juego1::volverARuleta, this, [this](){ cambiarDeNivel(nivel2); });
    connect(juegoHistoria, &juego2::volverARuleta, this, [this](){ cambiarDeNivel(nivel2); });
    connect(juegoPolitica, &juego3::volverARuleta, this, [this](){ cambiarDeNivel(nivel2); });
    connect(juegoCiencia, &juego4::volverARuleta, this, [this](){ cambiarDeNivel(nivel2); });*/

    connect(juegoArte, &juego1::volverARuleta, this, [this]() {
        nivel2->onJuegoCompletado(); // ← AGREGAR ESTO
        cambiarDeNivel(nivel2);
    });

    connect(juegoHistoria, &juego2::volverARuleta, this, [this]() {
        nivel2->onJuegoCompletado(); // ← AGREGAR ESTO
        cambiarDeNivel(nivel2);
    });

    connect(juegoPolitica, &juego3::volverARuleta, this, [this]() {
        nivel2->onJuegoCompletado(); // ← AGREGAR ESTO
        cambiarDeNivel(nivel2);
    });

    connect(juegoCiencia, &juego4::volverARuleta, this, [this]() {
        nivel2->onJuegoCompletado(); // ← AGREGAR ESTO
        cambiarDeNivel(nivel2);
    });

    // ========== NUEVA CONEXIÓN ==========
    // Ir a batalla cuando se elija el bando
    connect(nivel2, &nivel2Ruleta::irABatalla, this, &MainWindow::irABatalla);

    // ========== ✅ NUEVA CONEXIÓN: Batalla terminada ==========
    /*connect(nivel3, &nivel3Batalla::batallaTerminada, this, [this](QString ganador,int puntaje) {
        //nivel3->detenerMusica();
        if (ganador == "JUGADOR") {
            ranking->agregarPuntaje(jugadorActual.nombre, puntaje);
            QMessageBox::information(this, "¡Victoria!",
                                     "¡Has ganado la batalla filosófica!\n\n"
                                     "Has demostrado tu dominio del conocimiento.");
        } else {
            QMessageBox::information(this, "Derrota",
                                     "Has sido derrotado en la batalla...\n\n"
                                     "Estudia más y vuelve a intentarlo.");
        }
        cambiarDeNivel(Mapa);
    });*/

    connect(nivel3, &nivel3Batalla::batallaTerminada, this,
            [this](QString ganador, int puntaje) {

                // ⭐ GUARDAR NOMBRE PARA RESALTARLO
               Mapa->setJugadorActual(jugadorActual.nombre);

                // ⭐ AGREGAR AL RANKING (ahora sin límite)
                ranking->agregarPuntaje(jugadorActual.nombre, puntaje);

                if (ganador == "JUGADOR") {
                    QMessageBox::information(this, "¡Victoria!",
                                             QString("🎉 ¡Has ganado!\n\n"
                                                     "🏆 Puntaje: %1 pts").arg(puntaje));
                } else {
                    QMessageBox::information(this, "Derrota",
                                             QString("😔 Derrota\n\n"
                                                     "🏆 Puntaje: %1 pts").arg(puntaje));
                }
                grafoNiveles->completarNivel(3);
                cambiarDeNivel(Mapa);
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
    ui->lineNombre->setPlaceholderText("        INGRESA TU USUARIO");
}

void MainWindow::irABatalla(QString bando, int vidas)
{
    qDebug() << "🎮 Iniciando batalla:";
    qDebug() << "   Bando:" << bando;
    qDebug() << "   Vidas:" << vidas;

    // CRÍTICO: Configurar ANTES de cambiar de nivel
    nivel3->configurarBatalla(bando, vidas);

    // Ahora sí cambiar al nivel
    cambiarDeNivel(nivel3);
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
        //QMessageBox::information(this, "Nuevo jugador", "Se ha creado el jugador: " + jugadorActual.nombre);
    }

    // 4) Limpiar input, ir al mapa (ejemplo) y mantener foco si quieres
    ui->lineNombre->clear();
    ui->stackedWidget->setCurrentWidget(Mapa);
    Mapa->inicializarNivel();

    /*ui->stackedWidget->setCurrentWidget(lab);
    lab->inicializarNivel();*/
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

        //nivel1
        if (auto* nivel = qobject_cast<nivel1Video*>(nuevoNivel)) {
            nivel->inicializarNivel();
        }
        //nivel2
        if (auto* nivel = qobject_cast<nivel2Ruleta*>(nuevoNivel)) {
            nivel->inicializarNivel();
        }
        //nivel 3
        if (auto* nivel = qobject_cast<nivel3Batalla*>(nuevoNivel)) {
            nivel->inicializarNivel();
        }
        //nivel 4
        if (auto* nivel = qobject_cast<nivel4Casa*>(nuevoNivel)) {
            nivel->inicializarNivel();
        }//habitaciones
        else if (auto* nivel = qobject_cast<Cuarto*>(nuevoNivel)) {
            nivel->inicializarNivel();
        }
        else if (auto* nivel = qobject_cast<Laboratorio*>(nuevoNivel)) {
            nivel->inicializarNivel();
        }//juegos de a ruleta
        else if (auto* nivel = qobject_cast<juego1*>(nuevoNivel)) {
            nivel->inicializarNivel();
        }
        else if (auto* nivel = qobject_cast<juego2*>(nuevoNivel)) {
            nivel->inicializarNivel();
        }
        else if (auto* nivel = qobject_cast<juego3*>(nuevoNivel)) {
            nivel->inicializarNivel();
        }
        else if (auto* nivel = qobject_cast<juego4*>(nuevoNivel)) {
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

