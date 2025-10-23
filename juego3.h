#ifndef JUEGO3_H
#define JUEGO3_H

#include <QWidget>
#include "escenario.h"

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
};

#endif // JUEGO3_H

