#include"about_window.hpp"
#include"version_info.hpp"
#include<QLabel>
#include<QIcon>
// About Window constructor - (init)
AboutWindow::AboutWindow(QWidget *parent) : QDialog(parent) {
    setFixedSize(420, 120);
    setWindowTitle(tr("About"));
    setWindowIcon(QIcon(":/images/bluebrick.bmp"));
    QLabel *lbl_img = new QLabel(this);
    lbl_img->setGeometry(25, 25, 64, 64);
    lbl_img->setPixmap(QPixmap(":/images/brick.bmp").scaled(QSize(64, 64)));
    QLabel *lbl_txt = new QLabel(this);
    QString text;
    QTextStream stream(&text);
    stream << "(C) 2024 LostSideDead Software - v" << VERSION_INFO << " - GPL v3";
    lbl_txt->setText(text);
    lbl_txt->setGeometry(25+64+10, 25+16, 350, 45);
    QLabel *lbl_txt2 = new QLabel(this);
    lbl_txt2->setText("Level Editor - by Jared Bruni");
    lbl_txt2->setGeometry(25+64+10,20, 350, 45);

}