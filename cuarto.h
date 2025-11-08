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
    void inicializarNivel();
    void objetosInteractivos();

protected:
    void keyPressEvent(QKeyEvent *event) override;

signals:
    void volverAlLobby();

private slots:
    void on_btnInventario_clicked();
    void on_btnRompecabezas_clicked();

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
    int piezasRecolectadas = 0;
    void inventario();
    void agregarPiezaInventario(const QString &objeto, QGraphicsPixmapItem* obj);

    //======rompecabezas====
    QPushButton *btnRompecabezas;
    QFrame *panelRompecabezas;
    bool rompecabezasVisible = false;
    QLabel *labelBase;
    QVector<QLabel*> piezasColocadas; // Piezas ya colocadas en el rompecabezas
    QVector<bool> slotOcupado;
    QLabel *imagenArrastrada = nullptr; // QLabel temporal para la imagen que se arrastra
    QString piezaArrastradaNombre;
    QPoint posInicialArrastre;

    void inicializarRompecabezas();
    bool eventFilter(QObject *obj, QEvent *event) override;
    QPoint obtenerPosicionCorrecta(const QString &nombrePieza);
    bool verificarCercaDePosicion(const QPoint &posActual, const QPoint &posCorrecta);
    void verificarRompecabezasCompleto();
    QString obtenerRutaPieza(const QString &nombrePieza);
    QSize obtenerTamanoPieza(const QString &nombrePieza);

    //====preguntas======
    void verificarInteraccion();
    void mostrarPregunta(const QString &objeto);
};

#endif // CUARTO_H
