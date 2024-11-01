#include"export_window.hpp"
#include"main_window.hpp"
#include<QIcon>
#include<QComboBox>
#include<QPushButton>
#include<QFileDialog>
#include"../level.hpp"

// About Window constructor - (init)
ExportWindow::ExportWindow(QWidget *parent) : QDialog(parent) {
    setFixedSize(400, 120);
    setWindowTitle(tr("Export"));
    setWindowIcon(QIcon(":/images/bluebrick.bmp"));
    export_type = new QComboBox(this);
    export_type->setGeometry(15, 15, 375, 25);
    export_type->addItem(tr("Export as Text"));
    export_type->addItem(tr("Export as Binary"));
    export_file = new QPushButton("Export", this);
    export_file->setGeometry(400-125, 75, 100, 25);
    connect(export_file, SIGNAL(clicked()), this, SLOT(exportFile()));
}

void ExportWindow::setLevel(game::Level *lvl) {
    level = lvl;
}

void ExportWindow::setMainWindow(MainWindow *m) {
    main_window = m;
}

void ExportWindow::exportFile() {
    switch(export_type->currentIndex()) {
        case 0: {
            QString outfile = QFileDialog::getSaveFileName(this, tr("Export File"), "", "Txt Files (*.txt)");
            if(outfile != "") {
                level->saveLevelText(outfile.toStdString());
                QString text;
                QTextStream stream(&text);
                stream << "editor: Text file outputed to: " << outfile << "\n";
                main_window->debug_window->Log(text);
                hide();
            }
        }
            break;
        case 1: {
            QString outfile = QFileDialog::getSaveFileName(this, tr("Export File"), "", "Lvl Files (*.lvl)");
            if(outfile != "") {
                level->saveLevel(outfile.toStdString());
                QString text;
                QTextStream stream(&text);
                stream << "editor: Binary file outputed to: " << outfile << "\n";
                main_window->debug_window->Log(text);
                hide();
            }
        }
            break;
    }
}