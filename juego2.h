/*#ifndef JUEGO2_H
#define JUEGO2_H

#include <QWidget>
#include <QKeyEvent>
#include <QGraphicsPixmapItem>
#include <QPropertyAnimation>
#include "escenario.h"
#include "personaje.h"


namespace Ui {
class juego2;
}

class juego2 : public QWidget
{
    Q_OBJECT

public:
    explicit juego2(QWidget *parent = nullptr);
    ~juego2();

    void inicializarNivel();

signals:
    void volverARuleta();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    Ui::juego2 *ui;
    Escenario* escenario;
    Personaje* personaje;

    void objetosInteractivos();

    // 🔹 Método para animar las burbujas flotando
    void animarBurbuja(QGraphicsPixmapItem* burbuja, int desplazamiento = 20);

    //seguimiento
    // 🔹 Sistema de selección de burbujas
    QList<QGraphicsPixmapItem*> burbujas;
    int burbujaIndex = 0;
    QGraphicsEllipseItem* selectorBurbuja = nullptr;
    QGraphicsPixmapItem* burbujaSeleccionada = nullptr;

    // 🔹 Sistema de línea punteada
    QGraphicsPathItem* lineaPunteada = nullptr;

    // 🔹 Métodos para el sistema de burbujas
    void seleccionarSiguienteBurbuja();
    void dibujarLineaPunteada();
    void explotarBurbuja();
    void realizarExplosion();
};

#endif // JUEGO2_H
*/

/*#ifndef JUEGO2_H
#define JUEGO2_H

#include <QWidget>
#include <QKeyEvent>
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QGraphicsPathItem>
#include <QGraphicsEllipseItem>
#include <QPropertyAnimation>
#include "escenario.h"
#include "personaje.h"
#include "pregunta.h"  // 🔹 Incluir tu struct existente

namespace Ui {
class juego2;
}

class juego2 : public QWidget
{
    Q_OBJECT

public:
    explicit juego2(QWidget *parent = nullptr);
    ~juego2();
    void inicializarNivel();

signals:
    void volverARuleta();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    Ui::juego2 *ui;
    Escenario* escenario;
    Personaje* personaje;

    void objetosInteractivos();

    // 🔹 Método para animar las burbujas flotando
    void animarBurbuja(QGraphicsPixmapItem* burbuja, int desplazamiento = 20);

    // 🔹 Sistema de selección de burbujas
    QList<QGraphicsPixmapItem*> burbujas;
    int burbujaIndex = -1;  // 🔹 Cambiado a -1 para que coincida con el .cpp
    QGraphicsEllipseItem* selectorBurbuja = nullptr;
    QGraphicsPixmapItem* burbujaSeleccionada = nullptr;

    // 🔹 Sistema de línea punteada
    QGraphicsPathItem* lineaPunteada = nullptr;

    // 🔹 Métodos para el sistema de burbujas
    void seleccionarSiguienteBurbuja();
    void dibujarLineaPunteada();
    void explotarBurbuja();
    void realizarExplosion();

    // 🎯 Sistema de preguntas (usando tu struct existente)
    QList<Pregunta> preguntas;
    int preguntaActual = 0;
    int respuestasCorrectas = 0;

    // 🎯 Elementos gráficos de las preguntas
    QGraphicsTextItem* textoPregunta = nullptr;
    QList<QGraphicsTextItem*> textosRespuestas;
    QGraphicsTextItem* textoMarcador = nullptr;

    // 🎯 Métodos del sistema de preguntas
    void inicializarPreguntas();
    void mostrarPregunta();
    void verificarRespuesta();
    void siguientePregunta();
    void mostrarResultadoFinal();
    void actualizarMarcador();
};

#endif // JUEGO2_H*/


/*#ifndef JUEGO2_H
#define JUEGO2_H

#include <QWidget>
#include <QKeyEvent>
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QPropertyAnimation>
#include "escenario.h"
#include "personaje.h"
#include "preguntaburbuja.h"

namespace Ui {
class juego2;
}

class juego2 : public QWidget
{
    Q_OBJECT

public:
    explicit juego2(QWidget *parent = nullptr);
    ~juego2();

    void inicializarNivel();

signals:
    void volverARuleta();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    Ui::juego2 *ui;
    Escenario* escenario;
    Personaje* personaje;

    void objetosInteractivos();
    void animarBurbuja(QGraphicsPixmapItem* burbuja, int desplazamiento = 20);

    // Sistema de selección de burbujas
    QList<QGraphicsPixmapItem*> burbujas;
    int burbujaIndex = 0;
    QGraphicsEllipseItem* selectorBurbuja = nullptr;
    QGraphicsPixmapItem* burbujaSeleccionada = nullptr;

    // Sistema de línea punteada
    QGraphicsPathItem* lineaPunteada = nullptr;

    // Métodos para el sistema de burbujas
    void seleccionarSiguienteBurbuja();
    void dibujarLineaPunteada();
    void explotarBurbuja();
    void realizarExplosion();

    // ⭐ NUEVO: Sistema de preguntas
    QVector<PreguntaBurbuja> preguntas;
    int preguntaActual;
    int puntosObtenidos;
    bool preguntaRespondida;  // Para evitar responder dos veces

    // Elementos visuales de preguntas
    QGraphicsTextItem* textoPregunta;
    QList<QGraphicsTextItem*> textosRespuestas;
    QGraphicsRectItem* fondoPregunta;
    QGraphicsTextItem* textoPuntuacion;

    // Métodos de preguntas
    void inicializarPreguntas();
    void cargarPregunta();
    void verificarRespuesta(int indiceBurbuja);
    void avanzarSiguientePregunta();
    void finalizarJuego();
    void mostrarPuntuacion();
};

#endif // JUEGO2_H
*/

#ifndef JUEGO2_H
#define JUEGO2_H

#include <QWidget>
#include <QKeyEvent>
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QPropertyAnimation>
#include "escenario.h"
#include "personaje.h"
#include "preguntaburbuja.h"

namespace Ui {
class juego2;
}

class juego2 : public QWidget
{
    Q_OBJECT

public:
    explicit juego2(QWidget *parent = nullptr);
    ~juego2();

    void inicializarNivel();

signals:
    void volverARuleta();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    Ui::juego2 *ui;
    Escenario* escenario;
    Personaje* personaje;

    void objetosInteractivos();
    void animarBurbuja(QGraphicsPixmapItem* burbuja, int desplazamiento = 20);

    // Sistema de selección de burbujas
    QList<QGraphicsPixmapItem*> burbujas;
    QList<QPointF> posicionesOriginalesBurbujas; // ⭐ Guardar posiciones originales
    int burbujaIndex = 0;
    QGraphicsEllipseItem* selectorBurbuja = nullptr;
    QGraphicsPixmapItem* burbujaSeleccionada = nullptr;

    // Sistema de línea punteada
    QGraphicsPathItem* lineaPunteada = nullptr;

    // Métodos para el sistema de burbujas
    void seleccionarSiguienteBurbuja();
    void dibujarLineaPunteada();
    void explotarBurbuja();
    void realizarExplosion();

    // ⭐ NUEVO: Sistema de preguntas
    QVector<PreguntaBurbuja> preguntas;
    int preguntaActual;
    int puntosObtenidos;
    bool preguntaRespondida;  // Para evitar responder dos veces

    // Elementos visuales de preguntas
    QGraphicsTextItem* textoPregunta;
    QList<QGraphicsTextItem*> textosRespuestas;
    QGraphicsRectItem* fondoPregunta;
    QGraphicsTextItem* textoPuntuacion;

    // Métodos de preguntas
    void inicializarPreguntas();
    void cargarPregunta();
    void verificarRespuesta(int indiceBurbuja);
    void avanzarSiguientePregunta();
    void finalizarJuego();
    void mostrarPuntuacion();
};

#endif // JUEGO2_H
