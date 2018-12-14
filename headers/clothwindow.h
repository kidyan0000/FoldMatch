#ifndef CLOTHWINDOW_H
#define CLOTHWINDOW_H

#include <QMainWindow>

namespace Ui {
class ClothWindow;
}

class ClothWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ClothWindow(QWidget *parent = nullptr);
    ~ClothWindow();

private:
    Ui::ClothWindow *ui;
};

#endif // CLOTHWINDOW_H
