#include"run_window.hpp"
#include"main_window.hpp"
#include<QLabel>
#include<QDir>

// run window constructor - (init)
RunWindow::RunWindow(QWidget *parent) : QDialog(parent) {
    setFixedSize(400, 400);
    setWindowTitle(tr("Run Settings"));
    setWindowIcon(QIcon(":/images/col3.bmp"));
    QLabel *lbl_exec = new QLabel(tr("Path: "), this);
    lbl_exec->setGeometry(10, 10, 50, 20);
    exec_path = new QLineEdit(this);
    exec_path->setGeometry(50, 10, 330, 20);
    
    exec_path->setText(QDir::currentPath()+"/..");
    QLabel *lbl_res = new QLabel(tr("Resolution: "), this);
    lbl_res->setGeometry(10, 40, 100, 25);
    exec_res = new QComboBox(this);
    exec_res->setGeometry(100, 40, 400-110, 25);
    exec_res->addItem("640x360");
    exec_res->addItem("1280x720");
    exec_res->addItem("1920x1080");
    exec_res->addItem("3840x2160");
    exec_res->setCurrentIndex(1);
    QLabel *lbl_bg = new QLabel(tr("Background: "), this);
    lbl_bg->setGeometry(10, 80, 100, 25);
    exec_bg = new QLineEdit(this);
    exec_bg->setGeometry(100, 80, 400-115, 25);
    exec_bg->setText(QDir::currentPath() + "../scroller/assets/img/backgrounds/bg1.bmp");
}
// set main window parent
void RunWindow::setMainWindow(MainWindow *main) {
    main_window = main;
}