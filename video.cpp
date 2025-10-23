#include "video.h"
#include "ui_video.h"

Video::Video(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Video)
{
    ui->setupUi(this);
}

Video::~Video()
{
    delete ui;
}
