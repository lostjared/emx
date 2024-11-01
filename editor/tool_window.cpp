#include"tool_window.hpp"
#include<QLabel>

// tool window - constructor (init)
ToolWindow::ToolWindow(QWidget *parent) : QDialog(parent) {
    setFixedSize(150, 480);
    setWindowTitle(tr("Toolbox"));
    setWindowFlag(Qt::Tool);
    
    QLabel *lbl_tile = new QLabel(tr("Tiles: "), this);
    lbl_tile->setGeometry(20, 65, 75, 20);
    tiles = new QComboBox(this);
    tiles->setGeometry(10, 90, 120, 25);
    /*
    const char *fileNames[] = {  "black.bmp", "bluebrick.bmp", "bluesky.bmp", "brick.bmp", "eblock.bmp", "red_brick.bmp", "sand1.bmp", "sand2.bmp", "snow.bmp", "stone.bmp", "stone2.bmp", "stone3.bmp", "stone4.bmp", "grass.bmp", 0 };
    for(uint8_t i = 0; fileNames[i] != 0; ++i) {
        QString fn;
        QTextStream stream(&fn);
        stream << ":/images/" << fileNames[i];
        QPixmap pix(fn);
        tiles->addItem(fileNames[i]);
        tiles->setItemData(i,pix,Qt::DecorationRole);
    }
    tiles->setCurrentIndex(1);*/
    QLabel *lbl_tool = new QLabel(tr("Tool: "), this);
    lbl_tool->setGeometry(20, 10, 75, 20);
    tool = new QComboBox(this);
    tool->setGeometry(10, 30, 120, 25);
    tool->addItem("Pencil");
    tool->addItem("Eraser");
    tool->addItem("Fill");
    tool->addItem("Character");
    tool->setCurrentIndex(0);
    tool->setItemData(0, QPixmap(":/images/pencil.png"), Qt::DecorationRole);
    tool->setItemData(1, QPixmap(":/images/eraser.png"), Qt::DecorationRole);
    tool->setItemData(2, QPixmap(":/images/bucket.png"), Qt::DecorationRole);
    tool->setItemData(3, QPixmap(":/images/char.png"), Qt::DecorationRole);

    QLabel *cam_img = new QLabel(this);
    cam_img->setPixmap(QPixmap(":/images/camera.png"));
    cam_img->setGeometry(5,220, 32, 32);
    QLabel *cam_img2 = new QLabel(this);
    cam_img2->setPixmap(QPixmap(":/images/camera.png"));
    cam_img2->setGeometry(5, 280, 32, 32);

    lbl_x = new QLabel(tr("Camera X: "), this);
    lbl_x->setGeometry(40, 230, 125, 20);

    camera_x = new QScrollBar(Qt::Orientation::Horizontal, this);
    camera_x->setGeometry(10, 250, 120, 25);
    camera_x->setMinimum(0);
    camera_x->setMaximum(1280);

    lbl_y = new QLabel(tr("Camera Y: "), this);
    lbl_y->setGeometry(40, 290, 125, 20);

    camera_y = new QScrollBar(Qt::Orientation::Horizontal, this);
    camera_y->setGeometry(10, 310, 120, 25);
    camera_y->setMinimum(0);
    camera_y->setMaximum(720);

    camera_x->setSliderPosition(0);
    camera_y->setSliderPosition(0);

    QLabel *lbl_obj = new QLabel(tr("Objects"), this);
    lbl_obj->setGeometry(20, 120, 75, 20);

    tile_objects = new QComboBox(this);
    tile_objects->setGeometry(10, 145, 120, 25);

    hover_object = new QCheckBox(tr("Object Cursor"), this);
    hover_object->setGeometry(20, 180, 125, 20);

    QLabel *lbl_e = new QLabel(tr("Chars"), this);
    lbl_e->setGeometry(20, 340, 75, 25);

    char_objects = new QComboBox(this);
    char_objects->setGeometry(10, 340+25, 120, 25);

    char_objects->addItem(tr("Enemy #1"));

}