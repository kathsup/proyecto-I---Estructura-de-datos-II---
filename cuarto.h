#ifndef CUARTO_H
#define CUARTO_H

#include <QWidget>
#include "escenario.h"

namespace Ui {
class Cuarto;
}

class Cuarto : public QWidget
{
    Q_OBJECT

public:
    explicit Cuarto(QWidget *parent = nullptr);
    ~Cuarto();
    void inicializarNivel(); //metodo para inicializar nivel
    void objetosInteractivos();

protected:
    // Declarar aquí que se sobrescribe keyPressEvent
    void keyPressEvent(QKeyEvent *event) override;

signals:
    void volverAlLobby();

private:
    Ui::Cuarto *ui;
    Escenario *escenario;
};

#endif // CUARTO_H
