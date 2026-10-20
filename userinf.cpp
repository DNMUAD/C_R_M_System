#include "userinf.h"
#include <QTableView>
#include <QPushButton>
#include <QInputDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QHeaderView>
#include <QListWidget>
#include <QListWidgetItem>
#include <QSqlQuery>
#include <QRegExp>
#include <QMessageBox>
#include <QModelIndex>
#include <QSqlRecord>

#include "pack/dbinit.h"

class UserInfPrivate {
public:
    UserInfPrivate();

    QPushButton *m_pBtn[enum_user_inf_btn_all];

    QLabel      *m_pLabel[enum_user_inf_label_all];
    QLineEdit   *m_pLineEdit[enum_user_inf_label_all];
    QLineEdit   *m_pTelEdit;

    QListWidget *m_pListWidget1;


    QWidget* m_pWidget1; // 添加信息
    QWidget* m_pWidget2; // 模型视图

    QTableView *m_pTableView1;
};

UserInfPrivate::UserInfPrivate()
{
    for(int i = 0; i < enum_user_inf_btn_all; i++) {
        m_pBtn[i] = NULL;
    }

    for(int i = 0; i < enum_user_inf_label_all; i++) {
        m_pLabel[i] = NULL;
        m_pLineEdit[i] = NULL;
    }

    m_pTelEdit = NULL;

    m_pListWidget1 = NULL;

    m_pWidget1 = NULL;
    m_pWidget2 = NULL;

    m_pTableView1 = NULL;
}

UserInf::UserInf(QWidget *parent)
    : QDialog{parent},
    d_ptr(new UserInfPrivate)
{
    setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    setWindowTitle("用户信息");
    setMinimumSize(1600, 500);
    initWindow();
    initTable();
}

void UserInf::slotAddUserInf()
{
    QString name            = d_ptr->m_pLineEdit[enum_user_inf_label_name]->text();
    QString company         = d_ptr->m_pLineEdit[enum_user_inf_label_company]->text();
    QString construction    = d_ptr->m_pLineEdit[enum_user_inf_label_construction]->text();

    if(name.isEmpty())
        return;

    //以下就是插入函数的基本语句了
    int row = user_inf_model->rowCount();
    user_inf_model->insertRow(row);
    user_inf_model->setData(user_inf_model->index(row,1),name);
    user_inf_model->setData(user_inf_model->index(row,2),company);
    user_inf_model->setData(user_inf_model->index(row,3),construction);
    user_inf_model->setData(user_inf_model->index(row,4), 0); // 新增未删除用户

    if(user_inf_model->submitAll()) {
        // 插入用户基本信息成功
        QSqlRecord record = user_inf_model->record(row);
        QRegExp regexp("\\d{11}");
        for(int i = 0; i < d_ptr->m_pListWidget1->count(); i++) {
            if(!d_ptr->m_pListWidget1->item(i)->text().isEmpty() && d_ptr->m_pListWidget1->item(i)->text().count() == 11 && regexp.indexIn(d_ptr->m_pListWidget1->item(i)->text()) != -1) {
                QString queryStr(QString("INSERT INTO db_tel_inf (userId, tel)  VALUES(%1, %2)").arg(QString::number(record.value(0).toInt()), d_ptr->m_pListWidget1->item(i)->text()));
                QSqlQuery query(g_getDBObject().getDBObj());
                query.exec(queryStr);
            }
        }
    }
    flushTable();
    d_ptr->m_pLineEdit[enum_user_inf_label_name]->clear();
    d_ptr->m_pLineEdit[enum_user_inf_label_company]->clear();
    d_ptr->m_pLineEdit[enum_user_inf_label_construction]->clear();
    d_ptr->m_pListWidget1->clear();
}

void UserInf::slotDeleteUserInf()
{
    int curRow = d_ptr->m_pTableView1->currentIndex().row();
    QModelIndex idx = user_inf_model->index(curRow, 1);
    QString name = user_inf_model->data(idx).toString();
    if(QMessageBox::Ok == QMessageBox::warning(this, "删除所选用户", QString("确认删除用户-%1").arg(name), QMessageBox::Ok, QMessageBox::Cancel)) {
        if(user_inf_model->select())
            user_inf_model->removeRow(curRow);
        user_inf_model->submitAll();
        flushTable();
    }
}

void UserInf::slotAddTel()
{
    if(!d_ptr->m_pTelEdit->text().isEmpty()) {
        d_ptr->m_pListWidget1->addItem(new QListWidgetItem(d_ptr->m_pTelEdit->text()));
        d_ptr->m_pTelEdit->clear();
    }
}

void UserInf::initWindow()
{
    // 初始化按钮
    QStringList listBtn;
    listBtn << "添加新信息" << "删除所选行" << "附加号码";

    // 按钮
    for(int i = 0; i < enum_user_inf_btn_all; i++) {
        d_ptr->m_pBtn[i] = new QPushButton(this);
        d_ptr->m_pBtn[i]->setText(listBtn[i]);
        d_ptr->m_pBtn[i]->setMinimumSize(80, 40);
    }

    // 全局布局
    QHBoxLayout* hlayoutMain = new QHBoxLayout(this);
    hlayoutMain->setContentsMargins(10, 5, 10, 5);
    hlayoutMain->setSpacing(5);

    // Left
    QVBoxLayout* vlayoutLeft = new QVBoxLayout;

    QStringList listLabel;
    listLabel << "姓名" << "所属公司" << "所在地址";
    QGridLayout* gridlayoutTop = new QGridLayout;
    for(int i = 0 ; i < enum_user_inf_label_all; i++) {
        d_ptr->m_pLabel[i] = new QLabel(listLabel[i], this);
        d_ptr->m_pLineEdit[i] = new QLineEdit(this);

        gridlayoutTop->addWidget(d_ptr->m_pLabel[i],    i, 0, 1, 1);
        gridlayoutTop->addWidget(d_ptr->m_pLineEdit[i], i, 1, 1, 2);
    }

    d_ptr->m_pListWidget1 = new QListWidget(this);

    QHBoxLayout *hlayoutTel = new QHBoxLayout;
    d_ptr->m_pTelEdit = new QLineEdit(this);
    d_ptr->m_pTelEdit->setInputMask("DDD99999999");
    hlayoutTel->addWidget(d_ptr->m_pBtn[enum_user_tel_btn_Add]);
    hlayoutTel->addWidget(d_ptr->m_pTelEdit);

    vlayoutLeft->addLayout(gridlayoutTop);
    vlayoutLeft->addWidget(d_ptr->m_pListWidget1);
    vlayoutLeft->addLayout(hlayoutTel);
    vlayoutLeft->addWidget(d_ptr->m_pBtn[enum_user_inf_btn_Add]);

    connect(d_ptr->m_pBtn[enum_user_inf_btn_Add], SIGNAL(released()), this, SLOT(slotAddUserInf()));
    connect(d_ptr->m_pBtn[enum_user_tel_btn_Add], SIGNAL(released()), this, SLOT(slotAddTel()));


    // Right
    QVBoxLayout *vlayoutRight = new QVBoxLayout;

    d_ptr->m_pTableView1 = new QTableView(this);

    vlayoutRight->addWidget(d_ptr->m_pTableView1);
    vlayoutRight->addWidget(d_ptr->m_pBtn[enum_user_inf_btn_Delete]);

    connect(d_ptr->m_pBtn[enum_user_inf_btn_Delete], SIGNAL(released()), this, SLOT(slotDeleteUserInf()));

    hlayoutMain->addLayout(vlayoutLeft);
    hlayoutMain->addLayout(vlayoutRight);
}

void UserInf::initTable()
{
    user_inf_model = new QSqlTableModel(this, g_getDBObject().getDBObj());
    user_inf_model->setTable("db_user_inf");
    user_inf_model->setFilter("deleteFlag = 0");

    flushTable();

    d_ptr->m_pTableView1->setModel(user_inf_model);

    QStringList tables_user;
    tables_user << "ID" << "姓名" << "所属公司" << "所在地址" << "删除状态";
    for(int i = 0 ; i < tables_user.count(); i++)
        user_inf_model->setHeaderData(i,Qt::Horizontal,tables_user[i]);//设置显示框表头显示

    d_ptr->m_pTableView1->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置单元格不可编辑
    d_ptr->m_pTableView1->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    d_ptr->m_pTableView1->horizontalHeader()->setStretchLastSection(true);//设置最后一列填充后面表格
    d_ptr->m_pTableView1->setColumnHidden(0,true);//设置第0行隐藏
    d_ptr->m_pTableView1->setColumnHidden(4,true);//设置第4行隐藏

}

void UserInf::flushTable()
{
    if(user_inf_model) {
        user_inf_model->select();
    }
}

