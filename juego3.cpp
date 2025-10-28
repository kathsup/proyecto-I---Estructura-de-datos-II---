#include "juego3.h"
#include "ui_juego3.h"
#include <QMessageBox>


juego3::juego3(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::juego3)
    ,escenario(new Escenario(this))

{
    ui->setupUi(this);
    setFocusPolicy(Qt::StrongFocus);
}

juego3::~juego3()
{
    delete ui;
}

void juego3::inicializarNivel()
{
    // Configurar el escenario dentro del QGraphicsView del UI
    QPixmap pixFondo("C:/Users/Lenovo/Downloads/fondo.png");
    escenario->inicializar(ui->graphicsViewJ3, pixFondo);

    ui->graphicsViewJ3->setFocusPolicy(Qt::NoFocus);

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

    escenario->crearPersonaje(spritesDer, spritesIzq, spritesArriba, QPointF(350, 450));

    inicializarPreguntas();
    cargarPregunta();

    this->setFocus();



}

/*void juego3::keyPressEvent(QKeyEvent *event)
{
    if(escenario) {
        escenario->manejarTecla(event);
    }

    if (event->key() == Qt::Key_Escape) {
        emit volverARuleta();
    }

}*/

void juego3::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Escape) {
        emit volverARuleta();
    }
    else if (event->key() == Qt::Key_E) {
        if (florEnMano) depositarFlor();
        else {
            // buscar flor cercana al personaje (colisión simple)
            for (Flor* f : flores) {
                if (escenario->personaje->collidesWithItem(f)) {
                    recogerFlor(f);
                    break;
                }
            }
            if (florEnMano && escenario->personaje) {
                actualizarPosicionFlor();
            }
        }
    }
    else if (escenario) {
        escenario->manejarTecla(event); // movimiento
         if (florEnMano) actualizarPosicionFlor();
    }
}


void juego3::objetosInteractivos() {
    QPixmap pixcanasta1("C:/Users/Lenovo/Downloads/canasta.png");
    canasta = escenario->scene->addPixmap(
        pixcanasta1.scaled(250, 250, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    canasta->setPos(220, 450);

}

//agregado
void juego3::inicializarPreguntas() {
    preguntas = {
        {"Durante el renacimiento, el modelo de gobierno es uno de los siguientes:", {{"Monarquía absoluta", true}, {" Tiranía republicana", false}, {"Democracia participativa", false}, {"Liberalismo político", false}}},
        {"De los siguientes acontecimientos, selecciones el que inicia el período moderno:", {{"Toma de Constantinopla", false}, {" Tratado de paz de westfalia", true}, {"Toma de la Bastilla", false}, {"La ruta de la seda", false}}},
        {" Durante el siglo XV, la sociedad se estratifica en tres estamentos definidos", {{"Clase media, baja y alta", false}, {"nobleza, clero y estado llano", true}, {"Artesanos y guardianes ", false}, {"Gobernantes", false}}},
        {"Aparece el realismo político, que se basaba en un orden establecido, explicación de un sistema y recomendaciones de como gobernar", {{" Tomás Moro", false}, {" Jean Bodín", false}, {"Nicolas Maquiavelo", true}, {"Erasmo de Rotterdam", false}}},
        {"Terminada la edad media, en el contexto de la política resulta que", {{"La Iglesia resalta su poder", false}, {"La Iglesia pierde el papel rector en la política", true}, {"La Iglesia evangélica se posiciona en la política", false}, {"La política desaparece", false}}},
    };
}

void juego3::cargarPregunta() {
    // Limpiar flores anteriores
    for (Flor* f : flores) {
        escenario->scene->removeItem(f);
        delete f;
    }
    flores.clear();
    florEnMano = nullptr;

    // Mostrar pregunta (puedes usar QLabel arriba)
    ui->labelPregunta->setText(preguntas[preguntaActual].texto);

    // Crear flores en el césped
    int x = 50;
    for (auto& resp : preguntas[preguntaActual].respuestas) {
        QPixmap pix("C:/Users/Lenovo/Downloads/flor3.png"); // imagen genérica
        Flor* f = new Flor(pix.scaled(50,50), resp.texto, resp.esCorrecta);
        f->setPos(x, 350);
        escenario->scene->addItem(f);
        flores.push_back(f);
        x += 100;
    }
}

void juego3::recogerFlor(Flor* flor) {
    if (!flor) return;
    florEnMano = flor;
    flor->setZValue(1); // para que esté por encima

    actualizarPosicionFlor();

}

void juego3::depositarFlor() {
    if (!florEnMano) return;

    // Verificar si el personaje está cerca de la canasta
    QPointF posPersonaje = escenario->personaje->pos();
    QPointF posCanasta = canasta->pos();

    qreal distancia = QLineF(posPersonaje, posCanasta).length();

    if (distancia < 150) { // Ajusta esta distancia según necesites
        if (florEnMano->esRespuestaCorrecta()) {
            floresCorrectas++;
            // Ocultar o remover la flor
            escenario->scene->removeItem(florEnMano);
            flores.removeOne(florEnMano);
            delete florEnMano;
            florEnMano = nullptr;

            QMessageBox::information(this, "¡Correcto!", "¡Respuesta correcta!");
        }
        else {
            QMessageBox::warning(this, "Incorrecto", "Esa no es la respuesta correcta");
            florEnMano->setZValue(0);
            florEnMano = nullptr;
        }

        // Pasar a siguiente pregunta
        preguntaActual++;
        if (preguntaActual < preguntas.size()) {
            cargarPregunta();
        } else {
            finalizarJuego();
        }
    }
    else {
        QMessageBox::information(this, "Aviso", "Debes estar cerca de la canasta para depositar la flor");
    }
}

void juego3::finalizarJuego() {
    // Por ejemplo, mostrar un mensaje con las flores correctas
    QMessageBox::information(this, "Juego terminado",
                             "Has recolectado " + QString::number(floresCorrectas) + " flores correctas de " +
                                 QString::number(preguntas.size()));

    // Aquí puedes reiniciar, volver a la ruleta o cerrar el juego
}

void juego3::actualizarPosicionFlor() {
    if (florEnMano && escenario->personaje) {
        // Coloca la flor encima del personaje
        QPointF posPersonaje = escenario->personaje->pos();
        florEnMano->setPos(posPersonaje.x() + 25, posPersonaje.y() - 30); // ajusta estos valores según necesites
    }
}
