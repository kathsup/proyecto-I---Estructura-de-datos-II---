#include "cuarto.h"
#include "ui_cuarto.h"
#include "preguntawidget.h"
#include "mensajewidget.h"
#include <QTimer>
#include <QGraphicsColorizeEffect>
#include <QMouseEvent>
#include <QMessageBox>

Cuarto::Cuarto(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Cuarto)
    , escenario(new Escenario(this))
{
    ui->setupUi(this);
    setFocusPolicy(Qt::StrongFocus);
}

Cuarto::~Cuarto()
{
    delete ui;
}

void Cuarto::inicializarNivel()
{
    QPixmap pixFondo("C:/Users/Lenovo/Downloads/cuarto.png");
    escenario->inicializar(ui->graphicsViewCuarto, pixFondo);
    ui->graphicsViewCuarto->setFocusPolicy(Qt::NoFocus);

    QVector<QPixmap> spritesDer = {
        QPixmap("C:/Users/Lenovo/Downloads/hada1AD.png").scaled(100,100, Qt::KeepAspectRatio, Qt::SmoothTransformation),
        QPixmap("C:/Users/Lenovo/Pictures/hada2AD.png").scaled(100,100, Qt::KeepAspectRatio, Qt::SmoothTransformation),
        QPixmap("C:/Users/Lenovo/Downloads/hada3AD.png").scaled(100,100, Qt::KeepAspectRatio, Qt::SmoothTransformation)
    };

    QVector<QPixmap> spritesIzq = {
        QPixmap("C:/Users/Lenovo/Pictures/hada1Iz.png").scaled(100,100, Qt::KeepAspectRatio, Qt::SmoothTransformation),
        QPixmap("C:/Users/Lenovo/Pictures/hada2Iz.png").scaled(100,100, Qt::KeepAspectRatio, Qt::SmoothTransformation),
        QPixmap("C:/Users/Lenovo/Pictures/hada3Iz.png").scaled(100,100, Qt::KeepAspectRatio, Qt::SmoothTransformation)
    };

    QVector<QPixmap> spritesArriba = {
        QPixmap("C:/Users/Lenovo/Downloads/hada4Ar.png").scaled(100,100, Qt::KeepAspectRatio, Qt::SmoothTransformation),
        QPixmap("C:/Users/Lenovo/Downloads/hada5Ar.png").scaled(100,100, Qt::KeepAspectRatio, Qt::SmoothTransformation)
    };

    objetosInteractivos();
    escenario->crearPersonaje(spritesDer, spritesIzq, spritesArriba, QPointF(650, 490));
    inventario();
    inicializarRompecabezas();

    this->setFocus();
}

void Cuarto::keyPressEvent(QKeyEvent *event)
{
    if(escenario) {
        escenario->manejarTecla(event);
    }

    if (event->key() == Qt::Key_Escape) {
        emit volverAlLobby();
    }

    if (event->key() == Qt::Key_E) {
        verificarInteraccion();
    }
}

void Cuarto::objetosInteractivos() {
    QPixmap pixLibrero("C:/Users/Lenovo/Downloads/librero.png");
    librero = escenario->scene->addPixmap(
        pixLibrero.scaled(250, 250, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    librero->setPos(0, 190);

    QPixmap pixPlantaIzq("C:/Users/Lenovo/Downloads/plantaTecho.png");
    plantaIzq = escenario->scene->addPixmap(
        pixPlantaIzq.scaled(250, 250, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    plantaIzq->setPos(145, 0);

    QPixmap pixPizarra("C:/Users/Lenovo/Downloads/pizarraa.png");
    pizarra = escenario->scene->addPixmap(
        pixPizarra.scaled(250, 250, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    pizarra->setPos(390, 65);

    QPixmap pixPlantaDer("C:/Users/Lenovo/Downloads/plantaTecho.png");
    plantaDer = escenario->scene->addPixmap(
        pixPlantaDer.scaled(220, 220, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    plantaDer->setPos(705, 0);

    QPixmap pixSolar("C:/Users/Lenovo/Downloads/sistema solar.png");
    sisSolar = escenario->scene->addPixmap(
        pixSolar.scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    sisSolar->setPos(890, 60);

    QPixmap pixMesaIzq("C:/Users/Lenovo/Downloads/mesaCuarto.png");
    mesaIzq = escenario->scene->addPixmap(
        pixMesaIzq.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    mesaIzq->setPos(130, 500);

    QPixmap pixMesaDer("C:/Users/Lenovo/Downloads/mesaCuarto.png");
    mesaDer = escenario->scene->addPixmap(
        pixMesaDer.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    mesaDer->setPos(360, 500);

    QPixmap pixPlantaSuelo("C:/Users/Lenovo/Downloads/plantaSuelo.png");
    plantaSuelo = escenario->scene->addPixmap(
        pixPlantaSuelo.scaled(110, 110, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    plantaSuelo->setPos(650, 330);

    QPixmap pixPedestal("C:/Users/Lenovo/Downloads/pedestal.png");
    pedestal = escenario->scene->addPixmap(
        pixPedestal.scaled(115, 115, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    pedestal->setPos(880, 510);

    QPixmap pixEstelar("C:/Users/Lenovo/Downloads/mapaEstelar.png");
    estelarCompleto = escenario->scene->addPixmap(
        pixEstelar.scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    estelarCompleto->setPos(875, 440);

    QPixmap pixKant("C:/Users/Lenovo/Downloads/kant.png");
    kant = escenario->scene->addPixmap(
        pixKant.scaled(90, 90, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    kant->setPos(780, 490);
}

void Cuarto::inventario(){
    panelInventario = new QFrame(this);
    panelInventario->setGeometry(20, 80, 150, 565);
    panelInventario->setStyleSheet("background-color: rgba(255, 255, 255, 180); border: 2px solid #555; border-radius: 10px;");
    panelInventario->hide();

    for (int i = 0; i < 6; ++i) {
        QLabel *slot = new QLabel(panelInventario);
        slot->setGeometry(25, 20 + i * 90, 100, 80);
        slot->setStyleSheet("background-color: rgba(200,200,200,150); border: 2px dashed #888;");
        slot->setAlignment(Qt::AlignCenter);
        slot->installEventFilter(this);
        slotsInventario.append(slot);
    }

    panelInventario->raise();
}

void Cuarto::inicializarRompecabezas(){
    panelRompecabezas = new QFrame(this);
    panelRompecabezas->setGeometry(350, 150, 400, 380);
    panelRompecabezas->setStyleSheet("background-color: rgba(255, 255, 255, 190); border: 2px solid #555; border-radius: 10px;");
    panelRompecabezas->hide();

    QLabel *titulo = new QLabel("Arma el Misterio", panelRompecabezas);
    titulo->setGeometry(0, 10, 400, 30);
    titulo->setAlignment(Qt::AlignCenter);
    titulo->setStyleSheet("font-size: 16px; font-weight: bold; color: #333;");

    // Base del rompecabezas como guía visual
    labelBase = new QLabel(panelRompecabezas);
    labelBase->setGeometry(80, 50, 240, 240);
    QPixmap pixBase("C:/Users/Lenovo/Downloads/baseRompecabezas.png");
    labelBase->setPixmap(pixBase.scaled(240, 240, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    labelBase->setStyleSheet("border: 1px solid #888;");

    // Inicializar control de piezas
    slotOcupado.resize(6);
    for (int i = 0; i < 6; ++i) {
        slotOcupado[i] = false;
    }

    // Botón para abrir rompecabezas
    btnRompecabezas = new QPushButton("Armar el Misterio", panelInventario);
    btnRompecabezas->setGeometry(10, 520, 130, 35);
    btnRompecabezas->setStyleSheet("background-color: #4CAF50; color: white; font-weight: bold; border-radius: 5px;");
    btnRompecabezas->hide();
    connect(btnRompecabezas, &QPushButton::clicked, this, &Cuarto::on_btnRompecabezas_clicked);

    panelRompecabezas->raise();
}

void Cuarto::on_btnInventario_clicked()
{
    inventarioVisible = !inventarioVisible;
    panelInventario->setVisible(inventarioVisible);

    if (!inventarioVisible && rompecabezasVisible) {
        rompecabezasVisible = false;
        panelRompecabezas->hide();
    }
}

void Cuarto::on_btnRompecabezas_clicked()
{
    rompecabezasVisible = !rompecabezasVisible;
    panelRompecabezas->setVisible(rompecabezasVisible);
}

void Cuarto::verificarInteraccion()
{
    if (!escenario || !escenario->personaje) return;

    QGraphicsPixmapItem* obj = nullptr;
    QString nombreObj = "";

    if (escenario->personaje->collidesWithItem(librero)) {
        obj = librero; nombreObj = "librero";
    }
    else if (escenario->personaje->collidesWithItem(pizarra)) {
        obj = pizarra; nombreObj = "pizarra";
    }
    else if (escenario->personaje->collidesWithItem(sisSolar)) {
        obj = sisSolar; nombreObj = "sisSolar";
    }
    else if (escenario->personaje->collidesWithItem(plantaSuelo)) {
        obj = plantaSuelo; nombreObj = "planta";
    }
    else if (escenario->personaje->collidesWithItem(mesaIzq)) {
        obj = mesaIzq; nombreObj = "mesaIzq";
    }
    else if (escenario->personaje->collidesWithItem(mesaDer)) {
        obj = mesaDer; nombreObj = "mesaDer";
    }

    if (obj) {
        mostrarPregunta(nombreObj);
    }
}

void Cuarto::mostrarPregunta(const QString &objeto)
{
    PreguntaWidget *p = nullptr;

    if (objeto == "librero") {
        p = new PreguntaWidget("Seleccione el mandato cuya obligación viene del miedo al castigo o la búsqueda de un premio:",
                               {"Imperativo Hipotético", "Imperativo categórico", "Ambos", "Ninguno"},
                               "Imperativo Hipotético", escenario->scene, objeto);
    }
    else if (objeto == "pizarra") {
        p = new PreguntaWidget("Para Emanuel Kant, es posible conocer lo que las cosas nos permiten (como lo superficial) a través de nuestros sentidos:",
                               {"Conocimiento Noumenico", "Conocimiento fenoménico", "conocimiento Empírico", "Conocimiento Racional"},
                               "Conocimiento fenoménico", escenario->scene, objeto);
    }
    else if (objeto == "sisSolar") {
        p = new PreguntaWidget("Kant decía que el lema de la ilustración era Sapere aude, que significa:",
                               {"Sopesa tus acciones", "Saber a la fuerza", "Atrévete a saber por ti mismo", "Someterse al conocimiento"},
                               "Atrévete a saber por ti mismo", escenario->scene, objeto);
    }
    else if (objeto == "planta") {
        p = new PreguntaWidget("Kant (igual que Copérnico cambió el centro del universo de la tierra al sol), cambia el centro del conocimiento del objeto al sujeto, a esto se le llama: ",
                               {"Subjetivismo", "Prejuicio", "Suerte", "giro copernicano"},
                               "giro copernicano", escenario->scene, objeto);
    }
    else if (objeto == "mesaIzq") {
        p = new PreguntaWidget("La postura conciliadora de Kant respecto a los empiristas y racionalistas define que los datos experimentales son la fuente del conocimiento racional del sujeto: ",
                               {"Racionalismo", "Empirismo", "Criticismo", "Escepticismo"},
                               "Criticismo", escenario->scene, objeto);
    }
    else if (objeto == "mesaDer") {
        p = new PreguntaWidget("De las siguientes obras de Emanuel Kant, seleccione aquella que define su epistemología:",
                               {"Racionalismo", "Critica de la razón pura", "Critica del juicio", "Critica fenomenológica"},
                               "Critica de la razón pura", escenario->scene, objeto);
    }

    if (p) {
        connect(p, &PreguntaWidget::preguntaRespondida, this,
                [this, objeto](const QString &, bool esCorrecta) {
                    if (esCorrecta) {
                        MensajeWidget* mensaje = new MensajeWidget(
                            "¡Respuesta correcta!",
                            "C:/Users/Lenovo/Downloads/restanguloPreg.png", this);
                        mensaje->move((width()-mensaje->width())/2 + 250, (height()-mensaje->height())/2);
                        mensaje->show();

                        QGraphicsPixmapItem* objItem = nullptr;
                        if (objeto == "librero") objItem = librero;
                        else if (objeto == "pizarra") objItem = pizarra;
                        else if (objeto == "sisSolar") objItem = sisSolar;
                        else if (objeto == "planta") objItem = plantaSuelo;
                        else if (objeto == "mesaIzq") objItem = mesaIzq;
                        else if (objeto == "mesaDer") objItem = mesaDer;

                        if (objItem) agregarPiezaInventario(objeto, objItem);

                    } else {
                        MensajeWidget* mensaje = new MensajeWidget(
                            "Respuesta incorrecta",
                            "C:/Users/Lenovo/Downloads/restanguloPreg.png", this);
                        mensaje->move((width()-mensaje->width())/2 + 250, (height()-mensaje->height())/2);
                        mensaje->show();
                    }
                });
    }
}

void Cuarto::agregarPiezaInventario(const QString &objeto, QGraphicsPixmapItem* obj)
{
    for (QLabel* slot : slotsInventario) {
        QPixmap pixmapActual = slot->pixmap();

        if (pixmapActual.isNull()) {
            QString rutaPieza = obtenerRutaPieza(objeto);
            QPixmap pix(rutaPieza);
            slot->setPixmap(pix.scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            slot->setStyleSheet("background-color: rgba(255,255,255,150); border: 2px solid #666;");
            slot->setObjectName(objeto);

            piezasRecolectadas++;

            if (piezasRecolectadas == 6) {
                btnRompecabezas->show();
            }

            QGraphicsColorizeEffect* efecto = new QGraphicsColorizeEffect();
            efecto->setColor(Qt::yellow);
            ui->btnInventario->setGraphicsEffect(efecto);

            QTimer::singleShot(400, [this]() {
                ui->btnInventario->setGraphicsEffect(nullptr);
            });
            break;
        }
    }
}

QString Cuarto::obtenerRutaPieza(const QString &nombrePieza)
{
    if (nombrePieza == "librero") return "C:/Users/Lenovo/Downloads/pieza1.png";
    if (nombrePieza == "pizarra") return "C:/Users/Lenovo/Downloads/pieza2.png";
    if (nombrePieza == "sisSolar") return "C:/Users/Lenovo/Downloads/pieza3.png";
    if (nombrePieza == "planta") return "C:/Users/Lenovo/Downloads/pieza4.png";
    if (nombrePieza == "mesaIzq") return "C:/Users/Lenovo/Downloads/pieza5.png";
    if (nombrePieza == "mesaDer") return "C:/Users/Lenovo/Downloads/pieza6.png";
    return "";
}

QSize Cuarto::obtenerTamanoPieza(const QString &nombrePieza)
{
    if (nombrePieza == "librero") return QSize(120, 120);
    if (nombrePieza == "pizarra") return QSize(135, 135);
    if (nombrePieza == "sisSolar") return QSize(120, 120);
    if (nombrePieza == "planta") return QSize(135, 135);
    if (nombrePieza == "mesaIzq") return QSize(120, 120);
    if (nombrePieza == "mesaDer") return QSize(130, 130);
    return QSize(120, 120);
}

QPoint Cuarto::obtenerPosicionCorrecta(const QString &nombrePieza)
{
    // Posiciones relativas a labelBase (80, 50) en panelRompecabezas
    // Basadas en tu pruebaPOS original
    if (nombrePieza == "librero") return QPoint(80, 50);      // pieza1 - superior izq
    if (nombrePieza == "pizarra") return QPoint(160, 50);     // pieza2 - superior centro
    if (nombrePieza == "sisSolar") return QPoint(230, 50);    // pieza3 - superior der
    if (nombrePieza == "planta") return QPoint(80, 155);      // pieza4 - inferior izq
    if (nombrePieza == "mesaIzq") return QPoint(150, 170);    // pieza5 - inferior centro
    if (nombrePieza == "mesaDer") return QPoint(242, 154);    // pieza6 - inferior der
    return QPoint(0, 0);
}

bool Cuarto::eventFilter(QObject *obj, QEvent *event)
{
    QLabel *label = qobject_cast<QLabel*>(obj);
    if (!label) return QWidget::eventFilter(obj, event);

    // Inicio de arrastre desde inventario
    if (event->type() == QEvent::MouseButtonPress) {
        if (slotsInventario.contains(label) && !label->pixmap().isNull()) {
            piezaArrastradaNombre = label->objectName();
            posInicialArrastre = label->pos();

            // Crear QLabel temporal para arrastrar solo la imagen
            imagenArrastrada = new QLabel(this);
            imagenArrastrada->setPixmap(label->pixmap());
            imagenArrastrada->setFixedSize(label->pixmap().size());
            imagenArrastrada->setAttribute(Qt::WA_TransparentForMouseEvents, false);
            imagenArrastrada->raise();
            imagenArrastrada->show();

            // Posicionar en la posición del mouse
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            QPoint globalPos = label->mapToGlobal(mouseEvent->pos());
            QPoint parentPos = this->mapFromGlobal(globalPos);
            imagenArrastrada->move(parentPos - QPoint(imagenArrastrada->width()/2, imagenArrastrada->height()/2));

            return true;
        }
    }

    // Mover la imagen
    if (event->type() == QEvent::MouseMove && imagenArrastrada) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        QPoint globalPos = QCursor::pos();
        QPoint parentPos = this->mapFromGlobal(globalPos);
        imagenArrastrada->move(parentPos - QPoint(imagenArrastrada->width()/2, imagenArrastrada->height()/2));
        return true;
    }

    // Soltar la pieza
    if (event->type() == QEvent::MouseButtonRelease && imagenArrastrada) {
        QPoint globalPos = imagenArrastrada->mapToGlobal(QPoint(imagenArrastrada->width()/2, imagenArrastrada->height()/2));
        QPoint posEnRompecabezas = panelRompecabezas->mapFromGlobal(globalPos);

        if (panelRompecabezas->rect().contains(posEnRompecabezas) && rompecabezasVisible) {
            QPoint posCorrecta = obtenerPosicionCorrecta(piezaArrastradaNombre);

            if (verificarCercaDePosicion(posEnRompecabezas, posCorrecta)) {
                // Crear QLabel permanente para la pieza en el rompecabezas
                QLabel *piezaColocada = new QLabel(panelRompecabezas);
                QSize tamanoPieza = obtenerTamanoPieza(piezaArrastradaNombre);
                QString rutaPieza = obtenerRutaPieza(piezaArrastradaNombre);
                QPixmap pixPieza(rutaPieza);

                piezaColocada->setPixmap(pixPieza.scaled(tamanoPieza, Qt::KeepAspectRatio, Qt::SmoothTransformation));
                piezaColocada->setGeometry(posCorrecta.x(), posCorrecta.y(), tamanoPieza.width(), tamanoPieza.height());
                piezaColocada->setStyleSheet("background: transparent; border: none;");
                piezaColocada->show();
                piezaColocada->raise();

                piezasColocadas.append(piezaColocada);

                // Marcar slot como ocupado
                int slotIndex = -1;
                if (piezaArrastradaNombre == "librero") slotIndex = 0;
                else if (piezaArrastradaNombre == "pizarra") slotIndex = 1;
                else if (piezaArrastradaNombre == "sisSolar") slotIndex = 2;
                else if (piezaArrastradaNombre == "planta") slotIndex = 3;
                else if (piezaArrastradaNombre == "mesaIzq") slotIndex = 4;
                else if (piezaArrastradaNombre == "mesaDer") slotIndex = 5;

                if (slotIndex >= 0) {
                    slotOcupado[slotIndex] = true;
                }

                // Limpiar slot del inventario
                for (QLabel* slot : slotsInventario) {
                    if (slot->objectName() == piezaArrastradaNombre) {
                        slot->clear();
                        slot->setStyleSheet("background-color: rgba(200,200,200,150); border: 2px dashed #888;");
                        slot->setObjectName("");
                        break;
                    }
                }

                verificarRompecabezasCompleto();
            } else {
                // Posición incorrecta
                MensajeWidget* mensaje = new MensajeWidget(
                    "Esta pieza no va ahí",
                    "C:/Users/Lenovo/Downloads/restanguloPreg.png", this);
                mensaje->move((width()-mensaje->width())/2, (height()-mensaje->height())/2);
                mensaje->show();
            }
        }

        // Eliminar imagen temporal
        delete imagenArrastrada;
        imagenArrastrada = nullptr;
        piezaArrastradaNombre = "";
        return true;
    }

    return QWidget::eventFilter(obj, event);
}

bool Cuarto::verificarCercaDePosicion(const QPoint &posActual, const QPoint &posCorrecta)
{
    /*int tolerancia = 50; // Píxeles de tolerancia
    int distancia = (posActual - posCorrecta).manhattanLength();
    return distancia < tolerancia;*/

    int toleranciaX = 80; // Tolerancia horizontal
    int toleranciaY = 80; // Tolerancia vertical

    int dx = qAbs(posActual.x() - posCorrecta.x());
    int dy = qAbs(posActual.y() - posCorrecta.y());

    return (dx <= toleranciaX && dy <= toleranciaY);
}

void Cuarto::verificarRompecabezasCompleto()
{
     bool completo = true;
    for (bool ocupado : slotOcupado) {
        if (!ocupado) {
            completo = false;
            break;
        }
    }

    if (completo) {
        QTimer::singleShot(500, [this]() {
            QMessageBox::information(this, "¡Felicidades!",
                                     "¡Has completado el rompecabezas del misterio de Kant!\n\n"
                                     "Has demostrado gran conocimiento sobre la filosofía kantiana.");
        });
    }
}








