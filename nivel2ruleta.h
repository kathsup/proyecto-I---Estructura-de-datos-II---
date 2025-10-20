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
    void iniciarGiro();
    void rotarRuleta();
    void elegirOpcionAleatoria();
    void bloquearOpcion(const QString &nombre);

    int anguloElegido;

protected:
    // Declarar aquí que se sobrescribe keyPressEvent
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void on_btnGirarRuleta_clicked();


private:
    Ui::nivel2Ruleta *ui;
    Escenario *escenario; //la base de cada nivel

    // RULETA
    QGraphicsPixmapItem* ruletaItem;   // imagen de la ruleta
    QMap<int, QString> opcionesConAngulo; // nombres de las opciones (Arte, Historia...)
    QString opcionElegida;             // la que salga al azar
    QTimer* timerGiro;                 // para simular la rotación
    int anguloActual;                  // control del ángulo de giro
    QGraphicsPixmapItem* indicadorTriangulo;
    QMap<QString, QGraphicsPixmapItem*> puertas;






};

#endif // NIVEL2RULETA_H
