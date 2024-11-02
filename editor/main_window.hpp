#ifndef _EDITOR_H__
#define _EDITOR_H__

#include<QMainWindow>
#include<QMenu>
#include<QAction>
#include<QVector>
#include<QPainter>
#include<QProcess>
#include<QTextStream>
#include<vector>
#include"tool_window.hpp"
#include"new_window.hpp"
#include"run_window.hpp"
#include"debug_window.hpp"
#include"gfx_window.hpp"
#include"open_window.hpp"
#include"about_window.hpp"
#include"pref_window.hpp"
#include"export_window.hpp"
#include "level.hpp"

const int MAP_WIDTH=1280/16;
const int MAP_HEIGHT=720/16;
const int MAP_W = 1280;
const int HAP_H = 720;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow();
    void paintEvent(QPaintEvent *p) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void leaveEvent(QEvent *e) override;
    void keyPressEvent(QKeyEvent *e) override;
    void createdNewMap();
    void setTile(const QPoint &pos);
    void setObject(const QPoint &pos);
    void updateLabelText();
    void updateTitle();
    void drawLayer1(QPainter &paint);
    void drawLayer2(QPainter &paint);
    void drawLayer3(QPainter &paint);
    void closeEvent(QCloseEvent *c) override;
    void showGfx();
    bool loadGfx(const QString &filename, const QString &dir, const QString &background);
    void setNewGfx(const QString &filename);
    bool loadLevelFile(const QString &filename, const QString &gfx_file, const QString &background, const QString &dir);
    void setPenSize(int px, int py);
    void askSave();
    DebugWindow *debug_window;
public slots:
    void openNewMenu();
    void saveFile();
    void saveFileAs();
    void loadFile();
    void cameraChanged(int value);
    void levelUp();
    void levelDown();
    void levelLeft();
    void levelRight();
    void updateMap(int);
    void runSettings();
    void runExec();
    void shutdownProgram();
    void procStopped(int exitcode, QProcess::ExitStatus status);
    void readStdout();
    void levelGraphicsOpen();
    void showAbout();
    void openPref();
    void showTool();
    void exportFile();
private:
    QMenu *file_menu, *level_menu, *run_menu, *help_menu, *view_menu;
    QAction *file_new, *file_save,*file_save_as,*export_file, *file_load, *file_pref, *file_exit;
    QAction *level_left, *level_right, *level_down, *level_up, *level_gfx;
    QAction *run_settings, *run_exec;
    QAction *view_show_tool;
    QAction *help_about;
    ToolWindow *tool_window;  
    NewWindow *new_window;  
    RunWindow *run_window;
    GfxWindow *gfx_window;
    OpenWindow *open_window;
    AboutWindow *about_window;
    PrefWindow *pref_window;
    ExportWindow *export_window;
    std::vector<QImage> images;
    std::vector<QImage> col;
    std::vector<QImage> en;
    std::vector<game::Tile> tiles; 
    game::Level level;
    bool map_init;
    int pos_x, pos_y;
    QString file_name, graphics_file, background_file;
    bool cursor_visible;
    QPoint draw_pos;
    QProcess *proc = nullptr;
    bool proc_run = false;
    bool reset_window = false;
    QVector<QImage> img, obj;
    int pen_x = 1, pen_y = 1;
    bool modified = false;

    #ifdef __APPLE__
    const int offset_x = 0;
    const int offset_y = 0;
    #else
    const int offset_x = 0;
    const int offset_y = 24;
    #endif
};

#endif