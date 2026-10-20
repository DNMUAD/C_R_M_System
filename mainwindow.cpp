#include "mainwindow.h"
#include <QButtonGroup>
#include <QTableView>
#include <QToolButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QSqlError>
#include <QDebug>
#include <QHeaderView>

#include "pack/dbinit.h"

#include "userinf.h" // 用户信息
#include "taginf.h" // 标签信息
#include "goodsinf.h" // 商品信息
#include "shoppinginf.h" // 购物信息
#include "usertag.h" // 用户标签
#include "pricegearinf.h" // 价格档位信息
#include "goodsgearinf.h" // 商品档位信息

class InformationViewPrivate {
public:
    InformationViewPrivate();

    QToolButton *m_pBtn[enum_btn_all];

    QWidget* m_pWidget1;        // 客户信息页
    QWidget* m_pWidget2;        // 购物信息页

    QTableView *m_pTableView1;
    QTableView *m_pTableView2;

    QButtonGroup* m_pBtnGroup;
    QButtonGroup* m_pDialogGroup;

    UserInf                     *m_pUserInf;
    TagInf                      *m_pTagInf;
    GoodsInf                    *m_pGoodsInf;
    ShoppingInf                 *m_pShoppingInf;
    UserTag                     *m_pUserTag;
    PriceGearInf                *m_pPriceGearInf;
    GoodsGearInf                *m_pGoodsGearInf;
};

InformationViewPrivate::InformationViewPrivate()
{
    for(int i = 0; i < enum_btn_all; i++) {
        m_pBtn[i] = NULL;
    }

    m_pWidget1          = NULL;
    m_pWidget2          = NULL;
    m_pTableView1       = NULL;
    m_pTableView2       = NULL;

    m_pBtnGroup         = NULL;
    m_pDialogGroup      = NULL;

    m_pUserInf          = NULL;
    m_pTagInf           = NULL;
    m_pGoodsInf         = NULL;
    m_pShoppingInf      = NULL;
    m_pUserTag          = NULL;
    m_pPriceGearInf     = NULL;
    m_pGoodsGearInf     = NULL;
}

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent),
    d_ptr(new InformationViewPrivate)
{
    initWindow();
    initTable();
}

MainWindow::~MainWindow()
{
    deleteDBObject();
}

void MainWindow::slotInformationViewSwitch(int id)
{
    int size = m_pWidgetList.size();
    if(id < size) {
        for(int i = 0; i < size; ++i) {
            m_pWidgetList[i]->setVisible(false);
        }
        if(m_pWidgetList[id]) {
            m_pWidgetList[id]->setVisible(true);
        }
    }
}

void MainWindow::slotDialogViewSwitch(int id)
{
    int size = m_pDialogList.size();
    if(id < size) {
        m_pDialogList[id]->exec();
    }
}

void MainWindow::initTable()
{
    m_pUserInfModel         = new QSqlQueryModel(this);
    m_pShoppingInfModel     = new QSqlQueryModel(this);

    d_ptr->m_pTableView1->setModel(m_pUserInfModel);
    d_ptr->m_pTableView2->setModel(m_pShoppingInfModel);

    flushShowTable();

    QStringList tables_user;
    tables_user << "ID" << "姓名" << "手机号" << "公司" << "工地位置" << "删除状态";
    for(int i = 0 ; i < tables_user.count(); i++)
        m_pUserInfModel->setHeaderData(i,Qt::Horizontal,tables_user[i]);//设置显示框表头显示

    QStringList tables_shopping;
    tables_shopping << "ID" << "买家" << "买家手机号" << "商品名" << "商品图像" << "购买时间" << "价格档位" << "售价" << "购买备注";
    for(int i = 0 ; i < tables_shopping.count(); i++)
        m_pShoppingInfModel->setHeaderData(i,Qt::Horizontal,tables_shopping[i]);//设置显示框表头显示

    d_ptr->m_pTableView1->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置单元格不可编辑
    d_ptr->m_pTableView1->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    d_ptr->m_pTableView1->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    d_ptr->m_pTableView1->horizontalHeader()->setStretchLastSection(true);//设置最后一列填充后面表格
    d_ptr->m_pTableView1->verticalHeader()->setStretchLastSection(true);
    d_ptr->m_pTableView1->setColumnHidden(0,true);//设置第0行隐藏
    d_ptr->m_pTableView1->setColumnHidden(5,true);//设置第5行隐藏

    d_ptr->m_pTableView2->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置单元格不可编辑
    d_ptr->m_pTableView2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    d_ptr->m_pTableView2->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    d_ptr->m_pTableView2->horizontalHeader()->setStretchLastSection(true);//设置最后一列填充后面表格
    d_ptr->m_pTableView2->verticalHeader()->setStretchLastSection(true);
    d_ptr->m_pTableView2->setColumnHidden(0,true);//设置第0行隐藏
}

void MainWindow::initWindow()
{
    // 初始化按钮
    QStringList listBtn;
    listBtn << "客户信息页" << "购物信息页" << "用户信息" << "标签信息" << "商品信息" << "购物信息" << "用户标签" << "价格档位信息" << "商品档位信息";
    // 用户信息：添加删除修改用户
    // 标签信息：添加删除修改标签
    // 商品信息：添加删除修改商品
    // 购物信息：添加购物信息（包含某个用户购买了一个商品按照某个价格档位付了某个具体的价格并添加某个备注），删除修改购物信息
    // 价格档位信息：添加删除修改价格档位
    // 商品档位信息：查看修改商品默认档位对应价格

    // 按钮
    for(int i = 0; i < enum_btn_all; i++) {
        d_ptr->m_pBtn[i] = new QToolButton(this);
        d_ptr->m_pBtn[i]->setText(listBtn[i]);
        d_ptr->m_pBtn[i]->setMinimumSize(80, 40);
    }

    // Globe vlayout
    QVBoxLayout* vlayoutMain = new QVBoxLayout(this);
    vlayoutMain->setContentsMargins(10, 5, 10, 5);
    vlayoutMain->setSpacing(5);

    // 顶部按钮组
    QFrame * frame_top = new QFrame(this);
    frame_top->setFrameShadow(QFrame::Plain);
    frame_top->setFrameShape(QFrame::Box);
    frame_top->setObjectName("frametop");
    QHBoxLayout *hlayoutTop = new QHBoxLayout(frame_top);
    hlayoutTop->setDirection(QHBoxLayout::LeftToRight);
    hlayoutTop->setMargin(0);

    QHBoxLayout *hlayoutBtn_top = new QHBoxLayout;
    hlayoutBtn_top->setMargin(0);
    hlayoutBtn_top->addWidget(d_ptr->m_pBtn[enum_table_user]);
    hlayoutBtn_top->addWidget(d_ptr->m_pBtn[enum_table_shopping]);

    d_ptr->m_pBtnGroup = new QButtonGroup(this);
    d_ptr->m_pBtnGroup->addButton(d_ptr->m_pBtn[enum_table_user],       0);
    d_ptr->m_pBtnGroup->addButton(d_ptr->m_pBtn[enum_table_shopping],   1);
    d_ptr->m_pBtnGroup->setExclusive(true);

    connect(d_ptr->m_pBtnGroup, SIGNAL(buttonClicked(int)), this, SLOT(slotInformationViewSwitch(int)));

    d_ptr->m_pDialogGroup = new QButtonGroup(this);
    d_ptr->m_pDialogGroup->addButton(d_ptr->m_pBtn[enum_btn_user],          0);
    d_ptr->m_pDialogGroup->addButton(d_ptr->m_pBtn[enum_btn_tag],           1);
    d_ptr->m_pDialogGroup->addButton(d_ptr->m_pBtn[enum_btn_goods],         2);
    d_ptr->m_pDialogGroup->addButton(d_ptr->m_pBtn[enum_btn_shopping],      3);
    d_ptr->m_pDialogGroup->addButton(d_ptr->m_pBtn[enum_btn_user_tag],      4);
    d_ptr->m_pDialogGroup->addButton(d_ptr->m_pBtn[enum_btn_price_gear],    5);
    d_ptr->m_pDialogGroup->addButton(d_ptr->m_pBtn[enum_btn_goods_gear],    6);

    d_ptr->m_pUserInf       = new UserInf(this);
    d_ptr->m_pTagInf        = new TagInf(this);
    d_ptr->m_pGoodsInf      = new GoodsInf(this);
    d_ptr->m_pShoppingInf   = new ShoppingInf(this);
    d_ptr->m_pUserTag       = new UserTag(this);
    d_ptr->m_pPriceGearInf  = new PriceGearInf(this);
    d_ptr->m_pGoodsGearInf  = new GoodsGearInf(this);

    m_pDialogList.append((QDialog*)(d_ptr->m_pUserInf));
    m_pDialogList.append((QDialog*)(d_ptr->m_pTagInf));
    m_pDialogList.append((QDialog*)(d_ptr->m_pGoodsInf));
    m_pDialogList.append((QDialog*)(d_ptr->m_pShoppingInf));
    m_pDialogList.append((QDialog*)(d_ptr->m_pUserTag));
    m_pDialogList.append((QDialog*)(d_ptr->m_pPriceGearInf));
    m_pDialogList.append((QDialog*)(d_ptr->m_pGoodsGearInf));

    connect(d_ptr->m_pDialogGroup, SIGNAL(buttonClicked(int)), this, SLOT(slotDialogViewSwitch(int)));

    hlayoutTop->addLayout(hlayoutBtn_top);

    // 中间布局
    QVBoxLayout *vlayoutGroup = new QVBoxLayout;
    // 1
    d_ptr->m_pWidget1 = new QWidget(this);
    // d_ptr->m_pWidget1->setStyleSheet(QString::fromUtf8("border:3px solid blue"));
    d_ptr->m_pTableView1 = new QTableView(this);
    QGridLayout  *gridLayoutWidget1 = new QGridLayout(d_ptr->m_pWidget1);

    gridLayoutWidget1->addWidget(d_ptr->m_pTableView1, 0, 0);

    // 2
    d_ptr->m_pWidget2 = new QWidget(this);
    d_ptr->m_pTableView2 = new QTableView(this);
    QGridLayout  *gridLayoutWidget2 = new QGridLayout(d_ptr->m_pWidget2);

    gridLayoutWidget2->addWidget(d_ptr->m_pTableView2, 0, 0);

    // 中间布局
    vlayoutGroup->addWidget(d_ptr->m_pWidget1);
    vlayoutGroup->addWidget(d_ptr->m_pWidget2);

    // 底部按钮组
    QFrame * frame_bottom = new QFrame(this);
    frame_bottom->setFrameShadow(QFrame::Plain);
    frame_bottom->setFrameShape(QFrame::Box);
    frame_bottom->setObjectName("framebottom");
    QHBoxLayout *hlayoutBottom = new QHBoxLayout(frame_bottom);
    hlayoutBottom->setDirection(QHBoxLayout::LeftToRight);
    hlayoutBottom->setMargin(0);

    QHBoxLayout *hlayoutBtn_bottom = new QHBoxLayout;
    hlayoutBtn_bottom->setMargin(0);
    hlayoutBtn_bottom->addWidget(d_ptr->m_pBtn[enum_btn_user]);
    hlayoutBtn_bottom->addWidget(d_ptr->m_pBtn[enum_btn_tag]);
    hlayoutBtn_bottom->addWidget(d_ptr->m_pBtn[enum_btn_goods]);
    hlayoutBtn_bottom->addWidget(d_ptr->m_pBtn[enum_btn_shopping]);
    hlayoutBtn_bottom->addWidget(d_ptr->m_pBtn[enum_btn_user_tag]);
    hlayoutBtn_bottom->addWidget(d_ptr->m_pBtn[enum_btn_price_gear]);
    hlayoutBtn_bottom->addWidget(d_ptr->m_pBtn[enum_btn_goods_gear]);

    hlayoutBottom->addLayout(hlayoutBtn_bottom);

    // 整体布局
    vlayoutMain->addWidget(frame_top, 1, Qt::AlignTop);
    vlayoutMain->addLayout(vlayoutGroup);
    vlayoutMain->addWidget(frame_bottom, 1, Qt::AlignBottom);

    m_pWidgetList.append(d_ptr->m_pWidget1);
    m_pWidgetList.append(d_ptr->m_pWidget2);
    for(int i = 0; i < m_pWidgetList.size(); ++i) {
        m_pWidgetList[i]->setVisible(false);
    }
    d_ptr->m_pBtn[enum_table_user]->setChecked(true);
    slotInformationViewSwitch(0);
}

void MainWindow::flushShowTable()
{
    m_pUserInfModel->setQuery("SELECT db_user_inf.ID, db_user_inf.name, GROUP_CONCAT(db_tel_inf.tel,';') AS tel, db_user_inf.company, db_user_inf.construction FROM db_user_inf LEFT JOIN db_tel_inf ON db_user_inf.ID = db_tel_inf.userId WHERE db_user_inf.deleteFlag = 0 GROUP BY db_user_inf.ID", g_getDBObject().getDBObj());
    m_pShoppingInfModel->setQuery("SELECT db_shopping_inf.ID, db_user_inf.name, GROUP_CONCAT( db_tel_inf.tel, ';' ) AS tel, db_goods_inf.goodsName, db_goods_inf.image, db_shopping_inf.date, db_price_range.'desc', db_price_inf.price ,db_shopping_inf.'desc' FROM db_shopping_inf LEFT JOIN db_tel_inf INNER JOIN db_user_inf INNER JOIN db_goods_inf INNER JOIN db_price_inf INNER JOIN db_price_range ON db_shopping_inf.userId = db_user_inf.ID AND db_shopping_inf.goosId = db_goods_inf.ID AND db_shopping_inf.priceId = db_price_inf.ID AND db_price_inf.priceRangeId = db_price_range.ID AND db_user_inf.ID = db_tel_inf.userId",
                                    g_getDBObject().getDBObj());

}

