#include"new_window.hpp"
#include"main_window.hpp"
#include<QLabel>
#include<QFileDialog>
#include<QMessageBox>
#include<QDir>
// new window constructor - (init)
NewWindow::NewWindow(game::Level *lvl, QWidget *parent) : QDialog(parent), level{lvl} {
    setFixedSize(640, 320);
    setWindowTitle(tr("Create a New Map"));
    setWindowIcon(QIcon(":/images/col2.bmp"));
    page_width1 = new QSpinBox(this);
    page_height1 = new QSpinBox(this);
    page_width1->setValue(2);
    page_height1->setValue(2);
    page_width1->setMinimum(1);
    page_height1->setMinimum(1);

    QLabel *lbl_width = new QLabel(tr("Width: "), this);
    QLabel *lbl_height = new QLabel(tr("Height: "), this);

    lbl_width->setGeometry(25, 25, 75, 20);
    page_width1->setGeometry(100, 25, 100, 20);
    lbl_height->setGeometry(225, 25, 75, 20);
    page_height1->setGeometry(300, 25, 100, 20);

    page_create = new QPushButton(tr("Create"), this);
    page_create->setGeometry(640-125, 320-35, 100, 25);
    connect(page_create, SIGNAL(clicked()), this, SLOT(createMap()));

    QLabel *gfx_lbl = new QLabel(tr("Gfx File: "), this);
    gfx_lbl->setGeometry(25, 55, 75, 25);
    gfx_box = new QComboBox(this);
    gfx_box->setGeometry(75, 55, 200, 25);

    gfx_box->addItem(QDir::currentPath() + "/../scroller/assets/img/level.gfx");
    gfx_box->setCurrentIndex(0);

    page_new_gfx = new QPushButton(tr("New"), this);
    page_new_gfx->setGeometry(285, 55, 50, 25);
    connect(page_new_gfx, SIGNAL(clicked()), this, SLOT(createNewGfx()));
    
    extract_dir = new QLabel(tr("Set Extract Dir: "), this);
    extract_dir->setGeometry(285+50+10+40+5, 55, 640-(285+50+10+40+5)-20, 25);

    page_extract_dir = new QPushButton(tr("Dir"), this);
    page_extract_dir->setGeometry(285+50+10, 55, 40, 25);

    connect(page_extract_dir, SIGNAL(clicked()), this, SLOT(setDirectory()));

    QLabel *lbl_pg_name = new QLabel(tr("Level Name: "), this);
    lbl_pg_name->setGeometry(25, 100, 100, 25);

    page_name = new QLineEdit(this);
    page_name->setText(tr("Untitled Level"));
    page_name->setGeometry(125, 100, 200, 25);

    QLabel *lbl_bg_name = new QLabel(tr("Background: "), this);
    lbl_bg_name->setGeometry(25, 140, 120, 25);

    page_background = new QLineEdit(this);
    page_background->setText(QDir::currentPath() + "/../img/backgrounds/bg1.bmp");
    page_background->setGeometry(125, 140, 200, 25);

}
// create a new gfx file show gfx window
void NewWindow::createNewGfx() {
    main_window->showGfx();
}

// set directory
void NewWindow::setDirectory() {
    QString dir = QFileDialog::getExistingDirectory(this, "Directory", "");
    if(dir != "") {
        extract_dir->setText(dir);
    }    
}
// set main window
void NewWindow::setMainWindow(MainWindow *w) {
    main_window = w;
}
// create map
void NewWindow::createMap() {

    if(extract_dir->text() == tr("Set Extract Dir: ")) {
        QMessageBox msgbox;
        msgbox.setText("You must set directory extract path, click the Dir button");
        msgbox.setWindowTitle("Must set path");
        msgbox.setIcon(QMessageBox::Icon::Warning);
        msgbox.exec();
        return;
    }

    int width = page_width1->value();
    int height = page_height1->value();
    QString levelName = page_name->text();
    level->setLevelName(levelName.toStdString());
    level->create(MAP_WIDTH * width, MAP_HEIGHT * height, game::Tile{});
    if(main_window->loadGfx(gfx_box->currentText(), extract_dir->text(), page_background->text())) {
        main_window->createdNewMap();
    } 
    hide();
}