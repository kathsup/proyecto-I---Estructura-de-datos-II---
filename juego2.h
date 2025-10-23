#ifndef JUEGO2_H
#define JUEGO2_H

#include <QWidget>

namespace Ui {
class juego2;
}

class juego2 : public QWidget
{
    Q_OBJECT

public:
    explicit juego2(QWidget *parent = nullptr);
    ~juego2();

private:
    Ui::juego2 *ui;
};

#endif // JUEGO2_H
