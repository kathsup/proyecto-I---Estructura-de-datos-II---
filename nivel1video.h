#ifndef NIVEL1VIDEO_H
#define NIVEL1VIDEO_H

#include <QWidget>
#include "escenario.h"
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QAudioOutput>


namespace Ui {
class nivel1Video;
}

class nivel1Video : public QWidget
{
    Q_OBJECT

public:
    explicit nivel1Video(QWidget *parent = nullptr);
    ~nivel1Video();
    void inicializarNivel();
    void objetosInteractivos();

protected:
    void keyPressEvent(QKeyEvent *event) override;

signals:
    void volverAlMapa();

private:
    Ui::nivel1Video *ui;
    Escenario *escenario;

    QGraphicsRectItem* cuadro;
    bool personajeCercaDeCuadro();
    QFrame* panelInstrucciones = nullptr;
    void mostrarPanelInstrucciones();


};

#endif // NIVEL1VIDEO_H
