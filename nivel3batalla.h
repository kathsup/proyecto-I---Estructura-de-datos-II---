#ifndef NIVEL3BATALLA_H
#define NIVEL3BATALLA_H

#include <QWidget>
#include "escenario.h"

namespace Ui {
class nivel3Batalla;
}

class nivel3Batalla : public QWidget
{
    Q_OBJECT

public:
    explicit nivel3Batalla(QWidget *parent = nullptr);
    ~nivel3Batalla();

    void inicializarNivel(); //metodo para inicializar nivel
    void objetosInteractivos();

protected:
    // Declarar aquí que se sobrescribe keyPressEvent
    void keyPressEvent(QKeyEvent *event) override;

signals:
    void volverAlMapa();


private:
    Ui::nivel3Batalla *ui;
    Escenario *escenario;

};

#endif // NIVEL3BATALLA_H
