/*#include "nivel3batalla.h"
#include "ui_nivel3batalla.h"

nivel3Batalla::nivel3Batalla(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::nivel3Batalla)
    , escenario(new Escenario(this))
    , personajeRojo(nullptr)
    , personajeBlanco(nullptr)
{
    ui->setupUi(this);
    setFocusPolicy(Qt::StrongFocus);
}

nivel3Batalla::~nivel3Batalla()
{
    delete ui;
}

void nivel3Batalla::inicializarNivel()
{
    QPixmap pixFondo("C:/Users/Lenovo/Downloads/fondo.png");
    escenario->inicializar(ui->graphicsViewBatalla, pixFondo);
    ui->graphicsViewBatalla->setFocusPolicy(Qt::NoFocus);

    objetosInteractivos();

    // Crear personajes
    personajeRojo = new PersonajeBatalla(PersonajeBatalla::ROJO, escenario->scene, this);
    personajeBlanco = new PersonajeBatalla(PersonajeBatalla::BLANCO, escenario->scene, this);

    // Iniciar animaciones idle
    personajeRojo->iniciarAnimacionIdle();
    personajeBlanco->iniciarAnimacionIdle();

    this->setFocus();
}

void nivel3Batalla::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        emit volverAlMapa();
    }

    // TECLAS PARA PROBAR ANIMACIONES (temporal)
    if (event->key() == Qt::Key_1) {
        probarAtaqueRojo();
    }
    if (event->key() == Qt::Key_2) {
        probarAtaqueBlanco();
    }
    if (event->key() == Qt::Key_3) {
        probarDerrotaRojo();
    }
    if (event->key() == Qt::Key_4) {
        probarDerrotaBlanco();
    }
}

void nivel3Batalla::objetosInteractivos()
{
    // Por ahora vacío
}

// Funciones para probar animaciones
void nivel3Batalla::probarAtaqueRojo()
{
    if (personajeRojo && personajeBlanco) {
        personajeRojo->atacar(personajeBlanco);
    }
}

void nivel3Batalla::probarAtaqueBlanco()
{
    if (personajeRojo && personajeBlanco) {
        personajeBlanco->atacar(personajeRojo);
    }
}

void nivel3Batalla::probarDerrotaRojo()
{
    if (personajeRojo) {
        personajeRojo->mostrarDerrota();
    }
}

void nivel3Batalla::probarDerrotaBlanco()
{
    if (personajeBlanco) {
        personajeBlanco->mostrarDerrota();
    }
} solo visual
*/

/*este es el que me gusta como queda pero preguntas falta que calcule bien
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
    , textoPregunta(nullptr)
    , textoOpciones(nullptr)
    , textoCombate(nullptr)
{
    ui->setupUi(this);
    setFocusPolicy(Qt::StrongFocus);

    cargarPreguntas();
}

nivel3Batalla::~nivel3Batalla()
{
    delete ui;
}

// ✅ CONFIGURAR BATALLA (Llamado desde MainWindow)
void nivel3Batalla::configurarBatalla(QString bandoElegido, int vidas)
{
    bandoJugador = bandoElegido;
    vidasJugador = vidas;
    vidasComputadora = 4;

    qDebug() << "🎮 Batalla configurada:";
    qDebug() << "   Bando:" << bandoJugador;
    qDebug() << "   Vidas Jugador:" << vidasJugador;
}

void nivel3Batalla::inicializarNivel()
{
    // Configurar escenario
    QPixmap pixFondo("C:/Users/Lenovo/Downloads/fondo.png");
    escenario->inicializar(ui->graphicsViewBatalla, pixFondo);
    ui->graphicsViewBatalla->setFocusPolicy(Qt::NoFocus);

    // ========== CREAR PERSONAJES SEGÚN BANDO ==========
    if (bandoJugador == "Empirista") {
        // Jugador = ROJO, Computadora = BLANCO
        personajeJugador = new PersonajeBatalla(PersonajeBatalla::ROJO, escenario->scene, this);
        personajeComputadora = new PersonajeBatalla(PersonajeBatalla::BLANCO, escenario->scene, this);
    } else { // "Racionalista"
        // Jugador = BLANCO, Computadora = ROJO
        personajeJugador = new PersonajeBatalla(PersonajeBatalla::BLANCO, escenario->scene, this);
        personajeComputadora = new PersonajeBatalla(PersonajeBatalla::ROJO, escenario->scene, this);
    }

    personajeJugador->iniciarAnimacionIdle();
    personajeComputadora->iniciarAnimacionIdle();

    //barras
    personajeJugador->inicializarBarraVida(vidasJugador, 4);
    personajeComputadora->inicializarBarraVida(vidasComputadora, 4);

    // ========== CREAR BARRAS DE VIDA ==========
    crearBarrasDeVida();

    // ========== CREAR BOTONES DE RESPUESTA ==========
    botonA = new QPushButton("A)", this);
    botonB = new QPushButton("B)", this);
    botonC = new QPushButton("C)", this);
    botonD = new QPushButton("D)", this);

    // Estilo de botones
    QString estiloBoton =
        "QPushButton {"
        "   background-color: #3498db;"
        "   color: white;"
        "   border: 3px solid #2980b9;"
        "   border-radius: 10px;"
        "   font-size: 18px;"
        "   font-weight: bold;"
        "   padding: 15px;"
        "   min-width: 200px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #2980b9;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #1c598a;"
        "}"
        "QPushButton:disabled {"
        "   background-color: #95a5a6;"
        "   border-color: #7f8c8d;"
        "}";

    botonA->setStyleSheet(estiloBoton);
    botonB->setStyleSheet(estiloBoton);
    botonC->setStyleSheet(estiloBoton);
    botonD->setStyleSheet(estiloBoton);

    // Posicionar botones (centrados abajo)
    botonA->setGeometry(250, 500, 220, 60);
    botonB->setGeometry(530, 500, 220, 60);
    botonC->setGeometry(250, 580, 220, 60);
    botonD->setGeometry(530, 580, 220, 60);

    connect(botonA, &QPushButton::clicked, this, &nivel3Batalla::onRespuestaA);
    connect(botonB, &QPushButton::clicked, this, &nivel3Batalla::onRespuestaB);
    connect(botonC, &QPushButton::clicked, this, &nivel3Batalla::onRespuestaC);
    connect(botonD, &QPushButton::clicked, this, &nivel3Batalla::onRespuestaD);

    ocultarBotones();

    // ========== TIMER PARA COMPUTADORA ==========
    timerComputadora = new QTimer(this);
    connect(timerComputadora, &QTimer::timeout, this, [this]() {
        if (!computadoraRespondio) {
            qint64 tiempoActual = QDateTime::currentMSecsSinceEpoch();
            qint64 transcurrido = tiempoActual - tiempoInicioRonda;

            if (transcurrido >= tiempoReaccionComputadora) {
                computadoraRespondio = true;
                tiempoRespuestaComputadora = tiempoReaccionComputadora;

                // Determinar respuesta de la computadora
                int porcentaje = obtenerPorcentajeAcierto();
                int random = QRandomGenerator::global()->bounded(100);

                if (random < porcentaje) {
                    // Respuesta correcta
                    respuestaComputadora = preguntaActualData.respuestaCorrecta;
                } else {
                    // Respuesta incorrecta aleatoria
                    QStringList incorrectas = {"A", "B", "C", "D"};
                    incorrectas.removeOne(preguntaActualData.respuestaCorrecta);
                    respuestaComputadora = incorrectas[QRandomGenerator::global()->bounded(incorrectas.size())];
                }

                qDebug() << "🤖 Computadora respondió:" << respuestaComputadora
                         << "en" << tiempoRespuestaComputadora << "ms";

                // Si ambos respondieron, evaluar
                if (jugadorRespondio && computadoraRespondio) {
                    timerComputadora->stop();
                    QTimer::singleShot(500, this, &nivel3Batalla::evaluarRonda);
                }
            }
        }
    });

    // ========== INICIAR PRIMER COMBATE ==========
    QTimer::singleShot(1000, this, [this]() {
        iniciarCombate(1);
    });

    this->setFocus();
}

// ========== CARGAR PREGUNTAS ==========
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

// ========== CREAR BARRAS DE VIDA ==========
void nivel3Batalla::crearBarrasDeVida()
{
    // Etiqueta nombre jugador
    QLabel* labelJugador = new QLabel("TÚ (" + bandoJugador.toUpper() + ")", this);
    labelJugador->setStyleSheet("color: white; font-size: 16px; font-weight: bold;");
    labelJugador->setGeometry(50, 20, 200, 30);

    // Etiqueta nombre computadora
    QString bandoComputadora = (bandoJugador == "Empirista") ? "RACIONALISTA" : "EMPIRISTA";
    QLabel* labelComputadora = new QLabel("OPONENTE (" + bandoComputadora + ")", this);
    labelComputadora->setStyleSheet("color: white; font-size: 16px; font-weight: bold;");
    labelComputadora->setAlignment(Qt::AlignRight);
    labelComputadora->setGeometry(750, 20, 200, 30);

    // ========== SEGMENTOS JUGADOR ==========
    for (int i = 0; i < 4; i++) {
        QLabel* segmento = new QLabel(this);
        segmento->setGeometry(50 + (i * 45), 60, 40, 30);

        // Segmento lleno o vacío según vidas iniciales
        if (i < vidasJugador) {
            segmento->setStyleSheet("background-color: #e74c3c; border: 2px solid #c0392b; border-radius: 5px;");
        } else {
            segmento->setStyleSheet("background-color: #34495e; border: 2px solid #2c3e50; border-radius: 5px;");
        }

        segmentosJugador.append(segmento);
    }

    // ========== SEGMENTOS COMPUTADORA ==========
    for (int i = 0; i < 4; i++) {
        QLabel* segmento = new QLabel(this);
        segmento->setGeometry(770 + (i * 45), 60, 40, 30);
        segmento->setStyleSheet("background-color: #ecf0f1; border: 2px solid #bdc3c7; border-radius: 5px;");

        segmentosComputadora.append(segmento);
    }
}

// ========== ACTUALIZAR BARRA DE VIDA ==========
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

// ========== INICIAR COMBATE ==========
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
    // Limpiar texto anterior
    if (textoCombate) {
        escenario->scene->removeItem(textoCombate);
        delete textoCombate;
    }

    textoCombate = new QGraphicsTextItem("🔥 COMBATE POR " + nombreCombate.toUpper() + " 🔥");
    textoCombate->setFont(QFont("Arial", 28, QFont::Bold));
    textoCombate->setDefaultTextColor(Qt::yellow);
    textoCombate->setPos(200, 250);

    // Efecto de sombra
    QGraphicsDropShadowEffect* sombra = new QGraphicsDropShadowEffect();
    sombra->setBlurRadius(15);
    sombra->setColor(Qt::black);
    sombra->setOffset(3, 3);
    textoCombate->setGraphicsEffect(sombra);

    escenario->scene->addItem(textoCombate);
}

// ========== MOSTRAR PREGUNTA ==========
void nivel3Batalla::mostrarPregunta()
{
    estadoActual = MOSTRANDO_PREGUNTA;

    // Limpiar texto de combate
    if (textoCombate) {
        escenario->scene->removeItem(textoCombate);
        delete textoCombate;
        textoCombate = nullptr;
    }

    // Obtener pregunta actual
    preguntaActualData = bancoPreguntas[preguntaActual - 1];

    // Mostrar pregunta en el centro
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

    // Actualizar texto de botones
    botonA->setText("A) " + preguntaActualData.opcionA);
    botonB->setText("B) " + preguntaActualData.opcionB);
    botonC->setText("C) " + preguntaActualData.opcionC);
    botonD->setText("D) " + preguntaActualData.opcionD);

    mostrarBotones();

    // ========== INICIAR CRONÓMETRO ==========
    tiempoInicioRonda = QDateTime::currentMSecsSinceEpoch();
    jugadorRespondio = false;
    computadoraRespondio = false;

    // Generar tiempo de reacción de computadora (2-5 segundos)
    tiempoReaccionComputadora = 2000 + QRandomGenerator::global()->bounded(3000);

    qDebug() << "⏱️ Computadora responderá en:" << tiempoReaccionComputadora << "ms";

    timerComputadora->start(100); // Verificar cada 100ms

    estadoActual = ESPERANDO_RESPUESTAS;
}

// ========== PROCESAR RESPUESTAS DEL JUGADOR ==========
void nivel3Batalla::onRespuestaA() { procesarRespuestaJugador("A"); }
void nivel3Batalla::onRespuestaB() { procesarRespuestaJugador("B"); }
void nivel3Batalla::onRespuestaC() { procesarRespuestaJugador("C"); }
void nivel3Batalla::onRespuestaD() { procesarRespuestaJugador("D"); }

void nivel3Batalla::procesarRespuestaJugador(QString respuesta)
{
    if (jugadorRespondio) return; // Ya respondió

    jugadorRespondio = true;
    respuestaJugador = respuesta;
    tiempoRespuestaJugador = QDateTime::currentMSecsSinceEpoch() - tiempoInicioRonda;

    deshabilitarBotones();

    qDebug() << "👤 Jugador respondió:" << respuesta << "en" << tiempoRespuestaJugador << "ms";

    // Si ambos respondieron, evaluar
    if (jugadorRespondio && computadoraRespondio) {
        timerComputadora->stop();
        QTimer::singleShot(500, this, &nivel3Batalla::evaluarRonda);
    }
}

// ========== EVALUAR RONDA ==========
void nivel3Batalla::evaluarRonda()
{
    estadoActual = EVALUANDO;
    ocultarBotones();

    // Determinar quién fue más rápido
    bool jugadorMasRapido = tiempoRespuestaJugador < tiempoRespuestaComputadora;

    QString primerRespondedor = jugadorMasRapido ? "JUGADOR" : "COMPUTADORA";
    QString respuestaPrimero = jugadorMasRapido ? respuestaJugador : respuestaComputadora;
    bool acerto = (respuestaPrimero == preguntaActualData.respuestaCorrecta);

    qDebug() << "⚡ Más rápido:" << primerRespondedor;
    qDebug() << "   Respondió:" << respuestaPrimero;
    qDebug() << "   Correcto:" << preguntaActualData.respuestaCorrecta;
    qDebug() << "   ¿Acertó?" << acerto;

    QString ganadorRonda;

    if (jugadorMasRapido) {
        if (acerto) {
            // Jugador acertó → Computadora pierde vida
            ganadorRonda = "JUGADOR";
            vidasComputadora--;
        } else {
            // Jugador falló → Jugador pierde vida
            ganadorRonda = "COMPUTADORA";
            vidasJugador--;
        }
    } else {
        if (acerto) {
            // Computadora acertó → Jugador pierde vida
            ganadorRonda = "COMPUTADORA";
            vidasJugador--;
        } else {
            // Computadora falló → Computadora pierde vida
            ganadorRonda = "JUGADOR";
            vidasComputadora--;
        }
    }

    mostrarResultado(ganadorRonda, acerto);
}

// ========== MOSTRAR RESULTADO ==========
void nivel3Batalla::mostrarResultado(QString ganadorRonda, bool acerto)
{
    estadoActual = MOSTRANDO_RESULTADO;

    // Ocultar pregunta
    if (textoPregunta) {
        escenario->scene->removeItem(textoPregunta);
        delete textoPregunta;
        textoPregunta = nullptr;
    }

    // ========== ANIMACIÓN DE ATAQUE ==========
    if (ganadorRonda == "JUGADOR") {
        personajeJugador->atacar(personajeComputadora);
        personajeComputadora->actualizarBarraVida(vidasComputadora, 4);//barra
        actualizarBarraVida(false, vidasComputadora); // Actualizar barra de computadora
    } else {
        personajeComputadora->atacar(personajeJugador);
        personajeJugador->actualizarBarraVida(vidasJugador, 4);//barra
        actualizarBarraVida(true, vidasJugador); // Actualizar barra de jugador
    }

    // Mostrar feedback
    QString mensaje = acerto ? "¡CORRECTO! ✅" : "¡INCORRECTO! ❌";
    QGraphicsTextItem* feedback = new QGraphicsTextItem(mensaje);
    feedback->setFont(QFont("Arial", 24, QFont::Bold));
    feedback->setDefaultTextColor(acerto ? Qt::green : Qt::red);
    feedback->setPos(350, 300);
    escenario->scene->addItem(feedback);

    // Eliminar feedback después de 2 segundos
    QTimer::singleShot(2000, this, [this, feedback]() {
        escenario->scene->removeItem(feedback);
        delete feedback;
        verificarFinDeBatalla();
    });
}

// ========== VERIFICAR FIN DE BATALLA ==========
void nivel3Batalla::verificarFinDeBatalla()
{
    // ¿Knockout?
    if (vidasJugador == 0) {
        mostrarPantallaFinal("COMPUTADORA");
        return;
    }

    if (vidasComputadora == 0) {
        mostrarPantallaFinal("JUGADOR");
        return;
    }

    // ¿Quedan preguntas?
    if (preguntaActual < 8) {
        preguntaActual++;

        // ¿Cambió de combate? (cada 2 preguntas)
        if ((preguntaActual - 1) % 2 == 0 && preguntaActual > 1) {
            combateActual++;
            QTimer::singleShot(1000, this, [this]() {
                iniciarCombate(combateActual);
            });
        } else {
            // Siguiente pregunta del mismo combate
            QTimer::singleShot(1000, this, &nivel3Batalla::mostrarPregunta);
        }
    } else {
        // Terminaron las 8 preguntas → Victoria por puntos
        QString ganador = (vidasJugador > vidasComputadora) ? "JUGADOR" : "COMPUTADORA";

        if (vidasJugador == vidasComputadora) {
            ganador = "JUGADOR"; // Empate favorece al jugador
        }

        mostrarPantallaFinal(ganador);
    }
}

// ========== PANTALLA FINAL ==========
void nivel3Batalla::mostrarPantallaFinal(QString ganador)
{
    estadoActual = FIN_BATALLA;

    if (ganador == "JUGADOR") {
        // Animación de derrota del oponente
        personajeComputadora->mostrarDerrota();

        QGraphicsTextItem* textoVictoria = new QGraphicsTextItem("🏆 ¡VICTORIA! 🏆");
        textoVictoria->setFont(QFont("Arial", 36, QFont::Bold));
        textoVictoria->setDefaultTextColor(Qt::yellow);
        textoVictoria->setPos(250, 250);
        escenario->scene->addItem(textoVictoria);

        // Reproducir música según bando
        // TODO: Implementar música

    } else {
        // Animación de derrota del jugador
        personajeJugador->mostrarDerrota();

        QGraphicsTextItem* textoDerrota = new QGraphicsTextItem("💀 DERROTA 💀");
        textoDerrota->setFont(QFont("Arial", 36, QFont::Bold));
        textoDerrota->setDefaultTextColor(Qt::red);
        textoDerrota->setPos(300, 250);
        escenario->scene->addItem(textoDerrota);
    }

    // Mostrar estadísticas
    QGraphicsTextItem* stats = new QGraphicsTextItem(
        QString("Vidas finales:\nTú: %1  |  Oponente: %2")
            .arg(vidasJugador)
            .arg(vidasComputadora)
        );
    stats->setFont(QFont("Arial", 18));
    stats->setDefaultTextColor(Qt::white);
    stats->setPos(350, 350);
    escenario->scene->addItem(stats);

    // Botón para continuar (después de 3 segundos)
    QTimer::singleShot(3000, this, [this, ganador]() {
        emit batallaTerminada(ganador);
    });
}

// ========== FUNCIONES AUXILIARES ==========
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
    // Ajustar dificultad según el combate
    // Combate 1: 40% (fácil)
    // Combate 2: 50%
    // Combate 3: 60%
    // Combate 4: 70% (difícil)
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
*/


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
    , textoPregunta(nullptr)
    , textoOpciones(nullptr)
    , textoCombate(nullptr)
    , indicadorComputadora(nullptr)
{
    ui->setupUi(this);
    setFocusPolicy(Qt::StrongFocus);

    cargarPreguntas();
}

nivel3Batalla::~nivel3Batalla()
{
    delete ui;
}

void nivel3Batalla::configurarBatalla(QString bandoElegido, int vidas)
{
    bandoJugador = bandoElegido;
    vidasJugador = vidas;
    vidasComputadora = 4;

    qDebug() << "🎮 Batalla configurada:";
    qDebug() << "   Bando:" << bandoJugador;
    qDebug() << "   Vidas Jugador:" << vidasJugador;
}

void nivel3Batalla::inicializarNivel()
{
    // Configurar escenario
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

    // ========== CREAR BOTONES DE RESPUESTA ==========
    botonA = new QPushButton("A)", this);
    botonB = new QPushButton("B)", this);
    botonC = new QPushButton("C)", this);
    botonD = new QPushButton("D)", this);

    QString estiloBoton =
        "QPushButton {"
        "   background-color: #3498db;"
        "   color: white;"
        "   border: 3px solid #2980b9;"
        "   border-radius: 10px;"
        "   font-size: 18px;"
        "   font-weight: bold;"
        "   padding: 15px;"
        "   min-width: 200px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #2980b9;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #1c598a;"
        "}"
        "QPushButton:disabled {"
        "   background-color: #95a5a6;"
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

    // ========== TIMER PARA COMPUTADORA ==========
    timerComputadora = new QTimer(this);
    connect(timerComputadora, &QTimer::timeout, this, &nivel3Batalla::verificarRespuestaComputadora);

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

    textoCombate = new QGraphicsTextItem("🔥 COMBATE POR " + nombreCombate.toUpper() + " 🔥");
    textoCombate->setFont(QFont("Arial", 28, QFont::Bold));
    textoCombate->setDefaultTextColor(Qt::yellow);
    textoCombate->setPos(200, 250);

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

    // ========== MOSTRAR INDICADOR DE COMPUTADORA "PENSANDO" ==========
    mostrarIndicadorComputadora("⏳ Oponente pensando...");

    // ========== INICIAR CRONÓMETRO ==========
    tiempoInicioRonda = QDateTime::currentMSecsSinceEpoch();
    jugadorRespondio = false;
    computadoraRespondio = false;

    // Generar tiempo de reacción de computadora (2-6 segundos)
    tiempoReaccionComputadora = 2000 + QRandomGenerator::global()->bounded(4000);

    // Determinar si la computadora acertará
    int porcentaje = obtenerPorcentajeAcierto();
    int random = QRandomGenerator::global()->bounded(100);

    if (random < porcentaje) {
        respuestaComputadora = preguntaActualData.respuestaCorrecta;
    } else {
        QStringList incorrectas = {"A", "B", "C", "D"};
        incorrectas.removeOne(preguntaActualData.respuestaCorrecta);
        respuestaComputadora = incorrectas[QRandomGenerator::global()->bounded(incorrectas.size())];
    }

    qDebug() << "⏱️ Computadora responderá en:" << tiempoReaccionComputadora << "ms";
    qDebug() << "🤖 Respuesta computadora (predeterminada):" << respuestaComputadora;

    timerComputadora->start(100);

    estadoActual = ESPERANDO_RESPUESTAS;
}

// ========== INDICADOR VISUAL DE COMPUTADORA ==========
void nivel3Batalla::mostrarIndicadorComputadora(QString texto)
{
    if (indicadorComputadora) {
        escenario->scene->removeItem(indicadorComputadora);
        delete indicadorComputadora;
    }

    indicadorComputadora = new QGraphicsTextItem(texto);
    indicadorComputadora->setFont(QFont("Arial", 14, QFont::Bold));
    indicadorComputadora->setDefaultTextColor(QColor(255, 165, 0)); // Naranja
    indicadorComputadora->setPos(700, 120);
    escenario->scene->addItem(indicadorComputadora);
}

void nivel3Batalla::ocultarIndicadorComputadora()
{
    if (indicadorComputadora) {
        escenario->scene->removeItem(indicadorComputadora);
        delete indicadorComputadora;
        indicadorComputadora = nullptr;
    }
}

// ========== VERIFICAR SI COMPUTADORA RESPONDE ==========
void nivel3Batalla::verificarRespuestaComputadora()
{
    if (computadoraRespondio) return;

    qint64 tiempoActual = QDateTime::currentMSecsSinceEpoch();
    qint64 transcurrido = tiempoActual - tiempoInicioRonda;

    if (transcurrido >= tiempoReaccionComputadora) {
        computadoraRespondio = true;
        tiempoRespuestaComputadora = tiempoReaccionComputadora;

        // Mostrar que la computadora respondió
        mostrarIndicadorComputadora("✅ ¡Oponente respondió!");

        qDebug() << "🤖 Computadora respondió:" << respuestaComputadora
                 << "en" << tiempoRespuestaComputadora << "ms";

        // Si ambos respondieron, evaluar
        if (jugadorRespondio) {
            timerComputadora->stop();
            QTimer::singleShot(1000, this, &nivel3Batalla::evaluarRonda);
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

    qDebug() << "👤 Jugador respondió:" << respuesta << "en" << tiempoRespuestaJugador << "ms";

    // Resaltar botón seleccionado
    QPushButton* botonSeleccionado = nullptr;
    if (respuesta == "A") botonSeleccionado = botonA;
    else if (respuesta == "B") botonSeleccionado = botonB;
    else if (respuesta == "C") botonSeleccionado = botonC;
    else if (respuesta == "D") botonSeleccionado = botonD;

    if (botonSeleccionado) {
        botonSeleccionado->setStyleSheet(
            "QPushButton {"
            "   background-color: #f39c12;"
            "   color: white;"
            "   border: 3px solid #d68910;"
            "   border-radius: 10px;"
            "   font-size: 18px;"
            "   font-weight: bold;"
            "   padding: 15px;"
            "}"
            );
    }

    // Si ambos respondieron, evaluar
    if (computadoraRespondio) {
        timerComputadora->stop();
        QTimer::singleShot(1000, this, &nivel3Batalla::evaluarRonda);
    }
}

void nivel3Batalla::evaluarRonda()
{
    estadoActual = EVALUANDO;
    ocultarBotones();
    ocultarIndicadorComputadora();

    // ========== DETERMINAR QUIÉN FUE MÁS RÁPIDO ==========
    bool jugadorMasRapido = tiempoRespuestaJugador < tiempoRespuestaComputadora;

    QString primerRespondedor = jugadorMasRapido ? "TÚ" : "OPONENTE";
    QString respuestaPrimero = jugadorMasRapido ? respuestaJugador : respuestaComputadora;
    bool acerto = (respuestaPrimero == preguntaActualData.respuestaCorrecta);

    qDebug() << "⚡ Más rápido:" << primerRespondedor;
    qDebug() << "   Respondió:" << respuestaPrimero;
    qDebug() << "   Correcto:" << preguntaActualData.respuestaCorrecta;
    qDebug() << "   ¿Acertó?" << acerto;

    // ========== MOSTRAR QUIÉN RESPONDIÓ PRIMERO ==========
    QString mensajeVelocidad = QString("⚡ %1 RESPONDIÓ PRIMERO (%2 ms)")
                                   .arg(primerRespondedor)
                                   .arg(jugadorMasRapido ? tiempoRespuestaJugador : tiempoRespuestaComputadora);

    QGraphicsTextItem* textoVelocidad = new QGraphicsTextItem(mensajeVelocidad);
    textoVelocidad->setFont(QFont("Arial", 18, QFont::Bold));
    textoVelocidad->setDefaultTextColor(Qt::cyan);
    textoVelocidad->setPos(250, 350);
    escenario->scene->addItem(textoVelocidad);

    QTimer::singleShot(1500, this, [this, textoVelocidad]() {
        escenario->scene->removeItem(textoVelocidad);
        delete textoVelocidad;
    });

    // ========== EVALUAR RESULTADO ==========
    QString ganadorRonda;

    if (jugadorMasRapido) {
        if (acerto) {
            ganadorRonda = "JUGADOR";
            vidasComputadora--;
        } else {
            ganadorRonda = "COMPUTADORA";
            vidasJugador--;
        }
    } else {
        if (acerto) {
            ganadorRonda = "COMPUTADORA";
            vidasJugador--;
        } else {
            ganadorRonda = "JUGADOR";
            vidasComputadora--;
        }
    }

    QTimer::singleShot(1500, this, [this, ganadorRonda, acerto]() {
        mostrarResultado(ganadorRonda, acerto);
    });
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

    // ========== MOSTRAR FEEDBACK ==========
    QString mensaje = acerto ? "¡CORRECTO! ✅" : "¡INCORRECTO! ❌";
    QString submensaje = acerto ?
                             (ganadorRonda == "JUGADOR" ? "¡Golpeaste al oponente!" : "¡Te golpearon!") :
                             (ganadorRonda == "JUGADOR" ? "¡El oponente falló!" : "¡Fallaste!");

    QGraphicsTextItem* feedback = new QGraphicsTextItem(mensaje + "\n" + submensaje);
    feedback->setFont(QFont("Arial", 24, QFont::Bold));
    feedback->setDefaultTextColor(acerto ? Qt::green : Qt::red);
    feedback->setTextWidth(600);
    feedback->setPos(250, 300);
    escenario->scene->addItem(feedback);

    QTimer::singleShot(2500, this, [this, feedback]() {
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

    if (ganador == "JUGADOR") {
        personajeComputadora->mostrarDerrota();

        QGraphicsTextItem* textoVictoria = new QGraphicsTextItem("🏆 ¡VICTORIA! 🏆");
        textoVictoria->setFont(QFont("Arial", 36, QFont::Bold));
        textoVictoria->setDefaultTextColor(Qt::yellow);
        textoVictoria->setPos(250, 250);
        escenario->scene->addItem(textoVictoria);

    } else {
        personajeJugador->mostrarDerrota();

        QGraphicsTextItem* textoDerrota = new QGraphicsTextItem("💀 DERROTA 💀");
        textoDerrota->setFont(QFont("Arial", 36, QFont::Bold));
        textoDerrota->setDefaultTextColor(Qt::red);
        textoDerrota->setPos(300, 250);
        escenario->scene->addItem(textoDerrota);
    }

    QGraphicsTextItem* stats = new QGraphicsTextItem(
        QString("Vidas finales:\nTú: %1  |  Oponente: %2")
            .arg(vidasJugador)
            .arg(vidasComputadora)
        );
    stats->setFont(QFont("Arial", 18));
    stats->setDefaultTextColor(Qt::white);
    stats->setPos(350, 350);
    escenario->scene->addItem(stats);

    QTimer::singleShot(3000, this, [this, ganador]() {
        emit batallaTerminada(ganador);
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
        "   background-color: #3498db;"
        "   color: white;"
        "   border: 3px solid #2980b9;"
        "   border-radius: 10px;"
        "   font-size: 18px;"
        "   font-weight: bold;"
        "   padding: 15px;"
        "   min-width: 200px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #2980b9;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #1c598a;"
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
