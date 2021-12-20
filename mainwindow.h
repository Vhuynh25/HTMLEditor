#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSessionManager>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QScreen>
#include <QSaveFile>
#include <QCloseEvent>
#include <QTextStream>
#include <QWebEngineView>
#include <QUrl>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    QString load_File(const QString &file_Name);
    void load_Html_View(const QString &file_Name);
    void reload_Html_View();
    void load_Text_Edit(const QString &file_Name);

    ~MainWindow();

protected:
    void closeEvent(QCloseEvent* event) override;

private slots:
    void new_File();
    void open(std::string);
    bool save();
    bool save_As();
    void about();
    void document_Was_Edited();
//#ifndef QT_NO_SESSIONMANAGER
//    void commit_Data(QSessionManager &);
//#endif

private:
    Ui::MainWindow *ui;
    void create_Actions();
    void read_Settings();
    void write_Settings();
    bool maybe_Save();
    bool save_File(const QString &file_Name);
    void set_Current_File(const QString &file_Name);
    void set_Current_HTML(const QString &file_Name);
    QString stripped_Name(const QString &full_File_Name);
    QString current_File;
    QString current_HTML;
};

#endif // MAINWINDOW_H
