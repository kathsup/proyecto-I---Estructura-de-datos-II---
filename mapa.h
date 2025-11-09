#ifndef MAPA_H
#define MAPA_H

#include <QWidget>
#include "escenario.h"
#include <QKeyEvent>
#include "jugador.h"
#include "ranking.h"
#include "grafoniveles.h"

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
    void botones();
    void setRanking(Ranking* r) { rankingRef = r; }
    QString nombreJugadorActual;
    void setJugadorActual(const QString& nombre) {nombreJugadorActual = nombre;}
    void setGrafoNiveles(GrafoNiveles* grafo) { grafoNiveles = grafo; }
    void actualizarVisualesNiveles();


protected:
    // Declarar aquí que se sobrescribe keyPressEvent
    void keyPressEvent(QKeyEvent *event) override;

private:
    Ui::mapa *ui;
    Escenario *escenario;

    QFrame* panelRanking;
    QFrame* panelInstrucciones;

    void crearPanelRanking();
    void crearPanelInstrucciones();
    Ranking* rankingRef;
    GrafoNiveles* grafoNiveles;
    QGraphicsPixmapItem* objetoNivel1;
    QGraphicsPixmapItem* objetoNivel2;
    QGraphicsPixmapItem* objetoNivel3;
    QGraphicsPixmapItem* objetoNivel4;

signals:
    void solicitarCambioNivel(int numeroNivel); // señal para pedir cambio de nivel

public slots:
    void on_btnRanking_clicked();
    void on_btnInstrucciones_clicked();
    void actualizarRanking(const QVector<Jugador>& ranking);
};

#endif // MAPA_H
