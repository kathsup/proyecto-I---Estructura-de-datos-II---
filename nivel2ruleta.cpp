#include "nivel2ruleta.h"
#include "ui_nivel2ruleta.h"
#include <QTimer>
#include <QMessageBox>
#include <QRandomGenerator>

#include <QDateTime>



nivel2Ruleta::nivel2Ruleta(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::nivel2Ruleta)
    ,escenario(new Escenario(this))//incluir el escenario al constructor
{
    ui->setupUi(this);
    setFocusPolicy(Qt::StrongFocus);

    //ruleta
    opcionesConAngulo = {
        {0, "Historia"},
        {90, "Ciencia"},
        {180, "Arte"},
        {270, "Política"}
    };

    ruletaItem = nullptr;
    timerGiro = new QTimer(this);
    connect(timerGiro, &QTimer::timeout, this, &nivel2Ruleta::rotarRuleta);
    anguloActual = 0;

}

nivel2Ruleta::~nivel2Ruleta()
{
    delete ui;
}

void nivel2Ruleta::inicializarNivel()
{

    if (nivelInicializado) {
        this->setFocus();
        return;
    }

    // Marcar como inicializado
    nivelInicializado = true;

    // Configurar el escenario dentro del QGraphicsView del UI
    QPixmap pixFondo("C:/Users/Lenovo/Downloads/fondo.png");
    escenario->inicializar(ui->graphicsViewRuleta, pixFondo);

     ui->graphicsViewRuleta->setFocusPolicy(Qt::NoFocus);

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

    //RULETA
    // Crear la ruleta
    QPixmap pixRuleta("C:/Users/Lenovo/Downloads/ruleta.png");
    pixRuleta = pixRuleta.scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ruletaItem = escenario->scene->addPixmap(pixRuleta);

    // Colocar la ruleta en el centro de la pantalla (ajusta según tu fondo)
    ruletaItem->setPos(450, 450);
    ruletaItem->setTransformOriginPoint(pixRuleta.width() / 2, pixRuleta.height() / 2);

    // Triángulo indicador
    QPixmap pixIndicador("C:/Users/Lenovo/Downloads/indicador.png");
    pixIndicador = pixIndicador.scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    indicadorTriangulo = escenario->scene->addPixmap(pixIndicador);

    // Posiciónalo sobre la ruleta, apuntando hacia arriba (ajusta según tu ruleta)
    indicadorTriangulo->setPos(ruletaItem->x() + ruletaItem->pixmap().width()/2 - 25, ruletaItem->y() - 40);

    //BOTON
    QPixmap pix("C:/Users/Lenovo/Downloads/btnRuleta.png");
    pix = pix.scaled(ui->btnGirarRuleta->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    // Crear un icono y ponerlo en el botón
    ui->btnGirarRuleta->setIcon(QIcon(pix));
    ui->btnGirarRuleta->setIconSize(ui->btnGirarRuleta->size());

    // Opcional: quitar texto si solo quieres la imagen
    ui->btnGirarRuleta->setText("");

    ui->btnGirarRuleta->setStyleSheet(
        "QPushButton {"
        "border-radius: 30px;"               // esquinas redondeadas
        "border: 4px solid #e5989e;"            // borde opcional
        "background-color: white;"            // color de fondo si quieres
        "}"
        "QPushButton:hover {"
        "border: 4px solid #b84d5e;"            // cambio de borde al pasar mouse
        "background-color: #e0e0e0;"         // cambio de fondo al pasar mouse
        "}"
        );

    escenario->crearPersonaje(spritesDer, spritesIzq, spritesArriba, QPointF(350, 450));
    this->setFocus();


}

void nivel2Ruleta::keyPressEvent(QKeyEvent *event)
{
    if(escenario) {
        escenario->manejarTecla(event);
    }

    if (event->key() == Qt::Key_Escape) {
        emit volverAlMapa();
    }

    //que se pueda entrar solo si estan abiertas:
    if (event->key() == Qt::Key_G) {
        for (auto* obj : escenario->zonasInteractivas) {
            if (escenario->personaje->collidingItems().contains(obj)) {
                QString etiqueta = obj->data(0).toString();

                // Verificar si la puerta está abierta
                if (puertasAbiertas.contains(etiqueta) && puertasAbiertas[etiqueta]) {
                    emit solicitarCambioJuego(etiqueta);
                    break;
                } else if (puertasAbiertas.contains(etiqueta)) {
                    /*QMessageBox::warning(this, "Puerta cerrada",
                                         "Esta puerta está cerrada. Debes girar la ruleta.");*/
                }
            }
        }
    }
}

void nivel2Ruleta::objetosInteractivos() {
    QPixmap pixPuertaCerrada("C:/Users/Lenovo/Downloads/puerta cerrada.png");
    QPixmap pixEscalada = pixPuertaCerrada.scaled(260, 260, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    // Crear las puertas y guardarlas en el mapa
    puertas["Arte"] = escenario->scene->addPixmap(pixEscalada);
    puertas["Arte"]->setPos(35, 210);
    puertas["Arte"]->setData(0, "Arte");

    puertas["Historia"] = escenario->scene->addPixmap(pixEscalada);
    puertas["Historia"]->setPos(280, 210);
    puertas["Historia"]->setData(0, "Historia");

    puertas["Política"] = escenario->scene->addPixmap(pixEscalada);
    puertas["Política"]->setPos(535, 210);
    puertas["Política"]->setData(0, "Política");

    puertas["Ciencia"] = escenario->scene->addPixmap(pixEscalada);
    puertas["Ciencia"]->setPos(775, 210);
    puertas["Ciencia"]->setData(0, "Ciencia");

    puertasAbiertas["Arte"] = true;
    puertasAbiertas["Historia"] = true;
    puertasAbiertas["Política"] = true;
    puertasAbiertas["Ciencia"] = false;

    // Agregar a zonas interactivas
    for (auto puerta : puertas.values()) {
        escenario->zonasInteractivas.append(puerta);
    }
}




//RULETA
void nivel2Ruleta::rotarRuleta() {
    anguloActual += 20; // gira 20° por actualización
    if (anguloActual >= 360) anguloActual = 0;
    ruletaItem->setRotation(anguloActual);
}


void nivel2Ruleta::iniciarGiro() {
    if (opcionesConAngulo.isEmpty()) {
        QMessageBox::information(this, "Ruleta", "¡Ya se usaron todas las opciones!");
        return;
    }

    // Crea un nuevo generador con semilla basada en el tiempo actual
    QRandomGenerator generator(QDateTime::currentMSecsSinceEpoch());

    // Resetear a un ángulo aleatorio para simular giro caótico
    anguloActual = generator.bounded(360);
    ruletaItem->setRotation(anguloActual);

    timerGiro->start(50); // velocidad del giro

    // Parar el giro después de un tiempo aleatorio entre 2 y 4 segundos
    int duracion = 2000 + generator.bounded(2000);
    QTimer::singleShot(duracion, this, [this]() {
        timerGiro->stop();
        elegirOpcionAleatoria();
    });
}

void nivel2Ruleta::elegirOpcionAleatoria() {
    if (opcionesConAngulo.isEmpty()) {
        /*QMessageBox::information(this, "Ruleta", "¡Ya se usaron todas las opciones!");*/
        return;
    }

    // Crea un nuevo generador con semilla basada en el tiempo actual
    QRandomGenerator generator(QDateTime::currentMSecsSinceEpoch());

    // Elegir un índice aleatorio de los disponibles
    int randIdx = generator.bounded(opcionesConAngulo.size());
    int anguloOpcion = opcionesConAngulo.keys()[randIdx];
    opcionElegida = opcionesConAngulo[anguloOpcion];

    // Sin correcciones, usar el ángulo directamente
    int anguloRotacion = anguloOpcion;

    // Girar la ruleta con vueltas completas
    int vueltasCompletas = 1080;
    int anguloFinal = vueltasCompletas + anguloRotacion;

    int duracionAnimacion = 2000;
    int pasos = 50;
    int intervalo = duracionAnimacion / pasos;
    int paso = 0;

    float anguloInicio = anguloActual;
    float diferencia = anguloFinal - anguloInicio;

    QTimer *animTimer = new QTimer(this);
    connect(animTimer, &QTimer::timeout, this, [this, animTimer, paso, anguloInicio, diferencia, pasos, anguloFinal, anguloOpcion]() mutable {
        paso++;

        float t = (float)paso / pasos;
        float easingT = 1.0f - (1.0f - t) * (1.0f - t);

        float anguloActualizado = anguloInicio + (diferencia * easingT);
        ruletaItem->setRotation(anguloActualizado);

        if (paso >= pasos) {
            animTimer->stop();
            anguloActual = anguloFinal % 360;
            ruletaItem->setRotation(anguloActual);

            QMessageBox::information(this, "Ruleta", "La puerta elegida es: " + opcionElegida);


            opcionesConAngulo.remove(anguloOpcion);

            //marcar la puerta como abierta:
            if (puertasAbiertas.contains(opcionElegida)) {
            puertasAbiertas[opcionElegida] = true;
        }

            if (puertas.contains(opcionElegida)) {
                QPixmap pixPuertaAbierta("C:/Users/Lenovo/Downloads/puerta abierta.png");
                pixPuertaAbierta = pixPuertaAbierta.scaled(260, 260, Qt::KeepAspectRatio, Qt::SmoothTransformation);
                puertas[opcionElegida]->setPixmap(pixPuertaAbierta);
            }

            animTimer->deleteLater();
        }
    });

    animTimer->start(intervalo);
}

void nivel2Ruleta::on_btnGirarRuleta_clicked()
{
     iniciarGiro();
    this->setFocus();
}

