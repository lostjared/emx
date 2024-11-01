#ifndef _RUN_WINDOW_H_
#define _RUN_WINDOW_H_

#include<QDialog>
#include<QLineEdit>
#include<QComboBox>

class MainWindow;

class RunWindow : public QDialog {
    Q_OBJECT
public:
    RunWindow(QWidget *parent = 0);
    void setMainWindow(MainWindow *m);
    MainWindow *main_window;
    QLineEdit *exec_path, *exec_bg;
    QComboBox *exec_res;

};

#endif
