#ifndef STRING_SEARCH_H
#define STRING_SEARCH_H

#include <QDialog>

namespace Ui {
class String_Search;
}

class String_Search : public QDialog
{
    Q_OBJECT

public:
    explicit String_Search(QWidget *parent = nullptr);
    ~String_Search();

signals:
    void find_Next(QString);
    void find_Prev(QString);

private slots:
    void on_next_String_Button_clicked();

    void on_prev_String_Button_clicked();

    void on_button_Box_accepted();

    void on_button_Box_rejected();

private:
    Ui::String_Search *ui;
};

#endif // STRING_SEARCH_H
