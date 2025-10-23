#include "juego4.h"
#include "ui_juego4.h"

juego4::juego4(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::juego4)
{
    ui->setupUi(this);
}

juego4::~juego4()
{
    delete ui;
}
