#ifndef VIDEO_H
#define VIDEO_H

#include <QWidget>

namespace Ui {
class Video;
}

class Video : public QWidget
{
    Q_OBJECT

public:
    explicit Video(QWidget *parent = nullptr);
    ~Video();

private:
    Ui::Video *ui;
};

#endif // VIDEO_H
