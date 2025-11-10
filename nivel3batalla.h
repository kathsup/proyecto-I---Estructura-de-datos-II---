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
#include <QMediaPlayer>
#include <QAudioOutput>
#include <queue>


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
    void batallaTerminada(QString ganador,  int puntaje);
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
    std::queue<PreguntaBatalla> colaPreguntas;
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

    //=======musica=======
    QMediaPlayer* reproductorMusica;
    QAudioOutput* audioOutput;
    void reproducirMusicaFinal(QString ganador);

    //========RANKING ==========
    int respuestasCorrectas;
    int combatesGanados;
    int calcularPuntajeFinal();
};

#endif // NIVEL3BATALLA_H
