#include "juego1.h"
#include "ui_juego1.h"

juego1::juego1(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::juego1)
{
    ui->setupUi(this);
}

juego1::~juego1()
{
    delete ui;
}
