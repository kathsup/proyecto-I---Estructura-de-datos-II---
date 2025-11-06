/*#ifndef NIVEL3BATALLA_H
#define NIVEL3BATALLA_H

#include <QWidget>
#include "escenario.h"
#include "personajebatalla.h" // ← NUEVO

namespace Ui {
class nivel3Batalla;
}

class nivel3Batalla : public QWidget
{
    Q_OBJECT

public:
    explicit nivel3Batalla(QWidget *parent = nullptr);
    ~nivel3Batalla();
    void inicializarNivel();
    void objetosInteractivos();

    // Nuevas funciones para probar animaciones
    void probarAtaqueRojo();
    void probarAtaqueBlanco();
    void probarDerrotaRojo();
    void probarDerrotaBlanco();

protected:
    void keyPressEvent(QKeyEvent *event) override;

signals:
    void volverAlMapa();

private:
    Ui::nivel3Batalla *ui;
    Escenario *escenario;

    // Personajes de batalla
    PersonajeBatalla* personajeRojo;   // ← NUEVO (Jugador)
    PersonajeBatalla* personajeBlanco; // ← NUEVO (CPU)
};

#endif // NIVEL3BATALLA_H
*/


/*#ifndef NIVEL3BATALLA_H
#define NIVEL3BATALLA_H

#include <QWidget>
#include <QTimer>
#include <QLabel>
#include <QGraphicsTextItem>
#include <QPushButton>
#include "escenario.h"
#include "personajebatalla.h"

namespace Ui {
class nivel3Batalla;
}

class nivel3Batalla : public QWidget
{
    Q_OBJECT

public:
    explicit nivel3Batalla(QWidget *parent = nullptr);
    ~nivel3Batalla();

    void inicializarNivel();

    // ✅ NUEVA: Configurar batalla con datos de la ruleta
    void configurarBatalla(QString bandoElegido, int vidasJugador);

protected:
    void keyPressEvent(QKeyEvent *event) override;

signals:
    void volverAlMapa();
    void batallaTerminada(QString ganador); // NUEVA señal

private slots:
    // Slots para respuestas del jugador
    void onRespuestaA();
    void onRespuestaB();
    void onRespuestaC();
    void onRespuestaD();

private:
    Ui::nivel3Batalla *ui;
    Escenario *escenario;

    // ========== PERSONAJES ==========
    personajeBatalla* personajeJugador;    // El que controla el usuario
    PersonajeBatalla* personajeComputadora; // El que controla la IA

    // ========== CONFIGURACIÓN INICIAL ==========
    QString bandoJugador;           // "Empirista" o "Racionalista"
    int vidasJugador;               // 2, 3 o 4 (desde ruleta)
    int vidasComputadora;           // Siempre 4

    // ========== BARRAS DE VIDA ==========
    QLabel* barraVidaJugador;
    QLabel* barraVidaComputadora;
    QVector<QLabel*> segmentosJugador;      // 4 segmentos visuales
    QVector<QLabel*> segmentosComputadora;  // 4 segmentos visuales

    // ========== ESTADO DE LA BATALLA ==========
    enum EstadoBatalla {
        INTRO_COMBATE,
        MOSTRANDO_PREGUNTA,
        ESPERANDO_RESPUESTAS,
        EVALUANDO,
        MOSTRANDO_RESULTADO,
        FIN_BATALLA
    };

    EstadoBatalla estadoActual;
    int combateActual;              // 1 a 4
    int preguntaActual;             // 1 a 8

    // ========== PREGUNTAS ==========
    struct Pregunta {
        QString texto;
        QString opcionA;
        QString opcionB;
        QString opcionC;
        QString opcionD;
        QString respuestaCorrecta; // "A", "B", "C" o "D"
        QString combate;           // "Origen", "Verdad", "Sujeto", "Trascendental"
    };

    QVector<Pregunta> bancoPreguntas;
    Pregunta preguntaActualData;

    // ========== MECÁNICA DE RESPUESTA ==========
    qint64 tiempoInicioRonda;       // Timestamp en milisegundos
    bool jugadorRespondio;
    QString respuestaJugador;
    qint64 tiempoRespuestaJugador;

    bool computadoraRespondio;
    QString respuestaComputadora;
    qint64 tiempoRespuestaComputadora;
    int tiempoReaccionComputadora;  // Aleatorio 2000-5000ms

    QTimer* timerComputadora;       // Para simular respuesta de la IA

    // ========== UI DINÁMICA ==========
    QGraphicsTextItem* textoPregunta;
    QGraphicsTextItem* textoOpciones;
    QGraphicsTextItem* textoCombate;

    QPushButton* botonA;
    QPushButton* botonB;
    QPushButton* botonC;
    QPushButton* botonD;

    // ========== FUNCIONES PRINCIPALES ==========
    void cargarPreguntas();
    void crearBarrasDeVida();
    void actualizarBarraVida(bool esJugador, int vidas);

    void iniciarCombate(int numeroCombate);
    void mostrarIntroCombate(QString nombreCombate);
    void mostrarPregunta();
    void iniciarRespuestaComputadora();
    void procesarRespuestaJugador(QString respuesta);
    void evaluarRonda();
    void mostrarResultado(QString ganadorRonda, bool acerto);
    void verificarFinDeBatalla();
    void mostrarPantallaFinal(QString ganador);

    void ocultarBotones();
    void mostrarBotones();
    void deshabilitarBotones();

    QString obtenerNombreCombate(int numero);
    int obtenerPorcentajeAcierto(); // Dificultad de la IA
};

#endif // NIVEL3BATALLA_H
*/

/*#ifndef NIVEL3BATALLA_H
#define NIVEL3BATALLA_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsTextItem>
#include <QPushButton>
#include <QTimer>
#include <QKeyEvent>
#include <QLabel>
#include <QVector>
#include "escenario.h"
#include "personajebatalla.h"
#include <QProcess>

namespace Ui {
class nivel3Batalla;
}

struct PreguntaBatalla {
    QString texto;
    QString opcionA;
    QString opcionB;
    QString opcionC;
    QString opcionD;
    QString respuestaCorrecta; // "A", "B", "C", o "D"
    QString tema; // "Origen", "Verdad", "Sujeto", "Trascendental"
};

class nivel3Batalla : public QWidget
{
    Q_OBJECT

public:
    explicit nivel3Batalla(QWidget *parent = nullptr);
    ~nivel3Batalla();

    void configurarBatalla(QString bandoElegido, int vidas);
    void inicializarNivel();

signals:
    void batallaTerminada(QString ganador);
    void volverAlMapa();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void onRespuestaA();
    void onRespuestaB();
    void onRespuestaC();
    void onRespuestaD();
    void verificarRespuestaComputadora(); // NUEVA FUNCIÓN

private:
    Ui::nivel3Batalla *ui;

    // ========== ESCENARIO Y PERSONAJES ==========
    Escenario* escenario;
    PersonajeBatalla* personajeJugador;
    PersonajeBatalla* personajeComputadora;

    // ========== DATOS DE BATALLA ==========
    QString bandoJugador;
    int vidasJugador;
    int vidasComputadora;

    // ========== PREGUNTAS ==========
    QVector<PreguntaBatalla> bancoPreguntas;
    PreguntaBatalla preguntaActualData;
    int preguntaActual;
    int combateActual;

    // ========== ESTADO DE RONDA ==========
    enum Estado {
        INTRO_COMBATE,
        MOSTRANDO_PREGUNTA,
        ESPERANDO_RESPUESTAS,
        EVALUANDO,
        MOSTRANDO_RESULTADO,
        FIN_BATALLA
    };
    Estado estadoActual;

    // ========== RESPUESTAS Y TIEMPOS ==========
    bool jugadorRespondio;
    bool computadoraRespondio;
    QString respuestaJugador;
    QString respuestaComputadora;
    qint64 tiempoInicioRonda;
    qint64 tiempoRespuestaJugador;
    qint64 tiempoRespuestaComputadora;
    qint64 tiempoReaccionComputadora;

    // ========== TIMERS ==========
    QTimer* timerComputadora;

    // ========== UI ELEMENTS ==========
    QPushButton* botonA;
    QPushButton* botonB;
    QPushButton* botonC;
    QPushButton* botonD;

    QGraphicsTextItem* textoPregunta;
    QGraphicsTextItem* textoOpciones;
    QGraphicsTextItem* textoCombate;
    QGraphicsTextItem* indicadorComputadora; // NUEVO ELEMENTO

    // ========== BARRAS DE VIDA ==========
    QVector<QLabel*> segmentosJugador;
    QVector<QLabel*> segmentosComputadora;

    // ========== FUNCIONES ==========
    void cargarPreguntas();
    void crearBarrasDeVida();
    void actualizarBarraVida(bool esJugador, int vidas);

    void iniciarCombate(int numero);
    void mostrarIntroCombate(QString nombreCombate);
    void mostrarPregunta();

    void procesarRespuestaJugador(QString respuesta);
    void evaluarRonda();
    void mostrarResultado(QString ganadorRonda, bool acerto);

    void verificarFinDeBatalla();
    void mostrarPantallaFinal(QString ganador);

    // ========== FUNCIONES DE INDICADOR (NUEVAS) ==========
    void mostrarIndicadorComputadora(QString texto);
    void ocultarIndicadorComputadora();

    // ========== AUXILIARES ==========
    QString obtenerNombreCombate(int numero);
    int obtenerPorcentajeAcierto();

    void ocultarBotones();
    void mostrarBotones();
    void deshabilitarBotones();

    //=======musica=======
    QProcess* reproductorMusica;
    void reproducirMusicaFinal(QString ganador);
};

#endif // NIVEL3BATALLA_H

ESTA ES LA CORRECTA ANTES DE CAMBIAR TIPO DE RETROALIMENTACIÓN DE PREGUNTAS
*/


#ifndef NIVEL3BATALLA_H
#define NIVEL3BATALLA_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsTextItem>
#include <QGraphicsRectItem>
#include <QPushButton>
#include <QTimer>
#include <QKeyEvent>
#include <QLabel>
#include <QVector>
#include "escenario.h"
#include "personajebatalla.h"
#include <QProcess>

namespace Ui {
class nivel3Batalla;
}

struct PreguntaBatalla {
    QString texto;
    QString opcionA;
    QString opcionB;
    QString opcionC;
    QString opcionD;
    QString respuestaCorrecta;
    QString tema;
};

class nivel3Batalla : public QWidget
{
    Q_OBJECT

public:
    explicit nivel3Batalla(QWidget *parent = nullptr);
    ~nivel3Batalla();

    void configurarBatalla(QString bandoElegido, int vidas);
    void inicializarNivel();

signals:
    void batallaTerminada(QString ganador);
    void volverAlMapa();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void onRespuestaA();
    void onRespuestaB();
    void onRespuestaC();
    void onRespuestaD();
    void verificarRespuestaComputadora();
    void actualizarBarraTiempo(); // NUEVO

private:
    Ui::nivel3Batalla *ui;

    // ========== ESCENARIO Y PERSONAJES ==========
    Escenario* escenario;
    PersonajeBatalla* personajeJugador;
    PersonajeBatalla* personajeComputadora;

    // ========== DATOS DE BATALLA ==========
    QString bandoJugador;
    int vidasJugador;
    int vidasComputadora;

    // ========== PREGUNTAS ==========
    QVector<PreguntaBatalla> bancoPreguntas;
    PreguntaBatalla preguntaActualData;
    int preguntaActual;
    int combateActual;

    // ========== ESTADO DE RONDA ==========
    enum Estado {
        INTRO_COMBATE,
        MOSTRANDO_PREGUNTA,
        ESPERANDO_RESPUESTAS,
        EVALUANDO,
        MOSTRANDO_RESULTADO,
        FIN_BATALLA
    };
    Estado estadoActual;

    // ========== RESPUESTAS Y TIEMPOS ==========
    bool jugadorRespondio;
    bool computadoraRespondio;
    QString respuestaJugador;
    QString respuestaComputadora;
    qint64 tiempoInicioRonda;
    qint64 tiempoRespuestaJugador;
    qint64 tiempoRespuestaComputadora;
    qint64 tiempoReaccionComputadora;

    // ========== TIMERS ==========
    QTimer* timerComputadora;
    QTimer* timerBarraTiempo; // NUEVO

    // ========== UI ELEMENTS ==========
    QPushButton* botonA;
    QPushButton* botonB;
    QPushButton* botonC;
    QPushButton* botonD;

    QGraphicsTextItem* textoPregunta;
    QGraphicsTextItem* textoOpciones;
    QGraphicsTextItem* textoCombate;

    // ========== BARRA DE TIEMPO (NUEVO) ==========
    QGraphicsRectItem* barraFondoTiempo;
    QGraphicsRectItem* barraJugadorTiempo;
    QGraphicsRectItem* barraComputadoraTiempo;
    QGraphicsTextItem* tiempoJugadorTexto;
    QGraphicsTextItem* tiempoComputadoraTexto;

    // ========== INDICADORES DE ESTADO (NUEVO) ==========
    QGraphicsEllipseItem* indicadorJugador;
    QGraphicsEllipseItem* indicadorComputadora;

    // ========== BARRAS DE VIDA ==========
    QVector<QLabel*> segmentosJugador;
    QVector<QLabel*> segmentosComputadora;

    // ========== FUNCIONES ==========
    void cargarPreguntas();
    void crearBarrasDeVida();
    void actualizarBarraVida(bool esJugador, int vidas);

    void iniciarCombate(int numero);
    void mostrarIntroCombate(QString nombreCombate);
    void mostrarPregunta();

    void procesarRespuestaJugador(QString respuesta);
    void evaluarRonda();
    void mostrarResultado(QString ganadorRonda, bool acerto);
    void verificarFinDeBatalla();
    void mostrarPantallaFinal(QString ganador);

    // ========== NUEVAS FUNCIONES VISUALES ==========
    void crearBarraTiempo();
    void actualizarIndicadorEstado(bool esJugador, bool respondio, qint64 tiempo = 0);
    void mostrarRespuestasEnBotones();
    void limpiarBarraTiempo();

    // ========== AUXILIARES ==========
    QString obtenerNombreCombate(int numero);
    int obtenerPorcentajeAcierto();
    void ocultarBotones();
    void mostrarBotones();
    void deshabilitarBotones();

    // ========== MÚSICA ==========
    QProcess* reproductorMusica;
    void reproducirMusicaFinal(QString ganador);
};

#endif // NIVEL3BATALLA_H
