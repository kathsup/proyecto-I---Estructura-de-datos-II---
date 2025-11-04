/*#include "nivel3batalla.h"
#include "ui_nivel3batalla.h"

nivel3Batalla::nivel3Batalla(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::nivel3Batalla)
    ,escenario(new Escenario(this))
{
    ui->setupUi(this);
    setFocusPolicy(Qt::StrongFocus);
}

nivel3Batalla::~nivel3Batalla()
{
    delete ui;
}

void nivel3Batalla::inicializarNivel()
{
    // Configurar el escenario dentro del QGraphicsView del UI
    QPixmap pixFondo("C:/Users/Lenovo/Downloads/fondo.png");
    escenario->inicializar(ui->graphicsViewBatalla, pixFondo);

    ui->graphicsViewBatalla->setFocusPolicy(Qt::NoFocus);


    objetosInteractivos();




    this->setFocus();


}

void nivel3Batalla::keyPressEvent(QKeyEvent *event)
{
    if(escenario) {
        escenario->manejarTecla(event);
    }

    if (event->key() == Qt::Key_Escape) {
        emit volverAlMapa();
    }
}

void nivel3Batalla::objetosInteractivos() {

}
*/

#include "nivel3batalla.h"
#include "ui_nivel3batalla.h"

nivel3Batalla::nivel3Batalla(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::nivel3Batalla)
    , escenario(new Escenario(this))
    , personajeRojo(nullptr)
    , personajeBlanco(nullptr)
{
    ui->setupUi(this);
    setFocusPolicy(Qt::StrongFocus);
}

nivel3Batalla::~nivel3Batalla()
{
    delete ui;
}

void nivel3Batalla::inicializarNivel()
{
    QPixmap pixFondo("C:/Users/Lenovo/Downloads/fondo.png");
    escenario->inicializar(ui->graphicsViewBatalla, pixFondo);
    ui->graphicsViewBatalla->setFocusPolicy(Qt::NoFocus);

    objetosInteractivos();

    // Crear personajes
    personajeRojo = new PersonajeBatalla(PersonajeBatalla::ROJO, escenario->scene, this);
    personajeBlanco = new PersonajeBatalla(PersonajeBatalla::BLANCO, escenario->scene, this);

    // Iniciar animaciones idle
    personajeRojo->iniciarAnimacionIdle();
    personajeBlanco->iniciarAnimacionIdle();

    this->setFocus();
}

void nivel3Batalla::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        emit volverAlMapa();
    }

    // TECLAS PARA PROBAR ANIMACIONES (temporal)
    if (event->key() == Qt::Key_1) {
        probarAtaqueRojo();
    }
    if (event->key() == Qt::Key_2) {
        probarAtaqueBlanco();
    }
    if (event->key() == Qt::Key_3) {
        probarDerrotaRojo();
    }
    if (event->key() == Qt::Key_4) {
        probarDerrotaBlanco();
    }
}

void nivel3Batalla::objetosInteractivos()
{
    // Por ahora vacío
}

// Funciones para probar animaciones
void nivel3Batalla::probarAtaqueRojo()
{
    if (personajeRojo && personajeBlanco) {
        personajeRojo->atacar(personajeBlanco);
    }
}

void nivel3Batalla::probarAtaqueBlanco()
{
    if (personajeRojo && personajeBlanco) {
        personajeBlanco->atacar(personajeRojo);
    }
}

void nivel3Batalla::probarDerrotaRojo()
{
    if (personajeRojo) {
        personajeRojo->mostrarDerrota();
    }
}

void nivel3Batalla::probarDerrotaBlanco()
{
    if (personajeBlanco) {
        personajeBlanco->mostrarDerrota();
    }
}

