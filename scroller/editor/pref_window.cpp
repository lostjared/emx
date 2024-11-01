#include"pref_window.hpp"
#include"main_window.hpp"
#include<QMessageBox>

// preferences window constructor - (init)
PrefWindow::PrefWindow(QWidget *parent) : QDialog(parent) {
    setFixedSize(320, 240);
    QLabel *lbl_txt = new QLabel(tr("Pencil width: "), this);
    lbl_txt->setGeometry(10, 10, 100, 25);
    pref_pen_w = new QSpinBox(this);
    pref_pen_w->setValue(1);
    pref_pen_w->setMinimum(1);
    pref_pen_w->setMaximum(1280);
    pref_pen_w->setGeometry(110,10,100,20);
    QLabel *lbl_h = new QLabel(tr("Pencil height: "), this);
    lbl_h->setGeometry(10, 40, 100, 25);
    pref_pen_h = new QSpinBox(this);
    pref_pen_h->setValue(1);
    pref_pen_w->setMinimum(1);
    pref_pen_h->setMaximum(720);
    pref_pen_h->setGeometry(110, 40, 100, 25);
    pref_save = new QPushButton(tr("Save"), this);
    pref_save->setGeometry(320-70, 240-35, 60, 25);
    connect(pref_save, SIGNAL(clicked()), this, SLOT(saveDetails()));
}
// set main window parent
void PrefWindow::setMainWindow(MainWindow *m) {
    main_window = m;
}
// set max size 
void PrefWindow::setMaxSize(int sx, int sy) {
    pref_pen_w->setMinimum(1);
    pref_pen_w->setMaximum(sx);
    pref_pen_h->setMinimum(1);
    pref_pen_h->setMaximum(sy);
}

// save 
void PrefWindow::saveDetails() {
    int px = pref_pen_w->value();
    int py = pref_pen_h->value();
    if(px == 0 ||  py == 0) {
        QMessageBox box;
        box.setText(tr("Invalid Pencil Size"));
        box.setWindowTitle(tr("invalid size"));
        box.setIcon(QMessageBox::Icon::Warning);
        box.exec();
        return;        
    }
    main_window->setPenSize(px, py);
    hide();

}