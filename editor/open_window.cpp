#include "open_window.hpp"
#include"main_window.hpp"
#include<QDir>
#include<QFileDialog>
#include<QMessageBox>
// open window constructor - (init)
OpenWindow::OpenWindow(QWidget *parent) : QDialog(parent) {
    setFixedSize(320, 185);
    setWindowTitle("Open Level");
    setWindowIcon(QIcon(":/images/brick.bmp"));
    lbl_lvl = new QLabel(tr("Level: "), this);
    lbl_lvl->setGeometry(10, 10, 320-70, 25);
    QLabel *lbl_gfx = new QLabel(tr("Graphics: "), this);
    lbl_gfx->setGeometry(10, 40, 100, 25);
    QLabel *lbl_bg = new QLabel(tr("Background: "), this);
    lbl_bg->setGeometry(10, 70, 320-70, 25);
    lbl_dir = new QLabel(tr("Directory: "), this);
    lbl_dir->setGeometry(10, 100, 320-70, 25);
    open_gfx_box = new QComboBox(this);
    open_gfx_box->setGeometry(110, 40, 320-115-50, 25);
    open_gfx_box->addItem(QDir::currentPath() + "/../img/level.gfx");
    open_gfx = new QPushButton(tr("+"), this);
    open_gfx->setGeometry(320-55, 40, 45, 25);
    open_bg_box = new QComboBox(this);
    open_bg_box->setGeometry(110, 70, 320-115-50, 25);
    open_bg_box->addItem(QDir::currentPath() + "/../img/backgrounds/bg1.bmp");
    open_bg_box->addItem(QDir::currentPath() + "/../img/backgrounds/bg2.bmp");
    open_bg_box->addItem(QDir::currentPath() + "/../img/backgrounds/bg3.bmp");
    open_bg = new QPushButton(tr("+"), this);
    open_bg->setGeometry(320-55,70, 45, 25);
    open_level = new QPushButton(tr(".."), this);
    open_level->setGeometry(320-55, 10, 45, 25);
    open_func = new QPushButton(tr("Open"), this);
    open_func->setGeometry(320-75, 150, 65, 25);
    open_dir = new QPushButton(tr(".."), this);
    open_dir->setGeometry(320-55, 100, 45, 25);
    open_cancel = new QPushButton(tr("Cancel"),this);
    open_cancel->setGeometry(320-55-85-10, 150, 65, 25);
    connect(open_level, SIGNAL(clicked()), this, SLOT(openLevel()));
    connect(open_gfx, SIGNAL(clicked()), this, SLOT(openGfx()));
    connect(open_bg, SIGNAL(clicked()), this, SLOT(openBg()));
    connect(open_dir, SIGNAL(clicked()), this, SLOT(openDir()));
    connect(open_func, SIGNAL(clicked()), this, SLOT(openFunc()));
    connect(open_cancel, SIGNAL(clicked()), this, SLOT(openCancel()));
}

// set main window
void OpenWindow::setMainWindow(MainWindow *main) {
    main_window = main;
}

// open level
void OpenWindow::openLevel() {
    QString lvl_name = QFileDialog::getOpenFileName(this, "Open Level", "", "Level Files (*.lvl *.txt)");
    if(lvl_name != "") {
        lbl_lvl->setText(lvl_name);
        level_file = lvl_name;
    }
}
// open - select gfx file
void OpenWindow::openGfx() {

    QString gfx_file = QFileDialog::getOpenFileName(this, "Open GFX File", "", "GFX Files (*.gfx)");
    if(gfx_file != "") {
        open_gfx_box->addItem(gfx_file);
        open_gfx_box->setCurrentIndex(open_gfx_box->count()-1);
    }

}
// open - select bg file
void OpenWindow::openBg() {
    QString bg_file = QFileDialog::getOpenFileName(this, "Open bitmap", "", "Bitmaps (*.bmp)");
    if(bg_file != "") {
        open_bg_box->addItem(bg_file);
        open_bg_box->setCurrentIndex(open_bg_box->count()-1);
    }
}
// open - select dir
void OpenWindow::openDir() {
    QString d = QFileDialog::getExistingDirectory(this, "Open Extract Directory", "");
    if(d != "") {
        lbl_dir->setText(d);
        level_dir = d;
    }
}
// open level map
void OpenWindow::openFunc() {

    if(level_dir == "" || level_file == "") {
        QMessageBox msgbox;
        msgbox.setText(tr("Select Level File and Extraction Directory"));
        msgbox.setWindowTitle(tr("Select"));
        msgbox.setIcon(QMessageBox::Icon::Warning);
        msgbox.exec();
        return;
    }

    if(main_window->loadLevelFile(level_file, open_gfx_box->currentText(), open_bg_box->currentText(), level_dir)) {
        hide();
    }
}
// cancel
void OpenWindow::openCancel() {
    hide();
}