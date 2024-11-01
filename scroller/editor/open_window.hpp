#ifndef _OPEN_WINDOW_H__
#define _OPEN_WINDOW_H__

#include<QDialog>
#include<QComboBox>
#include<QLabel>
#include<QPushButton>

class MainWindow;

class OpenWindow : public QDialog {
    Q_OBJECT
public:
    OpenWindow(QWidget *parent = 0);
    void setMainWindow(MainWindow *m);
public slots:
    void openLevel();
    void openGfx();
    void openBg();
    void openDir();
    void openFunc();
    void openCancel();
protected:
    MainWindow *main_window;
    QPushButton *open_level, *open_gfx, *open_bg, *open_dir, *open_func, *open_cancel;
    QComboBox *open_gfx_box, *open_bg_box;
    QLabel *lbl_lvl, *lbl_dir;
    QString level_file, level_dir;
};


#endif