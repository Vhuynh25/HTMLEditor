#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "string_search.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    read_Settings();
    create_Actions();
    connect(this->ui->text_Edit->document(),&QTextDocument::contentsChanged,
            this,&MainWindow::document_Was_Edited);
//#ifndef QT_NO_SESSIONMANAGER
//    QGuiApplication::setFallbackSessionManagementEnabled(false);
//    connect(qApp, &QGuiApplication::commitDataRequest,
//            this,&MainWindow::commit_Data);
//#endif
    set_Current_File(QString());
    setUnifiedTitleAndToolBarOnMac(true);

    this->search_Dialog = new String_Search(this);

    connect(this->search_Dialog,&String_Search::find_Next,this,&MainWindow::search_Next_Text);
    connect(this->search_Dialog,&String_Search::find_Prev,this,&MainWindow::search_Prev_Text);
}

void MainWindow::closeEvent(QCloseEvent *event){
    if (maybe_Save()){
        write_Settings();
        event->accept();
    }
    else {event->ignore();}
}

void MainWindow::new_File(){
    if (maybe_Save()){
        this->ui->text_Edit->clear();
        set_Current_File(QString());
    }
}

void MainWindow::open(std::string mode){
    if (maybe_Save()){
        QString file_name = QFileDialog::getOpenFileName();
        if (!file_name.isEmpty()){
            if (mode == "FILE")
                load_Text_Edit(file_name);
            else if (mode == "HTML")
                load_Html_View(file_name);
        }
    }
}

bool MainWindow::save(){
    if (current_File_Name.isEmpty()){
        return save_As();
    }
    else {
        return save_File(current_File_Name);
    }
}

bool MainWindow::save_As(){
    QFileDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    if (dialog.exec() != QDialog::Accepted){
        return false;
    }
    return save_File(dialog.selectedFiles().first());
}

void MainWindow::about(){
    QMessageBox::about(this, tr("About Application"),
                       tr("This app allows users to edit html files "
                          "and see their changes in real-time"));
}

void MainWindow::document_Was_Edited(){
    setWindowModified(this->ui->text_Edit->document()->isModified());
}

void MainWindow::create_Actions(){
    QMenu *file_Menu = menuBar()->addMenu(tr("&File"));
    QMenu *help_Menu = menuBar()->addMenu(tr("&Help"));
    QToolBar *file_Tool_Bar = addToolBar(tr("File"));

    const QIcon new_Icon = QIcon("..\\icons\\file-plus.svg");
    QAction *new_Act = new QAction(new_Icon, tr("&New"),this);
    new_Act->setShortcuts(QKeySequence::New);
    new_Act->setStatusTip(tr("Create a new file"));
    connect(new_Act,&QAction::triggered,this,&MainWindow::new_File);
    file_Menu->addAction(new_Act);
    file_Tool_Bar->addAction(new_Act);

    const QIcon open_Icon = QIcon("..\\icons\\file.svg");
    QAction *open_File_Act = new QAction(open_Icon, tr("&Open..."),this);
    open_File_Act->setShortcuts(QKeySequence::Open);
    open_File_Act->setStatusTip(tr("Open an existing file to edit"));
    connect(open_File_Act,&QAction::triggered,this,[this]{open("FILE");});
    file_Menu->addAction(open_File_Act);
    file_Tool_Bar->addAction(open_File_Act);

    const QIcon global_Icon = QIcon("..\\icons\\globe.svg");
    QAction *open_Html_Act = new QAction(global_Icon, tr("&Open..."),this);
    open_Html_Act->setStatusTip(tr("Open an html file to view"));
    connect(open_Html_Act,&QAction::triggered,this,[this]{open("HTML");});
    file_Menu->addAction(open_Html_Act);
    file_Tool_Bar->addAction(open_Html_Act);

    const QIcon save_Icon = QIcon("..\\icons\\save.svg");
    QAction *save_Act = new QAction(save_Icon, tr("&Save"),this);
    save_Act->setShortcuts(QKeySequence::Save);
    save_Act->setStatusTip(tr("Save the current file"));
    connect(save_Act,&QAction::triggered,this,&MainWindow::save);
    file_Menu->addAction(save_Act);
    file_Tool_Bar->addAction(save_Act);

    const QIcon search_Icon = QIcon("..\\icons\\magnifying-glass.svg");
    QAction *search_Act = new QAction(search_Icon, tr("&search"),this);
    search_Act->setShortcuts(QKeySequence::Find);
    search_Act->setStatusTip(tr("Search the current file for a string"));
    connect(search_Act,&QAction::triggered,this,&MainWindow::show_Search);
    file_Menu->addAction(search_Act);
    file_Tool_Bar->addAction(search_Act);

    QAction *about_Qt_Act = help_Menu->addAction(tr("About &Qt"), qApp, &QApplication::aboutQt);
    about_Qt_Act->setStatusTip(tr("Show the Qt library's About box"));
}

void MainWindow::read_Settings(){
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    const QByteArray geometry = settings.value("geomentry",QByteArray()).toByteArray();
    //if (geometry.isEmpty()){
    //    const QRect available_geometry = QScreen()->availableGeometry();
    //    resize(available_geometry.width()/3,available_geometry.height()/2);
    //    move((available_geometry.width() - width())/2,
    //         (available_geometry.height() - height())/2);
    //}
    //else
    restoreGeometry(geometry);
}

void MainWindow::write_Settings(){
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue("geometry" , saveGeometry());
}

bool MainWindow::maybe_Save(){
    if (!this->ui->text_Edit->document()->isModified()){
        return true;
    }
    const QMessageBox::StandardButton ret =
            QMessageBox::warning(this, tr("Application"),
                                 tr("The document was modified.\n"
                                    "Do you want to save your changes?"),
                                 QMessageBox::Save|QMessageBox::Discard|QMessageBox::Cancel);
    switch (ret){
    case QMessageBox::Save:
        return save();
    case QMessageBox::Cancel:
        return false;
    default:
        break;
    }
    return true;

}

void MainWindow::load_Text_Edit(const QString &file_Name){
#ifndef QT_NO_CURSOR
    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    QString in = load_File(file_Name);
#ifndef QT_NO_CURSOR
    QGuiApplication::restoreOverrideCursor();
#endif
    if (!in.isNull()){
        this->ui->text_Edit->setPlainText(in);
        set_Current_File(file_Name);
    }
    else
        set_Current_File(in);

    statusBar()->showMessage(tr("File loaded"),2000);
}

void MainWindow::load_Html_View(const QString &file_Name){
    set_Current_HTML(file_Name);

    reload_Html_View();
    statusBar()->showMessage(tr("File loaded"),2000);
}

void MainWindow::reload_Html_View(){
    QUrl url = QUrl::fromLocalFile(this->current_HTML);
    this->ui->web_View->load(url);
    this->ui->web_View->show();
}

QString MainWindow::load_File(const QString &file_Name){
    QFile file(file_Name);
    if (!file.open(QFile::ReadOnly)){
        QString nothing;
        QMessageBox::warning(this,tr("Application"),
                             tr("Cannot read file %1:\n%2.").
                             arg(QDir::toNativeSeparators(file_Name), file.errorString()));
        return nothing;
    }
    QTextStream in(&file);
    return in.readAll();
}

bool MainWindow::save_File(const QString &file_Name){
    QString errorMessage;
    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
    QSaveFile file(file_Name);
    if (file.open(QFile::WriteOnly|QFile::Text)){
        QTextStream out(&file);
        out << this->ui->text_Edit->toPlainText();
        if (!file.commit()){
            errorMessage = tr("Cannot write file %1:\n%2")
                    .arg(QDir::toNativeSeparators(file_Name), file.errorString());
        }

    }
    else {
        errorMessage = tr("Cannot open file %1 for writing:\n%2")
                .arg(QDir::toNativeSeparators(file_Name), file.errorString());
    }
    QGuiApplication::restoreOverrideCursor();
    if (!errorMessage.isEmpty()){
        QMessageBox::warning(this,tr("Application"), errorMessage);
        return false;
    }
    set_Current_File(file_Name);
    reload_Html_View();
    statusBar()->showMessage(tr("File saved"),2000);
    return true;
}

void MainWindow::set_Current_File(const QString &file_Name){
    this->current_File_Name = file_Name;
    this->ui->text_Edit->document()->setModified(false);
    setWindowModified(false);
    QString shown_name = current_File_Name;
    if (current_File_Name.isEmpty())
        shown_name = "untitled.txt";
    setWindowFilePath(shown_name);
}

void MainWindow::set_Current_HTML(const QString &file_Name){
    this->current_HTML = file_Name;
}

QString MainWindow::stripped_Name(const QString &full_File_Name){
    return QFileInfo(full_File_Name).fileName();
}

void MainWindow::show_Search(){
    if (!search_Dialog->isVisible())
        search_Dialog->show();
}

void MainWindow::search_Next_Text(QString text){
    this->ui->text_Edit->find(text);
}

void MainWindow::search_Prev_Text(QString text){
    this->ui->text_Edit->find(text, QTextDocument::FindBackward);
}

MainWindow::~MainWindow()
{
    delete ui;
}

