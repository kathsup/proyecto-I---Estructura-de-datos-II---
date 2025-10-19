#ifndef NIVEL2RULETA_H
#define NIVEL2RULETA_H

#include <QWidget>
#include "escenario.h"
#include <QKeyEvent>

namespace Ui {
class nivel2Ruleta;
}

class nivel2Ruleta : public QWidget
{
    Q_OBJECT

public:
    explicit nivel2Ruleta(QWidget *parent = nullptr);
    ~nivel2Ruleta();

    void inicializarNivel(); //metodo para inicializar nivel
    void objetosInteractivos();

protected:
    // Declarar aquí que se sobrescribe keyPressEvent
    void keyPressEvent(QKeyEvent *event) override;

private:
    Ui::nivel2Ruleta *ui;
    Escenario *escenario; //la base de cada nivel
};

#endif // NIVEL2RULETA_H
