#ifndef JUEGO3_H
#define JUEGO3_H

#include <QWidget>

namespace Ui {
class juego3;
}

class juego3 : public QWidget
{
    Q_OBJECT

public:
    explicit juego3(QWidget *parent = nullptr);
    ~juego3();

private:
    Ui::juego3 *ui;
};

#endif // JUEGO3_H
