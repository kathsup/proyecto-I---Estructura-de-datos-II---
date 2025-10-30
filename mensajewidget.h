#ifndef MENSAJEWIDGET_H
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

#endif // MENSAJEWIDGET_H

