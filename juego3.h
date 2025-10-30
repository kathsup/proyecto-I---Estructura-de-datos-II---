/*#ifndef JUEGO3_H
#define JUEGO3_H

#include <QWidget>
#include "escenario.h"
#include "pregunta.h"
#include "flor.h"



namespace Ui {
class juego3;
}

class juego3 : public QWidget
{
    Q_OBJECT

public:
    explicit juego3(QWidget *parent = nullptr);
    ~juego3();
    void inicializarNivel(); //metodo para inicializar nivel
    void objetosInteractivos();

protected:
    // Declarar aquí que se sobrescribe keyPressEvent
    void keyPressEvent(QKeyEvent *event) override;

signals:
    void volverARuleta();

private:
    Ui::juego3 *ui;
    Escenario *escenario;

    //juego
    QVector<Pregunta> preguntas;
    int preguntaActual = 0;
    int floresCorrectas = 0;
    Flor* florEnMano = nullptr;
    QGraphicsPixmapItem* canasta = nullptr;
    QVector<Flor*> flores; // flores de la pregunta actual


    void cargarPregunta();
    void recogerFlor(Flor* flor);
    void depositarFlor();
    void finalizarJuego();
    void inicializarPreguntas();
    void actualizarPosicionFlor();
};

#endif // JUEGO3_H*/

#ifndef JUEGO3_H
#define JUEGO3_H

#include <QWidget>
#include <QKeyEvent>
#include <QGraphicsPixmapItem>
#include <QVector>
#include "escenario.h"
#include "flor.h"
#include "pregunta.h"

namespace Ui {
class juego3;
}

class juego3 : public QWidget
{
    Q_OBJECT

public:
    explicit juego3(QWidget *parent = nullptr);
    ~juego3();

    void inicializarNivel();

protected:
    void keyPressEvent(QKeyEvent* event) override;

signals:
    void volverARuleta();

private:
    Ui::juego3 *ui;
    Escenario* escenario;

    // Objetos del juego
    QGraphicsPixmapItem* canasta;
    QVector<Flor*> flores;
    Flor* florEnMano = nullptr;

    // Sistema de preguntas
    QVector<Pregunta> preguntas;
    int preguntaActual = 0;
    int floresCorrectas = 0;

    // Funciones
    void objetosInteractivos();
    void inicializarPreguntas();
    void cargarPregunta();
    void recogerFlor(Flor* flor);
    void depositarFlor();
    void finalizarJuego();
    void actualizarPosicionFlor();
};

#endif // JUEGO3_H

