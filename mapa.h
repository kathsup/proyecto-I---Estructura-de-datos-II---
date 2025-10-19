#ifndef MAPA_H
#define MAPA_H

#include <QWidget>
#include "escenario.h"
#include <QKeyEvent>

namespace Ui {
class mapa;
}

class mapa : public QWidget
{
    Q_OBJECT

public:
    explicit mapa(QWidget *parent = nullptr);
    ~mapa();

    void inicializarNivel(); //metodo para inicializar nivel
    void objetosInteractivos();

protected:
    // Declarar aquí que se sobrescribe keyPressEvent
    void keyPressEvent(QKeyEvent *event) override;

private:
    Ui::mapa *ui;
    Escenario *escenario;

signals:
    void solicitarCambioNivel(); // señal para pedir cambio de nivel
};

#endif // MAPA_H
