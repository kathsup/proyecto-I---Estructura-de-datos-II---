#ifndef JUEGO4_H
#define JUEGO4_H

#include <QWidget>

namespace Ui {
class juego4;
}

class juego4 : public QWidget
{
    Q_OBJECT

public:
    explicit juego4(QWidget *parent = nullptr);
    ~juego4();

private:
    Ui::juego4 *ui;
};

#endif // JUEGO4_H
