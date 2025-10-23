#ifndef NIVEL4CASA_H
#define NIVEL4CASA_H

#include <QWidget>
#include "escenario.h"

namespace Ui {
class nivel4Casa;
}

class nivel4Casa : public QWidget
{
    Q_OBJECT

public:
    explicit nivel4Casa(QWidget *parent = nullptr);
    ~nivel4Casa();
    void inicializarNivel(); //metodo para inicializar nivel
    void objetosInteractivos();

protected:
    // Declarar aquí que se sobrescribe keyPressEvent
    void keyPressEvent(QKeyEvent *event) override;

signals:
    void solicitarCambioHabitacion(const QString& habitacion);
    void volverAlMapa();

private:
    Ui::nivel4Casa *ui;
    Escenario *escenario;
};


#endif // NIVEL4CASA_H
