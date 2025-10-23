#include "juego2.h"
#include "ui_juego2.h"

juego2::juego2(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::juego2)
{
    ui->setupUi(this);
}

juego2::~juego2()
{
    delete ui;
}
