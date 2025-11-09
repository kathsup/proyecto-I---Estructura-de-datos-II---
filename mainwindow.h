#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include "jugador.h"
#include "nivel2Ruleta.h"
#include "mapa.h"
#include "nivel1Video.h"
#include "nivel3batalla.h"
#include "nivel4casa.h"
#include "cuarto.h"
#include "laboratorio.h"
#include "juego1.h"
#include "juego2.h"
#include "juego3.h"
#include "juego4.h"
#include "ranking.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    Ranking* obtenerRanking() { return ranking; }

private slots:
    void on_btnComenzar_clicked();
    void irABatalla(QString bando, int vidas);


private:
    Ui::MainWindow *ui;
    void iniciarConfiguracion();
    QVector<Jugador> jugadores;
    Jugador jugadorActual;

    Jugador* buscarJugador(const QString &nombre);
    void agregarJugador(const QString &nombre);

    nivel2Ruleta* nivel2; //se agrega el nivel
    nivel1Video* nivel1;
    nivel3Batalla* nivel3;
    nivel4Casa* nivel4;
    mapa* Mapa;
    Laboratorio* lab;
    Cuarto* room;
    juego1* juegoArte;
    juego2* juegoHistoria;
    juego3* juegoPolitica;
    juego4* juegoCiencia;
    Ranking* ranking;

public slots:
    void cambiarDeNivel(QWidget* nuevoNivel);



};
#endif // MAINWINDOW_H
