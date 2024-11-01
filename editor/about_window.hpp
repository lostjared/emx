#ifndef _ABOUT_WINDOW_H__
#define _ABOUT_WINDOW_H__

#include<QDialog>
#include<QTextStream>

class AboutWindow : public QDialog {
    Q_OBJECT
public:
    AboutWindow(QWidget *parent = 0);
    
};


#endif