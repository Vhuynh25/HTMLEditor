#include "string_search.h"
#include "ui_string_search.h"

String_Search::String_Search(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::String_Search)
{
    ui->setupUi(this);
}

String_Search::~String_Search()
{
    delete ui;
}




void String_Search::on_next_String_Button_clicked()
{
    emit find_Next(this->ui->string_To_Search->toPlainText());
}


void String_Search::on_prev_String_Button_clicked()
{
    emit find_Prev(this->ui->string_To_Search->toPlainText());
}


void String_Search::on_button_Box_accepted()
{
    on_next_String_Button_clicked();
}


void String_Search::on_button_Box_rejected()
{
    this->ui->string_To_Search->clear();
    this->hide();
}

