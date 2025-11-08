#ifndef LABORATORIO_H
#define LABORATORIO_H
#include <QWidget>
#include "escenario.h"
#include "npccasa.h"
#include <QLabel>
#include <QPushButton>


namespace Ui {
class Laboratorio;
}

class Laboratorio : public QWidget
{
    Q_OBJECT
public:
    explicit Laboratorio(QWidget *parent = nullptr);
    ~Laboratorio();
    void inicializarNivel();
    void objetosInteractivos();

protected:
    void keyPressEvent(QKeyEvent *event) override;

signals:
    void volverAlLobby();

private slots:
    void verificarProximidadNPC();
    void actualizarIndicadores();
    void onFrascoClicked(int numeroFrasco);

private:
    Ui::Laboratorio *ui;
    Escenario *escenario;

    // Mesas
    QGraphicsPixmapItem* mesa1;
    QGraphicsPixmapItem* mesa2;
    QGraphicsPixmapItem* mesa3;
    QGraphicsPixmapItem* mesa4;
    QGraphicsPixmapItem* caldero1;
    QGraphicsPixmapItem* caldero2;
    QGraphicsPixmapItem* caldero3;

    // NPC Descartes
    npcCasa* npcDes;
    QTimer* timerProximidad;
    bool nivelCompletado;
    bool dialogoVisible;
    void inicializarNPC();

    // Sistema de indicadores
    QMap<QString, QGraphicsEllipseItem*> indicadores;
    QTimer* timerParpadeo;
    bool indicadoresVisibles;

    // Control de mesas
    bool mesa1Completada;
    bool mesa2Completada;
    bool mesa3Completada;
    bool mesa4Completada;

    // Panel de mesa superior
    QFrame* panelMesaSuperior;
    QString mesaActual;

    // Elementos interactivos de Mesa 1
    QVector<QPushButton*> botonesBotellas;
    QLabel* lblPreguntaMesa1;
    QTimer* timerBrillo;
    int brilloAlpha;
    bool brilloSubiendo;
    QPushButton* botonCorrecto;

    void crearIndicadores();
    void ocultarIndicador(const QString &mesa);
    void verificarInteraccion();
    void mostrarMesaSuperior(const QString &mesa);
    void cerrarMesaSuperior();
    void desbloquearSiguienteMesa();

    // Métodos específicos para Mesa 1
    void configurarMesa1();
    void verificarRespuestaMesa1(int numeroFrasco);
    void animarBrilloBoton(QPushButton* boton);

    //mesa 2
    QVector<QPushButton*> botonesOpcionesMesa2;
    QLabel* lblPreguntaMesa2;
    void configurarMesa2();
    void verificarRespuestaMesa2(const QString& opcion);
};

#endif // LABORATORIO_H
