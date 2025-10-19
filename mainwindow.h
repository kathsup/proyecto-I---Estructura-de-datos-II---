#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include "jugador.h"
#include "nivel2Ruleta.h"
#include "mapa.h"

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

private slots:
    void on_btnComenzar_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    void iniciarConfiguracion();
    QVector<Jugador> jugadores;
    Jugador jugadorActual;

    Jugador* buscarJugador(const QString &nombre);
    void agregarJugador(const QString &nombre);

    nivel2Ruleta* nivel2; //se agrega el nivel
    mapa* Mapa;
};
#endif // MAINWINDOW_H
