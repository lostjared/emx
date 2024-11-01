#ifndef _EX_WINDOW_H__
#define _EX_WINDOW_H__

#include<QDialog>

class QComboBox;
class QPushButton;
namespace game {
    class Level;
}
class MainWindow;

class ExportWindow : public QDialog {
    Q_OBJECT
public:
    ExportWindow(QWidget *parent = 0);
    void setLevel(game::Level *level);
    void setMainWindow(MainWindow *m);
private:
    QComboBox *export_type;
    QPushButton *export_file;
    game::Level *level;
    MainWindow *main_window;
public slots:
    void exportFile();

};


#endif