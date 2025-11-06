#include "nivel2ruleta.h"
#include "ui_nivel2ruleta.h"
#include <QTimer>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QDateTime>



nivel2Ruleta::nivel2Ruleta(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::nivel2Ruleta)
    ,escenario(new Escenario(this))
    , estrellasObtenidas(0)
    , puertasJugadas(0)//desde aca para lo de gestionar bando y etc
    , eligiendoBando(false)
    , insigniaEmpirista(nullptr)
    , insigniaRacionalista(nullptr)
    , textoEleccion(nullptr)
    , textoEmpirista(nullptr)
    , textoRacionalista(nullptr)
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

    //vidas
    inicializarEstrellas();

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

//gestion
void nivel2Ruleta::onJuegoCompletado()
{
    puertasJugadas++;
    verificarFinDeRonda();
}

// NUEVO: Verificar si terminó la ronda
void nivel2Ruleta::verificarFinDeRonda()
{
    if (puertasJugadas >= 4) {
        if (estrellasObtenidas >= 1) {
            QTimer::singleShot(500, this, &nivel2Ruleta::mostrarEleccionBando);
        } else {
            QMessageBox::information(this, "Sin puntos",
                                     "No obtuviste ningún punto.\nLa ruleta se reiniciará.");
            QTimer::singleShot(2000, this, &nivel2Ruleta::reiniciarRuleta);
        }
    }
}

// NUEVO: Mostrar pantalla de elección de bando
void nivel2Ruleta::mostrarEleccionBando()
{
    eligiendoBando = true;
    ocultarRuleta();

    // Texto principal: "Elige tu bando:"
    textoEleccion = new QGraphicsTextItem("¡Has completado los desafíos!\n\nElige tu bando:");
    textoEleccion->setFont(QFont("Arial", 20, QFont::Bold));
    textoEleccion->setDefaultTextColor(Qt::white);
    textoEleccion->setPos(300, 100);
    textoEleccion->setTextWidth(400);
    escenario->scene->addItem(textoEleccion);

    // Insignia Empirista (izquierda)
    QPixmap pixEmpirista("C:/Users/Lenovo/Downloads/insigniaEmpirista.png");
    pixEmpirista = pixEmpirista.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    insigniaEmpirista = escenario->scene->addPixmap(pixEmpirista);
    insigniaEmpirista->setPos(150, 280);
    insigniaEmpirista->setData(0, "Empirista");

    // Texto Empirista
    textoEmpirista = new QGraphicsTextItem("EMPIRISTA");
    textoEmpirista->setFont(QFont("Arial", 18, QFont::Bold));
    textoEmpirista->setDefaultTextColor(QColor("#4CAF50"));
    textoEmpirista->setPos(160, 500);
    escenario->scene->addItem(textoEmpirista);

    // Insignia Racionalista (derecha)
    QPixmap pixRacionalista("C:/Users/Lenovo/Downloads/insigniaRacionalista.png");
    pixRacionalista = pixRacionalista.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    insigniaRacionalista = escenario->scene->addPixmap(pixRacionalista);
    insigniaRacionalista->setPos(550, 280);
    insigniaRacionalista->setData(0, "Racionalista");

    // Texto Racionalista
    textoRacionalista = new QGraphicsTextItem("RACIONALISTA");
    textoRacionalista->setFont(QFont("Arial", 18, QFont::Bold));
    textoRacionalista->setDefaultTextColor(QColor("#2196F3"));
    textoRacionalista->setPos(540, 500);
    escenario->scene->addItem(textoRacionalista);

    // Limpiar zonas interactivas y agregar insignias
    escenario->zonasInteractivas.clear();
    escenario->zonasInteractivas.append(insigniaEmpirista);
    escenario->zonasInteractivas.append(insigniaRacionalista);

    // Mover personaje al centro
    escenario->personaje->setPos(450, 400);
}

// NUEVO: Ocultar elementos de la ruleta
void nivel2Ruleta::ocultarRuleta()
{
    ruletaItem->setVisible(false);
    indicadorTriangulo->setVisible(false);
    ui->btnGirarRuleta->setVisible(false);

    for (auto puerta : puertas.values()) {
        puerta->setVisible(false);
    }
}

// NUEVO: Mostrar elementos de la ruleta
void nivel2Ruleta::mostrarRuleta()
{
    ruletaItem->setVisible(true);
    indicadorTriangulo->setVisible(true);
    ui->btnGirarRuleta->setVisible(true);

    for (auto puerta : puertas.values()) {
        puerta->setVisible(true);
    }
}

// NUEVO: Reiniciar todo para una nueva ronda
void nivel2Ruleta::reiniciarRuleta()
{
    puertasJugadas = 0;
    estrellasObtenidas = 0;

    // Resetear estrellas
    for (auto* estrella : iconosEstrellas) {
        estrella->setPixmap(
            QPixmap("C:/Users/Lenovo/Downloads/estrellaGris.png")
                .scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation)
            );
    }

    // Restaurar opciones de la ruleta
    opcionesConAngulo = {
        {0, "Historia"},
        {90, "Ciencia"},
        {180, "Arte"},
        {270, "Política"}
    };

    // Cerrar todas las puertas
    QPixmap pixPuertaCerrada("C:/Users/Lenovo/Downloads/puerta cerrada.png");
    pixPuertaCerrada = pixPuertaCerrada.scaled(260, 260, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    for (auto puerta : puertas.values()) {
        puerta->setPixmap(pixPuertaCerrada);
    }

    // Cerrar todas las puertas
    for (auto key : puertasAbiertas.keys()) {
        puertasAbiertas[key] = false;
    }
}


//VIDAS
void nivel2Ruleta::inicializarEstrellas()
{
    // Crear 4 labels con estrellas grises
    for (int i = 0; i < 4; i++) {
        QLabel *estrella = new QLabel(this);
        estrella->setPixmap(
            QPixmap("C:/Users/Lenovo/Downloads/estrellaGris.png")
                .scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation)
            );

        // Posicionar horizontalmente en la parte superior
        int xPos = 90 + (i * 100);  // Ajusta según tu diseño
        int yPos = 20;

        estrella->setGeometry(xPos, yPos, 50, 50);
        estrella->setStyleSheet("background-color: transparent;");
        estrella->show();

        iconosEstrellas.append(estrella);
    }
}

void nivel2Ruleta::actualizarEstrella()
{
    if (estrellasObtenidas >= 0 && estrellasObtenidas < iconosEstrellas.size()) {
        iconosEstrellas[estrellasObtenidas]->setPixmap(
            QPixmap("C:/Users/Lenovo/Downloads/estrellaColor.png")
                .scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation)
            );
    }
}

void nivel2Ruleta::marcarEstrellaObtenida()
{
    if (estrellasObtenidas < 4) { // Máximo 4 estrellas
        actualizarEstrella();
        estrellasObtenidas++;
    }
}



/*void nivel2Ruleta::keyPressEvent(QKeyEvent *event)
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
                    //QMessageBox::warning(this, "Puerta cerrada",
                                       //  "Esta puerta está cerrada. Debes girar la ruleta.");
                }
            }
        }
    }
}*/


void nivel2Ruleta::keyPressEvent(QKeyEvent *event)
{
    if (eligiendoBando) {
        // Modo elección de bando
        if (event->key() == Qt::Key_G || event->key() == Qt::Key_Return || event->key() == Qt::Key_Space) {
            for (auto* obj : escenario->zonasInteractivas) {
                if (escenario->personaje->collidingItems().contains(obj)) {
                    QString bando = obj->data(0).toString();

                    QMessageBox::information(this, "Bando elegido",
                                             "Has elegido el bando: " + bando + "\n\n¡Prepárate para la batalla!");

                    // Emitir señal para ir a batalla
                    emit irABatalla(bando, estrellasObtenidas);
                    return;
                }
            }
        }

        // Permitir mover al personaje para elegir
        if(escenario) {
            escenario->manejarTecla(event);
        }

        if (event->key() == Qt::Key_Escape) {
            emit volverAlMapa();
        }
    } else {
        // Modo ruleta normal (tu código existente)
        if(escenario) {
            escenario->manejarTecla(event);
        }

        if (event->key() == Qt::Key_Escape) {
            emit volverAlMapa();
        }

        if (event->key() == Qt::Key_G) {
            for (auto* obj : escenario->zonasInteractivas) {
                if (escenario->personaje->collidingItems().contains(obj)) {
                    QString etiqueta = obj->data(0).toString();

                    if (puertasAbiertas.contains(etiqueta) && puertasAbiertas[etiqueta]) {
                        emit solicitarCambioJuego(etiqueta);
                        break;
                    }
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
    puertasAbiertas["Ciencia"] = true;

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

