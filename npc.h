#ifndef NPC_H
#define NPC_H

#include <QGraphicsPixmapItem>
#include <QObject>

class NPC : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    explicit NPC(QString rutaImagen, QString nombre, QObject *parent = nullptr);

    QString getNombre() const;
    void mostrarDialogo();
    QString nombre;


signals:
    void iniciarPreguntas(QString nombreNPC);




};

#endif // NPC_H
