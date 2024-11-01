#ifndef _GFX_WINDOW_H__
#define _GFX_WINDOW_H__

#include<QDialog>
#include<QComboBox>
#include<QListWidget>
#include<QPushButton>
#include<QCheckBox>
#include<QSpinBox>

class MainWindow;

class GfxWindow : public QDialog {
    Q_OBJECT
public:
    GfxWindow(QWidget *parent = 0);
    void setMainWindow(MainWindow *main);
    void updateList();

public slots:
    void addFile();
    void rmvFile();
    void exportFile();
    void setIndex(int index);
    void loadGfxFile();
    
protected:
    MainWindow *main_window;
    QSpinBox *image_index;
    QComboBox *image_type;
    QListWidget *image_list;
    QComboBox *image_solid;
    QPushButton *image_add, *image_remove, *image_build, *image_open;
    QStringList tile_list, object_list, other_list;

};


#endif