/*#ifndef MENSAJEWIDGET_H
#define MENSAJEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QPixmap>

class MensajeWidget : public QWidget {
    Q_OBJECT
public:
    explicit MensajeWidget(const QString& texto, const QString& rutaFondo, QWidget* parent = nullptr);

signals:
    void cerrado();

private slots:
    void cerrarMensaje();

private:
    QLabel* labelTexto;
    QLabel* labelFondo;
    QPushButton* botonCerrar;
};

#endif // MENSAJEWIDGET_H*/

#ifndef MENSAJEWIDGET_H
#define MENSAJEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPixmap>
#include <QTimer>

class MensajeWidget : public QWidget {
    Q_OBJECT

public:
    explicit MensajeWidget(const QString& texto,
                           QWidget* parent = nullptr,
                           int ancho = 380,
                           int alto = 250,
                           int duracion = 4000); // duración en milisegundos (4 segundos por defecto)

signals:
    void cerrado();

private slots:
    void cerrarMensaje();

private:
    QLabel* labelTexto;
    QLabel* labelFondo;
    QTimer* temporizador;

    void ajustarTamanoTexto(const QString& texto, int anchoDisponible, int altoDisponible);
};

#endif // MENSAJEWIDGET_H
