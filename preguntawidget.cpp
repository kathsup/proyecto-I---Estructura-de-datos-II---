#include "preguntawidget.h"
#include <QGraphicsRectItem>
#include <QFont>
#include <QBrush>
#include <QGraphicsScene>
#include <QTimer>
#include <QDebug>

PreguntaWidget::PreguntaWidget(QString pregunta, QStringList opciones, QString respuestaCorrecta, QGraphicsScene *scene, QString idNPC)
    : correcta(respuestaCorrecta), npcId(idNPC), escena(scene), mensajeResultado(nullptr),respuestaFueCorrecta(false)
{
    // IMPORTANTE: No capturar eventos de mouse en el grupo
    setHandlesChildEvents(false);

    // Rectángulo de fondo
    QGraphicsPixmapItem *fondo = new QGraphicsPixmapItem(
        QPixmap("C:/Users/Lenovo/Downloads/restanguloPreg.png").scaled(600, 250, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    fondo->setPos(280, 250);
    addToGroup(fondo);

    // Texto de la pregunta
    QGraphicsTextItem *texto = new QGraphicsTextItem(pregunta);
    texto->setFont(QFont("Arial", 10, QFont::Bold));
    QColor cafeOscuro(101, 67, 33);
    texto->setDefaultTextColor(cafeOscuro);
    texto->setPos(340, 315);
    texto->setTextWidth(520); // Para que el texto se ajuste
    addToGroup(texto);

    // Crear los 4 botones HORIZONTALMENTE
    int anchoBoton = 120;
    int espaciado = 10;
    int xInicial = 280 + (600 - (4 * anchoBoton + 3 * espaciado)) / 2; // Centrar botones
    int y = 380;

    for (int i = 0; i < opciones.size(); ++i) {
        QPushButton *boton = new QPushButton(opciones[i]);
        boton->setFixedSize(anchoBoton, 40);
        boton->setCursor(Qt::PointingHandCursor); // Cambiar cursor al pasar sobre el botón

        // Estilo mejorado: fondo blanco con borde morado
        boton->setStyleSheet(
            "QPushButton {"
            "   background-color: white;"
            "   border: 3px solid #eac4ff;"
            "   border-radius: 8px;"
            "   font-size: 10px;"
            "   font-weight: bold;"
            "   color: #7b3d9e;"
            "   padding: 5px;"
            "}"
            "QPushButton:hover {"
            "   background-color: #f5e6ff;"
            "   border: 3px solid #d4a5ff;"
            "}"
            "QPushButton:pressed {"
            "   background-color: #eac4ff;"
            "}"
            );

        // Crear el proxy
        QGraphicsProxyWidget *proxy = new QGraphicsProxyWidget();
        proxy->setWidget(boton);
        proxy->setPos(xInicial + i * (anchoBoton + espaciado), y);

        // IMPORTANTE: Asegurar que el proxy reciba eventos
        proxy->setFlag(QGraphicsItem::ItemIsSelectable, false);
        proxy->setAcceptHoverEvents(true);

        addToGroup(proxy);

        botones.append(boton);
        proxies.append(proxy);

        // Conectar la señal CON LAMBDA para debug
        connect(boton, &QPushButton::clicked, this, [this, boton]() {
          //  qDebug() << "¡Botón clickeado!" << boton->text();
            verificarRespuesta();
        });

        // Test alternativo: conectar directamente
        connect(boton, &QPushButton::pressed, [boton]() {
           // qDebug() << "Botón presionado:" << boton->text();
        });
    }

    // Agregar el grupo completo a la escena
    scene->addItem(this);

    // Asegurar que el grupo esté visible y habilitado
    setVisible(true);
    setEnabled(true);
    setZValue(1000); // Poner al frente

   // qDebug() << "PreguntaWidget creado con" << botones.size() << "botones";
    //qDebug() << "Posición del grupo:" << pos();
    //qDebug() << "Grupo visible:" << isVisible() << "habilitado:" << isEnabled();

    // Verificar que los botones estén habilitados
    /*for (int i = 0; i < botones.size(); ++i) {
        qDebug() << "Botón" << i << ":" << botones[i]->text()
        << "habilitado:" << botones[i]->isEnabled()
        << "visible:" << botones[i]->isVisible();
    }*/
}

void PreguntaWidget::verificarRespuesta() {
    QPushButton *boton = qobject_cast<QPushButton*>(sender());
    if (!boton) {
       // qDebug() << "Error: sender no es un QPushButton";
        // Intentar obtener el botón de otra manera
        for (auto b : botones) {
            if (b->isDown()) {
                boton = b;
                //qDebug() << "Botón encontrado por isDown():" << boton->text();
                break;
            }
        }
        if (!boton) return;
    }

  //  qDebug() << "=== VERIFICANDO RESPUESTA ===" << boton->text();

    // Deshabilitar todos los botones inmediatamente
    for (auto b : botones) {
        b->setEnabled(false);
    }

    // Verificar si la respuesta es correcta
    bool esCorrecto = (boton->text() == correcta);
    respuestaFueCorrecta = esCorrecto;

    //qDebug() << "Respuesta correcta:" << esCorrecto;

    // Cambiar color del botón seleccionado
    if (esCorrecto) {
        boton->setStyleSheet(
            "background-color: #4CAF50;"
            "border: 3px solid #2e7d32;"
            "border-radius: 3px;"
            "font-size: 10px;"
            "font-weight: bold;"
            "color: white;"
            "padding: 5px;"
            );
    } else {
        boton->setStyleSheet(
            "background-color: #f44336;"
            "border: 3px solid #c62828;"
            "border-radius: 8px;"
            "font-size: 10px;"
            "font-weight: bold;"
            "color: white;"
            "padding: 5px;"
            );

        // Mostrar cuál era la correcta
        for (auto b : botones) {
            if (b->text() == correcta) {
                b->setStyleSheet(
                    "background-color: #4CAF50;"
                    "border: 3px solid #2e7d32;"
                    "border-radius: 8px;"
                    "font-size: 10px;"
                    "font-weight: bold;"
                    "color: white;"
                    "padding: 5px;"
                    );
            }
        }
    }

    // Mostrar mensaje de resultado
    mostrarResultado(esCorrecto);

    // Emitir señales
   /* if (esCorrecto) {
        emit respuestaCorrecta();
    } else {
        emit respuestaIncorrecta();
    }*/

    //emit preguntaRespondida(npcId, esCorrecto);

    // Cerrar el cuadro después de 2 segundos
    QTimer::singleShot(2000, this, &PreguntaWidget::cerrarCuadro);
}

void PreguntaWidget::mostrarResultado(bool esCorrecto) {
    // Crear mensaje de resultado
    mensajeResultado = new QGraphicsTextItem();

    if (esCorrecto) {
        mensajeResultado->setPlainText("¡CORRECTO!");
        mensajeResultado->setDefaultTextColor(QColor("#2e7d32"));
    } else {
        mensajeResultado->setPlainText("INCORRECTO");
        mensajeResultado->setDefaultTextColor(QColor("#934844"));
    }

    mensajeResultado->setFont(QFont("Arial", 14, QFont::Bold));

    // Centrar el mensaje
    QRectF bounds = mensajeResultado->boundingRect();
    mensajeResultado->setPos(560 - bounds.width()/2, 420);

    addToGroup(mensajeResultado);
}

void PreguntaWidget::cerrarCuadro() {
    //qDebug() << "Cerrando cuadro de pregunta";

    // Emitir señal de que la pregunta fue respondida
    emit preguntaRespondida(npcId, respuestaFueCorrecta);

    // Eliminar este widget de la escena
    if (escena && this->scene() == escena) {
        escena->removeItem(this);
    }

    deleteLater();
}
