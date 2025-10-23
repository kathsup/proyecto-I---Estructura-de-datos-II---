#include "juego3.h"
#include "ui_juego3.h"

juego3::juego3(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::juego3)
{
    ui->setupUi(this);
}

juego3::~juego3()
{
    delete ui;
}
