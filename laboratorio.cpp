#include "laboratorio.h"
#include "ui_laboratorio.h"
#include <QMessageBox>
#include <QLayoutItem>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QPropertyAnimation>

Laboratorio::Laboratorio(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Laboratorio)
    , escenario(new Escenario(this))
    , npcDes(nullptr)
    , nivelCompletado(false)
    , dialogoVisible(false)
    , indicadoresVisibles(true)
    , mesa1Completada(false)
    , mesa2Completada(false)
    , mesa3Completada(false)
    , mesa4Completada(false)
    , mesaActual("")
    , timerBrillo(nullptr)
    , brilloAlpha(255)
    , brilloSubiendo(false)
    , botonCorrecto(nullptr)
    , ingredientesCompletados(0)        // ← AGREGAR
    , timerBurbujeo(nullptr)            // ← AGREGAR
    , frameActualBurbujeo(0)
{
    ui->setupUi(this);
    setFocusPolicy(Qt::StrongFocus);

    // Timer para verificar proximidad con el NPC
    timerProximidad = new QTimer(this);
    connect(timerProximidad, &QTimer::timeout, this, &Laboratorio::verificarProximidadNPC);

    // Timer para indicadores parpadeantes
    timerParpadeo = new QTimer(this);
    connect(timerParpadeo, &QTimer::timeout, this, &Laboratorio::actualizarIndicadores);
}

Laboratorio::~Laboratorio()
{
    if (timerProximidad) {
        timerProximidad->stop();
        delete timerProximidad;
    }

    if (timerParpadeo) {
        timerParpadeo->stop();
        delete timerParpadeo;
    }

    if (timerBrillo) {
        timerBrillo->stop();
        delete timerBrillo;
    }

    if (timerBurbujeo) {              // ← AGREGAR
        timerBurbujeo->stop();        // ← AGREGAR
        delete timerBurbujeo;         // ← AGREGAR
    }

    delete ui;
}

void Laboratorio::inicializarNivel()
{
    // Configurar el escenario dentro del QGraphicsView del UI
    QPixmap pixFondo("C:/Users/Lenovo/Downloads/labb.png");
    escenario->inicializar(ui->graphicsViewLab, pixFondo);

    ui->graphicsViewLab->setFocusPolicy(Qt::NoFocus);

    // Crear el personaje
    QVector<QPixmap> spritesDer = {
        QPixmap("C:/Users/Lenovo/Downloads/hada1AD.png").scaled(120,120, Qt::KeepAspectRatio, Qt::SmoothTransformation),
        QPixmap("C:/Users/Lenovo/Pictures/hada2AD.png").scaled(120,120, Qt::KeepAspectRatio, Qt::SmoothTransformation),
        QPixmap("C:/Users/Lenovo/Downloads/hada3AD.png").scaled(120,120, Qt::KeepAspectRatio, Qt::SmoothTransformation)
    };

    QVector<QPixmap> spritesIzq = {
        QPixmap("C:/Users/Lenovo/Pictures/hada1Iz.png").scaled(120,120, Qt::KeepAspectRatio, Qt::SmoothTransformation),
        QPixmap("C:/Users/Lenovo/Pictures/hada2Iz.png").scaled(120,120, Qt::KeepAspectRatio, Qt::SmoothTransformation),
        QPixmap("C:/Users/Lenovo/Pictures/hada3Iz.png").scaled(120,120, Qt::KeepAspectRatio, Qt::SmoothTransformation)
    };

    QVector<QPixmap> spritesArriba = {
        QPixmap("C:/Users/Lenovo/Downloads/hada4Ar.png").scaled(120,120, Qt::KeepAspectRatio, Qt::SmoothTransformation),
        QPixmap("C:/Users/Lenovo/Downloads/hada5Ar.png").scaled(120,120, Qt::KeepAspectRatio, Qt::SmoothTransformation)
    };

    objetosInteractivos();
    inicializarNPC();
    crearIndicadores();

    // Crear panel de mesa superior
    panelMesaSuperior = new QFrame(this);
    panelMesaSuperior->setGeometry(200, 100, 600, 500);
    panelMesaSuperior->setStyleSheet(
        "background-color: rgba(255, 255, 255, 220);"
        "border: 3px solid #8B6F47;"
        "border-radius: 15px;"
        );
    panelMesaSuperior->hide();
    panelMesaSuperior->raise();

    timerProximidad->start(100);
    timerParpadeo->start(500);

    escenario->crearPersonaje(spritesDer, spritesIzq, spritesArriba, QPointF(400, 450));
    this->setFocus();
}

void Laboratorio::keyPressEvent(QKeyEvent *event)
{
    if (panelMesaSuperior->isVisible()) {
        if (event->key() == Qt::Key_Escape) {
            cerrarMesaSuperior();
            return;
        }
        return;
    }

    if(escenario) {
        escenario->manejarTecla(event);
    }

    if (event->key() == Qt::Key_E) {
        verificarInteraccion();
    }

    if (event->key() == Qt::Key_Escape) {
        emit volverAlLobby();
    }
}

void Laboratorio::objetosInteractivos()
{
    // Mesa 1 - DESBLOQUEADA
    QPixmap pixMesa1("C:/Users/Lenovo/Downloads/mesaIzq.png");
    mesa1 = escenario->scene->addPixmap(
        pixMesa1.scaled(250, 250, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    mesa1->setPos(50, 370);
    mesa1->setOpacity(1.0);

    // Mesa 2 - BLOQUEADA
    QPixmap pixMesa2("C:/Users/Lenovo/Downloads/mesaIzq.png");
    mesa2 = escenario->scene->addPixmap(
        pixMesa2.scaled(250, 250, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    mesa2->setPos(50, 540);
    mesa2->setOpacity(0.3);

    // Mesa 3 - BLOQUEADA
    QPixmap pixMesa3("C:/Users/Lenovo/Downloads/mesaDer.png");
    mesa3 = escenario->scene->addPixmap(
        pixMesa3.scaled(250, 250, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    mesa3->setPos(780, 370);
    mesa3->setOpacity(0.3);

    // Mesa 4 - BLOQUEADA
    QPixmap pixMesa4("C:/Users/Lenovo/Downloads/mesaDer.png");
    mesa4 = escenario->scene->addPixmap(
        pixMesa4.scaled(250, 250, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    mesa4->setPos(780, 540);
    mesa4->setOpacity(0.3);

    // Caldero central
    QPixmap pixCaldero1("C:/Users/Lenovo/Downloads/caldero1.png");
    caldero1 = escenario->scene->addPixmap(
        pixCaldero1.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    caldero1->setPos(550, 390);
}

void Laboratorio::inicializarNPC()
{
    QPixmap pixDes("C:/Users/Lenovo/Downloads/descartes.png");
    npcDes = new npcCasa(pixDes.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    npcDes->setPos(480, 350);

    QVector<QString> dialogosInicio = {
        "¡Bienvenido a mi laboratorio!",
        "Estoy creando la Poción\nde la Verdad Absoluta,\nuna esencia que revela\nel conocimiento certero.",
        "Pero para crearla necesito\n4 ingredientes mágicos\nque solo aparecerán si aplicas\ncorrectamente mi método filosófico.",
        "Debes visitar cada mesa y\nresponder las preguntas para\nobtener los ingredientes necesarios.",
        "¡Atrévete a dudar de todo\npara encontrar la verdad!"
    };

    QVector<QString> dialogosFinal = {
        "¡Excelente trabajo!\nHas completado la poción.",
        "Has aplicado correctamente\nmi método de la duda metódica.",
        "Comprendes que solo mediante\nla razón encontramos certezas.",
        "Cogito, ergo sum.\nPienso, luego existo.",
        "Has demostrado que la razón\nes el camino hacia la verdad absoluta."
    };

    npcDes->establecerDialogos(dialogosInicio, dialogosFinal);
    escenario->scene->addItem(npcDes);
    npcDes->iniciarAnimacion();
}

void Laboratorio::verificarProximidadNPC()
{
    if (!escenario || !escenario->personaje || !npcDes) return;

    QPointF posPersonaje = escenario->personaje->pos();
    QPointF posNpc = npcDes->pos();

    qreal distanciaX = qAbs(posPersonaje.x() - posNpc.x());
    qreal distanciaY = qAbs(posPersonaje.y() - posNpc.y());
    qreal distancia = qSqrt(distanciaX * distanciaX + distanciaY * distanciaY);

    qreal radioActivacion = 150.0;

    if (distancia < radioActivacion && !dialogoVisible) {
        QString dialogo = npcDes->obtenerDialogoActual(nivelCompletado);
        npcDes->mostrarDialogo(dialogo);
        dialogoVisible = true;
    }
    else if (distancia >= radioActivacion && dialogoVisible) {
        npcDes->ocultarDialogo();
        dialogoVisible = false;
    }
}

void Laboratorio::crearIndicadores()
{
    QMap<QString, QGraphicsPixmapItem*> objetos;
    objetos["mesa1"] = mesa1;

    for (auto it = objetos.begin(); it != objetos.end(); ++it) {
        QString nombre = it.key();
        QGraphicsPixmapItem* obj = it.value();

        QGraphicsEllipseItem* indicador = new QGraphicsEllipseItem(0, 0, 25, 25);
        QBrush brush(QColor(255, 255, 0, 200));
        indicador->setBrush(brush);
        QPen pen(QColor(255, 215, 0), 3);
        indicador->setPen(pen);

        QRectF rect = obj->boundingRect();
        qreal x = obj->pos().x() + (rect.width() / 2) - 12;
        qreal y = obj->pos().y() + 20;
        indicador->setPos(x, y);

        escenario->scene->addItem(indicador);
        indicador->setZValue(1000);
        indicadores[nombre] = indicador;
    }
}

void Laboratorio::actualizarIndicadores()
{
    indicadoresVisibles = !indicadoresVisibles;
    for (auto indicador : indicadores.values()) {
        if (indicador) {
            indicador->setVisible(indicadoresVisibles);
        }
    }
}

void Laboratorio::ocultarIndicador(const QString &mesa)
{
    if (indicadores.contains(mesa)) {
        QGraphicsEllipseItem* indicador = indicadores[mesa];
        if (indicador) {
            escenario->scene->removeItem(indicador);
            delete indicador;
            indicadores.remove(mesa);
        }
    }
}

void Laboratorio::verificarInteraccion()
{
    if (!escenario || !escenario->personaje) return;

    QGraphicsPixmapItem* obj = nullptr;
    QString nombreMesa = "";

    if (!mesa1Completada && escenario->personaje->collidesWithItem(mesa1)) {
        obj = mesa1;
        nombreMesa = "mesa1";
    }
    else if (mesa1Completada && !mesa2Completada &&
             escenario->personaje->collidesWithItem(mesa2)) {
        obj = mesa2;
        nombreMesa = "mesa2";
    }
    else if (mesa2Completada && !mesa3Completada &&
             escenario->personaje->collidesWithItem(mesa3)) {
        obj = mesa3;
        nombreMesa = "mesa3";
    }
    else if (mesa3Completada && !mesa4Completada &&    // ← CAMBIAR ESTA LÍNEA
             escenario->personaje->collidesWithItem(mesa4)) {
        obj = mesa4;
        nombreMesa = "mesa4";
    }

    if (obj && !nombreMesa.isEmpty()) {
        mostrarMesaSuperior(nombreMesa);
    }
}

void Laboratorio::mostrarMesaSuperior(const QString &mesa)
{
    mesaActual = mesa;

    // 1. Detener animaciones
    if (timerBrillo) {
        timerBrillo->stop();
    }

    // 2. ELIMINAR FÍSICAMENTE todos los widgets hijos
    QList<QWidget*> widgets = panelMesaSuperior->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly);
    for (QWidget* widget : widgets) {
        widget->deleteLater();
    }

    // 3. Eliminar el layout existente
    if (panelMesaSuperior->layout()) {
        delete panelMesaSuperior->layout();
    }

    // 4. Limpiar vectores de referencias
    botonesBotellas.clear();
    botonesOpcionesMesa2.clear();
    botonesVelasMesa3.clear();      // ← AGREGAR ESTA LÍNEA
    botonesTarjetasMesa4.clear();
    lblPreguntaMesa1 = nullptr;
    lblPreguntaMesa2 = nullptr;
    lblPreguntaMesa3 = nullptr;     // ← AGREGAR ESTA LÍNEA
    lblPreguntaMesa4 = nullptr;
    botonCorrecto = nullptr;

    // 5. Configurar la mesa correspondiente
    if (mesa == "mesa1") {
        configurarMesa1();
    }
    else if (mesa == "mesa2") {
        configurarMesa2();
    }
    else if (mesa == "mesa3") {
        configurarMesa3();          // ← DESCOMENTAR O AGREGAR ESTA LÍNEA
    }
    else if (mesa == "mesa4") {
        configurarMesa4();
    }

    // 6. Mostrar el panel
    panelMesaSuperior->show();
    panelMesaSuperior->raise();
}

void Laboratorio::configurarMesa1()
{
    QVBoxLayout* layout = new QVBoxLayout(panelMesaSuperior);
    layout->setSpacing(15);
    layout->setContentsMargins(20, 20, 20, 20);

    // Título
    QLabel* lblTitulo = new QLabel("🔍 MESA DE LA EVIDENCIA", panelMesaSuperior);
    lblTitulo->setAlignment(Qt::AlignCenter);
    lblTitulo->setStyleSheet(
        "font-size: 22px;"
        "font-weight: bold;"
        "color: #8B6F47;"
        "background: transparent;"
        "border: none;"
        );
    layout->addWidget(lblTitulo);

    // Imagen de la mesa superior
    QLabel* lblImagenMesa = new QLabel(panelMesaSuperior);
    QPixmap pixMesaSup("C:/Users/Lenovo/Downloads/mesaSuperior.png");
    lblImagenMesa->setPixmap(pixMesaSup.scaled(550, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    lblImagenMesa->setAlignment(Qt::AlignCenter);
    lblImagenMesa->setStyleSheet("border: none; background: transparent;");
    layout->addWidget(lblImagenMesa);

    // Pregunta
    lblPreguntaMesa1 = new QLabel(
        "Son los pasos o razones de la duda metódica:",
        panelMesaSuperior
        );
    lblPreguntaMesa1->setAlignment(Qt::AlignCenter);
    lblPreguntaMesa1->setStyleSheet(
        "font-size: 16px;"
        "font-weight: bold;"
        "color: #333;"
        "background: transparent;"
        "border: none;"
        "padding: 10px;"
        );
    layout->addWidget(lblPreguntaMesa1);

    // Layout horizontal para las botellas
    QHBoxLayout* layoutBotellas = new QHBoxLayout();
    layoutBotellas->setSpacing(10);

    // Limpiar vector previo
    botonesBotellas.clear();

    // Crear 4 botellas
    for (int i = 1; i <= 4; i++) {
        QPushButton* btnBotella = new QPushButton(panelMesaSuperior);

        QString rutaImagen = QString("C:/Users/Lenovo/Downloads/frasco%1.png").arg(i);
        QPixmap pixBotella(rutaImagen);

        if (!pixBotella.isNull()) {
            btnBotella->setIcon(QIcon(pixBotella));
            btnBotella->setIconSize(QSize(100, 120));
        }

        btnBotella->setFixedSize(110, 130);
        btnBotella->setStyleSheet(
            "QPushButton {"
            "  background-color: transparent;"
            "  border: 2px solid #AAA;"
            "  border-radius: 10px;"
            "}"
            "QPushButton:hover {"
            "  border: 3px solid #FFD700;"
            "  background-color: rgba(255, 215, 0, 30);"
            "}"
            );

        btnBotella->setProperty("numeroFrasco", i);
        connect(btnBotella, &QPushButton::clicked, this, [this, i]() {
            onFrascoClicked(i);
        });

        layoutBotellas->addWidget(btnBotella);
        botonesBotellas.append(btnBotella);
    }

    layout->addLayout(layoutBotellas);

    // Botón cerrar
    QPushButton* btnCerrar = new QPushButton("❌ Cerrar [ESC]", panelMesaSuperior);
    btnCerrar->setStyleSheet(
        "QPushButton {"
        "  background-color: #E8E8E8;"
        "  border: 2px solid #AAA;"
        "  border-radius: 10px;"
        "  padding: 10px;"
        "  font-size: 13px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #D0D0D0;"
        "}"
        );
    connect(btnCerrar, &QPushButton::clicked, this, &Laboratorio::cerrarMesaSuperior);
    layout->addWidget(btnCerrar);
}

void Laboratorio::onFrascoClicked(int numeroFrasco)
{
    verificarRespuestaMesa1(numeroFrasco);
}

void Laboratorio::verificarRespuestaMesa1(int numeroFrasco)
{
    if (numeroFrasco == 2) {
        // Respuesta correcta
        lblPreguntaMesa1->setText("✅ ¡Correcto! Has obtenido el primer ingrediente");
        lblPreguntaMesa1->setStyleSheet(
            "font-size: 16px;"
            "font-weight: bold;"
            "color: #2E7D32;"
            "background: transparent;"
            "border: none;"
            "padding: 10px;"
            );

        // Deshabilitar todos los botones
        for (QPushButton* btn : botonesBotellas) {
            btn->setEnabled(false);
        }

        // Animar brillo en el botón correcto
        if (numeroFrasco - 1 < botonesBotellas.size()) {
            botonCorrecto = botonesBotellas[numeroFrasco - 1];
            animarBrilloBoton(botonCorrecto);
        }

        // Marcar mesa como completada después de 2 segundos
        QTimer::singleShot(2000, this, [this]() {
            cerrarMesaSuperior();                    // ← Mover primero
            QTimer::singleShot(300, this, [this]() { // ← Esperar un poco
                agregarIngredienteAlCaldero(1);      // ← AGREGAR ESTO
                mesa1Completada = true;
                ocultarIndicador("mesa1");
                desbloquearSiguienteMesa();
            });
        });

    } else {
        // Respuesta incorrecta
        lblPreguntaMesa1->setText("❌ Incorrecto. Intenta nuevamente.");
        lblPreguntaMesa1->setStyleSheet(
            "font-size: 16px;"
            "font-weight: bold;"
            "color: #C62828;"
            "background: transparent;"
            "border: none;"
            "padding: 10px;"
            );

        // Restaurar texto original después de 1.5 segundos
        QTimer::singleShot(1500, this, [this]() {
            lblPreguntaMesa1->setText("Son los pasos o razones de la duda metódica:");
            lblPreguntaMesa1->setStyleSheet(
                "font-size: 16px;"
                "font-weight: bold;"
                "color: #333;"
                "background: transparent;"
                "border: none;"
                "padding: 10px;"
                );
        });
    }
}

void Laboratorio::animarBrilloBoton(QPushButton* boton)
{
    if (!boton) return;

    botonCorrecto = boton;
    brilloAlpha = 255;
    brilloSubiendo = false;

    if (!timerBrillo) {
        timerBrillo = new QTimer(this);
        connect(timerBrillo, &QTimer::timeout, this, [this]() {
            if (!botonCorrecto) {
                timerBrillo->stop();
                return;
            }

            if (brilloSubiendo) {
                brilloAlpha += 15;
                if (brilloAlpha >= 255) {
                    brilloAlpha = 255;
                    brilloSubiendo = false;
                }
            } else {
                brilloAlpha -= 15;
                if (brilloAlpha <= 100) {
                    brilloAlpha = 100;
                    brilloSubiendo = true;
                }
            }

            botonCorrecto->setStyleSheet(
                QString("QPushButton {"
                        "  background-color: rgba(255, 215, 0, %1);"
                        "  border: 3px solid #FFD700;"
                        "  border-radius: 10px;"
                        "}").arg(brilloAlpha)
                );
        });
    }

    timerBrillo->start(50);
}

void Laboratorio::cerrarMesaSuperior()
{
    // Detener animaciones
    if (timerBrillo) {
        timerBrillo->stop();
    }

    // Limpiar referencias
    botonesBotellas.clear();
    botonesOpcionesMesa2.clear();
    botonesVelasMesa3.clear();          // ← AGREGAR ESTA LÍNEA
    botonesTarjetasMesa4.clear();
    tarjetasVolteadas.clear();
    lblPreguntaMesa1 = nullptr;
    lblPreguntaMesa2 = nullptr;
    lblPreguntaMesa3 = nullptr;         // ← AGREGAR ESTA LÍNEA
    lblPreguntaMesa4 = nullptr;
    botonCorrecto = nullptr;
    mesaActual = "";

    // Ocultar panel
    panelMesaSuperior->hide();
}

void Laboratorio::desbloquearSiguienteMesa()
{
    QGraphicsPixmapItem* siguienteMesa = nullptr;
    QString nombreSiguiente = "";

    if (mesa1Completada && !mesa2Completada) {
        mesa2->setOpacity(1.0);
        siguienteMesa = mesa2;
        nombreSiguiente = "mesa2";
    }
    else if (mesa2Completada && !mesa3Completada) {
        mesa3->setOpacity(1.0);
        siguienteMesa = mesa3;
        nombreSiguiente = "mesa3";
    }
    else if (mesa3Completada && !mesa4Completada) {
        mesa4->setOpacity(1.0);
        siguienteMesa = mesa4;
        nombreSiguiente = "mesa4";
    }

    if (siguienteMesa && !nombreSiguiente.isEmpty()) {
        QGraphicsEllipseItem* indicador = new QGraphicsEllipseItem(0, 0, 25, 25);
        QBrush brush(QColor(255, 255, 0, 200));
        indicador->setBrush(brush);
        QPen pen(QColor(255, 215, 0), 3);
        indicador->setPen(pen);

        QRectF rect = siguienteMesa->boundingRect();
        qreal x = siguienteMesa->pos().x() + (rect.width() / 2) - 12;
        qreal y = siguienteMesa->pos().y() + 20;
        indicador->setPos(x, y);

        escenario->scene->addItem(indicador);
        indicador->setZValue(1000);
        indicadores[nombreSiguiente] = indicador;
    }
}

void Laboratorio::configurarMesa2()
{
    QVBoxLayout* layout = new QVBoxLayout(panelMesaSuperior);
    layout->setSpacing(15);
    layout->setContentsMargins(15, 15, 20, 15);

    // 🌟 TÍTULO
    QLabel* lblTitulo = new QLabel("📜 MESA DEL CONOCIMIENTO", panelMesaSuperior);
    lblTitulo->setAlignment(Qt::AlignCenter);
    lblTitulo->setStyleSheet(
        "font-size: 22px;"
        "font-weight: bold;"
        "color: #8B6F47;"
        "background: transparent;"
        "border: none;"
        );
    layout->addWidget(lblTitulo);

    // 🌟 CONTENEDOR VISUAL (solo para centrar la mesa)
    QWidget* contenedorVisual = new QWidget(panelMesaSuperior);
    contenedorVisual->setFixedHeight(280);
    contenedorVisual->setStyleSheet("background: transparent; border: none;");
    QHBoxLayout* layoutVisual = new QHBoxLayout(contenedorVisual);
    layoutVisual->setContentsMargins(0, 0, 0, 0);
    layoutVisual->setSpacing(0);

    // 🌟 IMAGEN DE LA MESA + PERGAMINO
    QLabel* lblMesaPergamino = new QLabel(contenedorVisual);
    QPixmap pixMesaPergamino("C:/Users/Lenovo/Downloads/mesa_pergamino.png");
    lblMesaPergamino->setPixmap(pixMesaPergamino.scaled(600, 300, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    lblMesaPergamino->setAlignment(Qt::AlignCenter);
    lblMesaPergamino->setStyleSheet("border: none; background: transparent;");
    layoutVisual->addWidget(lblMesaPergamino);
    layout->addWidget(contenedorVisual);

    // 🌟 PREGUNTA SOBRE EL PERGAMINO
    lblPreguntaMesa2 = new QLabel("El primer principio de Descartes es:", lblMesaPergamino);
    lblPreguntaMesa2->setGeometry(80, 40, 400, 60); // ajusta según tu imagen
    lblPreguntaMesa2->setAlignment(Qt::AlignCenter);
    lblPreguntaMesa2->setStyleSheet(
        "font-size: 18px;"
        "font-weight: bold;"
        "color: #3B2F2F;"
        "background: transparent;"
        );

    // 🌟 BOTONES DE OPCIONES SOBRE LA MESA
    botonesOpcionesMesa2.clear();
    QStringList opciones = {
        "A) Es preciso dudar",
        "B) Debemos confiar\nciegamente",
        "C) Nada es importante",
        "D) Todo es posible"
    };

    // posiciones manuales para que queden centrados sobre la mesa
    int startX = 100;   // margen desde la izquierda
    int startY = 100;  // altura sobre la mesa
    int sepX = 190;    // separación horizontal
    int sepY = 40;     // separación vertical

    for (int i = 0; i < 4; i++) {
        QPushButton* btnOpcion = new QPushButton(opciones[i], lblMesaPergamino);
        btnOpcion->setFixedSize(160, 38);
        btnOpcion->move(startX + (i % 2) * sepX, startY + (i / 2) * sepY);
        btnOpcion->setStyleSheet(
            "QPushButton {"
            "  background-color: #f7e3b1;"
            "  border: 2px solid #b58a4c;"
            "  border-radius: 8px;"
            "  font-size: 13px;"
            "  font-weight: bold;"
            "  color: #3B2F2F;"
            "}"
            "QPushButton:hover {"
            "  background-color: #ffefb3;"
            "  border: 2px solid #d4a74e;"
            "}"
            );

        QString letra = QString(QChar('A' + i));
        btnOpcion->setProperty("opcion", letra);
        connect(btnOpcion, &QPushButton::clicked, this, [this, letra]() {
            verificarRespuestaMesa2(letra);
        });
        botonesOpcionesMesa2.append(btnOpcion);
    }

    // 🌟 BOTÓN CERRAR ABAJO
    QPushButton* btnCerrar = new QPushButton("❌ Cerrar [ESC]", panelMesaSuperior);
    btnCerrar->setStyleSheet(
        "QPushButton {"
        "  background-color: #E8E8E8;"
        "  border: 2px solid #AAA;"
        "  border-radius: 10px;"
        "  padding: 10px;"
        "  font-size: 13px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #D0D0D0;"
        "}"
        );
    connect(btnCerrar, &QPushButton::clicked, this, &Laboratorio::cerrarMesaSuperior);
    layout->addWidget(btnCerrar, 0, Qt::AlignCenter);
}


void Laboratorio::verificarRespuestaMesa2(const QString& opcion)
{
    if (opcion == "A") {
        // Respuesta correcta
        lblPreguntaMesa2->setText("✅ ¡Correcto! \"Es preciso dudar\" - Segundo ingrediente obtenido");
        lblPreguntaMesa2->setStyleSheet(
            "font-size: 16px;"
            "font-weight: bold;"
            "color: #2E7D32;"
            "background: transparent;"
            "border: none;"
            "padding: 10px;"
            );

        // Deshabilitar todos los botones
        for (QPushButton* btn : botonesOpcionesMesa2) {
            btn->setEnabled(false);
        }

        // Animar brillo en el botón correcto (opción A = índice 0)
        if (!botonesOpcionesMesa2.isEmpty()) {
            botonCorrecto = botonesOpcionesMesa2[0];
            animarBrilloBoton(botonCorrecto);
        }

        // Marcar mesa como completada después de 2 segundos
        QTimer::singleShot(2000, this, [this]() {
            cerrarMesaSuperior();                    // ← Mover primero
            QTimer::singleShot(300, this, [this]() { // ← Esperar un poco
                agregarIngredienteAlCaldero(2);      // ← AGREGAR ESTO
                mesa2Completada = true;
                ocultarIndicador("mesa2");
                desbloquearSiguienteMesa();
            });
        });

    } else {
        // Respuesta incorrecta
        lblPreguntaMesa2->setText("❌ Incorrecto. Intenta nuevamente.");
        lblPreguntaMesa2->setStyleSheet(
            "font-size: 16px;"
            "font-weight: bold;"
            "color: #C62828;"
            "background: transparent;"
            "border: none;"
            "padding: 10px;"
            );

        // Restaurar texto original después de 1.5 segundos
        QTimer::singleShot(1500, this, [this]() {
            lblPreguntaMesa2->setText("El primer principio de Descartes es:");
            lblPreguntaMesa2->setStyleSheet(
                "font-size: 16px;"
                "font-weight: bold;"
                "color: #333;"
                "background: transparent;"
                "border: none;"
                "padding: 10px;"
                );
        });
    }
}

// ==================== MESA 3: VELAS ENCENDIBLES ====================

void Laboratorio::configurarMesa3()
{
    QVBoxLayout* layout = new QVBoxLayout(panelMesaSuperior);
    layout->setSpacing(15);
    layout->setContentsMargins(15, 15, 20, 15);

    // Título
    QLabel* lblTitulo = new QLabel("🕯️ MESA DEL MÉTODO", panelMesaSuperior);
    lblTitulo->setAlignment(Qt::AlignCenter);
    lblTitulo->setStyleSheet(
        "font-size: 22px;"
        "font-weight: bold;"
        "color: #8B6F47;"
        "background: transparent;"
        "border: none;"
        );
    layout->addWidget(lblTitulo);

    // Contenedor visual
    QWidget* contenedorVisual = new QWidget(panelMesaSuperior);
    contenedorVisual->setFixedHeight(280);
    contenedorVisual->setStyleSheet("background: transparent; border: none;");
    QHBoxLayout* layoutVisual = new QHBoxLayout(contenedorVisual);
    layoutVisual->setContentsMargins(0, 0, 0, 0);
    layoutVisual->setSpacing(0);

    // Imagen de la mesa
    QLabel* lblMesa = new QLabel(contenedorVisual);
    QPixmap pixMesa("C:/Users/Lenovo/Downloads/mesaSuperior.png");
    lblMesa->setPixmap(pixMesa.scaled(600, 300, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    lblMesa->setAlignment(Qt::AlignCenter);
    lblMesa->setStyleSheet("border: none; background: transparent;");
    layoutVisual->addWidget(lblMesa);
    layout->addWidget(contenedorVisual);

    // Pregunta sobre la mesa
    lblPreguntaMesa3 = new QLabel(
        "Una de las siguientes es considerada de las\ncuatro reglas del método en Descartes:",
        lblMesa
        );
    lblPreguntaMesa3->setGeometry(80, 20, 450, 60);
    lblPreguntaMesa3->setAlignment(Qt::AlignCenter);
    lblPreguntaMesa3->setStyleSheet(
        "font-size: 15px;"
        "font-weight: bold;"
        "color: #3B2F2F;"
        "background: transparent;"
        );

    // Velas sobre la mesa
    botonesVelasMesa3.clear();

    QStringList opciones = {"A) Hipótesis", "B) Deducción", "C) Evidencia", "D) Inducción"};

    int startX = 30;
    int startY = 75;
    int velaWidth = 110;
    int velaSpacing = 20;

    for (int i = 0; i < 4; i++) {
        QPushButton* btnVela = new QPushButton(lblMesa);
        btnVela->setFixedSize(velaWidth, 150);
        btnVela->move(startX + i * (velaWidth + velaSpacing), startY);

        // Imagen de vela apagada
        QLabel* lblImagenVela = new QLabel(btnVela);
        QPixmap pixVela("C:/Users/Lenovo/Downloads/velaApagada.png");
        lblImagenVela->setPixmap(pixVela.scaled(80, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        lblImagenVela->setGeometry(15, 10, 80, 100);
        lblImagenVela->setAlignment(Qt::AlignCenter);
        lblImagenVela->setObjectName("imagenVela");

        // Texto de la opción debajo de la vela
        QLabel* lblOpcion = new QLabel(opciones[i], btnVela);
        lblOpcion->setGeometry(5, 115, 100, 30);
        lblOpcion->setAlignment(Qt::AlignCenter);
        lblOpcion->setWordWrap(true);
        lblOpcion->setStyleSheet(
            "font-size: 13px;"
            "font-weight: bold;"
            "color: #3B2F2F;"
            "background: transparent;"
            "border: none;"
            );

        btnVela->setStyleSheet(
            "QPushButton {"
            "  background: transparent;"
            "  border: 2px solid #8B7355;"
            "  border-radius: 12px;"
            "}"
            "QPushButton:hover {"
            "  border: 3px solid #DAA520;"
            "  background-color: rgba(255, 215, 0, 30);"
            "}"
            );

        btnVela->setProperty("indiceVela", i);
        connect(btnVela, &QPushButton::clicked, this, [this, i]() {
            verificarRespuestaMesa3(i);
        });

        botonesVelasMesa3.append(btnVela);
    }

    // Botón cerrar
    QPushButton* btnCerrar = new QPushButton("❌ Cerrar [ESC]", panelMesaSuperior);
    btnCerrar->setStyleSheet(
        "QPushButton {"
        "  background-color: #E8E8E8;"
        "  border: 2px solid #AAA;"
        "  border-radius: 10px;"
        "  padding: 10px;"
        "  font-size: 13px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #D0D0D0;"
        "}"
        );
    connect(btnCerrar, &QPushButton::clicked, this, &Laboratorio::cerrarMesaSuperior);
    layout->addWidget(btnCerrar, 0, Qt::AlignCenter);
}

void Laboratorio::verificarRespuestaMesa3(int indiceVela)
{
    if (indiceVela == 2) { // C) Evidencia es la correcta
        // Encender la vela correcta
        QPushButton* velaCorrecta = botonesVelasMesa3[indiceVela];
        animarEncendidoVela(velaCorrecta);

        lblPreguntaMesa3->setText("✅ ¡Correcto! Evidencia - Tercer ingrediente obtenido");
        lblPreguntaMesa3->setStyleSheet(
            "font-size: 15px;"
            "font-weight: bold;"
            "color: #2E7D32;"
            "background: transparent;"
            );

        // Deshabilitar todas las velas
        for (QPushButton* btn : botonesVelasMesa3) {
            btn->setEnabled(false);
        }

        // Completar mesa después de 2 segundos
        QTimer::singleShot(2000, this, [this]() {
            cerrarMesaSuperior();                    // ← Mover primero
            QTimer::singleShot(300, this, [this]() { // ← Esperar un poco
                agregarIngredienteAlCaldero(3);      // ← AGREGAR ESTO
                mesa3Completada = true;
                ocultarIndicador("mesa3");
                desbloquearSiguienteMesa();
            });
        });

    } else {
        // Respuesta incorrecta
        lblPreguntaMesa3->setText("❌ Incorrecto. La vela no se enciende. Intenta nuevamente.");
        lblPreguntaMesa3->setStyleSheet(
            "font-size: 15px;"
            "font-weight: bold;"
            "color: #C62828;"
            "background: transparent;"
            );

        // Restaurar texto original después de 1 segundo
        QTimer::singleShot(1000, this, [this]() {
            lblPreguntaMesa3->setText(
                "Una de las siguientes es considerada de las\ncuatro reglas del método en Descartes:"
                );
            lblPreguntaMesa3->setStyleSheet(
                "font-size: 15px;"
                "font-weight: bold;"
                "color: #3B2F2F;"
                "background: transparent;"
                );
        });
    }
}

void Laboratorio::animarEncendidoVela(QPushButton* vela)
{
    if (!vela) return;

    // Encontrar el QLabel de la imagen dentro del botón
    QLabel* lblImagenVela = vela->findChild<QLabel*>("imagenVela");
    if (lblImagenVela) {
        // Cambiar a imagen de vela encendida
        QPixmap pixVelaEncendida("C:/Users/Lenovo/Downloads/velaEncendida.png");
        lblImagenVela->setPixmap(pixVelaEncendida.scaled(80, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }

    // Animar con brillo dorado
    botonCorrecto = vela;
    brilloAlpha = 255;
    brilloSubiendo = false;

    if (!timerBrillo) {
        timerBrillo = new QTimer(this);
        connect(timerBrillo, &QTimer::timeout, this, [this]() {
            if (!botonCorrecto) {
                timerBrillo->stop();
                return;
            }

            if (brilloSubiendo) {
                brilloAlpha += 15;
                if (brilloAlpha >= 255) {
                    brilloAlpha = 255;
                    brilloSubiendo = false;
                }
            } else {
                brilloAlpha -= 15;
                if (brilloAlpha <= 100) {
                    brilloAlpha = 100;
                    brilloSubiendo = true;
                }
            }

            botonCorrecto->setStyleSheet(
                QString("QPushButton {"
                        "  background-color: rgba(255, 215, 0, %1);"
                        "  border: 3px solid #FFD700;"
                        "  border-radius: 12px;"
                        "}").arg(brilloAlpha)
                );
        });
    }

    timerBrillo->start(50);
}


// ==================== MESA 4: TARJETAS GIRATORIAS ====================

void Laboratorio::configurarMesa4()
{
    QVBoxLayout* layout = new QVBoxLayout(panelMesaSuperior);
    layout->setSpacing(15);
    layout->setContentsMargins(15, 15, 20, 15);

    // 🌟 TÍTULO
    QLabel* lblTitulo = new QLabel("🎴 MESA DE LA EVIDENCIA", panelMesaSuperior);
    lblTitulo->setAlignment(Qt::AlignCenter);
    lblTitulo->setStyleSheet(
        "font-size: 22px;"
        "font-weight: bold;"
        "color: #8B6F47;"
        "background: transparent;"
        "border: none;"
        );
    layout->addWidget(lblTitulo);

    // 🌟 CONTENEDOR VISUAL
    QWidget* contenedorVisual = new QWidget(panelMesaSuperior);
    contenedorVisual->setFixedHeight(280);
    contenedorVisual->setStyleSheet("background: transparent; border: none;");
    QHBoxLayout* layoutVisual = new QHBoxLayout(contenedorVisual);
    layoutVisual->setContentsMargins(0, 0, 0, 0);
    layoutVisual->setSpacing(0);

    // 🌟 IMAGEN DE LA MESA
    QLabel* lblMesa = new QLabel(contenedorVisual);
    QPixmap pixMesa("C:/Users/Lenovo/Downloads/mesaSuperior.png");
    lblMesa->setPixmap(pixMesa.scaled(600, 300, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    lblMesa->setAlignment(Qt::AlignCenter);
    lblMesa->setStyleSheet("border: none; background: transparent;");
    layoutVisual->addWidget(lblMesa);
    layout->addWidget(contenedorVisual);

    // 🌟 PREGUNTA SOBRE LA MESA
    lblPreguntaMesa4 = new QLabel(
        "Solo debemos aceptar como verdadero aquel conocimiento\nque sea EVIDENTE, CLARO Y DISTINTO:",
        lblMesa
        );
    lblPreguntaMesa4->setGeometry(80, 20, 450, 60);
    lblPreguntaMesa4->setAlignment(Qt::AlignCenter);
    lblPreguntaMesa4->setStyleSheet(
        "font-size: 15px;"
        "font-weight: bold;"
        "color: #3B2F2F;"
        "background: transparent;"
        );

    // 🌟 TARJETAS SOBRE LA MESA
    botonesTarjetasMesa4.clear();
    tarjetasVolteadas = {false, false, false, false};

    QStringList respuestas = {
        "René\nDescartes",
        "David\nHume",
        "George\nBerkeley",
        "Aristóteles"
    };

    int startX = 20;
    int startY = 90;
    int cardWidth = 110;
    int cardSpacing = 20;

    for (int i = 0; i < 4; i++) {
        QPushButton* btnTarjeta = new QPushButton("💮", lblMesa);
        btnTarjeta->setFixedSize(cardWidth, 140);
        btnTarjeta->move(startX + i * (cardWidth + cardSpacing), startY);

        btnTarjeta->setStyleSheet(
            "QPushButton {"
            "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
            "    stop:0 #f8d9e0, stop:1 #f0b6c3);"  // tonos rosados suaves
            "  border: 3px solid #c48a91;"
            "  border-radius: 15px;"
            "  font-size: 42px;"
            "  color: white;"
            "  font-weight: bold;"
            "  box-shadow: 0px 4px 10px rgba(0,0,0,0.2);"
            "}"
            "QPushButton:hover {"
            "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
            "    stop:0 #fbd1dc, stop:1 #eca5b6);"
            "  border: 3px solid #e0a3aa;"
            "}"
            );


        btnTarjeta->setProperty("indice", i);
        btnTarjeta->setProperty("respuesta", respuestas[i]);

        connect(btnTarjeta, &QPushButton::clicked, this, [this, i]() {
            voltearTarjeta(i);
        });

        botonesTarjetasMesa4.append(btnTarjeta);
    }

    // 🌟 BOTÓN CERRAR
    QPushButton* btnCerrar = new QPushButton("❌ Cerrar [ESC]", panelMesaSuperior);
    btnCerrar->setStyleSheet(
        "QPushButton {"
        "  background-color: #E8E8E8;"
        "  border: 2px solid #AAA;"
        "  border-radius: 10px;"
        "  padding: 10px;"
        "  font-size: 13px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #D0D0D0;"
        "}"
        );
    connect(btnCerrar, &QPushButton::clicked, this, &Laboratorio::cerrarMesaSuperior);
    layout->addWidget(btnCerrar, 0, Qt::AlignCenter);
}

void Laboratorio::voltearTarjeta(int indice)
{
    if (indice < 0 || indice >= botonesTarjetasMesa4.size()) return;
    if (tarjetasVolteadas[indice]) return; // Ya está volteada

    QPushButton* tarjeta = botonesTarjetasMesa4[indice];
    QString respuesta = tarjeta->property("respuesta").toString();

    // Animación de volteo (efecto visual simple con escala)
    QPropertyAnimation* animShrink = new QPropertyAnimation(tarjeta, "maximumWidth");
    animShrink->setDuration(150);
    animShrink->setStartValue(tarjeta->width());
    animShrink->setEndValue(0);

    QPropertyAnimation* animGrow = new QPropertyAnimation(tarjeta, "maximumWidth");
    animGrow->setDuration(150);
    animGrow->setStartValue(0);
    animGrow->setEndValue(110);

    connect(animShrink, &QPropertyAnimation::finished, this, [this, tarjeta, respuesta, indice, animGrow]() {
        // Cambiar el contenido de la tarjeta
        tarjeta->setText(respuesta);
        tarjeta->setStyleSheet(
            "QPushButton {"
            "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
            "    stop:0 #ffeaf0, stop:1 #fcd9e3);"  // rosa muy claro tipo “papel de carta”
            "  border: 3px solid #c98b93;"
            "  border-radius: 15px;"
            "  font-size: 15px;"
            "  font-weight: bold;"
            "  color: #5b3b3b;"
            "  padding: 8px;"
            "  box-shadow: 0px 3px 6px rgba(0,0,0,0.2);"
            "}"
            "QPushButton:hover {"
            "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
            "    stop:0 #ffe0eb, stop:1 #fbc1d1);"
            "}"
            );
        animGrow->start(QAbstractAnimation::DeleteWhenStopped);
    });

    connect(animGrow, &QPropertyAnimation::finished, this, [this, indice]() {
        verificarRespuestaMesa4(indice);
    });

    animShrink->start(QAbstractAnimation::DeleteWhenStopped);
}

void Laboratorio::verificarRespuestaMesa4(int indice)
{
    if (indice == 0) { // René Descartes es la correcta
        tarjetasVolteadas[indice] = true;
        QPushButton* tarjetaCorrecta = botonesTarjetasMesa4[indice];

        lblPreguntaMesa4->setText("✅ ¡Correcto! René Descartes - Cuarto ingrediente obtenido");
        lblPreguntaMesa4->setStyleSheet(
            "font-size: 15px;"
            "font-weight: bold;"
            "color: #2E7D32;"
            "background: transparent;"
            );

        // Deshabilitar todas las tarjetas
        for (QPushButton* btn : botonesTarjetasMesa4) {
            btn->setEnabled(false);
        }

        // Animar brillo dorado en tarjeta correcta
        botonCorrecto = tarjetaCorrecta;
        animarBrilloBoton(botonCorrecto);

        // Completar mesa después de 2 segundos
        QTimer::singleShot(2000, this, [this]() {
            cerrarMesaSuperior();                    // ← Mover primero
            QTimer::singleShot(300, this, [this]() { // ← Esperar un poco
                agregarIngredienteAlCaldero(4);      // ← AGREGAR ESTO
                mesa4Completada = true;
                ocultarIndicador("mesa4");
                nivelCompletado = true;

                // Esperar a que termine la animación del ingrediente
                QTimer::singleShot(1500, this, [this]() {
                    if (npcDes) {
                        QString dialogoFinal = npcDes->obtenerDialogoActual(true); // true = nivel completado
                        npcDes->mostrarDialogo(dialogoFinal);
                        dialogoVisible = true; // Mostrar diálogos finales
                    }
                });
            });
        });

    } else {
        // Respuesta incorrecta
        lblPreguntaMesa4->setText("❌ Incorrecto. Intenta nuevamente.");
        lblPreguntaMesa4->setStyleSheet(
            "font-size: 15px;"
            "font-weight: bold;"
            "color: #C62828;"
            "background: transparent;"
            );

        QPushButton* tarjetaIncorrecta = botonesTarjetasMesa4[indice];

        // Voltear de nuevo la tarjeta después de 1 segundo
        QTimer::singleShot(1000, this, [this, tarjetaIncorrecta, indice]() {
            QPropertyAnimation* animShrink = new QPropertyAnimation(tarjetaIncorrecta, "maximumWidth");
            animShrink->setDuration(150);
            animShrink->setStartValue(tarjetaIncorrecta->width());
            animShrink->setEndValue(0);

            QPropertyAnimation* animGrow = new QPropertyAnimation(tarjetaIncorrecta, "maximumWidth");
            animGrow->setDuration(150);
            animGrow->setStartValue(0);
            animGrow->setEndValue(110);

            connect(animShrink, &QPropertyAnimation::finished, this, [this, tarjetaIncorrecta, animGrow]() {
                tarjetaIncorrecta->setText("💮");
                tarjetaIncorrecta->setStyleSheet(
                    "QPushButton {"
                    "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
                    "    stop:0 #f8d9e0, stop:1 #f0b6c3);"  // tonos rosados suaves
                    "  border: 3px solid #c48a91;"
                    "  border-radius: 15px;"
                    "  font-size: 42px;"
                    "  color: white;"
                    "  font-weight: bold;"
                    "  box-shadow: 0px 4px 10px rgba(0,0,0,0.2);"
                    "}"
                    "QPushButton:hover {"
                    "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
                    "    stop:0 #fbd1dc, stop:1 #eca5b6);"
                    "  border: 3px solid #e0a3aa;"
                    "}"
                    );
                animGrow->start(QAbstractAnimation::DeleteWhenStopped);
            });

            connect(animGrow, &QPropertyAnimation::finished, this, [this]() {
                lblPreguntaMesa4->setText(
                    "Solo debemos aceptar como verdadero aquel conocimiento\nque sea EVIDENTE, CLARO Y DISTINTO:"
                    );
                lblPreguntaMesa4->setStyleSheet(
                    "font-size: 15px;"
                    "font-weight: bold;"
                    "color: #3B2F2F;"
                    "background: transparent;"
                    );
            });

            animShrink->start(QAbstractAnimation::DeleteWhenStopped);
        });
    }
}

// ==================== SISTEMA DE CALDERO E INGREDIENTES ====================

void Laboratorio::agregarIngredienteAlCaldero(int numeroMesa)
{
    // Definir rutas de sprites de ingredientes
    QMap<int, QString> rutasIngredientes;
    rutasIngredientes[1] = "C:/Users/Lenovo/Downloads/r1.png";
    rutasIngredientes[2] = "C:/Users/Lenovo/Downloads/estrella.jpg";
    rutasIngredientes[3] = "C:/Users/Lenovo/Downloads/r3.png";
    rutasIngredientes[4] = "C:/Users/Lenovo/Downloads/flor5.png";

    // Crear sprite del ingrediente
    QPixmap pixIngrediente(rutasIngredientes[numeroMesa]);
    QGraphicsPixmapItem* ingrediente = escenario->scene->addPixmap(
        pixIngrediente.scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );

    // Posición inicial (más cerca del caldero, como flotando hacia él)
    QPointF posicionInicial(caldero1->pos().x() - 80, caldero1->pos().y() + 10);
    ingrediente->setPos(posicionInicial);
    ingrediente->setZValue(1000);

    // Posición final (centro del caldero)
    QPointF posicionFinal(caldero1->pos().x() + 25, caldero1->pos().y() + 40);

    // Animación hacia el caldero
    QVariantAnimation* animCaida = new QVariantAnimation();
    animCaida->setDuration(1200);
    animCaida->setStartValue(posicionInicial);
    animCaida->setEndValue(posicionFinal);
    animCaida->setEasingCurve(QEasingCurve::InOutQuad);

    connect(animCaida, &QVariantAnimation::valueChanged, this, [ingrediente](const QVariant &value) {
        ingrediente->setPos(value.toPointF());
    });

    connect(animCaida, &QVariantAnimation::finished, this, [this, ingrediente]() {
        escenario->scene->removeItem(ingrediente);
        delete ingrediente;

        ingredientesCompletados++;
        actualizarCaldero(ingredientesCompletados);
        animarBurbujasCaldero();
    });

    animCaida->start(QAbstractAnimation::DeleteWhenStopped);
}


// ==================== ACTUALIZAR SPRITE DEL CALDERO ====================

void Laboratorio::actualizarCaldero(int numIngredientes)
{
    QString rutaCaldero;
    QSize tamanoCaldero;

    // Cambiar sprite según la cantidad de ingredientes
    if (numIngredientes == 1) {
        rutaCaldero = "C:/Users/Lenovo/Downloads/caldero1.png";
        tamanoCaldero = QSize(100, 100);
    }
    else if (numIngredientes == 2) {
        rutaCaldero = "C:/Users/Lenovo/Downloads/caldero2.png";
        tamanoCaldero = QSize(90, 90);
    }
    else if (numIngredientes >= 3) {
        rutaCaldero = "C:/Users/Lenovo/Downloads/caldero3.png";
        tamanoCaldero = QSize(150, 150);
    }

    QPixmap pixCaldero(rutaCaldero);
    if (!pixCaldero.isNull()) {
        caldero1->setPixmap(pixCaldero.scaled(tamanoCaldero, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    }
}


// ==================== EFECTO DE BURBUJAS (CAMBIO DE SPRITES) ====================

void Laboratorio::animarBurbujasCaldero()
{
    if (timerBurbujeo) {
        timerBurbujeo->stop();
        delete timerBurbujeo;
        timerBurbujeo = nullptr;
    }

    frameActualBurbujeo = 0;
    timerBurbujeo = new QTimer(this);

    connect(timerBurbujeo, &QTimer::timeout, this, [this]() {
        static bool alternar = false;

        // Alternar entre los dos sprites de burbujeo
        QString ruta = alternar
                           ? "C:/Users/Lenovo/Downloads/caldero3.png"
                           : "C:/Users/Lenovo/Downloads/caldero2.png";

        QPixmap pix(ruta);
        if (!pix.isNull()) {
            caldero1->setPixmap(pix.scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }

        alternar = !alternar;
        frameActualBurbujeo++;

        // Después de unas alternancias, dejarlo en el último estado (caldero 3)
        if (frameActualBurbujeo >= 10) {
            timerBurbujeo->stop();
            QPixmap pixFinal("C:/Users/Lenovo/Downloads/caldero3.png");
            caldero1->setPixmap(pixFinal.scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
    });

    timerBurbujeo->start(200);
}


