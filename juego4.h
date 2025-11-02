/*#ifndef JUEGO4_H
#define JUEGO4_H

#include <QWidget>
#include "escenario.h"
#include "QPropertyAnimation"

namespace Ui {
class juego4;
}

class juego4 : public QWidget
{
    Q_OBJECT

public:
    explicit juego4(QWidget *parent = nullptr);
    ~juego4();
    void inicializarNivel(); //metodo para inicializar nivel
    void objetosInteractivos();
    void moverPersonajeAPosicion(int numeroPosicion); //nuevo


protected:
    // Declarar aquí que se sobrescribe keyPressEvent
    void keyPressEvent(QKeyEvent *event) override;

signals:
    void volverARuleta();

private:
    Ui::juego4 *ui;
    Escenario *escenario;
    //nuevo
    int posicionActual; // Para trackear en qué posición está
    QVector<QPointF> posiciones;
};

#endif // JUEGO4_H
*/

#ifndef JUEGO4_H
#define JUEGO4_H

#include <QWidget>
#include <QTimeLine>
#include "escenario.h"
#include "preguntawidget.h"

namespace Ui {
class juego4;
}

class juego4 : public QWidget
{
    Q_OBJECT
public:
    explicit juego4(QWidget *parent = nullptr);
    ~juego4();
    void inicializarNivel();
    void objetosInteractivos();
    void moverPersonajeAPosicion(int numeroPosicion);

    // Nuevas funciones para las preguntas
    void mostrarPregunta();
    void onPreguntaRespondida(QString npcId, bool correcta);
    void actualizarContador();

protected:
    void keyPressEvent(QKeyEvent *event) override;

signals:
    void volverARuleta();

private:
    Ui::juego4 *ui;
    Escenario *escenario;
    int posicionActual;
    QVector<QPointF> posiciones;

    // Nuevas variables para el sistema de preguntas
    int preguntaActual;
    int respuestasCorrectas;
    QGraphicsTextItem* textoContador;

    // Estructura para las preguntas
    struct Pregunta {
        QString pregunta;
        QStringList opciones;
        QString respuestaCorrecta;
    };
    QVector<Pregunta> preguntas;
};

#endif // JUEGO4_H
