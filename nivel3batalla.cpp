#include "nivel3batalla.h"
#include "ui_nivel3batalla.h"
#include <QMessageBox>
#include <QDateTime>
#include <QRandomGenerator>
#include <QGraphicsDropShadowEffect>




nivel3Batalla::nivel3Batalla(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::nivel3Batalla)
    , escenario(new Escenario(this))
    , personajeJugador(nullptr)
    , personajeComputadora(nullptr)
    , estadoActual(INTRO_COMBATE)
    , combateActual(1)
    , preguntaActual(1)
    , jugadorRespondio(false)
    , computadoraRespondio(false)
    , vidasComputadora(4)
    , timerComputadora(nullptr)
    , timerBarraTiempo(nullptr)
    , textoPregunta(nullptr)
    , textoOpciones(nullptr)
    , textoCombate(nullptr)
    , barraFondoTiempo(nullptr)
    , barraJugadorTiempo(nullptr)
    , barraComputadoraTiempo(nullptr)
    , tiempoJugadorTexto(nullptr)
    , tiempoComputadoraTexto(nullptr)
    , indicadorJugador(nullptr)
    , indicadorComputadora(nullptr)
    , reproductorMusica(new QMediaPlayer(this))
    , audioOutput(new QAudioOutput(this))
    , respuestasCorrectas(0)
    , combatesGanados(0)
{
    ui->setupUi(this);
    setFocusPolicy(Qt::StrongFocus);

    // Configurar audio
    reproductorMusica->setAudioOutput(audioOutput);
    audioOutput->setVolume(0.5); // Volumen al 50%

    cargarPreguntas();
}

nivel3Batalla::~nivel3Batalla()
{

    if (reproductorMusica) {
        reproductorMusica->stop();
    }

    delete ui;
}

void nivel3Batalla::configurarBatalla(QString bandoElegido, int vidas)
{
    bandoJugador = bandoElegido;
    vidasJugador = vidas;
    vidasComputadora = 4;

}

void nivel3Batalla::inicializarNivel()
{
    QPixmap pixFondo("C:/Users/Lenovo/Downloads/fondo.png");
    escenario->inicializar(ui->graphicsViewBatalla, pixFondo);
    ui->graphicsViewBatalla->setFocusPolicy(Qt::NoFocus);

    // ========== CREAR PERSONAJES SEGÚN BANDO ==========
    if (bandoJugador == "Empirista") {
        personajeJugador = new PersonajeBatalla(PersonajeBatalla::ROJO, escenario->scene, this);
        personajeComputadora = new PersonajeBatalla(PersonajeBatalla::BLANCO, escenario->scene, this);
    } else {
        personajeJugador = new PersonajeBatalla(PersonajeBatalla::BLANCO, escenario->scene, this);
        personajeComputadora = new PersonajeBatalla(PersonajeBatalla::ROJO, escenario->scene, this);
    }

    personajeJugador->iniciarAnimacionIdle();
    personajeComputadora->iniciarAnimacionIdle();

    personajeJugador->inicializarBarraVida(vidasJugador, 4);
    personajeComputadora->inicializarBarraVida(vidasComputadora, 4);

    crearBarrasDeVida();
    crearBarraTiempo();

    // ========== CREAR BOTONES DE RESPUESTA ==========
    botonA = new QPushButton("A)", this);
    botonB = new QPushButton("B)", this);
    botonC = new QPushButton("C)", this);
    botonD = new QPushButton("D)", this);

    QString estiloBoton =
        "QPushButton {"
        "   background-color: #c9aae3;"
        "   color: white;"
        "   border: 3px solid #c9aae3;"
        "   border-radius: 10px;"
        "   font-size: 18px;"
        "   font-weight: bold;"
        "   padding: 15px;"
        "   min-width: 200px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #bf94e3;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #ba85e6;"
        "}"
        "QPushButton:disabled {"
        "   background-color: #ceaee8;"
        "   border-color: #7f8c8d;"
        "}";

    botonA->setStyleSheet(estiloBoton);
    botonB->setStyleSheet(estiloBoton);
    botonC->setStyleSheet(estiloBoton);
    botonD->setStyleSheet(estiloBoton);

    botonA->setGeometry(250, 500, 220, 60);
    botonB->setGeometry(530, 500, 220, 60);
    botonC->setGeometry(250, 580, 220, 60);
    botonD->setGeometry(530, 580, 220, 60);

    connect(botonA, &QPushButton::clicked, this, &nivel3Batalla::onRespuestaA);
    connect(botonB, &QPushButton::clicked, this, &nivel3Batalla::onRespuestaB);
    connect(botonC, &QPushButton::clicked, this, &nivel3Batalla::onRespuestaC);
    connect(botonD, &QPushButton::clicked, this, &nivel3Batalla::onRespuestaD);

    ocultarBotones();

    // ========== TIMERS ==========
    timerComputadora = new QTimer(this);
    connect(timerComputadora, &QTimer::timeout, this, &nivel3Batalla::verificarRespuestaComputadora);

    timerBarraTiempo = new QTimer(this);
    connect(timerBarraTiempo, &QTimer::timeout, this, &nivel3Batalla::actualizarBarraTiempo);

    // ========== INICIAR PRIMER COMBATE ==========
    QTimer::singleShot(1000, this, [this]() {
        iniciarCombate(1);
    });

    this->setFocus();
}

void nivel3Batalla::cargarPreguntas()
{
    // COMBATE 1: ORIGEN
    bancoPreguntas.append({
        "Para algunos de los siguientes filósofos, el criterio de verdad es la evidencia sensible:",
        "Empiristas", "Criticistas", "Racionalistas", "Dogmáticos",
        "A", "Origen"
    });

    bancoPreguntas.append({
        "De las siguientes, una de ellas es la corriente filosófica que en general tiende a negar la posibilidad de la metafísica y a sostener que hay conocimiento únicamente de los fenómenos.",
        "Racionalistas", "Empiristas", "Escolásticos", "Escépticos",
        "B", "Origen"
    });

    // COMBATE 2: VERDAD
    bancoPreguntas.append({
        "Para unos de los siguientes filósofos, la experiencia como única fuente del conocimiento.",
        "Epistemólogos", "Racionalistas", "Empiristas", "Escépticos",
        "C", "Verdad"
    });

    bancoPreguntas.append({
        "Filósofos para quienes la única fuente del conocimiento es la razón.",
        "Epistemólogos", "Racionalistas", "Empiristas", "Escépticos",
        "B", "Verdad"
    });

    // COMBATE 3: SUJETO
    bancoPreguntas.append({
        "Filósofos que postulan las ideas innatas en el sujeto:",
        "Empiristas", "Idealistas", "Racionalistas", "Innatistas",
        "C", "Sujeto"
    });

    bancoPreguntas.append({
        "De los siguientes filósofos selecciones el que no se considera Racionalista: ",
        "David Hume", "John Locke", "Nicolas Malebranch", "Francis Bacon",
        "C", "Sujeto"
    });

    // COMBATE 4: TRASCENDENTAL
    bancoPreguntas.append({
        "Es la doctrina que establece que todos nuestros conocimientos provienen de la razón:",
        "Empirismo", "Criticismo", "Racionalismo", "Epistemología",
        "C", "Trascendental"
    });

    bancoPreguntas.append({
        "Uno de los siguientes filósofos, postula las ideas innatas en el sujeto:",
        "George Berkeley", "David Hume", "Leibniz", "Hipatía",
        "C", "Trascendental"
    });
}

void nivel3Batalla::crearBarrasDeVida()
{
    QLabel* labelJugador = new QLabel("TÚ (" + bandoJugador.toUpper() + ")", this);
    labelJugador->setStyleSheet("color: white; font-size: 16px; font-weight: bold;");
    labelJugador->setGeometry(50, 20, 200, 30);

    QString bandoComputadora = (bandoJugador == "Empirista") ? "RACIONALISTA" : "EMPIRISTA";
    QLabel* labelComputadora = new QLabel("OPONENTE (" + bandoComputadora + ")", this);
    labelComputadora->setStyleSheet("color: white; font-size: 16px; font-weight: bold;");
    labelComputadora->setAlignment(Qt::AlignRight);
    labelComputadora->setGeometry(750, 20, 200, 30);

    for (int i = 0; i < 4; i++) {
        QLabel* segmento = new QLabel(this);
        segmento->setGeometry(50 + (i * 45), 60, 40, 30);

        if (i < vidasJugador) {
            segmento->setStyleSheet("background-color: #e74c3c; border: 2px solid #c0392b; border-radius: 5px;");
        } else {
            segmento->setStyleSheet("background-color: #34495e; border: 2px solid #2c3e50; border-radius: 5px;");
        }

        segmentosJugador.append(segmento);
    }

    for (int i = 0; i < 4; i++) {
        QLabel* segmento = new QLabel(this);
        segmento->setGeometry(770 + (i * 45), 60, 40, 30);
        segmento->setStyleSheet("background-color: #ecf0f1; border: 2px solid #bdc3c7; border-radius: 5px;");

        segmentosComputadora.append(segmento);
    }
}

// ========== NUEVA FUNCIÓN: CREAR BARRA DE TIEMPO ==========
void nivel3Batalla::crearBarraTiempo()
{
    // Barra de fondo
    barraFondoTiempo = new QGraphicsRectItem(200, 110, 600, 30);
    barraFondoTiempo->setBrush(QBrush(QColor(50, 50, 50)));
    barraFondoTiempo->setPen(QPen(QColor(30, 30, 30), 2));
    escenario->scene->addItem(barraFondoTiempo);
    barraFondoTiempo->setVisible(false);

    // Barra del jugador (izquierda, roja)
    barraJugadorTiempo = new QGraphicsRectItem(200, 110, 0, 30);
    barraJugadorTiempo->setBrush(QBrush(QColor(231, 76, 60)));
    barraJugadorTiempo->setPen(Qt::NoPen);
    escenario->scene->addItem(barraJugadorTiempo);
    barraJugadorTiempo->setVisible(false);

    // Barra de la computadora (derecha, blanca/gris)
    barraComputadoraTiempo = new QGraphicsRectItem(800, 110, 0, 30);
    barraComputadoraTiempo->setBrush(QBrush(QColor(236, 240, 241)));
    barraComputadoraTiempo->setPen(Qt::NoPen);
    escenario->scene->addItem(barraComputadoraTiempo);
    barraComputadoraTiempo->setVisible(false);

    // Textos de tiempo
    tiempoJugadorTexto = new QGraphicsTextItem();
    tiempoJugadorTexto->setFont(QFont("Arial", 12, QFont::Bold));
    tiempoJugadorTexto->setDefaultTextColor(Qt::white);
    tiempoJugadorTexto->setPos(150, 108);
    escenario->scene->addItem(tiempoJugadorTexto);
    tiempoJugadorTexto->setVisible(false);

    tiempoComputadoraTexto = new QGraphicsTextItem();
    tiempoComputadoraTexto->setFont(QFont("Arial", 12, QFont::Bold));
    tiempoComputadoraTexto->setDefaultTextColor(Qt::white);
    tiempoComputadoraTexto->setPos(810, 108);
    escenario->scene->addItem(tiempoComputadoraTexto);
    tiempoComputadoraTexto->setVisible(false);

    // Indicadores circulares sobre personajes
    indicadorJugador = new QGraphicsEllipseItem(80, 380, 20, 20);
    indicadorJugador->setBrush(QBrush(QColor(231, 76, 60, 100)));
    indicadorJugador->setPen(QPen(QColor(231, 76, 60), 3));
    escenario->scene->addItem(indicadorJugador);
    indicadorJugador->setVisible(false);

    indicadorComputadora = new QGraphicsEllipseItem(900, 380, 20, 20);
    indicadorComputadora->setBrush(QBrush(QColor(236, 240, 241, 100)));
    indicadorComputadora->setPen(QPen(QColor(236, 240, 241), 3));
    escenario->scene->addItem(indicadorComputadora);
    indicadorComputadora->setVisible(false);
}

void nivel3Batalla::actualizarBarraVida(bool esJugador, int vidas)
{
    QVector<QLabel*>& segmentos = esJugador ? segmentosJugador : segmentosComputadora;
    QString colorLleno = esJugador ? "#e74c3c" : "#ecf0f1";
    QString colorBorde = esJugador ? "#c0392b" : "#bdc3c7";

    for (int i = 0; i < 4; i++) {
        if (i < vidas) {
            segmentos[i]->setStyleSheet(
                "background-color: " + colorLleno + "; "
                                                    "border: 2px solid " + colorBorde + "; "
                               "border-radius: 5px;"
                );
        } else {
            segmentos[i]->setStyleSheet(
                "background-color: #34495e; "
                "border: 2px solid #2c3e50; "
                "border-radius: 5px;"
                );
        }
    }
}

void nivel3Batalla::iniciarCombate(int numero)
{
    combateActual = numero;
    estadoActual = INTRO_COMBATE;

    QString nombreCombate = obtenerNombreCombate(numero);
    mostrarIntroCombate(nombreCombate);

    QTimer::singleShot(2000, this, &nivel3Batalla::mostrarPregunta);
}

void nivel3Batalla::mostrarIntroCombate(QString nombreCombate)
{
    if (textoCombate) {
        escenario->scene->removeItem(textoCombate);
        delete textoCombate;
    }

    textoCombate = new QGraphicsTextItem("COMBATE POR " + nombreCombate.toUpper());
    textoCombate->setFont(QFont("Arial", 28, QFont::Bold));
    textoCombate->setDefaultTextColor(Qt::blue);
    textoCombate->setPos(250, 230);

    QGraphicsDropShadowEffect* sombra = new QGraphicsDropShadowEffect();
    sombra->setBlurRadius(15);
    sombra->setColor(Qt::black);
    sombra->setOffset(3, 3);
    textoCombate->setGraphicsEffect(sombra);

    escenario->scene->addItem(textoCombate);
}

void nivel3Batalla::mostrarPregunta()
{
    estadoActual = MOSTRANDO_PREGUNTA;

    if (textoCombate) {
        escenario->scene->removeItem(textoCombate);
        delete textoCombate;
        textoCombate = nullptr;
    }

    preguntaActualData = bancoPreguntas[preguntaActual - 1];

    if (textoPregunta) {
        escenario->scene->removeItem(textoPregunta);
        delete textoPregunta;
    }

    textoPregunta = new QGraphicsTextItem(preguntaActualData.texto);
    textoPregunta->setFont(QFont("Arial", 16, QFont::Bold));
    textoPregunta->setDefaultTextColor(Qt::white);
    textoPregunta->setTextWidth(700);
    textoPregunta->setPos(150, 180);
    escenario->scene->addItem(textoPregunta);

    botonA->setText("A) " + preguntaActualData.opcionA);
    botonB->setText("B) " + preguntaActualData.opcionB);
    botonC->setText("C) " + preguntaActualData.opcionC);
    botonD->setText("D) " + preguntaActualData.opcionD);

    mostrarBotones();

    // ========== MOSTRAR BARRA DE TIEMPO E INDICADORES ==========
    barraFondoTiempo->setVisible(true);
    barraJugadorTiempo->setVisible(true);
    barraComputadoraTiempo->setVisible(true);
    tiempoJugadorTexto->setVisible(true);
    tiempoComputadoraTexto->setVisible(true);

    barraJugadorTiempo->setRect(200, 110, 0, 30);
    barraComputadoraTiempo->setRect(800, 110, 0, 30);

    tiempoJugadorTexto->setPlainText("");
    tiempoComputadoraTexto->setPlainText("");

    // Mostrar indicadores pulsantes
    indicadorJugador->setVisible(true);
    indicadorComputadora->setVisible(true);
    actualizarIndicadorEstado(true, false);
    actualizarIndicadorEstado(false, false);

    // ========== INICIAR CRONÓMETRO ==========
    tiempoInicioRonda = QDateTime::currentMSecsSinceEpoch();
    jugadorRespondio = false;
    computadoraRespondio = false;

    tiempoReaccionComputadora = 2000 + QRandomGenerator::global()->bounded(4000);

    int porcentaje = obtenerPorcentajeAcierto();
    int random = QRandomGenerator::global()->bounded(100);

    if (random < porcentaje) {
        respuestaComputadora = preguntaActualData.respuestaCorrecta;
    } else {
        QStringList incorrectas = {"A", "B", "C", "D"};
        incorrectas.removeOne(preguntaActualData.respuestaCorrecta);
        respuestaComputadora = incorrectas[QRandomGenerator::global()->bounded(incorrectas.size())];
    }

    timerComputadora->start(100);
    timerBarraTiempo->start(50);

    estadoActual = ESPERANDO_RESPUESTAS;
}

// ========== ACTUALIZAR BARRA DE TIEMPO ==========
void nivel3Batalla::actualizarBarraTiempo()
{
    qint64 tiempoActual = QDateTime::currentMSecsSinceEpoch();
    qint64 transcurrido = tiempoActual - tiempoInicioRonda;

    // Actualizar barra del jugador
    if (!jugadorRespondio) {
        float progreso = qMin(300.0, (float)transcurrido);
        barraJugadorTiempo->setRect(200, 110, progreso, 30);

        // Animación pulsante del indicador
        static int pulso = 0;
        pulso = (pulso + 10) % 100;
        int alpha = 100 + (pulso > 50 ? 100 - pulso : pulso);
        indicadorJugador->setBrush(QBrush(QColor(231, 76, 60, alpha)));
    }

    // Actualizar barra de la computadora
    if (!computadoraRespondio) {
        float progreso = qMin(300.0, (float)transcurrido);
        barraComputadoraTiempo->setRect(800 - progreso, 110, progreso, 30);

        static int pulso2 = 50;
        pulso2 = (pulso2 + 10) % 100;
        int alpha = 100 + (pulso2 > 50 ? 100 - pulso2 : pulso2);
        indicadorComputadora->setBrush(QBrush(QColor(236, 240, 241, alpha)));
    }

    // Detener si ambos respondieron
    if (jugadorRespondio && computadoraRespondio) {
        timerBarraTiempo->stop();
    }
}

// ========== ACTUALIZAR INDICADOR DE ESTADO ==========
void nivel3Batalla::actualizarIndicadorEstado(bool esJugador, bool respondio, qint64 tiempo)
{
    QGraphicsEllipseItem* indicador = esJugador ? indicadorJugador : indicadorComputadora;
    QGraphicsTextItem*& textoTiempo = esJugador ? tiempoJugadorTexto : tiempoComputadoraTexto;

    if (respondio) {
        // Detener pulsación y mostrar tiempo
        QColor color = esJugador ? QColor(231, 76, 60) : QColor(236, 240, 241);
        indicador->setBrush(QBrush(color));
        indicador->setPen(QPen(color.darker(120), 3));

        textoTiempo->setPlainText(QString::number(tiempo) + " ms");
    }
}

void nivel3Batalla::verificarRespuestaComputadora()
{
    if (computadoraRespondio) return;

    qint64 tiempoActual = QDateTime::currentMSecsSinceEpoch();
    qint64 transcurrido = tiempoActual - tiempoInicioRonda;

    if (transcurrido >= tiempoReaccionComputadora) {
        computadoraRespondio = true;
        tiempoRespuestaComputadora = tiempoReaccionComputadora;

        actualizarIndicadorEstado(false, true, tiempoRespuestaComputadora);

        if (jugadorRespondio) {
            timerComputadora->stop();
            timerBarraTiempo->stop();
            QTimer::singleShot(1500, this, &nivel3Batalla::evaluarRonda);
        }
    }
}

void nivel3Batalla::onRespuestaA() { procesarRespuestaJugador("A"); }
void nivel3Batalla::onRespuestaB() { procesarRespuestaJugador("B"); }
void nivel3Batalla::onRespuestaC() { procesarRespuestaJugador("C"); }
void nivel3Batalla::onRespuestaD() { procesarRespuestaJugador("D"); }

void nivel3Batalla::procesarRespuestaJugador(QString respuesta)
{
    if (jugadorRespondio) return;

    jugadorRespondio = true;
    respuestaJugador = respuesta;
    tiempoRespuestaJugador = QDateTime::currentMSecsSinceEpoch() - tiempoInicioRonda;

    deshabilitarBotones();
    actualizarIndicadorEstado(true, true, tiempoRespuestaJugador);

    // Resaltar botón seleccionado (naranja mientras espera)
    QPushButton* botonSeleccionado = nullptr;
    if (respuesta == "A") botonSeleccionado = botonA;
    else if (respuesta == "B") botonSeleccionado = botonB;
    else if (respuesta == "C") botonSeleccionado = botonC;
    else if (respuesta == "D") botonSeleccionado = botonD;

    if (botonSeleccionado) {
        botonSeleccionado->setStyleSheet(
            "QPushButton {"
            "   background-color: #f79ef6;"
            "   color: white;"
            "   border: 3px solid #c47cc3;"
            "   border-radius: 10px;"
            "   font-size: 18px;"
            "   font-weight: bold;"
            "   padding: 15px;"
            "}"
            );
    }

    if (computadoraRespondio) {
        timerComputadora->stop();
        timerBarraTiempo->stop();
        QTimer::singleShot(1500, this, &nivel3Batalla::evaluarRonda);
    }
}

void nivel3Batalla::evaluarRonda()
{
    estadoActual = EVALUANDO;

    // Mostrar respuestas en botones ANTES de ocultar
    mostrarRespuestasEnBotones();

    QTimer::singleShot(2500, this, [this]() {
        ocultarBotones();
        limpiarBarraTiempo();

        // ========== DETERMINAR GANADOR ==========
        bool jugadorMasRapido = tiempoRespuestaJugador < tiempoRespuestaComputadora;
        QString respuestaPrimero = jugadorMasRapido ? respuestaJugador : respuestaComputadora;
        bool acerto = (respuestaPrimero == preguntaActualData.respuestaCorrecta);

        QString ganadorRonda;
        if (jugadorMasRapido) {
            ganadorRonda = acerto ? "JUGADOR" : "COMPUTADORA";
            if (acerto) {
                vidasComputadora--;
                respuestasCorrectas++;
            }
            else vidasJugador--;
        } else {
            ganadorRonda = acerto ? "COMPUTADORA" : "JUGADOR";
            if (acerto) vidasJugador--;
            else vidasComputadora--;
        }

        mostrarResultado(ganadorRonda, acerto);
    });
}

// ========== MOSTRAR RESPUESTAS EN BOTONES ==========
void nivel3Batalla::mostrarRespuestasEnBotones()
{
    // Obtener botón correcto
    QPushButton* botonCorrecto = nullptr;
    if (preguntaActualData.respuestaCorrecta == "A") botonCorrecto = botonA;
    else if (preguntaActualData.respuestaCorrecta == "B") botonCorrecto = botonB;
    else if (preguntaActualData.respuestaCorrecta == "C") botonCorrecto = botonC;
    else if (preguntaActualData.respuestaCorrecta == "D") botonCorrecto = botonD;

    // Obtener botones de jugador y computadora
    QPushButton* botonJugador = nullptr;
    if (respuestaJugador == "A") botonJugador = botonA;
    else if (respuestaJugador == "B") botonJugador = botonB;
    else if (respuestaJugador == "C") botonJugador = botonC;
    else if (respuestaJugador == "D") botonJugador = botonD;

    QPushButton* botonComputadora = nullptr;
    if (respuestaComputadora == "A") botonComputadora = botonA;
    else if (respuestaComputadora == "B") botonComputadora = botonB;
    else if (respuestaComputadora == "C") botonComputadora = botonC;
    else if (respuestaComputadora == "D") botonComputadora = botonD;

    // Estilo para respuesta correcta (VERDE)
    QString estiloVerde =
        "QPushButton {"
        "   background-color: #84a189;"
        "   color: white;"
        "   border: 3px solid #486e4f;"
        "   border-radius: 10px;"
        "   font-size: 18px;"
        "   font-weight: bold;"
        "   padding: 15px;"
        "}";

    // Estilo para respuesta incorrecta del jugador (ROJO)
    QString estiloRojo =
        "QPushButton {"
        "   background-color: #cf756b;"
        "   color: white;"
        "   border: 3px solid #cf6155;"
        "   border-radius: 10px;"
        "   font-size: 18px;"
        "   font-weight: bold;"
        "   padding: 15px;"
        "}";

    // Estilo para respuesta de la computadora (BORDE BRILLANTE)
    QString estiloBordeComputadora =
        "QPushButton {"
        "   background-color: #3498db;"
        "   color: white;"
        "   border: 5px solid #ecf0f1;"
        "   border-radius: 10px;"
        "   font-size: 18px;"
        "   font-weight: bold;"
        "   padding: 15px;"
        "}";

    // Aplicar estilos
    if (botonCorrecto) {
        botonCorrecto->setStyleSheet(estiloVerde);
    }

    // Si el jugador se equivocó, marcar su botón en rojo
    if (botonJugador && botonJugador != botonCorrecto) {
        botonJugador->setStyleSheet(estiloRojo);
    }

    // Si la computadora eligió diferente, marcar con borde brillante
    if (botonComputadora && botonComputadora != botonJugador) {
        // Si la computadora acertó y el jugador también, solo verde
        if (botonComputadora == botonCorrecto && botonJugador == botonCorrecto) {
            // Ya está verde
        }
        // Si la computadora se equivocó pero eligió diferente al jugador
        else if (botonComputadora != botonCorrecto) {
            botonComputadora->setStyleSheet(estiloBordeComputadora);
        }
    }
}

// ========== LIMPIAR BARRA DE TIEMPO ==========
void nivel3Batalla::limpiarBarraTiempo()
{
    barraFondoTiempo->setVisible(false);
    barraJugadorTiempo->setVisible(false);
    barraComputadoraTiempo->setVisible(false);
    tiempoJugadorTexto->setVisible(false);
    tiempoComputadoraTexto->setVisible(false);
    indicadorJugador->setVisible(false);
    indicadorComputadora->setVisible(false);
}

void nivel3Batalla::mostrarResultado(QString ganadorRonda, bool acerto)
{
    estadoActual = MOSTRANDO_RESULTADO;

    if (textoPregunta) {
        escenario->scene->removeItem(textoPregunta);
        delete textoPregunta;
        textoPregunta = nullptr;
    }

    // ========== ANIMACIÓN DE ATAQUE ==========
    if (ganadorRonda == "JUGADOR") {
        personajeJugador->atacar(personajeComputadora);
        personajeComputadora->actualizarBarraVida(vidasComputadora, 4);
        actualizarBarraVida(false, vidasComputadora);
    } else {
        personajeComputadora->atacar(personajeJugador);
        personajeJugador->actualizarBarraVida(vidasJugador, 4);
        actualizarBarraVida(true, vidasJugador);
    }

    // ========== MENSAJE SIMPLE ==========
    QString mensaje;
    QColor colorMensaje;

    if (ganadorRonda == "JUGADOR") {
        mensaje = acerto ? "CORRECTO - GOLPEAS" : "OPONENTE FALLÓ";
        colorMensaje = Qt::green;
    } else {
        mensaje = acerto ? "OPONENTE ACERTÓ" : "INCORRECTO";
        colorMensaje = Qt::red;
    }

    QGraphicsTextItem* feedback = new QGraphicsTextItem(mensaje);
    feedback->setFont(QFont("Arial", 26, QFont::Bold));
    feedback->setDefaultTextColor(colorMensaje);
    feedback->setPos(300, 230);
    escenario->scene->addItem(feedback);

    QTimer::singleShot(2000, this, [this, feedback]() {
        escenario->scene->removeItem(feedback);
        delete feedback;
        verificarFinDeBatalla();
    });
}

void nivel3Batalla::verificarFinDeBatalla()
{
    if (vidasJugador == 0) {
        mostrarPantallaFinal("COMPUTADORA");
        return;
    }

    if (vidasComputadora == 0) {
        combatesGanados++;
        mostrarPantallaFinal("JUGADOR");
        return;
    }

    if (preguntaActual < 8) {
        preguntaActual++;

        if ((preguntaActual - 1) % 2 == 0 && preguntaActual > 1) {
            combateActual++;
            QTimer::singleShot(1000, this, [this]() {
                iniciarCombate(combateActual);
            });
        } else {
            QTimer::singleShot(1000, this, &nivel3Batalla::mostrarPregunta);
        }
    } else {
        QString ganador = (vidasJugador > vidasComputadora) ? "JUGADOR" : "COMPUTADORA";

        if (vidasJugador == vidasComputadora) {
            ganador = "JUGADOR";
        }

        mostrarPantallaFinal(ganador);
    }
}

void nivel3Batalla::mostrarPantallaFinal(QString ganador)
{
    estadoActual = FIN_BATALLA;

    reproducirMusicaFinal(ganador);

    QTimer::singleShot(15000, this, [this]() {
        if (reproductorMusica) {
            reproductorMusica->stop();
            qDebug() << "🔇 Música detenida después de 15 segundos";
        }
    });

    if (ganador == "JUGADOR") {
        personajeComputadora->mostrarDerrota();

        QGraphicsTextItem* textoVictoria = new QGraphicsTextItem("VICTORIA");
        textoVictoria->setFont(QFont("Arial", 42, QFont::Bold));
        textoVictoria->setDefaultTextColor(Qt::yellow);
        textoVictoria->setPos(350, 230);
        escenario->scene->addItem(textoVictoria);

    } else {
        personajeJugador->mostrarDerrota();

        QGraphicsTextItem* textoDerrota = new QGraphicsTextItem("DERROTA");
        textoDerrota->setFont(QFont("Arial", 42, QFont::Bold));
        textoDerrota->setDefaultTextColor(Qt::red);
        textoDerrota->setPos(350, 230);
        escenario->scene->addItem(textoDerrota);
    }

    // CALCULAR PUNTAJE ANTES DE MOSTRARLO
    int puntajeFinal = calcularPuntajeFinal();

    //MOSTRAR ESTADÍSTICAS CON PUNTAJE
    QGraphicsTextItem* stats = new QGraphicsTextItem(
        QString("Vidas finales:\nTú: %1  |  Oponente: %2\n\n🏆 Puntaje obtenido: %3 pts\n✅ Respuestas correctas: %4")
            .arg(vidasJugador)
            .arg(vidasComputadora)
            .arg(puntajeFinal)              // ⬅️ MOSTRAR PUNTAJE
            .arg(respuestasCorrectas)        // ⬅️ MOSTRAR ACIERTOS
        );
    stats->setFont(QFont("Arial", 16, QFont::Bold));
    stats->setDefaultTextColor(Qt::white);
    stats->setPos(300, 330);  // Ajustar posición para que quepa todo
    escenario->scene->addItem(stats);

    //EMITIR SEÑAL CON PUNTAJE (siempre, ganes o pierdas)
    QTimer::singleShot(5000, this, [this, ganador, puntajeFinal]() {
        emit batallaTerminada(ganador, puntajeFinal);
    });
}



QString nivel3Batalla::obtenerNombreCombate(int numero)
{
    switch (numero) {
    case 1: return "EL ORIGEN";
    case 2: return "LA VERDAD";
    case 3: return "EL SUJETO";
    case 4: return "LO TRASCENDENTAL";
    default: return "DESCONOCIDO";
    }
}

int nivel3Batalla::obtenerPorcentajeAcierto()
{
    return 30 + (combateActual * 10);
}

void nivel3Batalla::ocultarBotones()
{
    botonA->hide();
    botonB->hide();
    botonC->hide();
    botonD->hide();
}

void nivel3Batalla::mostrarBotones()
{
    QString estiloBoton =
        "QPushButton {"
        "   background-color: #c9aae3;"
        "   color: white;"
        "   border: 3px solid #ceaee8;"
        "   border-radius: 10px;"
        "   font-size: 18px;"
        "   font-weight: bold;"
        "   padding: 15px;"
        "   min-width: 200px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #bf94e3;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #ba85e6;"
        "}";




    botonA->setStyleSheet(estiloBoton);
    botonB->setStyleSheet(estiloBoton);
    botonC->setStyleSheet(estiloBoton);
    botonD->setStyleSheet(estiloBoton);

    botonA->show();
    botonB->show();
    botonC->show();
    botonD->show();
    botonA->setEnabled(true);
    botonB->setEnabled(true);
    botonC->setEnabled(true);
    botonD->setEnabled(true);
}

void nivel3Batalla::deshabilitarBotones()
{
    botonA->setEnabled(false);
    botonB->setEnabled(false);
    botonC->setEnabled(false);
    botonD->setEnabled(false);
}

void nivel3Batalla::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        emit volverAlMapa();
    }
}

void nivel3Batalla::reproducirMusicaFinal(QString ganador)
{
    QString rutaMusica;

    // Determinar qué música reproducir según el ganador
    if (ganador == "JUGADOR") {
        // Si el jugador gana, reproducir la música de su bando
        if (bandoJugador == "Empirista") {
            rutaMusica = "C:/Users/Lenovo/Downloads/musicaEmpirista.mp3";
        } else { // Racionalista
            rutaMusica = "C:/Users/Lenovo/Downloads/musicaRacionalista.mp3";
        }
    } else { // COMPUTADORA gana
        // Si la computadora gana, reproducir la música del bando contrario
        if (bandoJugador == "Empirista") {
            rutaMusica = "C:/Users/Lenovo/Downloads/musicaRacionalista.mp3";
        } else { // Racionalista
            rutaMusica = "C:/Users/Lenovo/Downloads/musicaEmpirista.mp3";
        }
    }

    // Reproducir la música
    reproductorMusica->setSource(QUrl::fromLocalFile(rutaMusica));
    reproductorMusica->play();

    qDebug() << "🎵 Reproduciendo música:" << rutaMusica;
}

int nivel3Batalla::calcularPuntajeFinal() {
    int puntaje = 0;

    // 50 puntos por vida restante
    puntaje += vidasJugador * 50;

    // 100 puntos por respuesta correcta
    puntaje += respuestasCorrectas * 100;

    // 500 bonus por victoria total
    if (vidasJugador > 0 && vidasComputadora == 0) {
        puntaje += 500;
    }

    return puntaje;
}
