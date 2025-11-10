#include "mensajewidget.h"
#include <QFontMetrics>
#include <QFont>

MensajeWidget::MensajeWidget(const QString& texto,
                             QWidget* parent,
                             int ancho,
                             int alto,
                             int duracion)
    : QWidget(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setFixedSize(ancho, alto);

    // Imagen de fondo - usar la ruta que especificaste
    labelFondo = new QLabel(this);
    QPixmap fondo("C:/Users/Lenovo/Downloads/mensaje.png");

    if (!fondo.isNull()) {
        // Usar IgnoreAspectRatio para que se ajuste exactamente al tamaño del widget
        labelFondo->setPixmap(fondo.scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    }
    labelFondo->setGeometry(0, 0, width(), height());

    // Texto del mensaje
    labelTexto = new QLabel(texto, this);
    labelTexto->setWordWrap(true);
    labelTexto->setAlignment(Qt::AlignCenter);
    labelTexto->setStyleSheet("color: transparent; font-family: 'Arial'; font-weight: bold; background: transparent;");

    // Ajustar tamaño del texto automáticamente
    // Dejar márgenes del 15% en cada lado
    int anchoDisponible = ancho * 0.7;
    int altoDisponible = alto * 0.7;
    ajustarTamanoTexto(texto, anchoDisponible, altoDisponible);

    // Layout
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addStretch(2); // Más espacio arriba
    layout->addWidget(labelTexto, 0, Qt::AlignCenter);
    layout->addStretch(1); // Menos espacio abajo
    setLayout(layout);

    // Temporizador para cerrar automáticamente
    temporizador = new QTimer(this);
    temporizador->setSingleShot(true);
    connect(temporizador, &QTimer::timeout, this, &MensajeWidget::cerrarMensaje);
    temporizador->start(duracion);
}

void MensajeWidget::ajustarTamanoTexto(const QString& texto, int anchoDisponible, int altoDisponible) {
    // Empezar con un tamaño grande e ir reduciendo hasta que encaje
    int tamanoFuente = 24;
    QFont fuente("Arial", tamanoFuente, QFont::Bold);

    while (tamanoFuente > 8) {
        fuente.setPointSize(tamanoFuente);
        QFontMetrics fm(fuente);
        QRect rect = fm.boundingRect(QRect(0, 0, anchoDisponible, altoDisponible),
                                     Qt::AlignCenter | Qt::TextWordWrap,
                                     texto);

        // Si el texto cabe, usar este tamaño
        if (rect.width() <= anchoDisponible && rect.height() <= altoDisponible) {
            break;
        }

        tamanoFuente--;
    }

    labelTexto->setFont(fuente);
    labelTexto->setStyleSheet(QString("color: #8B4513; font-family: 'Arial'; font-size: %1px; font-weight: bold; background: transparent;")
                                  .arg(tamanoFuente));
}

void MensajeWidget::cerrarMensaje() {
    emit cerrado();
    close();
}
