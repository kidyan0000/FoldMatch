#ifndef CLOTH_WINDOW_H
#define CLOTH_WINDOW_H

#include <QMainWindow>

namespace Ui {
class Cloth_Window;
}

class Cloth_Window : public QMainWindow
{
    Q_OBJECT

public:
    explicit Cloth_Window(QWidget *parent = nullptr);
    ~Cloth_Window();

private:
    Ui::Cloth_Window *ui;
};

#endif // CLOTH_WINDOW_H
