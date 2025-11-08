#ifndef NPCCASA_H
#define NPCCASA_H

#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QVector>
#include <QString>

class npcCasa : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    explicit npcCasa(const QPixmap &sprite, QObject *parent = nullptr);
    ~npcCasa();

    void iniciarAnimacion();
    void detenerAnimacion();

    // Obtener diálogo según el estado del juego
    QString obtenerDialogoActual(bool nivelCompletado);

    // Configurar diálogos
    void establecerDialogos(const QVector<QString> &dialogosInicio,
                            const QVector<QString> &dialogosFinal);

    // Gestión del cuadro de diálogo
    void mostrarDialogo(const QString &texto);
    void ocultarDialogo();
    QGraphicsRectItem* getCuadroDialogo() { return cuadroDialogo; }
    QGraphicsTextItem* getTextoDialogo() { return textoDialogo; }

private slots:
    void animarFlotacion();
    void cambiarTextoDialogo();

private:
    QTimer *timerAnimacion;
    QTimer *timerDialogo;
    qreal posicionInicialY;
    qreal desplazamiento;
    bool moviendoArriba;
    qreal velocidadFlotacion;
    qreal rangoFlotacion;

    // Diálogos del NPC
    QVector<QString> dialogosInicio;
    QVector<QString> dialogosFinal;
    int indiceDialogoActual;
    bool usandoDialogosInicio;

    // Elementos visuales del diálogo
    QGraphicsRectItem* cuadroDialogo;
    QGraphicsTextItem* textoDialogo;

    void crearCuadroDialogo();
    void actualizarPosicionDialogo();
};

#endif // NPCCASA_H
