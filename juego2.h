/*#ifndef JUEGO2_H
#define JUEGO2_H

#include <QWidget>
#include "escenario.h"

namespace Ui {
class juego2;
}

class juego2 : public QWidget
{
    Q_OBJECT

public:
    explicit juego2(QWidget *parent = nullptr);
    ~juego2();
    void inicializarNivel(); //metodo para inicializar nivel
    void objetosInteractivos();

protected:
    // Declarar aquí que se sobrescribe keyPressEvent
    void keyPressEvent(QKeyEvent *event) override;

signals:
    void volverARuleta();

private:
    Ui::juego2 *ui;
    Escenario *escenario;
};

#endif // JUEGO2_H
*/
#ifndef JUEGO2_H
#define JUEGO2_H

#include <QWidget>
#include <QKeyEvent>
#include <QGraphicsPixmapItem>
#include <QPropertyAnimation>
#include "escenario.h"

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

    void objetosInteractivos();

    // 🔹 Método para animar las burbujas flotando
    void animarBurbuja(QGraphicsPixmapItem* burbuja, int desplazamiento = 20);
};

#endif // JUEGO2_H
