#ifndef PREGUNTAWIDGET_H
#define PREGUNTAWIDGET_H

#include <QGraphicsItemGroup>
#include <QPushButton>
#include <QGraphicsProxyWidget>
#include <QGraphicsTextItem>

class PreguntaWidget : public QObject, public QGraphicsItemGroup
{
    Q_OBJECT

public:
    // Constructor con 5 parámetros
    PreguntaWidget(QString pregunta,
                   QStringList opciones,
                   QString respuestaCorrecta,
                   QGraphicsScene *scene,
                   QString idNPC);



signals:
    /*void respuestaCorrecta();
    void respuestaIncorrecta();
    void preguntaRespondida(QString idNPC);*/
    void preguntaRespondida(QString npcId, bool esCorrecta);
    void respuestaCorrecta();
    void respuestaIncorrecta();

private slots:
    void verificarRespuesta();

private:
    QString correcta;
    QString npcId;
    QList<QPushButton*> botones;
    QList<QGraphicsProxyWidget*> proxies;
    QGraphicsTextItem *mensajeResultado;
    QGraphicsScene *escena;

    bool respuestaFueCorrecta;

    void mostrarResultado(bool esCorrecto);
    void cerrarCuadro();
};

#endif // PREGUNTAWIDGET_H
