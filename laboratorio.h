#ifndef LABORATORIO_H
#define LABORATORIO_H

#include <QWidget>
#include "escenario.h"

namespace Ui {
class Laboratorio;
}

class Laboratorio : public QWidget
{
    Q_OBJECT

public:
    explicit Laboratorio(QWidget *parent = nullptr);
    ~Laboratorio();
    void inicializarNivel(); //metodo para inicializar nivel
    void objetosInteractivos();

protected:
    // Declarar aquí que se sobrescribe keyPressEvent
    void keyPressEvent(QKeyEvent *event) override;

signals:
    void volverAlLobby();

private:
    Ui::Laboratorio *ui;
    Escenario *escenario;
};

#endif // LABORATORIO_H
