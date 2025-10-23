#ifndef JUEGO2_H
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
