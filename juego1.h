#ifndef JUEGO1_H
#define JUEGO1_H

#include <QWidget>
#include "escenario.h"
#include <QList>
#include "npc.h"
#include <QLabel>

namespace Ui {
class juego1;
}

class juego1 : public QWidget
{
    Q_OBJECT

public:
    explicit juego1(QWidget *parent = nullptr);
    ~juego1();
    void inicializarNivel(); //metodo para inicializar nivel
    void objetosInteractivos();

    //zonas de colision
    QGraphicsRectItem* ladoIzq;
    QGraphicsRectItem* ladoDer;
    QGraphicsRectItem* arribaCuadro;
    QGraphicsRectItem* ladoArriba;
    QGraphicsRectItem* ladoAbajo;
    QGraphicsRectItem* abajoCuadro;
    QGraphicsRectItem* enmedio;
    QGraphicsRectItem* uno;
    QGraphicsRectItem* dos;
    QGraphicsRectItem* tres;
    QGraphicsRectItem* cuatro;
    QGraphicsRectItem* cinco;
    QGraphicsRectItem* seis;
    QGraphicsRectItem* siete;
    QGraphicsRectItem* ocho;
    QGraphicsRectItem* nueve;
    QGraphicsRectItem* diez;
    QGraphicsRectItem* once;
    QGraphicsRectItem* doce;



    void zonasColision();
    QSet<QString> npcsRespondidos;


    // Sistema de llaves
    int llavesObtenidas = 0;
    QList<QLabel*> iconosLlaves; // 5 labels para mostrar las llaves
    QGraphicsPixmapItem *cofre; // El cofre
    bool cofreAbierto = false;

    void inicializarLlaves(); // Nueva función
    void actualizarLlave(int indice); // Nueva función
    void verificarCofre();

protected:
    // Declarar aquí que se sobrescribe keyPressEvent
    void keyPressEvent(QKeyEvent *event) override;

signals:
    void volverARuleta();
    void estrellaObtenida();

private:
    Ui::juego1 *ui;
    Escenario *escenario;
    QList<NPC*> npcs;
};

#endif // JUEGO1_H
