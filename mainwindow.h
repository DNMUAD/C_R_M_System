#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QSqlQueryModel>
#include <QDialog>

// 按钮
enum MainButton {
    enum_table_user = 0,    // 1 // 首页: 客户信息页；包含通过手机号查询，通过名字查询，通过标签查询，通过公司查询，通过位置查询
    enum_table_shopping,    // 2 // 次页: 购物信息页；包含通过时间排序，通过客户名查询，通过商品名查询

    enum_btn_user,
    enum_btn_tag,
    enum_btn_goods,
    enum_btn_shopping,
    enum_btn_user_tag,
    enum_btn_price_gear,
    enum_btn_goods_gear,

    enum_btn_all
};

class InformationViewPrivate;
class DBObject;

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void slotInformationViewSwitch(int);

    void slotDialogViewSwitch(int);

private:
    void initTable();

    void initWindow();

    void flushShowTable();

    InformationViewPrivate*     d_ptr;

    QList<QWidget*>             m_pWidgetList;

    QSqlQueryModel*             m_pUserInfModel;
    QSqlQueryModel*             m_pShoppingInfModel;

    QList<QDialog*>             m_pDialogList;
};
#endif // MAINWINDOW_H
