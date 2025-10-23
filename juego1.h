#ifndef JUEGO1_H
#define JUEGO1_H

#include <QWidget>

namespace Ui {
class juego1;
}

class juego1 : public QWidget
{
    Q_OBJECT

public:
    explicit juego1(QWidget *parent = nullptr);
    ~juego1();

private:
    Ui::juego1 *ui;
};

#endif // JUEGO1_H
