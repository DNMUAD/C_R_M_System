#ifndef USERINF_H
#define USERINF_H

#include <QDialog>
#include <QSqlTableModel>

enum UserInfButton {
    enum_user_inf_btn_Add = 0,
    enum_user_inf_btn_Delete,
    enum_user_tel_btn_Add,

    enum_user_inf_btn_all
};

enum UserInfLabel {
    enum_user_inf_label_name = 0,
    enum_user_inf_label_company,
    enum_user_inf_label_construction,

    enum_user_inf_label_all
};

class UserInfPrivate;

class UserInf : public QDialog
{
    Q_OBJECT
public:
    explicit UserInf(QWidget *parent = nullptr);

private slots:
    void slotAddUserInf();

    void slotDeleteUserInf();

    void slotAddTel();

private:
    void initWindow();

    void initTable();

    void flushTable();

    UserInfPrivate *d_ptr;

    QSqlTableModel *user_inf_model;
};

#endif // USERINF_H
