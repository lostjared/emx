#include"gfx_window.hpp"
#include"main_window.hpp"
#include"gfx_file.hpp"
#include<QLabel>
#include<QListWidgetItem>
#include<QFileDialog>
#include<QMessageBox>
// GfxWindow constructor - (init)
GfxWindow::GfxWindow(QWidget *parent) : QDialog(parent) {
    setFixedSize(400, 400);
    setWindowTitle("Graphics");
    setWindowIcon(QIcon(":/images/col3.bmp"));
    QLabel *lbl_type = new QLabel(tr("Type:"), this);
    lbl_type->setGeometry(10, 10, 75, 20);
    image_type = new QComboBox(this);
    image_type->setGeometry(80, 10, 300, 25);
    image_type->addItem(tr("Tile"));
    image_type->addItem(tr("Object"));
    image_type->addItem(tr("Other"));
    image_type->setCurrentIndex(0);
    image_list = new QListWidget(this);
    image_list->setGeometry(10, 50, 380, 300);
    image_add = new QPushButton(tr("+"), this);
    image_add->setGeometry(10,360,50, 25);
    image_remove = new QPushButton(tr("-"), this);
    image_remove->setGeometry(65,360,50,25);
    image_solid = new QComboBox(this);
    image_solid->setGeometry(65+50+5,360,85,25);

    image_solid->addItem(tr("Clear"));
    image_solid->addItem(tr("Solid"));
    image_solid->addItem(tr("Kill"));
    image_solid->addItem(tr("Complete"));

    image_build = new QPushButton(tr("Export"), this);
    image_build->setGeometry(400-10-60, 360, 60, 25);
    image_open = new QPushButton(tr("Open"), this);
    image_open->setGeometry(400-10-60-60-5, 360, 60, 25);

    connect(image_add, SIGNAL(clicked()), this, SLOT(addFile()));
    connect(image_remove, SIGNAL(clicked()), this, SLOT(rmvFile()));
    connect(image_build, SIGNAL(clicked()), this, SLOT(exportFile()));
    connect(image_type, SIGNAL(currentIndexChanged(int)), this, SLOT(setIndex(int)));
    connect(image_open, SIGNAL(clicked()), this, SLOT(loadGfxFile()));

    image_index = new QSpinBox(this);
    image_index->setValue(0);
    image_index->setMinimum(0);
    image_index->setGeometry(400-10-60-60-5-55, 360, 40, 25);

}

// set MainWindow parent
void GfxWindow::setMainWindow(MainWindow *main) {
    main_window = main;
}
// update the list of items
void GfxWindow::updateList() {

    image_list->clear();

    switch(image_type->currentIndex()) {
        case 0:
        for(int i = 0; i < tile_list.size(); ++i) {
            image_list->addItem(tile_list[i]);
        }
        break;
        case 1:
        for(int i = 0; i < object_list.size(); ++i) {
            image_list->addItem(object_list[i]);
        }
        break;
        case 2:
        for(int i = 0; i < other_list.size(); ++i) {
            image_list->addItem(other_list[i]);
        }
        break;
    }
}

// set new index
void GfxWindow::setIndex(int) {
    updateList();    
}
// add file to list
void GfxWindow::addFile() {
    QStringList filename = QFileDialog::getOpenFileNames(this, tr("Add Image"), "", "Bitmaps (*.bmp)");
    if(filename.size() > 0) {
        for(int i = 0; i < filename.size(); ++i) {
            QString text;
            QTextStream stream(&text);
            stream << filename[i] << ":" << image_solid->currentIndex();
            switch(image_type->currentIndex()) {
                case 0:
                    tile_list.append(text);
                     break;
                case 1:
                    object_list.append(text);
                    break;
                case 2:
                    other_list.append(text);
                    break;
            }
        }
        updateList();
    }
}
// remove file
void GfxWindow::rmvFile() {

    int index = image_list->currentRow();

    switch(image_type->currentIndex()) {
        case 0:
            tile_list.removeAt(index);
        break;
        case 1:
            object_list.removeAt(index);
        break;
        case 2:
            other_list.removeAt(index);
        break;
    }

    updateList();
}
// export file to disk (concaat)
void GfxWindow::exportFile() {
    QString outfile = QFileDialog::getSaveFileName(this, tr("Export File"), "", "Gfx Files (*.gfx)");

    static auto toTextSolid = [](const std::string &text, int &solid, std::string &file) {
            auto pos = text.rfind(":");
            solid = atoi(text.substr(pos+1, text.length()).c_str());
            file = text.substr(0, pos);
    };

    if(outfile != "") {

        game::GfxTable table;
        for(int i = 0; i < tile_list.size(); ++i) {
            int solid;
            std::string file;
            toTextSolid(tile_list[i].toStdString(), solid, file);
            table.addItem(i, solid, 0, file);
        }
        for(int i = 0; i < object_list.size(); ++i) {
            int solid;
            std::string file;
            toTextSolid(object_list[i].toStdString(), solid, file);
            table.addItem(i, solid, 1, file);
        }
        for(int i = 0; i < other_list.size(); ++i) {
            int solid;
            std::string file;
            toTextSolid(other_list[i].toStdString(), solid, file);
            table.addItem(i, solid, image_index->value(), file);
        }

        game::GfxCompress cmp;
        if(cmp.open(outfile.toStdString())) {
            if(cmp.compress(table)) {
                QMessageBox msgbox;
                msgbox.setText(tr("Successfully wrote file"));
                msgbox.setWindowTitle(tr("Sucess"));
                msgbox.setIcon(QMessageBox::Icon::Information);
                msgbox.exec();
                cmp.close();
                main_window->setNewGfx(outfile);
                main_window->debug_window->Log("editor: Exported file to: " + outfile + "\n");
            } else {
                main_window->debug_window->Log("editor: Error could not save file...\n");
                QMessageBox msgbox;
                msgbox.setText(tr("Could not write file error"));
                msgbox.setWindowTitle(tr("Error"));
                msgbox.setIcon(QMessageBox::Icon::Warning);
                msgbox.exec();
            }
        }
    }
}

// load gfx file to temporary directory
void GfxWindow::loadGfxFile() {
    QString infile = QFileDialog::getOpenFileName(this, tr("Open Graphics File"), "", "Gfx Files (*.gfx)");
    if(infile != "") {

        QString edir = QFileDialog::getExistingDirectory(this, tr("Extract Directory"), "");
        if(edir != "") {
            game::GfxTable table;
            game::GfxExtract extract;
            if(extract.open(infile.toStdString())) {
                if(extract.extract(table, edir.toStdString())) {
                    tile_list.clear();
                    object_list.clear();
                    for(std::vector<game::GfxItem>::size_type i = 0; i < table.table.size(); i++) {
                        QString text;
                        QTextStream stream(&text);
                        stream << edir << "/" << table.table[i].filename.c_str() << ":" << table.table[i].solid;
                        switch(table.table[i].obj) {
                            case 0:
                            tile_list.append(text);
                            break;
                            case 1:
                            object_list.append(text);
                            break;
                            default:
                            other_list.append(text);
                            break;
                        }
                    }
                    updateList();
                    main_window->setNewGfx(infile);
                    main_window->debug_window->Log(tr("Extracted to dir: ") + edir + "\n");
                }
            }
        }

    }
}
