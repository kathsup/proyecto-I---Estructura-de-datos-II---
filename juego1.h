#ifndef JUEGO1_H
#define JUEGO1_H

#include <QWidget>
#include "escenario.h"

namespace Ui {
class juego1;
}

class juego1 : public QWidget
{
    Q_OBJECT

public:
    explicit juego1(QWidget *parent = nullptr);
    ~juego1();
    void inicializarNivel(); //metodo para inicializar nivel
    void objetosInteractivos();

protected:
    // Declarar aquí que se sobrescribe keyPressEvent
    void keyPressEvent(QKeyEvent *event) override;

signals:
    void volverARuleta();

private:
    Ui::juego1 *ui;
    Escenario *escenario;
};

#endif // JUEGO1_H
