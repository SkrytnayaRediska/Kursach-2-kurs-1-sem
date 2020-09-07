#include "kurs.h"
#include "ui_kurs.h"
#include <memory>
#include <QtWidgets>
#include <qmessagebox.h>
#include <iostream>
#include <QCryptographicHash>
#include <string.h>
#include <QSqlQuery>
#include <QtSql>

kurs::kurs(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::kurs)
{
    ui->setupUi(this);
}

void kurs::registration(){
    admins_db = QSqlDatabase::addDatabase("QSQLITE");
    admins_db.setDatabaseName("admins_db.db");
    admins_db.setPassword("1357cvghyu2468");
    admins_db.setConnectOptions("QSQLITE_CREATE_KEY");
    if (admins_db.open()){
        QSqlQuery query;
        query.exec("CREATE TABLE IF NOT EXISTS ADMINS(ID INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, LOGIN TEXT NOT NULL,"\
               "PASSWORD TEXT NOT NULL);");

        QString login = ui->loginEdit->text();
        QString password = ui->passwordEdit->text();

        if ((ui->loginEdit->text().isEmpty())||(ui->passwordEdit->text().isEmpty())){
            QMessageBox::information(this, "!!!",  "Incorrect Login or Password!");
        }
        else{
            QByteArray byte_arr = password.toUtf8();
            byte_arr = QCryptographicHash::hash(byte_arr, QCryptographicHash::Sha256);
            byte_arr = QCryptographicHash::hash(byte_arr, QCryptographicHash::Sha256);

            QString qstr_pass = QString::fromStdString(byte_arr.toStdString());

            QString qstr = QString("INSERT INTO ADMINS (LOGIN, PASSWORD) VALUES('%1', '%2');").arg(login, qstr_pass);
            query.exec(qstr);
            ui->loginEdit->setText("");
            ui->passwordEdit->setText("");
            QMessageBox::information(this, "!!!",  "Successfully");
        }
        admins_db.close();
    }
    else{
        QMessageBox::information(this, "!!!",  "NOT OPEN");
    }
}

bool kurs::isExists(QString log)
{
    QSqlQuery querySelect;
    querySelect.prepare("SELECT * FROM ADMINS WHERE LOGIN = ':log'");
    querySelect.bindValue(":log", log);

    if(!querySelect.exec())
    {
        return false;
    }
    else
    {
        return true;
    }
}

void kurs::log_in(){
    QString login = ui->loginEdit->text();
    QString password = ui->passwordEdit->text();
    std::string pass_from_db;
    int found = 0;

    admins_db = QSqlDatabase::addDatabase("QSQLITE");
    admins_db.setDatabaseName("admins_db.db");
    admins_db.setPassword("1357cvghyu2468");
    admins_db.setConnectOptions("QSQLITE_CREATE_KEY");
    if (admins_db.open()){
        if ((ui->loginEdit->text().isEmpty())||(ui->passwordEdit->text().isEmpty())){
            QMessageBox::information(this, "!!!",  "Incorrect Login or Password!");
        }
        else{
            QByteArray byte_arr = password.toUtf8();
            byte_arr = QCryptographicHash::hash(byte_arr, QCryptographicHash::Sha256);
            byte_arr = QCryptographicHash::hash(byte_arr, QCryptographicHash::Sha256);
            QString qstr_pass = QString::fromStdString(byte_arr.toStdString());
            std::string str_hashed_pass = qstr_pass.toStdString();

            if (isExists(login)){
                QSqlQuery querySelect;
                querySelect.exec("SELECT LOGIN, PASSWORD FROM ADMINS");

                while(querySelect.next()){
                    std::string login_from_db = (querySelect.value(0).toString()).toStdString();
                    pass_from_db = (querySelect.value(1).toString()).toStdString();

                    if ((pass_from_db == str_hashed_pass)&&(QString::fromStdString(login_from_db) == login)){
                        std::string str_for_label = ("Вы вошли как ") + login_from_db;
                        ui->name->setText(QString::fromStdString(str_for_label));
                        found++;
                        ui->loginEdit->setText("");
                        ui->passwordEdit->setText("");
                    }
                    else{
                    }

                }
                if (found == 0){
                    QMessageBox::information(this, "!!!",  "INCORRECT LOGIN OR PASSWORD");
                }
            }
            else{
                QMessageBox::information(this, "!!!",  "INCORRECT LOGIN");
            }
        }
        admins_db.close();
    }
    else{
        QMessageBox::information(this, "!!!",  "CAN NOT FIND ADMINS");
    }
}

void kurs::create_db(){
    current_db = QSqlDatabase::addDatabase("QSQLITE");
    current_db.setDatabaseName("admins_db.db");
    current_db.setPassword("1357cvghyu2468");
    current_db.setConnectOptions("QSQLITE_CREATE_KEY");
    bool bOk;
    QString db_name = QInputDialog::getText( 0,"AIS Persons","Input Database Name",QLineEdit::Normal,"",&bOk);
    if (!bOk) {
        // Была нажата кнопка Cancel
    }
    else{
        db_name = db_name + (".db");
        current_db.setDatabaseName(db_name);
        if (current_db.open()){

            QSqlQuery query;
            query.exec("CREATE TABLE IF NOT EXISTS PERSONS (ID INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, SURNAME TEXT NOT NULL,"\
                "NAME TEXT NOT NULL, PATRONYMIC TEXT NOT NULL, BRTH_DATE TEXT NOT NULL, JOB_POST TEXT NOT NULL,"\
                "QUALITY TEXT NOT NULL, EDUCATION TEXT NOT NULL, QUALITY_COURSES TEXT NOT NULL,"\
                "ATTESTATION TEXT NOT NULL, INN TEXT NOT NULL, SNILS TEXT NOT NULL, PROGRESS TEXT NOT NULL,"\
                "ADDITIONS TEXT NOT NULL, PASP_SERIA TEXT NOT NULL, PASP_NUMBER TEXT NOT NULL, PASP_BY_WHOM TEXT NOT NULL,"\
                "PASP_DATE_VID TEXT NOT NULL);");

            QMessageBox::information(this, "!!!",  "Database created.");
            current_db.close();
        }

    }

}

void kurs::open_db(){
    if (ui->name->text().isEmpty()){
        QMessageBox::information(this, "!!!",  "You can not open the database because you are not authorized.");
    }
    else{
        QString db_name = QFileDialog::getOpenFileName(0, "Open Database", "", "*.db");
        QStringList list_file_path = db_name.split("/");
        ui->db_nameLabel->setText(list_file_path.last());

        current_db = QSqlDatabase::database();
        QStringList *all_Lines_from_db = new QStringList();
        all_Lines_from_db->clear();

        QSqlQuery query;
        query.exec("SELECT SURNAME, NAME, PATRONYMIC FROM PERSONS");
        while(query.next()){
            QString FIO = (query.value(0).toString()) + (" ") + (query.value(1).toString()) + (" ") + (query.value(2).toString());
            all_Lines_from_db->append(FIO);
            }


        QStringListModel *linesModel = new QStringListModel(*all_Lines_from_db, NULL);
        linesModel->setStringList(*all_Lines_from_db);

        ui->persListBut->setModel(linesModel);

        current_db.close();
    }
}

QStringList kurs::get_selected_person(){
    QModelIndexList selectedIndexes = ui->persListBut->selectionModel()->selectedIndexes();
        QStringList selectedTexts;
        foreach(const QModelIndex &idx, selectedIndexes)
        {
            selectedTexts << idx.data(Qt::DisplayRole).toString();
        }
    QString selected_person = selectedTexts.join(',');

    QStringList list_selected_person_parts = selected_person.split(" ");
    return list_selected_person_parts;
}


void kurs::select_person(){
    QStringList list_selected_person_parts = get_selected_person();

    QString cur_fam = (list_selected_person_parts[0]);
    QString cur_nam = (list_selected_person_parts[1]);
    QString cur_otc = (list_selected_person_parts[2]);
    current_Surname = cur_fam;
    current_Name = cur_nam;
    current_Patronymic = cur_otc;

    current_db = QSqlDatabase::database();
    if (current_db.open()){
        QSqlQuery query;
        query.prepare("SELECT SURNAME, NAME, PATRONYMIC, BRTH_DATE, JOB_POST, QUALITY, EDUCATION,"\
              "QUALITY_COURSES, ATTESTATION, INN, SNILS, PROGRESS, ADDITIONS, PASP_SERIA, PASP_NUMBER,"\
              "PASP_BY_WHOM, PASP_DATE_VID FROM PERSONS WHERE SURNAME = ? AND NAME = ? AND PATRONYMIC = ?");
        query.bindValue(0, list_selected_person_parts[0]);
        query.bindValue(1, list_selected_person_parts[1]);
        query.bindValue(2, list_selected_person_parts[2]);
        query.exec();
        while(query.next()){
            ui->FamEdit->setText(query.value(0).toString());
            ui->NameEdit->setText(query.value(1).toString());
            ui->OtchEdit->setText(query.value(2).toString());
            ui->birthEdit->setText(query.value(3).toString());
            ui->jobEdit->setText(query.value(4).toString());
            ui->qualifEdit->setText(query.value(5).toString());
            ui->eduEdit->setText(query.value(6).toString());
            ui->coursEdit->setText(query.value(7).toString());
            ui->attEdit->setText(query.value(8).toString());
            ui->innEdit->setText(query.value(9).toString());
            ui->snilsEdit->setText(query.value(10).toString());
            ui->progrEdit->setText(query.value(11).toString());
            ui->additionsEdit->setText(query.value(12).toString());
            ui->pasp_SerieEdit->setText(query.value(13).toString());
            ui->pasp_NumEdit->setText(query.value(14).toString());
            ui->pasp_FMSEdit->setText(query.value(15).toString());
            ui->pasp_DateEdit->setText(query.value(16).toString());
        }
        current_db.close();
    }
}

void kurs::add_person(){
    if (ui->name->text().isEmpty()){
        QMessageBox::information(this, "!!!",  "You can't add a person because you are not authorized.");
    }
    else{
        QSqlQuery query;
        if (ui->db_nameLabel->text().isEmpty()){
            QMessageBox::information(this, "!!!",  "No Database opened.");
        }
        else{
            current_db = QSqlDatabase::database();

            if (current_db.open()){
                query.exec("CREATE TABLE IF NOT EXISTS PERSONS (ID INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, SURNAME TEXT NOT NULL,"\
                    "NAME TEXT NOT NULL, PATRONYMIC TEXT NOT NULL, BRTH_DATE TEXT NOT NULL, JOB_POST TEXT NOT NULL,"\
                    "QUALITY TEXT NOT NULL, EDUCATION TEXT NOT NULL, QUALITY_COURSES TEXT NOT NULL,"\
                    "ATTESTATION TEXT NOT NULL, INN TEXT NOT NULL, SNILS TEXT NOT NULL, PROGRESS TEXT NOT NULL,"\
                    "ADDITIONS TEXT NOT NULL, PASP_SERIA TEXT NOT NULL, PASP_NUMBER TEXT NOT NULL, PASP_BY_WHOM TEXT NOT NULL,"\
                    "PASP_DATE_VID TEXT NOT NULL);");

                query.prepare("INSERT INTO PERSONS(SURNAME, NAME, PATRONYMIC, BRTH_DATE, JOB_POST, QUALITY, EDUCATION,"\
                              "QUALITY_COURSES, ATTESTATION, INN, SNILS, PROGRESS, ADDITIONS, PASP_SERIA, PASP_NUMBER,"\
                              "PASP_BY_WHOM, PASP_DATE_VID) VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);");

                query.bindValue(0, ui->FamEdit->text());
                query.bindValue(1, ui->NameEdit->text());
                query.bindValue(2, ui->OtchEdit->text());
                query.bindValue(3, ui->birthEdit->text());
                query.bindValue(4, ui->jobEdit->text());
                query.bindValue(5, ui->qualifEdit->text());
                query.bindValue(6, ui->eduEdit->text());
                query.bindValue(7, ui->coursEdit->text());
                query.bindValue(8, ui->attEdit->text());
                query.bindValue(9, ui->innEdit->text());
                query.bindValue(10, ui->snilsEdit->text());
                query.bindValue(11, ui->progrEdit->text());
                query.bindValue(12, ui->additionsEdit->text());
                query.bindValue(13, ui->pasp_SerieEdit->text());
                query.bindValue(14, ui->pasp_NumEdit->text());
                query.bindValue(15, ui->pasp_FMSEdit->text());
                query.bindValue(16, ui->pasp_DateEdit->text());
                query.exec();

                QStringList *all_Lines_from_db = new QStringList();
                all_Lines_from_db->clear();

                query.exec("SELECT SURNAME, NAME, PATRONYMIC FROM PERSONS");
                while(query.next()){
                    QString FIO = (query.value(0).toString()) + (" ") + (query.value(1).toString()) + (" ") + (query.value(2).toString());
                    all_Lines_from_db->append(FIO);
                    }


                QStringListModel *linesModel = new QStringListModel(*all_Lines_from_db, NULL);
                linesModel->setStringList(*all_Lines_from_db);

                ui->persListBut->setModel(linesModel);
                }
            current_db.close();
            }
    }
}

void kurs::save_changes(){
    if (ui->name->text().isEmpty()){
        QMessageBox::information(this, "!!!",  "You can not open the database because you are not authorized.");
    }
    else{
        current_db = QSqlDatabase::database();
        if (current_db.open()){
            QSqlQuery query;
            query.prepare("UPDATE PERSONS SET SURNAME = ?, NAME = ?, PATRONYMIC = ?, BRTH_DATE = ?, JOB_POST = ?, QUALITY = ?, EDUCATION = ?,"\
                  "QUALITY_COURSES = ?, ATTESTATION = ?, INN = ?, SNILS = ?, PROGRESS = ?, ADDITIONS = ?, PASP_SERIA = ?, PASP_NUMBER = ?,"\
                  "PASP_BY_WHOM = ?, PASP_DATE_VID = ? WHERE SURNAME = ? AND NAME = ? AND PATRONYMIC = ?;");
            query.bindValue(0, ui->FamEdit->text());
            query.bindValue(1, ui->NameEdit->text());
            query.bindValue(2, ui->OtchEdit->text());
            query.bindValue(3, ui->birthEdit->text());
            query.bindValue(4, ui->jobEdit->text());
            query.bindValue(5, ui->qualifEdit->text());
            query.bindValue(6, ui->eduEdit->text());
            query.bindValue(7, ui->coursEdit->text());
            query.bindValue(8, ui->attEdit->text());
            query.bindValue(9, ui->innEdit->text());
            query.bindValue(10, ui->snilsEdit->text());
            query.bindValue(11, ui->progrEdit->text());
            query.bindValue(12, ui->additionsEdit->text());
            query.bindValue(13, ui->pasp_SerieEdit->text());
            query.bindValue(14, ui->pasp_NumEdit->text());
            query.bindValue(15, ui->pasp_FMSEdit->text());
            query.bindValue(16, ui->pasp_DateEdit->text());
            query.bindValue(17, current_Surname);
            query.bindValue(18, current_Name);
            query.bindValue(19, current_Patronymic);
            query.exec();
            current_db.close();
        }
    }
}

void kurs::delete_person(){
    if (ui->name->text().isEmpty()){
        QMessageBox::information(this, "!!!",  "You can not open the database because you are not authorized.");
    }
    else{
        current_db = QSqlDatabase::database();
        if (current_db.open()){
            QSqlQuery query;
            query.prepare("DELETE FROM PERSONS WHERE SURNAME = ? AND NAME = ? AND PATRONYMIC = ?;");
            query.bindValue(0, current_Surname);
            query.bindValue(1, current_Name);
            query.bindValue(2, current_Patronymic);
            query.exec();

            QStringList *all_Lines_from_db = new QStringList();
            all_Lines_from_db->clear();

            query.exec("SELECT SURNAME, NAME, PATRONYMIC FROM PERSONS");
            while(query.next()){
                QString FIO = (query.value(0).toString()) + (" ") + (query.value(1).toString()) + (" ") + (query.value(2).toString());
                all_Lines_from_db->append(FIO);
                }


            QStringListModel *linesModel = new QStringListModel(*all_Lines_from_db, NULL);
            linesModel->setStringList(*all_Lines_from_db);

            ui->persListBut->setModel(linesModel);

            ui->FamEdit->setText("");
            ui->NameEdit->setText("");
            ui->OtchEdit->setText("");
            ui->birthEdit->setText("");
            ui->jobEdit->setText("");
            ui->qualifEdit->setText("");
            ui->eduEdit->setText("");
            ui->coursEdit->setText("");
            ui->attEdit->setText("");
            ui->innEdit->setText("");
            ui->snilsEdit->setText("");
            ui->progrEdit->setText("");
            ui->additionsEdit->setText("");
            ui->pasp_SerieEdit->setText("");
            ui->pasp_NumEdit->setText("");
            ui->pasp_FMSEdit->setText("");
            ui->pasp_DateEdit->setText("");

            current_db.close();
        }
    }

}

kurs::~kurs()
{
    delete ui;
}

