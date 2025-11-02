#ifndef JUEGO4_H
#define JUEGO4_H

#include <QWidget>
#include "escenario.h"

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

    QGraphicsRectItem* uno;
    QGraphicsRectItem* dos;
    QGraphicsRectItem* tres;
    QGraphicsRectItem* cuatro;
    QGraphicsRectItem* cinco;
    QGraphicsRectItem* seis;

    void zonasColision();

protected:
    // Declarar aquí que se sobrescribe keyPressEvent
    void keyPressEvent(QKeyEvent *event) override;

signals:
    void volverARuleta();

private:
    Ui::juego4 *ui;
    Escenario *escenario;
};

#endif // JUEGO4_H

