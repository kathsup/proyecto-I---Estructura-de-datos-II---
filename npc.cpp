#include "npc.h"
#include <QMessageBox>

NPC::NPC(QString rutaImagen, QString nombre, QObject *parent)
    : QObject(parent), QGraphicsPixmapItem(), nombre(nombre)
{
    setPixmap(QPixmap(rutaImagen).scaled(50, 50)); // tamaño del NPC
}

QString NPC::getNombre() const {
    return nombre;
}

void NPC::mostrarDialogo() {
    QMessageBox::information(nullptr, "Interacción",
                             "¡Has encontrado a " + nombre + "!");
    emit iniciarPreguntas(nombre);  // para abrir el minijuego correspondiente
}
