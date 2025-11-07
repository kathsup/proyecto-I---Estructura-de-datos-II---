#ifndef CUARTO_H
#define CUARTO_H

#include <QWidget>
#include "escenario.h"
#include "personaje.h"
#include <QPushButton>
#include <QLabel>

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

private slots:
    void on_btnInventario_clicked();

private:
    Ui::Cuarto *ui;
    Escenario *escenario;
    Personaje *personaje;

    //========objetos interactivos ======
    QGraphicsPixmapItem* librero;
    QGraphicsPixmapItem* plantaIzq;
    QGraphicsPixmapItem* plantaDer;
    QGraphicsPixmapItem* pizarra;
    QGraphicsPixmapItem* sisSolar;
    QGraphicsPixmapItem* plantaSuelo;
    QGraphicsPixmapItem* mesaIzq;
    QGraphicsPixmapItem* mesaDer;
    QGraphicsPixmapItem* pedestal;
    QGraphicsPixmapItem* estelarCompleto;
    QGraphicsPixmapItem* kant;

    //======inventario====
    QPushButton *btnInventario;
    QFrame *panelInventario;
    QVector<QLabel*> slotsInventario;
    bool inventarioVisible = false;
    void inventario();

    //====preguntas======
    void verificarInteraccion();
    void mostrarPregunta(const QString &objeto);
};

#endif // CUARTO_H
