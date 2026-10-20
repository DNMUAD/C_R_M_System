#include "dbinit.h"
#include <QDebug>
#include <QThread>
#include <QApplication>
#include <QSqlError>
#include <QSqlQuery>

DBInit::DBInit(QSqlDatabase &db):
    DB(db)
{
    init_DB();
}

DBInit::~DBInit()
{
    if(DB.isOpen())
        DB.close();
}

// 初始化数据库
void DBInit::init_DB()
{
    assert(!DB.isOpen());

    qDebug() << QSqlDatabase::drivers() << QThread::currentThreadId();
    QString connectName = QString("db_%1").arg((qint64)QThread::currentThreadId());
    QString dbName      = QString("%1%2").arg(QApplication::applicationDirPath(), "/data/C_R_M.db");
    //确保多次打开不会重复添加
    if(QSqlDatabase::contains(connectName)) {
        DB = QSqlDatabase::database(connectName);
    } else {
        DB = QSqlDatabase::addDatabase("QSQLITE",connectName);
    }
    DB.setDatabaseName(dbName); //数据库名称
    if(!DB.open()){
        qDebug() << DB.lastError();
        abort();
    } else {
        qDebug() << "DB.open() successful";
        // 日志数据库连接成功
        QSqlQuery sqlQuery(DB);
        // 如果日志数据库中没有数据表则创建
        if (!sqlQuery.exec("select * from db_goods_inf where name='goodsName'") && !sqlQuery.exec("select * from db_user_inf where name='name'")) {
            qDebug() << "Table db_goods_inf and table  db_user_inf is not exist" << sqlQuery.lastError();
        }
    }
}

DBObject &DBObject::getInstance()
{
    static DBObject *m_pDBObject;

    if(m_pDBObject == NULL) {
        m_pDBObject = new DBObject();
    }
    return *m_pDBObject;
}

QSqlDatabase &DBObject::getDBObj()
{
    return m_db;
}

void DBObject::deleteLater()
{
    delete this;
}

DBObject::DBObject()
{
    m_pHandle = QSharedPointer<DBInit>(new DBInit(m_db));
    if(!m_db.isOpen()) {
        assert(m_db.open());
    }
}

DBObject::~DBObject()
{
}
