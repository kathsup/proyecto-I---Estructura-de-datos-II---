#include "mensajewidget.h"

MensajeWidget::MensajeWidget(const QString& texto, const QString& rutaFondo, QWidget* parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setFixedSize(300, 250); // Tamaño del cartel

    // Imagen de fondo
    labelFondo = new QLabel(this);
    QPixmap fondo(rutaFondo);
    labelFondo->setPixmap(fondo.scaled(size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    labelFondo->setGeometry(0, 0, width(), height());

    // Texto del mensaje
    labelTexto = new QLabel(texto, this);
    labelTexto->setWordWrap(true);
    labelTexto->setAlignment(Qt::AlignCenter);
    labelTexto->setStyleSheet("color: white; font-family: 'Arial'; font-size: 12px; font-weight: bold; background: transparent;");

    // Botón cerrar
    botonCerrar = new QPushButton("Cerrar", this);
    botonCerrar->setStyleSheet(
        "background-color: #f8bbd0; border: 2px solid #ec407a; border-radius: 10px; "
        "font-weight: bold; color: #4a148c;"
        );
    connect(botonCerrar, &QPushButton::clicked, this, &MensajeWidget::cerrarMensaje);

    // Layout
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addStretch();
    layout->addWidget(labelTexto, 0, Qt::AlignCenter);
    layout->addWidget(botonCerrar, 0, Qt::AlignCenter);
    layout->addStretch();

    setLayout(layout);
}

void MensajeWidget::cerrarMensaje() {
    emit cerrado();
    close();
}
