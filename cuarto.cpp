#include "cuarto.h"
#include "ui_cuarto.h"
#include "preguntawidget.h"
#include "mensajewidget.h"

Cuarto::Cuarto(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Cuarto)
    ,escenario(new Escenario(this))
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
    // Configurar el escenario dentro del QGraphicsView del UI
    QPixmap pixFondo("C:/Users/Lenovo/Downloads/cuarto.png");
    escenario->inicializar(ui->graphicsViewCuarto, pixFondo);

    ui->graphicsViewCuarto->setFocusPolicy(Qt::NoFocus);

    // Crear el personaje
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

 //personaje = new Personaje(spritesDer, spritesIzq, spritesArriba, QPointF(350, 450));

    // ✅ Agregarlo al escenario
    //escenario->agregarPersonaje(personaje);
    escenario->crearPersonaje(spritesDer, spritesIzq, spritesArriba, QPointF(650, 490));

    objetosInteractivos();
    inventario();

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

void Cuarto:: inventario(){

    // Crear panel de inventario (inicialmente oculto)
    panelInventario = new QFrame(this);
    panelInventario->setGeometry(20, 80, 150, 565);
    panelInventario->setStyleSheet("background-color: rgba(255, 255, 255, 180); border: 2px solid #555; border-radius: 10px;");
    panelInventario->hide(); // oculto al inicio

    // Crear "slots" (por ejemplo, 4 huecos para piezas)
    for (int i = 0; i < 6; ++i) {
        QLabel *slot = new QLabel(panelInventario);
        slot->setGeometry(25, 20 + i * 90, 100, 80);
        slot->setStyleSheet("background-color: rgba(200,200,200,150); border: 2px dashed #888;");
        slot->setAlignment(Qt::AlignCenter);
        slotsInventario.append(slot);
    }

    panelInventario->raise();

}



void Cuarto::on_btnInventario_clicked()
{
    inventarioVisible = !inventarioVisible;
    panelInventario->setVisible(inventarioVisible);
}

void Cuarto::verificarInteraccion()
{
    if (!escenario || !escenario->personaje) return;

    QGraphicsPixmapItem* obj = nullptr;
    QString nombreObj = "";

    // Detectar con qué objeto colisiona
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
        p = new PreguntaWidget("Kant decía que el lema de la ilustración era “Sapere aude”, que significa:",
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
