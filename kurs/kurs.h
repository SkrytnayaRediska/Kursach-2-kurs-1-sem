#ifndef KURS_H
#define KURS_H

#include <QMainWindow>
#include <QtWidgets>
#include <memory>
#include <qmessagebox.h>
#include <QSqlQuery>
#include <QtSql>

QT_BEGIN_NAMESPACE
namespace Ui { class kurs; }
QT_END_NAMESPACE

class kurs : public QMainWindow
{
    Q_OBJECT

public:
    kurs(QWidget *parent = nullptr);
    ~kurs();

private slots:
    void registration();
    void log_in();
    bool isExists(QString log);
    void create_db();
    void open_db();
    void add_person();
    void save_changes();
    void select_person();
    void delete_person();
    QStringList get_selected_person();

private:
    Ui::kurs *ui;
    QSqlDatabase admins_db;
    QSqlDatabase current_db;

    QListView persList;

    QString current_Surname;
    QString current_Name;
    QString current_Patronymic;

    QLineEdit *loginEdit;
    QLineEdit *passwordEdit;

    QLineEdit *FamEdit;
    QLineEdit *NameEdit;
    QLineEdit *OtchEdit;
    QLineEdit *birthEdit;
    QLineEdit *jobEdit;
    QLineEdit *qualifEdit;
    QLineEdit *eduEdit;
    QLineEdit *coursEdit;
    QLineEdit *attEdit;
    QLineEdit *inndit;
    QLineEdit *snilsEdit;
    QLineEdit *progrEdit;
    QLineEdit *additionsEdit;
    QLineEdit *pasp_SerieEdit;
    QLineEdit *pasp_NumEdit;
    QLineEdit *pasp_FMSEdit;
    QLineEdit *pasp_DateEdit;

    QPushButton *registrateBut;
    QPushButton *entryBut;

    QGroupBox *group_in;

    QLabel *name;
    QLabel *db_nameLabel;
};
#endif // KURS_H
