#include"main_window.hpp"
#include<QPainter>
#include<QMenuBar>
#include<QAction>
#include<QDir>
#include<QApplication>
#include<QMouseEvent>
#include<QKeyEvent>
#include<QFileDialog>
#include<QMessageBox>
#include<QProcess>
#include<QThread>
#include "gfx_file.hpp"

// constructor - (window init)
MainWindow::MainWindow() {
    file_name = "Untitled.lvl";
    map_init = false;
    pos_x = pos_y = 0;
    setGeometry(230, 10, 1280, 720+offset_y);
    setFixedSize(1280, 720+offset_y);
    setWindowIcon(QIcon(":/images/col1.bmp"));
    setWindowTitle(tr("Editor [Please Create/Open a Map]"));

    en.push_back(QImage(":/images/char.png"));

    tool_window = new ToolWindow(this);
    tool_window->setGeometry(10, 10, 150, 480);
    tool_window->show(); 

    connect(tool_window->camera_x, SIGNAL(valueChanged(int)), this, SLOT(cameraChanged(int)));
    connect(tool_window->camera_y, SIGNAL(valueChanged(int)), this, SLOT(cameraChanged(int)));    

    new_window = new NewWindow(&level, this);
    new_window->setMainWindow(this);
    new_window->hide();

    run_window = new RunWindow(this);
    run_window->setMainWindow(this);
    run_window->hide();

    debug_window = new DebugWindow(this);
    debug_window->setGeometry(230, 830+offset_y, 640, 320);
    debug_window->show();

    gfx_window = new GfxWindow(this);
    gfx_window->setMainWindow(this);
    gfx_window->setGeometry(450, 175, 800, 600);
    gfx_window->hide();

    open_window = new OpenWindow(this);
    open_window->setMainWindow(this);
    open_window->hide();

    about_window = new AboutWindow(this);
    about_window->hide();

    pref_window = new PrefWindow(this);
    pref_window->setMainWindow(this);
    pref_window->hide();

    export_window = new ExportWindow(this);
    export_window->setLevel(&level);
    export_window->setMainWindow(this);
    export_window->hide();

    file_menu = menuBar()->addMenu(tr("&File"));
    file_new = new QAction(tr("New Map"), this);
    file_new->setShortcut(tr("Ctrl+N"));
    connect(file_new, SIGNAL(triggered()), this, SLOT(openNewMenu()));
    file_menu->addAction(file_new);
    file_menu->addSeparator();
    //setMouseTracking(true);
    file_load = new QAction(tr("Open Map"), this);
    file_load->setShortcut(tr("Ctrl+O"));
    connect(file_load, SIGNAL(triggered()), this, SLOT(loadFile()));
    file_menu->addAction(file_load);
    file_save = new QAction(tr("Save Map"), this);
    file_save->setShortcut(tr("Ctrl+S"));
    connect(file_save, SIGNAL(triggered()), this, SLOT(saveFile()));
    file_menu->addAction(file_save);
    file_save_as = new QAction(tr("Save Map As"), this);
    file_save_as->setShortcut(tr("Ctrl+A"));
    connect(file_save_as, SIGNAL(triggered()), this, SLOT(saveFileAs()));
    file_menu->addAction(file_save_as);

    export_file = new QAction(tr("E&xport"), this);
    export_file->setShortcut(tr("Ctrl+E"));
    connect(export_file, SIGNAL(triggered()), this, SLOT(exportFile()));

    file_menu->addAction(export_file);
    
    file_menu->addSeparator();
    
    
    file_pref = new QAction(tr("Preferences"), this);

    connect(file_pref, SIGNAL(triggered()), this, SLOT(openPref()));

    file_menu->addAction(file_pref);
    file_menu->addSeparator();

    file_exit = new QAction(tr("E&xit"), this);
    connect(file_exit, SIGNAL(triggered()), this, SLOT(shutdownProgram()));
    file_menu->addAction(file_exit);

    level_menu = menuBar()->addMenu(tr("&Level"));
    level_left = new QAction(tr("Scroll Left"));
    level_left->setShortcut(tr("Ctrl+Left"));
    connect(level_left, SIGNAL(triggered()), this, SLOT(levelLeft()));
    level_menu->addAction(level_left);

    level_right = new QAction(tr("Scroll Right"));
    level_right->setShortcut(tr("Ctrl+Right"));
    connect(level_right, SIGNAL(triggered()), this, SLOT(levelRight()));
    level_menu->addAction(level_right);

    level_up = new QAction(tr("Scroll Up"));
    level_up->setShortcut(tr("Ctrl+Up"));
    connect(level_up, SIGNAL(triggered()), this, SLOT(levelUp()));
    level_menu->addAction(level_up);

    level_down = new QAction(tr("Scroll Down"));
    level_down->setShortcut(tr("Ctrl+Down"));
    connect(level_down, SIGNAL(triggered()), this, SLOT(levelDown()));
    level_menu->addAction(level_down);
    connect(tool_window->hover_object, SIGNAL(stateChanged(int)), this, SLOT(updateMap(int)));

    level_menu->addSeparator();

    level_gfx = new QAction(tr("Level Graphics"));
    connect(level_gfx, SIGNAL(triggered()), this, SLOT(levelGraphicsOpen()));
    level_gfx->setShortcut(tr("Ctrl+G"));
    level_menu->addAction(level_gfx);

    run_menu = menuBar()->addMenu(tr("&Run"));
    run_settings = new QAction(tr("Run &Settings"));
    run_settings->setShortcut(tr("Ctrl+E"));
    connect(run_settings, SIGNAL(triggered()), this, SLOT(runSettings()));
    run_menu->addAction(run_settings);
    run_menu->addSeparator();
    run_exec = new QAction(tr("&Run"));
    run_exec->setShortcut(tr("Ctrl+R"));
    connect(run_exec, SIGNAL(triggered()), this, SLOT(runExec()));
    run_menu->addAction(run_exec);

    view_menu = menuBar()->addMenu(tr("&View"));
    view_show_tool = new QAction(tr("Show ToolBox"));
    view_show_tool->setShortcut(tr("Ctrl+T"));
    connect(view_show_tool, SIGNAL(triggered()), this, SLOT(showTool()));

    view_menu->addAction(view_show_tool);

    help_menu = menuBar()->addMenu(tr("&Help"));
    help_about = new QAction(tr("&About"));
    connect(help_about, SIGNAL(triggered()), this, SLOT(showAbout()));
    help_menu->addAction(help_about);    

    setMouseTracking(true);
    debug_window->clear();
    debug_window->Log("editor: successfully initalized..\n");
    cursor_visible = false;
}
// close event
void MainWindow::closeEvent(QCloseEvent *) {
    if(proc_run == true) {
        proc->terminate();
        proc->waitForFinished();
        proc_run = false;
        delete proc;
        proc = nullptr;
    } else if(modified == true && map_init == true) {
        askSave();
   }
}
// shutdown the program
void MainWindow::shutdownProgram() {
    if(proc_run == true) {
        proc->terminate();
        proc->waitForFinished();
        proc_run = false;
        delete proc;
        proc = nullptr;
    } else if(modified == true && map_init == true) {
        askSave();
    }
    QApplication::exit(0);
}
// ask to save
void MainWindow::askSave() {
    QMessageBox msgbox;
    msgbox.setWindowTitle("Do you wish to save?");
    msgbox.setText("Do you wish to save?");
    msgbox.setIcon(QMessageBox::Icon::Question);
    msgbox.setStandardButtons(QMessageBox::Yes);
    msgbox.addButton(QMessageBox::No);
    msgbox.setDefaultButton(QMessageBox::Yes);
    if(msgbox.exec() == QMessageBox::Yes) {
        saveFile();
        modified = false;
    }
}
// read from standard output on QProcess
void MainWindow::readStdout() {
    if(proc_run) {
        QString data = proc->readAll();
        std::cout << data.toStdString() << "\n";
        debug_window->Log(data);
    }
}

// paint the window to screen
void MainWindow::paintEvent(QPaintEvent *) {

    QPainter paint(this);
    paint.fillRect(QRect(0, 0, 1280, 720), QColor(255,255,255));

    for(int i = 0; i < MAP_WIDTH; ++i) {
        for(int z = 0; z < MAP_HEIGHT; ++z) {
            int x = (i*16)+offset_x;
            int y = (z*16)+offset_y;
            game::Tile *tile = level.at(pos_x+i, pos_y+z);
            if(map_init == false) {
                paint.fillRect(QRect(x, y, 15, 15), QColor(0, 0, 0));
            } else {
                if(images.size()>0 && tile->img < images.size())
                   paint.drawImage((i*16)+offset_x, (z*16)+offset_y, images[tile->img], 0, 0, 16, 16);
            }
        }
    }
    drawLayer1(paint);
    drawLayer2(paint);
    drawLayer3(paint);

    if(cursor_visible == true) {
        
        if(images.size()>0) {
            QImage &img = (tool_window->hover_object->isChecked()) ? col[tool_window->tile_objects->currentIndex()] : images[tool_window->tiles->currentIndex()];
            int cx = draw_pos.x(), cy = draw_pos.y();
            int zx = 0, zy = 0;
            if(game::atPoint(cx, cy-offset_y, 16, 16, zx, zy)) {
                cx = (zx*16)+offset_x, cy = (zy*16)+offset_y;
                paint.drawImage(cx, cy, img);
                int im_width = img.width();
                int im_height = img.height();

                if(tool_window->hover_object->isChecked() == false) {
                    im_width = pen_x * 16;
                    im_height = pen_y * 16;
                }
                paint.fillRect(QRect(cx, cy-1, im_width, 1), QColor(qRgb(255, 255, 255)));
                paint.fillRect(QRect(cx, cy+im_height, im_width, 1), QColor(qRgb(255, 255, 255)));
                paint.fillRect(QRect(cx, cy-1, 1, im_height), QColor(qRgb(255,255,255)));
                paint.fillRect(QRect(cx+im_width, cy-1, 1, im_height), QColor(qRgb(255,255,255)));
            }
        }
    }
}

// update the map
void MainWindow::updateMap(int) {        
    update();
}

// draw layer 1
void MainWindow::drawLayer1(QPainter & paint) {
    for(int i = 0; i < MAP_WIDTH; ++i) {
        for(int z = 0; z < MAP_HEIGHT; ++z) {
            int x = (i*16)+offset_x;
            int y = (z*16)+offset_y;
            game::Tile *tile = level.at(pos_x+i, pos_y+z);
            if(tile != nullptr) {
                if(tile->layers[2] > 0) {
                    paint.drawImage(x, y, en[tile->layers[2]-1]);
                } else if(tile->layers[0] > 0 && tile->layers[0] <= col.size()) {
                    paint.drawImage(x, y, col[tile->layers[0]-1]);
                }
            }
        }
    }
}
 
void MainWindow::drawLayer2(QPainter & /*paint*/) {

}
 
void MainWindow::drawLayer3(QPainter & /*paint*/) {

}

// mouse move event
void MainWindow::mouseMoveEvent(QMouseEvent *e) {
    if(map_init == true) { 
            cursor_visible = true;
            draw_pos = e->pos();
    } 
    if(e->buttons() & Qt::MouseButton::LeftButton) {
        setTile(e->pos());
    } else if(e->buttons() & Qt::MouseButton::RightButton) {
        setObject(e->pos());
    } 
    update();
    
}

// mouse press event
void MainWindow::mousePressEvent(QMouseEvent *e) {
    if(e->button() == Qt::MouseButton::LeftButton) {
        setTile(e->pos());
        update();
    } else if(e->button() == Qt::MouseButton::RightButton) {
        setObject(e->pos());
        update();
    }
}

// mouse leave event
void MainWindow::leaveEvent(QEvent *) {
   cursor_visible = false;
   update();
}

// key press event
void MainWindow::keyPressEvent(QKeyEvent *e) {
    switch(e->key()) {
        case Qt::Key::Key_Left:
        if(pos_x > 0) pos_x--;
        break;
        case Qt::Key::Key_Right:
        if(pos_x < level.width - (1280/16))
        pos_x++;
        break;
        case Qt::Key::Key_Up:
        if(pos_y > 0) pos_y--;
        break;
        case Qt::Key::Key_Down:
        if(pos_y < level.height - (720/16))
        pos_y++;
        break;
    }
    tool_window->camera_x->setSliderPosition(pos_x);
    tool_window->camera_y->setSliderPosition(pos_y);
    updateLabelText();
    update();
}

// set the tile from mouse event
void MainWindow::setTile(const QPoint &pos) {
    if(map_init == true) {
        modified = true;
        int x,y;
        if(game::atPoint(pos.x(), pos.y()-offset_y,16,16,x, y)) {
            game::Tile *tile = level.at(pos_x+x, pos_y+y);
            if(tile != nullptr) {
                //tile->img = 2;
                switch(tool_window->tool->currentIndex()) {
                    case 0: {
                        int pos_start_x = pos_x+x;
                        int pos_start_y = pos_y+y;
                        for(int i = pos_start_x; i < pos_start_x+pen_x; ++i) {
                            for(int z = pos_start_y; z < pos_start_y+pen_y; ++z) {
                                game::Tile *tile = level.at(i, z);
                                if(tile != nullptr) {
                                    tile->setTile(tiles[tool_window->tiles->currentIndex()]);
                                }
                            }
                        }
                    }
                        break;
                    case 1: {
                        int pos_start_x = pos_x+x;
                        int pos_start_y = pos_y+y;
                        for(int i = pos_start_x; i < pos_start_x+pen_x; ++i) {
                            for(int z = pos_start_y; z < pos_start_y+pen_y; ++z) {
                                game::Tile *tile = level.at(i, z);
                                if(tile != nullptr) {
                                    tile->setTile(tiles[0]);
                                }
                            }
                        }
                    }
                        break;
                    case 2:
                       for(int i = pos_x; i < pos_x+(1280/16) && i < level.width; ++i) {
                            for(int z = pos_y; z < pos_y+(720/16) && z < level.height; ++z) {
                                game::Tile *tile = level.at(i, z);
                                if(tile != nullptr) {
                                    *tile = tiles[tool_window->tiles->currentIndex()];
                                }
                            }
                        }
                        break;
                    case 3: {
                        game::Tile *tile = level.at(pos_x+x, pos_y+y);
                        if(tile != nullptr) {
                            tile->layers[2] = 1+tool_window->char_objects->currentIndex();
                        }
                    }
                        break;
                }
            } 
        }
    }
}

// set object triggered by mouse event
void MainWindow::setObject(const QPoint &pos) {
    if(map_init == true) {
        modified = true;
        if(tool_window->tool->currentIndex() == 1) {
            int width = col[tool_window->tile_objects->currentIndex()].width();
            int height = col[tool_window->tile_objects->currentIndex()].height();
            int px = pos.x();
            int py = pos.y()-offset_y;
            for(int i = 0; i < MAP_WIDTH; ++i) {
                for(int z = 0; z < MAP_HEIGHT; ++z) {
                    game::Tile *tile = level.at(pos_x+i, pos_y+z);
                    if(tile != nullptr) {
                        int x = (i*16-16);
                        int y = (z*16-16);
                        if(tile->layers[0] > 0) {
                            if(px >= x && px <= x+width && py >= y && py <= y+height) {
                                tile->layers[0] = 0;
                                return;
                            }
                        }
                    }
                }
            }
            return;
        }
        int x,y;
        if(game::atPoint(pos.x(), pos.y()-offset_y, 16,16, x, y)) {
            game::Tile *tile = level.at(pos_x+x, pos_y+y);
            if(tile != nullptr) {
                switch(tool_window->tool->currentIndex()) {
                    case 0:
                        tile->layers[0] = tool_window->tile_objects->currentIndex()+1;
                    break;
                }
            } 
        }
    }   
}

void MainWindow::exportFile() {
    if(map_init == true)
        export_window->show();
    else {
        // print out message
        QMessageBox msgbox;
        msgbox.setText("Must create/open map to export");
        msgbox.setWindowTitle("Error exporting map");
        msgbox.setIcon(QMessageBox::Icon::Warning);
        msgbox.setWindowIcon(QIcon(":/images/col1.bmp"));
        msgbox.exec();
    }
}


// created a new map
void MainWindow::createdNewMap() { 
    updateTitle();
    tool_window->camera_x->setMinimum(0);
    tool_window->camera_x->setMaximum(level.width-(1280/16));
    tool_window->camera_y->setMinimum(0);
    tool_window->camera_y->setMaximum(level.height-(720/16));
    tool_window->camera_x->setSliderPosition(0);
    tool_window->camera_y->setSliderPosition(0);
    map_init = true;
    update();
    QString txt;
    QTextStream stream(&txt);
    stream << tr("editor: Created new map: [") << level.width << "x" << level.height << "]\n";
    debug_window->Log(txt);
    modified = false;
}

// update title
void MainWindow::updateTitle() {
    QString title;
    QTextStream stream(&title);
    stream << file_name << " - Map [" << level.width << "x" << level.height << "] - " << level.getLevelName().c_str();
    setWindowTitle(title);
    pref_window->setMaxSize(level.width, level.height);

}

// camera scroll bar changed
void MainWindow::cameraChanged(int) {
    pos_x = tool_window->camera_x->sliderPosition();
    pos_y = tool_window->camera_y->sliderPosition();
    updateLabelText();
    update();
}

// update labels
void MainWindow::updateLabelText() {
    QString lbl;
    QTextStream stream(&lbl);
    stream << "Camera X: " << pos_x;
    tool_window->lbl_x->setText(lbl);
    lbl = "";
    stream << "Camera Y: " << pos_y;
    tool_window->lbl_y->setText(lbl);
}

// open new menu
void MainWindow::openNewMenu() {
     if(modified == true) {
        askSave();
    }
    new_window->show();
}
// save file to disk
void MainWindow::saveFile() {

    if(file_name == "Untitled.lvl")
        saveFileAs();
    else {
        if(!level.saveLevel(file_name.toStdString())) {
                QMessageBox msgbox;
                msgbox.setText("Error on load of map");
                msgbox.setWindowTitle("Error loading map");
                msgbox.setIcon(QMessageBox::Icon::Warning);
                msgbox.setWindowIcon(QIcon(":/images/col1.bmp"));
                msgbox.exec();
            } else {
                updateTitle();
                debug_window->Log(tr("editor: Saved level.\n"));
                modified = false;
            }
    }
}
// save file as
void MainWindow::saveFileAs() {
    if(map_init == true) {
        QString filename = QFileDialog::getSaveFileName(nullptr, tr("Save File"), "", tr("LVL (*.lvl)"));
        if(filename != "") {
            if(!level.saveLevel(filename.toStdString())) {
                QMessageBox msgbox;
                msgbox.setText("Error on load of map");
                msgbox.setWindowTitle("Error loading map");
                msgbox.setIcon(QMessageBox::Icon::Warning);
                msgbox.setWindowIcon(QIcon(":/images/col1.bmp"));
                msgbox.exec();
            } else {
                file_name = filename;
                debug_window->Log(tr("editor: Saved level as: ") + file_name + "\n");
                updateTitle();
                modified = false;
            }
        }
    }
}

// TODO: Replace with Open Dialog window
void MainWindow::loadFile() {
    debug_window->Log("editor: Select Level File, Graphics File, Extraction location, and background bitmap\n");
    open_window->show();
}

// load level file from disk
bool MainWindow::loadLevelFile(const QString &filename, const QString &gfx_file, const QString &background, const QString &dir) {
    if(!loadGfx(gfx_file, dir, background)) {
        return false;
    }
    if(level.loadLevelByName(filename.toStdString())) {
        createdNewMap();
        file_name = filename;
        updateTitle();
        update();
    } else {
        QMessageBox msgbox;
        msgbox.setText("Could not load map");
        msgbox.setWindowTitle("Error on load");
        msgbox.setIcon(QMessageBox::Icon::Warning);
        msgbox.setWindowIcon(QIcon(":/images/col1.bmp"));
        msgbox.exec();
        return false;
    }
    modified = false;
    return true;
}

// scroll level  - up
void MainWindow::levelUp() {
   if(pos_y > 0) pos_y--;
    tool_window->camera_y->setSliderPosition(pos_y);
    updateLabelText();
    update();
}
// scroll level  - down 
void MainWindow::levelDown() {
    if(pos_y < level.height - (720/16))
        pos_y++;

    tool_window->camera_y->setSliderPosition(pos_y);
    updateLabelText();
    update();
}
// scroll level  - left
void MainWindow::levelLeft() {
    if(pos_x > 0)
        pos_x--;

    tool_window->camera_x->setSliderPosition(pos_x);
    updateLabelText();
    update();
}
// scroll level  - right
void MainWindow::levelRight() {
    if(pos_x < level.width - (1280/16))
        pos_x++;
    tool_window->camera_x->setSliderPosition(pos_x);
    updateLabelText();
    update();
}
// show level graphics window
void MainWindow::levelGraphicsOpen() {
    gfx_window->show();
}

// show run settings
void MainWindow::runSettings() {
    run_window->show();
}

// process stopped
void MainWindow::procStopped(int, QProcess::ExitStatus) {
    std::cout << proc->readAllStandardOutput().toStdString();
    run_exec->setText(tr("&Run"));
    debug_window->Log("editor: Map successfully exited.\n");
    proc_run = false;
}
// run test executable with map
void MainWindow::runExec() {
    saveFile();
    if(file_name != "Untitled.lvl") {
        QString path = run_window->exec_path->text();
        if(proc_run == false) {
            proc = new QProcess(this);
            QStringList args;
            static game::Point p[] = { game::Point(640, 360), game::Point(1280, 720), game::Point(1920, 1080), game::Point(3840, 2160) };
            args << "-p " << (QDir::currentPath() + "/scroller/assets") << "-l " << file_name << "-g " << graphics_file << "-b " << run_window->exec_bg->text();
            QStringList argsx;
            QString coord_str;
            QTextStream stream(&coord_str);
            switch(run_window->exec_res->currentIndex()) {
                case 0:
                    argsx << "640" << "360";
                break;
                case 1:
                    argsx << "1280" << "720";
                break;
                case 2:
                    argsx << "1920" << "1080";
                break;
                case 3:
                    argsx << "3840" << "2160";
                    break;
            } 
           QString command_string;
           QTextStream command(&command_string);
            command << path+"/scroller -r " << argsx.at(0) << "x" << argsx.at(1) << " ";
            connect(proc, SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(procStopped(int, QProcess::ExitStatus)));
            connect(proc, SIGNAL(readyReadStandardOutput()), this, SLOT(readStdout()));
            proc->setWorkingDirectory(path+"/");
            proc->start(path+"/scroller", args);
            if(proc->waitForStarted()) {
                run_exec->setText(tr("&Stop"));
                debug_window->clear();
                debug_window->Log(tr("editor: Command: ") + command_string + "\n");
                debug_window->Log("editor: Started Level: " + file_name + "\n");
                proc_run = true;
            }
            else {
                run_exec->setText(tr("&Run"));
                delete proc;
                proc = nullptr;
                QMessageBox msgbox;
                msgbox.setText("Error on execution of map executable");
                msgbox.setWindowTitle("Error loading map");
                msgbox.setIcon(QMessageBox::Icon::Warning);
                msgbox.setWindowIcon(QIcon(":/images/col1.bmp"));
                msgbox.exec();
                debug_window->Log("editor: Could not open map executable file: " + path+"/test-game\n");
            }
        } else {
            proc->terminate();
            proc->waitForFinished();
            proc_run = false;
            delete proc;
            proc = nullptr;
            run_exec->setText(tr("&Run"));
        }
    } else {
        QMessageBox msgbox;
        msgbox.setText("You must create a map");
        msgbox.setWindowTitle("Error loading map");
        msgbox.setIcon(QMessageBox::Icon::Warning);
        msgbox.setWindowIcon(QIcon(":/images/col1.bmp"));
        msgbox.exec();
    }
}

// show gfx window
void MainWindow::showGfx() {
    gfx_window->show();
}

// load graphics file
bool MainWindow::loadGfx(const QString &filename, const QString &dir, const QString &background) {

    background_file = background;
    run_window->exec_bg->setText(background_file);
    game::GfxTable table;
    game::GfxExtract extract;

    if(extract.open(filename.toStdString())) {
        if(extract.extract(table, dir.toStdString())) {
            debug_window->Log("editor: Extracted to dir: " + dir + "\n");
            // load graphics
            if(!images.empty())
                images.erase(images.begin(), images.end());

            if(!col.empty()) 
                col.erase(col.begin(), col.end());

            if(!tiles.empty())
                tiles.erase(tiles.begin(), tiles.end());

            tool_window->tiles->clear();
            tool_window->tile_objects->clear();

            int index[2] = {0};

            for(std::vector<game::GfxItem>::size_type i = 0; i < table.table.size(); ++i) {
                QString text;
                QTextStream stream(&text);
                stream << dir << "/" << table.table[i].filename.c_str();
                uint8_t s = table.table[i].solid;
                switch(table.table[i].obj) {
                    case 0: {
                        QImage img(text);
                        images.push_back(img);
                        QString output;
                        QTextStream out(&output);
                        out << "editor: Loaded: " << text << " size: " << img.width() << "x" << img.height() << "\n";
                        tiles.push_back(game::Tile{0, s, static_cast<uint8_t>(i)});
                        tool_window->tiles->addItem(table.table[i].filename.c_str());
                        QPixmap pix(text);
                        QPixmap img_s = pix.scaled(16,16,Qt::IgnoreAspectRatio);
                        tool_window->tiles->setItemData(index[0]++, img_s, Qt::DecorationRole);
    
                        debug_window->Log(output);
                    }
                    break;
                    case 1: {
                        QImage img(text);
                        QImage img_t = img.convertToFormat(QImage::Format_ARGB32);
                        for(int i = 0; i < img.width(); ++i) {
                            for(int z = 0; z < img.height(); ++z) {
                                if(img_t.pixel(i, z) == qRgb(255, 255, 255)) {
                                    img_t.setPixel(i, z, qRgba(0, 0, 0, 0));
                                }
                            }
                        }
                        col.push_back(img_t);
                        QString output;
                        QTextStream out(&output);
                        out << "editor: Loaded: " << text << " size: " << img.width() << "x" << img.height() << "\n";
                        debug_window->Log(output);
                        QPixmap pix(text);
                        QPixmap img_s = pix.scaled(16,16,Qt::IgnoreAspectRatio);
                        tool_window->tile_objects->addItem(table.table[i].filename.c_str());
                        tool_window->tile_objects->setItemData(index[1]++, img_s, Qt::DecorationRole);
                    }
                    break;
                }
            }

            QString txt;
            QTextStream st(&txt);
            st << "editor: Loaded: " << images.size() << " tiles\neditor: Loaded: " << col.size() << " objects\n";
            debug_window->Log(txt);
            graphics_file = filename;            
            update();
        }
        else {
            QMessageBox msgbox;
            msgbox.setText("Error opening resource file");
            msgbox.setWindowTitle("Error opening file");
            msgbox.setIcon(QMessageBox::Icon::Warning);
            msgbox.exec();
            debug_window->Log("editor: Failed to extract resource file: " + filename + "\n");
            return false;
        }
    } else {
        QMessageBox msgbox;
        msgbox.setText("Error opening resource file");
        msgbox.setWindowTitle("Error opening file");
        msgbox.setIcon(QMessageBox::Icon::Warning);
        msgbox.exec();
        debug_window->Log("editor: Failed to open resource file: " + filename + "\n");
        return false;
    }

    return true;
    
}

// set a new gfx window
void MainWindow::setNewGfx(const QString &filename) {
    new_window->gfx_box->addItem(filename);
    new_window->gfx_box->setCurrentIndex(new_window->gfx_box->count()-1);
}

// show about window
void MainWindow::showAbout() {
    debug_window->Log("editor: written by Jared Bruni.\n");
    about_window->show();
}

// open preferences
void MainWindow::openPref() {
    pref_window->show();
}

// set cursor pen size in 16x16 tiles
void MainWindow::setPenSize(int px, int py) {
    pen_x = px;
    pen_y = py;
    QString text;
    QTextStream stream(&text);
    stream << tr("editor: Set Pencil Size: ") << pen_x << ", " << pen_y << "\n";
    debug_window->Log(text);
}

// show tool window
void MainWindow::showTool() {
    tool_window->show();
}