#ifndef NIVEL1VIDEO_H
#define NIVEL1VIDEO_H

#include <QWidget>
#include "escenario.h"

namespace Ui {
class nivel1Video;
}

class nivel1Video : public QWidget
{
    Q_OBJECT

public:
    explicit nivel1Video(QWidget *parent = nullptr);
    ~nivel1Video();

    void inicializarNivel(); //metodo para inicializar nivel
    void objetosInteractivos();

protected:
    // Declarar aquí que se sobrescribe keyPressEvent
    void keyPressEvent(QKeyEvent *event) override;

signals:
    void volverAlMapa();

private:
    Ui::nivel1Video *ui;
    Escenario *escenario; //la base de cada nivel
};

#endif // NIVEL1VIDEO_H
